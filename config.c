#include <stdio.h>
#include <stdlib.h>
#include "config.h"

// 全局变量定义
SlaveConfig device_configs[MAX_DEVICES][MAX_CONFIGS];
SlaveConfig sorted_device_configs[MAX_DEVICES][MAX_CONFIGS];

// 初始化设备配置数组
void init_device_configs() {
    for (int i = 0; i < MAX_DEVICES; i++) {
        for (int j = 0; j < MAX_CONFIGS; j++) {
            device_configs[i][j].slave_start_address = undefineValue;
            device_configs[i][j].data_length = undefineValue;
            device_configs[i][j].master_start_address = undefineValue;
        }
    }
}

// 初始化排序以后的设备配置
void init_sorted_device_configs() {
    for (int i = 0; i < MAX_DEVICES; i++) {
        for (int j = 0; j < MAX_CONFIGS; j++) {
            sorted_device_configs[i][j].slave_start_address = undefineValue;
            sorted_device_configs[i][j].data_length = undefineValue;
            sorted_device_configs[i][j].master_start_address =undefineValue;
        }
    }
}

// 添加设备配置
void add_config(int device_id, int config_id, int slave_start_address, int data_length, int master_start_address) {
    if (device_id < 0 || device_id >= MAX_DEVICES || config_id < 0 || config_id >= MAX_CONFIGS) {
        printf("无效的设备ID或配置ID！\n");
        return;
    }

    device_configs[device_id][config_id].slave_start_address = slave_start_address;
    device_configs[device_id][config_id].data_length = data_length;
    device_configs[device_id][config_id].master_start_address = master_start_address;
    device_configs[device_id][config_id].config_id = config_id;
}

// 打印某设备的原始配置（未排序）
void print_configs(int device_id) {
    if (device_id < 0 || device_id >= MAX_DEVICES) {
        printf("无效的设备！\n");
        return;
    }

    printf("设备ID %d 的原始配置信息：\n", device_id);
    for (int i = 0; i < MAX_CONFIGS; i++) {
        if (device_configs[device_id][i].slave_start_address != -1) {
            printf("配置ID %d: 从机起始地址: %d, 数据长度: %d, 主机起始地址: %d\n",
                   i,
                   device_configs[device_id][i].slave_start_address,
                   device_configs[device_id][i].data_length,
                   device_configs[device_id][i].master_start_address);
        }
    }
}

// 比较函数（按从机起始地址）
int compare_by_slave_address(const void *a, const void *b) {
    const SlaveConfig *configA = (const SlaveConfig *)a;
    const SlaveConfig *configB = (const SlaveConfig *)b;
    return configA->slave_start_address - configB->slave_start_address;
}

// 对单个设备配置排序（不改变原始数组）
void sort_configs_by_slave_address(int device_id) {
    if (device_id < 0 || device_id >= MAX_DEVICES) {
        printf("无效的设备ID！\n");
        return;
    }

    int count = 0;

    // 拷贝非空配置到 sorted 数组
    for (int i = 0; i < MAX_CONFIGS; i++) {
        if (device_configs[device_id][i].slave_start_address != 0) {
            sorted_device_configs[device_id][count++] = device_configs[device_id][i];
        }
    }

    // 排序
    qsort(sorted_device_configs[device_id], count, sizeof(SlaveConfig), compare_by_slave_address);
}

// 打印某设备排序后的配置
void print_sorted_configs(int device_id) {
    if (device_id < 0 || device_id >= MAX_DEVICES) {
        printf("无效的设备ID！\n");
        return;
    }

    printf("设备ID %d 排序后的配置信息（按从机起始地址）：\n", device_id);
    for (int i = 0; i < MAX_CONFIGS; i++) {
        if (sorted_device_configs[device_id][i].slave_start_address != -1) {
            int start = sorted_device_configs[device_id][i].slave_start_address;
            int length = sorted_device_configs[device_id][i].data_length;
            printf("从机起始地址: %d, 数据长度: %d, 范围全闭: %d--%d, 主机起始地址: %d\n",
                   start,
                   length,
                   start,
                   start + length - 1,
                   sorted_device_configs[device_id][i].master_start_address);
        }
    }
}

// 打印所有设备排序后的配置
void print_all_sorted_configs() {
    for (int device_id = 0; device_id < MAX_DEVICES; device_id++) {
        print_sorted_configs(device_id);
    }
}
