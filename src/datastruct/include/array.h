#ifndef ARRAY_H
#define ARRAY_H
#include <stddef.h>
#define T Array

typedef struct T T;

typedef enum {
Array_types_default,
Array_types_hashmap,
Array_types_array,
} Array_types;

typedef void *(Array_pop)(T *array);
typedef void (Array_push)(T *array, void * value, Array_types type);
typedef void *(Array_get)(T *array,size_t index, Array_types **type);
typedef void (Array_destructor)(T *array);
typedef char* (Array_to_json)(T *array);

struct T {
  Array_destructor *destructor;
  Array_get *get;
  Array_pop *pop;
  Array_push *push;
  Array_to_json *to_json;
  size_t length;
  void *__private;
};

T *array_constructor(size_t initial_size);

void array_destructor(T* darray);

#undef T
#endif
