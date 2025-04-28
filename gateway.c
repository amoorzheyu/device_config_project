#include <stdio.h>
#include <string.h>
#include "queue.h"
#include "config.h"
#include "gateway.h"
#include "slave.h"

int gateway_addresses[MAX_ADDRESS] = {0};

void replace_slave_data(int master_start_address,
                        int master_data_length,
                        int master_data[])
{
    int range_start = master_start_address;
    int range_end = master_start_address + master_data_length - 1;

    // 遍历设备与其配置表
    for (int dev = 0; dev < MAX_DEVICES; dev++)
    {
        for (int cfg = 0; cfg < MAX_CONFIGS; cfg++)
        {
            const SlaveConfig *c = &sorted_device_configs[dev][cfg];

            // 跳过未定义的配置
            if (c->slave_start_address == -1)
            {
                continue;
            }

            int m0 = c->master_start_address;
            int m1 = m0 + c->data_length - 1;

            // 计算主机地址区间 [range_start, range_end] 与 [m0, m1] 的交集
            int ov_start = (m0 > range_start) ? m0 : range_start;
            int ov_end = (m1 < range_end) ? m1 : range_end;

            if (ov_start > ov_end)
                continue; // 无重叠，跳过

            // 相交长度
            int overlap_length = ov_end - ov_start + 1;

            //相交开始位置相对于数据起始位置的下标
            int data_offset = ov_start - c->master_start_address;

            //从机起始地址
            int slave_start_address = c->slave_start_address+data_offset;

            //使用memcpy截取master_data中data_offset下标开始，长度为overlap_length的数据长度
            int *slice_data = malloc(overlap_length * sizeof(int));
            memcpy(slice_data, master_data+(ov_start-range_start), overlap_length * sizeof(int)); 

            //打印偏移量
            printf("偏移量：%d\n", data_offset);

            //打印相关信息：从机设备号，从机起始地址，数据长度，数据数组
            printf("设备号： %d, 配置id:%d,从机起始地址: %d, 数据长度: %d, 数据: \n", dev,cfg, slave_start_address, overlap_length);

            for (int i = 0; i < overlap_length; i++)
            {
                printf("%d ", slice_data[i]);
            }
            printf("\n");
        }
    }
}

// 处理排序后的配置，存入队列
void process_sorted_configs(Queue *q, int max_length)
{
    for (int device_id = 0; device_id < MAX_DEVICES; device_id++)
    {
        for (int i = 0; i < MAX_CONFIGS; i++)
        {
            SlaveConfig *config = &sorted_device_configs[device_id][i];
            if (config->slave_start_address == -1)
                continue;

            // 创建一个队列节点（每条配置作为一段）
            QueueNode *node = create_node(
                config->slave_start_address,
                config->data_length,
                device_id,
                i, // sort_config_start_index
                i  // sort_config_end_index
            );

            // 加入队列（合并由 merge_or_add 控制）
            merge_or_add(q, node, max_length);
        }
    }
}

// 根据起始地址、长度和数据数组将数据依次赋值给网关数组
void assign_gateway_data(int start_index, int length, int data[])
{
    // 检查索引是否越界
    if (start_index < 0 || start_index + length > MAX_ADDRESS)
    {
        // 打印错误信息并退出程序
        fprintf(stderr, "Error: Index out of bounds. Start index: %d, Length: %d, Max Address: %d\n", start_index, length, MAX_ADDRESS);
        exit(EXIT_FAILURE); // 退出程序
    }

    // 使用memcpy来一次性复制数据
    memcpy(&gateway_addresses[start_index], data, length * sizeof(int));
}

// 打印数组所有内容
void print_gateway_addresses()
{
    FILE *file = fopen("gateway_data.txt", "w"); // 打开文件用于写入
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open output file.\n");
        return;
    }

    fprintf(file, "Gateway Addresses:\n");
    for (int i = 0; i < MAX_ADDRESS; ++i)
    {
        fprintf(file, "Index %d: %d\n", i, gateway_addresses[i]);
    }

    fclose(file); // 关闭文件
    printf("Gateway addresses successfully written to gateway_output.txt\n");
}

// 根据队列和设备配置，将数据存入网关模拟数组
void process_device_configs(Queue *q, SlaveConfig device_configs[MAX_DEVICES][MAX_CONFIGS])
{
    QueueNode *current_node = q->front;

    while (current_node != NULL)
    {
        int device_id = current_node->device_id;
        int node_start = current_node->slave_start_address;
        int node_length = current_node->data_length;
        int node_end = node_start + node_length;

        // 获取从机数据
        int *data = get_slave_addresses(device_id, node_start, node_length);
        if (data == NULL)
        {
            fprintf(stderr, "Error: Failed to get slave addresses for device %d.\n", device_id);
            exit(EXIT_FAILURE);
        }

        // 遍历整个设备的配置，找与当前 node 有交集的配置
        for (int config_id = 0; config_id < MAX_CONFIGS; ++config_id)
        {
            SlaveConfig *config = &device_configs[device_id][config_id];
            if (config->slave_start_address == -1)
                continue;

            int config_start = config->slave_start_address;
            int config_length = config->data_length;
            int config_end = config_start + config_length;

            // 跳过不重叠的部分
            if (config_end <= node_start || config_start >= node_end)
                continue;

            // 找出交集
            int overlap_start = (config_start > node_start) ? config_start : node_start;
            int overlap_end = (config_end < node_end) ? config_end : node_end;
            int overlap_len = overlap_end - overlap_start;

            if (overlap_len <= 0)
                continue;

            // 在 data[] 中的起始位置
            int data_offset = overlap_start - node_start;

            int *slice_data = malloc(overlap_len * sizeof(int));
            if (!slice_data)
            {
                fprintf(stderr, "Memory allocation failed (length=%d).\n", overlap_len);
                exit(EXIT_FAILURE);
            }

            memcpy(slice_data, &data[data_offset], overlap_len * sizeof(int));

            // 网关目标地址偏移 = 当前 overlap 在 config 中的偏移
            int master_offset = overlap_start - config_start;
            int gateway_address = config->master_start_address + master_offset;

            assign_gateway_data(gateway_address, overlap_len, slice_data);

            free(slice_data);
        }

        current_node = current_node->next;
    }
}
