#include "synchronizer.h"
#include <stdio.h>

Sync *sync_init(Sync_state state) {
    Sync* s = malloc(sizeof(Sync));
    s->state = state;
    pthread_mutex_init(&s->mutex, NULL);
    pthread_cond_init(&s->cond, NULL);
    return s;
}

void sync_wait_on_state(Sync *sync, Sync_state state){
  pthread_mutex_lock(&sync->mutex);
    while (sync->state != state) {
        pthread_cond_wait(&sync->cond, &sync->mutex);
    }
    pthread_mutex_unlock(&sync->mutex);
}

void sync_set_state(Sync *sync, Sync_state state) {
  pthread_mutex_lock(&sync->mutex);
  sync->state = state;
  pthread_cond_signal(&sync->cond);
  pthread_mutex_unlock(&sync->mutex);
}
