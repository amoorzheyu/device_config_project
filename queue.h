#ifndef QUEUE_H
#define QUEUE_H

// 队列节点结构
typedef struct QueueNode {
    struct QueueNode *next;
    int slave_start_address; //从机开始地址
    int data_length; //数据长度
    int device_id; //设备id
    int sort_config_start_index; //排序后配置开始索引（即数组下标）
    int sort_config_end_index; //排序后配置结束索引（即数组下标）
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

void calculate_sorted_index_range();
QueueNode *create_node(int slave_start_address, int data_length, int device_id, int sort_config_start_index, int sort_config_end_index);
void merge_or_add(Queue *q, QueueNode *new_item, int max_length);

void print_queue(Queue *q);

#endif
