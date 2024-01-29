#ifndef ARRAY_H
#define ARRAY_H
#include <stddef.h>
#include "item.h"

#define T Array

typedef struct T T;

typedef void (Array_push)(T *self, Item item);
typedef Item (Array_get)(T *self,size_t index);
typedef void (Array_destructor)(T *self);
typedef char* (Array_to_json)(T *self);
typedef size_t (Array_length)(T *self);
typedef size_t (Array_capacity)(T *self);

struct T {
  Array_destructor *destructor;
  Array_get *get;
  Array_push *push;
  Array_length *length;
  Array_capacity *capacity;
  Array_to_json *to_json;
  void *__private;
};

T *array_constructor(size_t initial_size);

void array_destructor(T* self);

#undef T
#endif
