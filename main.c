#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "gateway.h"
#include "queue.h"
#include "slave.h"

// 函数声明
void read_configs();
void sort_all_configs();
void print_all_sorted_configs();
void test_get_slave_addresses();
int main() {
    Queue q;                  // 创建队列实例
    init_queue(&q);           // 初始化队列
    init_device_configs();// 初始化设备配置
    init_sorted_device_configs();// 初始化已排序设备配置

    read_configs();           // 读取配置文件
    sort_all_configs();       // 对每个设备配置排序
    print_all_sorted_configs(); // 仅打印排序后的配置（调试用）

    
    process_sorted_configs(&q, 120); // 利用排序结果生成队列元素

    printf("------------------------------------队列元素：\n");
    print_queue(&q);

    process_device_configs(&q,&device_configs);
    print_gateway_addresses();

    free_queue(&q);           // 清理分配的内存
    getchar();
    
    return 0;
}

void test_get_slave_addresses(){
     // 测试返回内容
    int* arr= get_slave_addresses(1, 100, 4);
    if(arr){
        for(int i=0; i<4; i++){
            printf("%d \n", arr[i]);
        }
    }
}

void read_configs() {
    FILE *file = fopen("config.txt", "r");
    if (file == NULL) {
        perror("打开配置文件失败");
        return;
    }

    int device_id, config_id, slave_start_address, data_length, master_start_address;

    while (fscanf(file, "%d %d %d %d %d", &device_id, &config_id,
                  &slave_start_address, &data_length, &master_start_address) == 5) {
        add_config(device_id, config_id, slave_start_address, data_length, master_start_address);
    }

    fclose(file);
}

void sort_all_configs() {
    for (int i = 0; i < MAX_DEVICES; i++) {
        sort_configs_by_slave_address(i);
    }
}

