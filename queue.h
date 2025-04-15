#ifndef QUEUE_H
#define QUEUE_H

// 队列节点结构
typedef struct QueueNode {
    struct QueueNode *next;
    int slave_start_address;
    int data_length;
    int device_id;
    int config_start_id;
    int config_end_id;
} QueueNode;

// 队列结构体（链表头尾）
typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;

// 函数声明
void init_queue(Queue *q);
int is_empty(Queue *q);
void enqueue(Queue *q, QueueNode *item);
QueueNode *dequeue(Queue *q);
void free_queue(Queue *q);

QueueNode *create_node(int slave_start_address, int data_length, int device_id, int start_id, int end_id);
void merge_or_add(Queue *q, QueueNode *new_item, int max_length);

void print_queue(Queue *q);

#endif
