#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

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
QueueNode *create_node(int slave_start_address, int data_length, int device_id, int start_id, int end_id) {
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    if (node == NULL) {
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }
    node->slave_start_address = slave_start_address;
    node->data_length = data_length;
    node->device_id = device_id;
    node->config_start_id = start_id;
    node->config_end_id = end_id;
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

// 合并指令或拆分存入队列
void merge_or_add(Queue *q, QueueNode *new_item, int max_length) {
    if (is_empty(q)) {
        enqueue(q, new_item);
        return;
    }

    // 获取最后一个节点
    QueueNode *last = q->rear;

    // 同设备 + 地址连续或相交
    int last_end = last->slave_start_address + last->data_length;
    int new_start = new_item->slave_start_address;
    int new_end = new_item->slave_start_address + new_item->data_length;

    if (last->device_id == new_item->device_id &&
        last_end >= new_start) {  // 允许连续或重叠

        int merged_start = last->slave_start_address;
        int merged_ideal_end = (new_end > last_end) ? new_end : last_end;
        int merged_total_len = merged_ideal_end - merged_start;

        if (merged_total_len <= max_length) {
            //  合并整个区域
            last->data_length = merged_total_len;
            if (new_item->config_end_id > last->config_end_id)
                last->config_end_id = new_item->config_end_id;
            free(new_item);
            return;
        } else {
            //  超出 max_length，仅合并部分
            int appendable_len = max_length - last->data_length;
            if (appendable_len > 0) {
                last->data_length += appendable_len;
                last->config_end_id = new_item->config_end_id;

                // 创建剩余部分
                int remaining_len = new_item->data_length - appendable_len;
                int remaining_start = new_item->slave_start_address + appendable_len;

                QueueNode *split_part = create_node(
                    remaining_start,
                    remaining_len,
                    new_item->device_id,
                    new_item->config_start_id,
                    new_item->config_end_id
                );
                enqueue(q, split_part);
            } else {
                enqueue(q, new_item);
                return;
            }
            free(new_item);
            return;
        }
    }

    // 设备不同 或地址不连续：直接入队
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
        printf("指令 %d -> 设备ID: %d, 起始地址: %d, 长度: %d, 配置ID范围: %d-%d\n",
               index++,
               curr->device_id,
               curr->slave_start_address,
               curr->data_length,
               curr->config_start_id,
               curr->config_end_id);
        curr = curr->next;
    }
}
