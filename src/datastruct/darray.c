#include "darray.h"
#include <stdlib.h>
#define T Darray

typedef struct {
  void**array;
}__private;

void* __darray_get(T* darray, size_t index);
void __darray_push(T* darray, void* element);
void __darray_destructor(T* darray);

Darray* darray_constructor(size_t size) {
  T* darray = malloc(sizeof(T));
  darray->length = 0;
  darray->__private = malloc(sizeof(__private));
  void **array = malloc(sizeof(void*) * size);
  __private *p = darray->__private;
  p->array = array;
  darray->get = __darray_get;
  darray->push = __darray_push;
  darray->pop = NULL;
  darray->destructor = __darray_destructor;
  return darray;
}

void* __darray_get(T* darray, size_t index) {
  __private *p = darray->__private;
  if(index >= darray->length) return NULL;
  if(index < 0) return NULL;
  return p->array[index];
}

void __darray_push(T* darray, void* element) {
  __private *p = darray->__private;
  p->array[darray->length] = element;
  darray->length++;
}

void __darray_destructor(T* darray) {
  __private *p = darray->__private;
  free(p->array);
  free(darray->__private);
  free(darray);
}
#undef T
