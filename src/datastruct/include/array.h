#ifndef ARRAY_H
#define ARRAY_H
#include <stddef.h>
#define T Array

typedef struct T T;

typedef enum {
Array_types_default,
Array_types_hashmap,
Array_types_array,
Array_types_empty,
} Array_types;

typedef struct {
  Array_types type;
  void*value;
} Array_item;

typedef void (Array_push)(T *self, Array_item item);
typedef Array_item (Array_get)(T *self,size_t index);
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
