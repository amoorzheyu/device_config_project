#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slave.h"

// 拼接字符串 device_id + start_address → 转换为整数作为数组首项
int* get_slave_addresses(int device_id, int start_address, int length) {
    if (length <= 0) {
        return NULL;
    }

    int* addresses = (int*)malloc(sizeof(int) * (length + 1)); // 多一个位置存拼接值
    if (addresses == NULL) {
        perror("内存分配失败");
        return NULL;
    }

    // 拼接 device_id 和 start_address 成字符串
    char buffer[32]; // 足够大
    snprintf(buffer, sizeof(buffer), "%d0%d", device_id, start_address);
    addresses[0] = atoi(buffer); // 转换为整数

    // 填充地址列表
    for (int i = 0; i < length; i++) {
        addresses[i + 1] = addresses[0] + i+1;
    }

    return addresses;
}
