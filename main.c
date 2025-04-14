#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "gateway.h"
#include "queue.h"


// 读取文件配置信息
void read_configs() ;

// 打印设备配置信息
void print_all_configs();

int main() {
   
    read_configs();// 读取配置文件
    print_all_configs();// 打印配置信息

       // 创建队列
    Queue* queue = create_queue();

    // 创建临时数组
    TempArray temp_array;

    // 初始化临时数组
    initialize_temp_array(&temp_array);

    // 遍历设备配置，填充临时数组
    process_device_configs(device_configs, &temp_array);

    // 从临时数组中提取连续的1，并存入队列
    for (int i = 0; i < MAX_DEVICES; i++) {
        extract_ranges_to_queue(&temp_array, queue, i);
    }

    // 打印队列内容
    print_queue(queue);

    // 清理内存
    free(queue);
    getchar();
    return 0;
}


void read_configs() {
    FILE *file = fopen("config.txt", "r");
    if (file == NULL) {
        perror("打开配置文件失败");
        return 1;
    }

    int device_id, config_id, slave_start_address, data_length, master_start_address;

    // 从文件逐行读取配置
    while (fscanf(file, "%d %d %d %d %d", &device_id, &config_id, 
                   &slave_start_address, &data_length, &master_start_address) == 5) {
        // 添加读取到的配置
        add_config(device_id, config_id, slave_start_address, data_length, master_start_address);
    }

    fclose(file);
}

void print_all_configs() {
     // 打印设备配置
     for (int i = 0; i < MAX_DEVICES; i++) {
        print_configs(i);  // 打印每个设备的配置
    }
}