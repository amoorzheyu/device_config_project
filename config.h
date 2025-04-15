#ifndef CONFIG_H
#define CONFIG_H

// 定义从机配置结构体
typedef struct {
    int slave_start_address; // 从机起始地址
    int data_length;         // 数据长度
    int master_start_address; // 主机起始地址
} SlaveConfig;

#define MAX_DEVICES 10
#define MAX_CONFIGS 50
#define MAX_ADDRESS 256 

extern SlaveConfig device_configs[MAX_DEVICES][MAX_CONFIGS];
extern SlaveConfig sorted_device_configs[MAX_CONFIGS]; // 用于保存排序后的结果

// 函数声明
void add_config(int device_id, int config_id, int slave_start_address, int data_length, int master_start_address);
void print_configs(int device_id);
void sort_configs_by_slave_address(int device_id);
void print_sorted_configs();

#endif
