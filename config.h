#ifndef CONFIG_H
#define CONFIG_H

// 定义从机配置结构体
typedef struct {
    int slave_start_address; // 从机起始地址
    int data_length;         // 数据长度
    int master_start_address; // 主机起始地址
} SlaveConfig;

// 每个设备ID对应一个配置数组
#define MAX_DEVICES 10

// 每个设备最多50个配置
#define MAX_CONFIGS 50

// 每个设备最多256个地址
#define MAX_ADDRESS 256 

// 所有从机设备的配置
extern SlaveConfig device_configs[MAX_DEVICES][MAX_CONFIGS];

// 函数声明
void add_config(int device_id, int config_id, int slave_start_address, int data_length, int master_start_address);
void print_configs(int device_id);

#endif
