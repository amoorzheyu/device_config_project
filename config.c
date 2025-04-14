#include <stdio.h>
#include "config.h"

// 每个设备的配置数组
SlaveConfig device_configs[MAX_DEVICES][MAX_CONFIGS];

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
    if (device_id < 0 || device_id >= MAX_CONFIGS) {
        printf("无效的设备！\n");
        return;
    }

    printf("设备ID %d 的配置信息：\n", device_id);
    for (int i = 0; i < MAX_CONFIGS; i++) {
        if (device_configs[device_id][i].slave_start_address != 0) { // 如果配置不为空
            printf("配置ID %d: 从机起始地址: %d, 数据长度: %d, 主机起始地址: %d\n",
                   i, device_configs[device_id][i].slave_start_address,
                   device_configs[device_id][i].data_length, device_configs[device_id][i].master_start_address);
        }
    }
}
