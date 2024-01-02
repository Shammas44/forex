#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H
#include <pthread.h>
#include <stdlib.h>

typedef enum Sync_state  {
  SYNC_STATE_BARS,
  SYNC_STATE_ORDERS,
  SYNC_STATE_EXCHANGE,
} Sync_state;

typedef struct Sync {
  Sync_state state;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} Sync;

void sync_wait_on_state(Sync *sync, Sync_state state);

void sync_set_state(Sync *sync, Sync_state state);

Sync *sync_init(Sync_state state);

#endif 
