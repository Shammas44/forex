#ifndef ISITERABLE_H
#define ISITERABLE_H
#include "item.h"
#include "isDestroyable.h"
#include <stddef.h>
#define T IsIterable

typedef struct T T;

typedef void(IsIterable_destructor_callback)(void*value);
typedef void(IsIterable_destructor)(T *self);
typedef void(IsIterable_push)(T *self, const char *key, Item item);
typedef Item(IsIterable_get)(T *self, void *key);
typedef char *(IsIterable_to_json)(T *self);
typedef size_t(IsIterable_capacity)(T *self);
typedef size_t (IsIterable_length)(T *self);
typedef Item** (IsIterable_values)(T *self);
typedef char** (IsIterable_keys)(T *self);

typedef struct T {
  IsDestroyable __destructor;
  IsIterable_destructor *destructor;
  IsIterable_push *push;
  IsIterable_get *get;
  IsIterable_to_json *to_json;
  IsIterable_capacity *capacity;
  IsIterable_length *length;
  IsIterable_values *values;
  IsIterable_keys *keys;
  void *__private;
}T;

#undef T
#endif
