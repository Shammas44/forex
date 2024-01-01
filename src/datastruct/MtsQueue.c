#include "MtsQueue.h"
#include <openssl/ssl.h>
#define T MtsQueue

typedef struct Node {
  void *data;
  struct Node *next;
} Node;

typedef struct {
  Node *front;
  Node *rear;
  int count;
  int max_messages;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} Private;

static void __add_item(T* self, void *data);
static void *__remove_item(T* self);
static void __enqueue(T* self, void *data);
static void *__dequeue(T* self);

T* mtsQueue_constructor(int max_messages) {
    T* self = malloc(sizeof(T));
    Private * private = malloc(sizeof(Private));
    private->front = private->rear = NULL;
    private->count = 0;
    private->max_messages = max_messages;
    pthread_mutex_init(&private->mutex, NULL);
    pthread_cond_init(&private->cond, NULL);
    self->__private = private;
    self->dequeue = __dequeue;
    self->enqueue = __enqueue;
    return self;
}

static void __enqueue(T* self, void *data) {
    Private * private = (Private*)self->__private;
    pthread_mutex_lock(&private->mutex);
    while (private->count == private->max_messages) {
        pthread_cond_wait(&private->cond, &private->mutex);
    }
    __add_item(self, data);
    pthread_mutex_unlock(&private->mutex);
    pthread_cond_signal(&private->cond);
}

static void __add_item(T* self, void *data){
    Private * private = (Private*)self->__private;
    Node* newNode = malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;

    if (!private->rear) {
        private->front = private->rear = newNode;
    } else {
        private->rear->next = newNode;
        private->rear = newNode;
    }
    private->count++;
}

static void* __remove_item(T* self) {
  Private * private = (Private*)self->__private;
  Node* temp = private->front;
  void *data = temp->data;
  private->front = private->front->next;
  if (!private->front) {
    private->rear = NULL;
  }
  private->count--;
  free(temp);
  return data;
}

static void __replace(T* self, void *data) {
    Private * private = (Private*)self->__private;
    pthread_mutex_lock(&private->mutex);

    if(private->count > 0){
      __remove_item(self);
    }
    __add_item(self, data);
    pthread_mutex_unlock(&private->mutex);
    pthread_cond_signal(&private->cond);
}

static void *__dequeue(T* self) {
    Private * private = (Private*)self->__private;
    pthread_mutex_lock(&private->mutex);
    while (private->count == 0) {
        pthread_cond_wait(&private->cond, &private->mutex);
    }
    void *data = __remove_item(self);
    pthread_mutex_unlock(&private->mutex);
    pthread_cond_signal(&private->cond);
    return data;
}

static void *__dequeue_copy(T* self) {
    Private * private = (Private*)self->__private;
    pthread_mutex_lock(&private->mutex);
    while (private->count == 0) {
        pthread_cond_wait(&private->cond, &private->mutex);
    }
    void *data = private->front->data;
    pthread_mutex_unlock(&private->mutex);
    pthread_cond_signal(&private->cond);

    return data;
}

// void mtsQueue_free_list(Mtqueue_list *list) {
//     // Free bars queue
//     while (list->bars->front != NULL) {
//         Node *temp = list->bars->front;
//         list->bars->front = list->bars->front->next;
//         free(temp->data);
//         free(temp);
//     }
//     pthread_mutex_destroy(&list->bars->mutex);
//     pthread_cond_destroy(&list->bars->cond);
//     free(list->bars);

//     while (list->ticks->front != NULL) {
//         Node *temp = list->ticks->front;
//         list->ticks->front = list->ticks->front->next;
//         free(temp->data);
//         free(temp);
//     }

//     pthread_mutex_destroy(&list->ticks->mutex);
//     pthread_cond_destroy(&list->ticks->cond);
//     free(list->ticks);

//     while (list->orders->front != NULL) {
//         Node *temp = list->orders->front;
//         list->orders->front = list->orders->front->next;
//         free(temp->data);
//         free(temp);
//     }

//     pthread_mutex_destroy(&list->orders->mutex);
//     pthread_cond_destroy(&list->orders->cond);
//     free(list->orders);
//     free(list->metadata);
//     free(list->sync);
//     SSL_free(list->ssl);
// }
#undef T
