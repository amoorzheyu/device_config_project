#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "config.h"

// 创建队列
Queue* create_queue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    return queue;
}

// 入队操作
void enqueue(Queue* queue, int device_id, int start_index, int length) {
    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
    new_node->device_id = device_id;
    new_node->start_index = start_index;
    new_node->length = length;
    new_node->next = NULL;

    if (queue->rear == NULL) {
        queue->front = queue->rear = new_node;
        return;
    }

    queue->rear->next = new_node;
    queue->rear = new_node;
}

// 出队操作
int dequeue(Queue* queue) {
    if (is_empty(queue)) {
        printf("队列为空，无法出队！\n");
        return -1;
    }

    QueueNode* temp = queue->front;
    int device_id = temp->device_id;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    return device_id;
}

// 判断队列是否为空
int is_empty(Queue* queue) {
    return queue->front == NULL;
}

// 打印队列内容
void print_queue(Queue* queue) {
    int count= 0;
    QueueNode* current = queue->front;
    while (current != NULL) {
        printf("设备ID: %d, 起始位置: %d, 长度: %d\n", 
               current->device_id, current->start_index, current->length);
        current = current->next;
        count++;
    }
    printf("队列长度为: %d\n", count);
    double total_count = MAX_CONFIGS*MAX_DEVICES;
    printf("通信优化率为: %lf\n", (total_count-count)/total_count);
}
