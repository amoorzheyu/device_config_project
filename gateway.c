#include <stdio.h>
#include <string.h>
#include "queue.h"
#include "config.h"
#include "gateway.h"
#include "slave.h"

int gateway_addresses[MAX_ADDRESS] = {0};

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
                config->config_id,  // config_start_id
                config->config_id   // config_end_id（只有自己）
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
        int slave_start_address = current_node->slave_start_address;
        int data_length = current_node->data_length;
        int config_start_id = current_node->config_start_id;
        int config_end_id = current_node->config_end_id;

        // 获取从机数据
        int *data = get_slave_addresses(device_id, slave_start_address, data_length);

        if (data == NULL)
        {
            fprintf(stderr, "Error: Failed to get slave addresses for device %d.\n", device_id);
            exit(EXIT_FAILURE);
        }

        // data切片索引
        int var_data_start_index = 0;
        int var_data_end_index = 0;
        int var_data_length = 0;
        int var_master_start_address = 0;

        // 遍历配置段
        for (int config_id = config_start_id; config_id <= config_end_id; ++config_id)
        {
            SlaveConfig *config = &device_configs[device_id][config_id];

            if (config_id == config_start_id)
            {
                var_data_start_index = 0;
                var_data_end_index = config->data_length - 1;
            }
            else
            {
                SlaveConfig *lastConfig = &device_configs[device_id][config_id - 1];
                var_data_start_index += lastConfig->data_length;
                var_data_end_index += config->data_length;
            }

            var_data_length = var_data_end_index - var_data_start_index + 1;
            var_master_start_address = config->master_start_address;

            // 越界检查
            if (var_data_length <= 0 || var_data_start_index + var_data_length > data_length)
            {
                fprintf(stderr, "Invalid data slice: start=%d, length=%d, total=%d\n",
                        var_data_start_index, var_data_length, data_length);
                exit(EXIT_FAILURE);
            }

            int *slice_data = malloc(var_data_length * sizeof(int));
            if (!slice_data)
            {
                fprintf(stderr, "Memory allocation failed (length=%d).\n", var_data_length);
                exit(EXIT_FAILURE);
            }

            memcpy(slice_data, &data[var_data_start_index], var_data_length * sizeof(int));

            // 写入网关数组
            assign_gateway_data(var_master_start_address, var_data_length, slice_data);

            // ✅ 一定要释放！
            free(slice_data);
        }

        current_node = current_node->next;
    }
}
