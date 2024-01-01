#ifndef MTSQUEUE_H
#define MTSQUEUE_H

#define T MtsQueue

typedef struct T T;

typedef void (MtsQueue_enqueue)(T *self, void *data);

typedef void (MtsQueue_destructor)(T *self);

typedef void *(MtsQueue_dequeue)(T *self);

// typedef void (MtsQueue_free_list)(MtsQueue_list *list);

typedef void *(MtsQueue_dequeue_copy)(T* self);

typedef void (MtsQueue_replace)(T* self, void *data);

typedef struct T {
  MtsQueue_destructor*destructor;
  MtsQueue_enqueue*enqueue;
  MtsQueue_dequeue*dequeue;
  void*__private;
} T;

T* mtsQueue_constructor(int max_messages);

#undef T
#endif
