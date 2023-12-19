#ifndef DARRAY_H
#define DARRAY_H
#include <stddef.h>
#define T Darray

typedef struct T T;

typedef void *(Darray_pop)(T *array);
typedef void (Darray_push)(T *array, void *);
typedef void *(Darray_get)(T *array,size_t index);
typedef void (Darray_destructor)(T *array);

struct T {
  Darray_destructor *destructor;
  Darray_get *get;
  Darray_pop *pop;
  Darray_push *push;
  size_t length;
  void *__private;
};

T *darray_constructor(size_t initial_size);
#undef T
#endif
