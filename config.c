#include <stdio.h>
#include <stdlib.h> 
#include "config.h"

SlaveConfig device_configs[MAX_DEVICES][MAX_CONFIGS];
SlaveConfig sorted_device_configs[MAX_CONFIGS];

// 添加设备配置
void add_config(int device_id, int config_id, int slave_start_address, int data_length, int master_start_address) {
    if (device_id < 0 || device_id >= MAX_DEVICES || config_id < 0 || config_id >= MAX_CONFIGS) {
        printf("无效的设备ID或配置ID！\n");
        return;
    }

    device_configs[device_id][config_id].slave_start_address = slave_start_address;
    device_configs[device_id][config_id].data_length = data_length;
    device_configs[device_id][config_id].master_start_address = master_start_address;
}

// 打印设备配置
void print_configs(int device_id) {
    if (device_id < 0 || device_id >= MAX_DEVICES) {
        printf("无效的设备！\n");
        return;
    }

    printf("设备ID %d 的配置信息：\n", device_id);
    for (int i = 0; i < MAX_CONFIGS; i++) {
        if (device_configs[device_id][i].slave_start_address != 0) {
            printf("配置ID %d: 从机起始地址: %d, 数据长度: %d, 主机起始地址: %d\n",
                   i, device_configs[device_id][i].slave_start_address,
                   device_configs[device_id][i].data_length, device_configs[device_id][i].master_start_address);
        }
    }
}

// 比较函数
int compare_by_slave_address(const void *a, const void *b) {
    SlaveConfig *configA = (SlaveConfig *)a;
    SlaveConfig *configB = (SlaveConfig *)b;
    return configA->slave_start_address - configB->slave_start_address;
}


// 按从机起始地址排序（不修改原数组）
void sort_configs_by_slave_address(int device_id) {
    int count = 0;

    // 收集非空配置
    for (int i = 0; i < MAX_CONFIGS; i++) {
        if (device_configs[device_id][i].slave_start_address != 0) {
            sorted_device_configs[count++] = device_configs[device_id][i];
        }
    }

    // 使用标准库 qsort 排序
    qsort(sorted_device_configs, count, sizeof(SlaveConfig), compare_by_slave_address);
}

// 打印排序后的配置
void print_sorted_configs() {
    printf("排序后的配置信息（按从机起始地址）：\n");
    for (int i = 0; i < MAX_CONFIGS; i++) {
        if (sorted_device_configs[i].slave_start_address != 0) {
            printf("从机起始地址: %d, 数据长度: %d,范围全闭：%d--%d, 主机起始地址: %d\n",
                   sorted_device_configs[i].slave_start_address,
                   sorted_device_configs[i].data_length,
                   sorted_device_configs[i].slave_start_address,
                   sorted_device_configs[i].slave_start_address+sorted_device_configs[i].data_length-1,
                   sorted_device_configs[i].master_start_address);
        }
    }
}
