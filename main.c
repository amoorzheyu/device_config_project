#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "gateway.h"
#include "queue.h"

// 函数声明
void read_configs();
void sort_all_configs();
void print_all_sorted_configs();

int main() {
    Queue q;                  // 创建队列实例
    init_queue(&q);           // 初始化队列

    read_configs();           // 读取配置文件
    sort_all_configs();       // 对每个设备配置排序
    print_all_sorted_configs(); // 仅打印排序后的配置（调试用）

    process_sorted_configs(&q, 100); // 利用排序结果生成队列元素（不打印队列）

    printf("------------------------------------队列元素：\n");
    print_queue(&q);

    free_queue(&q);           // 清理分配的内存
    getchar();
    return 0;
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

void print_all_sorted_configs() {
    for (int i = 0; i < MAX_DEVICES; i++) {
        sort_configs_by_slave_address(i); // 可选，调试时重复排序无影响
        printf("设备 %d 的排序后配置信息：\n", i);
        print_sorted_configs();
        printf("\n");
    }
}
