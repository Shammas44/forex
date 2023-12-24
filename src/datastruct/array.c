#include "array.h"
#include <stdlib.h>
#define T Array

typedef struct {
  void**array;
}__private;

void* __array_get(T* darray, size_t index);
void __array_push(T* darray, void* element);

T* array_constructor(size_t size) {
  T* darray = malloc(sizeof(T));
  darray->length = 0;
  darray->__private = malloc(sizeof(__private));
  void **array = malloc(sizeof(void*) * size);
  __private *p = darray->__private;
  p->array = array;
  darray->get = __array_get;
  darray->push = __array_push;
  darray->pop = NULL;
  darray->destructor = array_destructor;
  return darray;
}

void array_destructor(T* darray) {
  __private *p = darray->__private;
  free(p->array);
  free(darray->__private);
  free(darray);
}

void* __array_get(T* darray, size_t index) {
  __private *p = darray->__private;
  if(index >= darray->length) return NULL;
  if(index < 0) return NULL;
  return p->array[index];
}

void __array_push(T* darray, void* element) {
  __private *p = darray->__private;
  p->array[darray->length] = element;
  darray->length++;
}

#undef T
