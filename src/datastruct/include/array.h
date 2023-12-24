#ifndef ARRAY_H
#define ARRAY_H
#include <stddef.h>
#define T Array

typedef struct T T;

typedef void *(Array_pop)(T *array);
typedef void (Array_push)(T *array, void *);
typedef void *(Array_get)(T *array,size_t index);
typedef void (Array_destructor)(T *array);

struct T {
  Array_destructor *destructor;
  Array_get *get;
  Array_pop *pop;
  Array_push *push;
  size_t length;
  void *__private;
};

T *array_constructor(size_t initial_size);

void array_destructor(T* darray);

#undef T
#endif
