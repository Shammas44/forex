#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>

// Define the Node structure
typedef struct Queue_node {
    void *data;
    struct Queue_node* next;
} Queue_node;

// Define the Queue structure
typedef struct Queue {
    Queue_node* front;
    Queue_node* rear;
} Queue;

Queue* queue_init(); 

void queue_enqueue(Queue* q, void *data);

void* queue_dequeue(Queue* q);

int queue_is_empty(Queue* q);
#endif 
