#ifndef GATEWAY_H
#define GATEWAY_H
#include "queue.h"
#include <stdio.h>  // 用于打印错误信息
#include <stdlib.h> // 用于退出程序

//网关最大地址
#define MAX_ADDRESS 300

//模拟网关数组
extern int gateway_addresses[MAX_ADDRESS];

// 函数声明

// 处理排序后的配置，并将其存入队列
void process_sorted_configs(Queue *q, int max_length);

// 新函数：根据起始地址、长度和数据数组将数据赋值给网关数组
void assign_gateway_data(int start_index, int length, int data[]);

// 打印数组所有内容
void print_gateway_addresses();

void process_device_configs(Queue *q, SlaveConfig device_configs[MAX_DEVICES][MAX_CONFIGS]);

#endif // GATEWAY_H
