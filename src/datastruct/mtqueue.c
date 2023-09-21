#include "mtqueue.h"
#include <openssl/ssl.h>

void add_item(Mtqueue* q, void *data);
void *remove_item(Mtqueue* q);

Mtqueue* mtqueue_init(int max_messages) {
    Mtqueue* q = malloc(sizeof(Mtqueue));
    q->front = q->rear = NULL;
    q->count = 0;
    q->max_messages = max_messages;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->cond, NULL);
    return q;
}

void mtqueue_enqueue(Mtqueue* q, void *data) {
    pthread_mutex_lock(&q->mutex);
    while (q->count == q->max_messages) {
        pthread_cond_wait(&q->cond, &q->mutex);
    }
    add_item(q, data);
    pthread_mutex_unlock(&q->mutex);
    pthread_cond_signal(&q->cond);
}

void add_item(Mtqueue* q, void *data){
    Node* newNode = malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;

    if (!q->rear) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->count++;
}

void* remove_item(Mtqueue* q) {
  Node* temp = q->front;
  void *data = temp->data;
  q->front = q->front->next;
  if (!q->front) {
    q->rear = NULL;
  }
  q->count--;
  free(temp);
  return data;
}

void mtqueue_replace(Mtqueue* q, void *data) {
    pthread_mutex_lock(&q->mutex);

  if(q->count > 0){
  remove_item(q);
  }
    add_item(q, data);
    pthread_mutex_unlock(&q->mutex);
    pthread_cond_signal(&q->cond);
}

void *mtqueue_dequeue(Mtqueue* q) {
    pthread_mutex_lock(&q->mutex);
    while (q->count == 0) {
        pthread_cond_wait(&q->cond, &q->mutex);
    }
    void *data = remove_item(q);
    pthread_mutex_unlock(&q->mutex);
    pthread_cond_signal(&q->cond);
    return data;
}

void *mtqueue_dequeue_copy(Mtqueue* q) {
    pthread_mutex_lock(&q->mutex);
    while (q->count == 0) {
        pthread_cond_wait(&q->cond, &q->mutex);
    }
    void *data = q->front->data;
    pthread_mutex_unlock(&q->mutex);
    pthread_cond_signal(&q->cond);

    return data;
}

void mtqueue_free_list(Mtqueue_list *list) {
    // Free bars queue
    while (list->bars->front != NULL) {
        Node *temp = list->bars->front;
        list->bars->front = list->bars->front->next;
        free(temp->data);
        free(temp);
    }
    pthread_mutex_destroy(&list->bars->mutex);
    pthread_cond_destroy(&list->bars->cond);
    free(list->bars);

    while (list->ticks->front != NULL) {
        Node *temp = list->ticks->front;
        list->ticks->front = list->ticks->front->next;
        free(temp->data);
        free(temp);
    }

    pthread_mutex_destroy(&list->ticks->mutex);
    pthread_cond_destroy(&list->ticks->cond);
    free(list->ticks);

    while (list->orders->front != NULL) {
        Node *temp = list->orders->front;
        list->orders->front = list->orders->front->next;
        free(temp->data);
        free(temp);
    }

    pthread_mutex_destroy(&list->orders->mutex);
    pthread_cond_destroy(&list->orders->cond);
    free(list->orders);
    free(list->metadata);
    free(list->sync);
    SSL_free(list->ssl);
}

