#include <stdio.h>
#include "gateway.h"

// 初始化临时数组为 0
void initialize_temp_array(TempArray* temp_array) {
    for (int i = 0; i < MAX_ADDRESS; i++) {
        temp_array->temp_array[i] = 0;  // 将临时数组的所有元素设置为 0
    }
}

// 遍历设备配置并填充临时数组
void process_device_configs(SlaveConfig device_configs[MAX_DEVICES][MAX_CONFIGS], TempArray* temp_array) {
    // 遍历每个设备的配置
    for (int i = 0; i < MAX_DEVICES; i++) {
        for (int j = 0; j < MAX_CONFIGS; j++) {
            if (device_configs[i][j].slave_start_address != 0) {  // 如果设备存在
                // 将临时数组中设备的从机地址范围标记为1
                for (int k = device_configs[i][j].slave_start_address; k < device_configs[i][j].slave_start_address + device_configs[i][j].data_length; k++) {
                    if (k < MAX_ADDRESS) {
                        temp_array->temp_array[k] = 1;
                    }
                }
            }
        }
    }
}

// 将连续的 1 区域转换为开始位置和长度，并存入队列
void extract_ranges_to_queue(TempArray* temp_array, Queue* queue, int device_id) {
    int start = -1;
    for (int i = 0; i < MAX_ADDRESS; i++) {
        if (temp_array->temp_array[i] == 1) {
            if (start == -1) {
                start = i;  // 找到连续1的起始位置
            }
        } else {
            if (start != -1) {
                // 找到连续的1的结束位置，计算长度并加入队列
                int length = i - start;
                enqueue(queue, device_id, start, length);
                start = -1;  // 重置起始位置
            }
        }
    }
    // 如果末尾是1，最后一个区间需要特殊处理
    if (start != -1) {
        enqueue(queue, device_id, start, MAX_ADDRESS - start);
    }
}


