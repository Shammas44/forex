#include "queue.h"

// Function to initialize the queue
Queue* queue_init() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

// Function to enqueue an item
void queue_enqueue(Queue* q, void *data) {
    Queue_node* newNode = (Queue_node*)malloc(sizeof(Queue_node));
    newNode->data = data;
    newNode->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }

    // Add the new node at the end of the queue and update rear
    q->rear->next = newNode;
    q->rear = newNode;
}

// Function to dequeue an item
void* queue_dequeue(Queue* q) {
    if (q->front == NULL) {
        printf("Queue is empty!\n");
        return NULL; // Indicate an error or empty queue
    }

    Queue_node* temp = q->front;
    void *data = temp->data;

    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    return data;
}

// Function to check if the queue is empty
int queue_is_empty(Queue* q) {
    return (q->front == NULL);
}

// int main() {
//     Queue* q = initQueue();

//     enqueue(q, 10);
//     enqueue(q, 20);
//     enqueue(q, 30);

//     printf("Dequeued: %d\n", dequeue(q));
//     printf("Dequeued: %d\n", dequeue(q));

//     enqueue(q, 40);
//     enqueue(q, 50);

//     printf("Dequeued: %d\n", dequeue(q));

//     // Clean up the queue at the end of your program if needed
//     while (!isEmpty(q)) {
//         dequeue(q);
//     }
//     free(q);

//     return 0;
// }

