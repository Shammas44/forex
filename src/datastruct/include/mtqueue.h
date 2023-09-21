#ifndef MTQUEUE_H
#define MTQUEUE_H
#include "tsmetadata.h"
#include "synchronizer.h"
#include <openssl/rand.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  void *data;
  struct Node *next;
} Node;

typedef struct Mtqueue {
  Node *front;
  Node *rear;
  int count;
  int max_messages;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} Mtqueue;

typedef struct Mtqueue_list {
  Mtqueue *bars;
  Mtqueue *ticks;
  Mtqueue *orders;
  SSL *ssl;
  Tsmetadata *metadata;
  Sync *sync;
} Mtqueue_list;

Mtqueue* mtqueue_init(int max_messages);

void mtqueue_enqueue(Mtqueue *q, void *data);

void *mtqueue_dequeue(Mtqueue *q);

void mtqueue_free_list(Mtqueue_list *list);

void *mtqueue_dequeue_copy(Mtqueue* q);

void mtqueue_replace(Mtqueue* q, void *data);

#endif
