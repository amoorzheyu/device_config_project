#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "config.h"

// 初始化队列
void init_queue(Queue *q) {
    q->front = NULL; //前驱
    q->rear = NULL; //后继
}

// 判断队列是否为空
int is_empty(Queue *q) {
    return q->front == NULL;
}

// 创建节点
QueueNode *create_node(int slave_start_address, int data_length, int device_id, int sort_config_start_index, int sort_config_end_index) {
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    if (node == NULL) {
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }
    node->slave_start_address = slave_start_address;
    node->data_length = data_length;
    node->device_id = device_id;
    node->sort_config_start_index = sort_config_start_index;
    node->sort_config_end_index = sort_config_end_index;
    node->next = NULL;
    return node;
}

// 入队
void enqueue(Queue *q, QueueNode *item) {
    if (q->rear == NULL) {
        q->front = q->rear = item;
    } else {
        q->rear->next = item;
        q->rear = item;
    }
}

// 出队
QueueNode *dequeue(Queue *q) {
    if (is_empty(q)) return NULL;
    QueueNode *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    return temp;
}

// 释放所有队列节点
void free_queue(Queue *q) {
    while (!is_empty(q)) {
        QueueNode *node = dequeue(q);
        free(node);
    }
}

void calculate_sorted_index_range(
    int device_id,
    int start_addr,
    int data_length,
    int *out_start_index,
    int *out_end_index,
    SlaveConfig sorted_device_configs[MAX_DEVICES][MAX_CONFIGS]
) {
    int end_addr = start_addr + data_length;
    *out_start_index = -1;
    *out_end_index = -1;

    for (int i = 0; i < MAX_CONFIGS; ++i) {
        SlaveConfig *cfg = &sorted_device_configs[device_id][i];
        if (cfg->slave_start_address == -1)
            continue;

        int cfg_start = cfg->slave_start_address;
        int cfg_end = cfg_start + cfg->data_length;

        if (cfg_end <= start_addr || cfg_start >= end_addr)
            continue;

        if (*out_start_index == -1 || i < *out_start_index) *out_start_index = i;
        if (*out_end_index == -1 || i > *out_end_index) *out_end_index = i;
    }
}


// 合并指令或拆分存入队列
void merge_or_add(Queue *q, QueueNode *new_item, int max_length) {
    if (is_empty(q)) {
        enqueue(q, new_item);
        return;
    }

    QueueNode *last = q->rear;

    int last_end = last->slave_start_address + last->data_length;
    int new_start = new_item->slave_start_address;
    int new_end = new_item->slave_start_address + new_item->data_length;

    if (last->device_id == new_item->device_id && last_end >= new_start) {
        int merged_start = last->slave_start_address;
        int merged_ideal_end = (new_end > last_end) ? new_end : last_end;
        int merged_total_len = merged_ideal_end - merged_start;

        if (merged_total_len <= max_length) {
            last->data_length = merged_total_len;

            if (new_item->sort_config_start_index < last->sort_config_start_index)
                last->sort_config_start_index = new_item->sort_config_start_index;
            if (new_item->sort_config_end_index > last->sort_config_end_index)
                last->sort_config_end_index = new_item->sort_config_end_index;

            free(new_item);
            return;
        }

        // 超限，尝试拆分
        int space_left = max_length - (last_end - merged_start);

        if (space_left > 0 && space_left < new_item->data_length) {
            last->data_length += space_left;

            if (new_item->sort_config_start_index < last->sort_config_start_index)
                last->sort_config_start_index = new_item->sort_config_start_index;
            if (new_item->sort_config_end_index > last->sort_config_end_index)
                last->sort_config_end_index = new_item->sort_config_end_index;

            int remaining_start = new_item->slave_start_address + space_left;
            int remaining_len = new_item->data_length - space_left;

            int new_cfg_start, new_cfg_end;
            calculate_sorted_index_range(
                new_item->device_id,
                remaining_start,
                remaining_len,
                &new_cfg_start,
                &new_cfg_end,
                sorted_device_configs
            );

            QueueNode *remaining = create_node(
                remaining_start,
                remaining_len,
                new_item->device_id,
                new_cfg_start,
                new_cfg_end
            );

            free(new_item);
            merge_or_add(q, remaining, max_length);
            return;
        } else {
            // 无法合并或拆分
            enqueue(q, new_item);
            return;
        }
    }

    enqueue(q, new_item);
}


void print_queue(Queue *q) {
    QueueNode *curr = q->front;
    int index = 0;
    if (curr == NULL) {
        printf("队列为空。\n");
        return;
    }

    printf("队列内容如下：\n");
    while (curr != NULL) {
        printf("指令 %d -> 设备ID: %d, 起始地址: %d, 长度: %d, 排序后配置下标范围: %d-%d\n",
               index++,
               curr->device_id,
               curr->slave_start_address,
               curr->data_length,
               curr->sort_config_start_index,
               curr->sort_config_end_index);
        curr = curr->next;
    }
}
