#include "slidingwindow.h"

Slidingwindow* slidingwindow_init(int len, size_t dataSize) {
    Slidingwindow* window = (Slidingwindow*)malloc(sizeof(Slidingwindow));
    if (window == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    window->front = NULL;
    window->rear = NULL;
    window->len = len;
    window->count = 0;
    return window;
}

void slidingwindow_init_from_null(Slidingwindow* window, int len) {
    window->front = NULL;
    window->rear = NULL;
    window->len = len;
    window->count = 0;
}

void slidingwindow_add(Slidingwindow* window, void *element) {
    if (window->count >= window->len) {
        Slidingwindow_node* temp = window->front;
        window->front = temp->next;
        free(temp->data);
        free(temp);
        window->count--;
    }

    Slidingwindow_node* newNode = (Slidingwindow_node*)malloc(sizeof(Slidingwindow_node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = element;
    newNode->next = NULL;
    if (window->front == NULL) {
        window->front = newNode;
        window->rear = newNode;
    } else {
        window->rear->next = newNode;
        window->rear = newNode;
    }
    window->count++;
}

void* slidingwindow_last(Slidingwindow* window) {
    if (window->rear != NULL) {
        return window->rear->data;
    }
    return NULL;
}

void slidingwindow_free(Slidingwindow* window) {
    Slidingwindow_node* current = window->front;
    while (current != NULL) {
        Slidingwindow_node* temp = current;
        current = current->next;
        free(temp->data);
        free(temp);
    }
    free(window);
}
