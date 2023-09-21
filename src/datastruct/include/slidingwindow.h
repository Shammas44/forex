#ifndef SLIDINGWINDOW_H
#define SLIDINGWINDOW_H
#include <stdio.h>
#include <stdlib.h>

typedef struct Slidingwindow_node {
    void* data;
    struct Slidingwindow_node* next;
} Slidingwindow_node;

typedef struct Slidingwindow {
    Slidingwindow_node* front;
    Slidingwindow_node* rear;
    int len;
    int count;
} Slidingwindow;

Slidingwindow* slidingwindow_init(int len, size_t datasize);

void slidingwindow_init_from_null(Slidingwindow* window, int len);

void slidingwindow_add(Slidingwindow* window, void *element);

void *slidingwindow_last(Slidingwindow* window);

void slidingwindow_free(Slidingwindow* window);

#endif 
