#include <stdio.h>
#include "queue.h"
#include "config.h"
#include "gateway.h"


// 处理排序后的配置，存入队列
void process_sorted_configs(Queue *q, int max_length) {
    for (int device_id = 0; device_id < MAX_DEVICES; device_id++) {
        sort_configs_by_slave_address(device_id); // 排序当前设备的配置

        // 遍历当前设备的所有配置
        for (int i = 0; i < MAX_CONFIGS; i++) {
            if (sorted_device_configs[i].slave_start_address != 0) { // 如果配置不为空
                int start_addr = sorted_device_configs[i].slave_start_address;
                int len = sorted_device_configs[i].data_length;

                // 创建新的队列节点
                QueueNode *node = create_node(start_addr, len, device_id, i, i);

                // 将指令合并或拆分后加入队列
                merge_or_add(q, node, max_length);
            }
        }
    }
}
