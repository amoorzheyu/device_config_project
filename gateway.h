#ifndef GATEWAY_H
#define GATEWAY_H

#include "config.h"  // 引入配置文件
#include "queue.h"   // 引入 queue.h 以使用队列操作函数

// 临时数组结构体，用于存储配置
typedef struct {
    int temp_array[MAX_ADDRESS]; // 临时一维数组，用于标记地址范围
} TempArray;

// 函数声明
void initialize_temp_array(TempArray* temp_array);                        // 初始化临时数组
void process_device_configs(SlaveConfig device_configs[MAX_DEVICES][MAX_CONFIGS], TempArray* temp_array);  // 处理设备配置
void extract_ranges_to_queue(TempArray* temp_array, Queue* queue, int device_id); // 提取地址区间到队列                                        // 打印队列内容

#endif // GATEWAY_H
