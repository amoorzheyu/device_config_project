#ifndef QUEUE_H
#define QUEUE_H

// 队列节点结构体
typedef struct QueueNode {
    int device_id;
    int start_index;
    int length;
    struct QueueNode* next;
} QueueNode;

// 队列结构体
typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// 函数声明
Queue* create_queue();                // 创建队列
void enqueue(Queue* queue, int device_id, int start_index, int length);  // 入队操作
int dequeue(Queue* queue);            // 出队操作
int is_empty(Queue* queue);           // 判断队列是否为空
void print_queue(Queue* queue);       // 打印队列内容

#endif // QUEUE_H
