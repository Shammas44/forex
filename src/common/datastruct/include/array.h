#ifndef ARRAY_H
#define ARRAY_H
#include <stddef.h>
#include "item.h"
#include "IsDestroyable.h"

#define T Array

typedef struct T T;

typedef struct {
  char *key;
  Item_type type;
  void*value;
} Array_Entry;

typedef void(Array_destructor)(T *self);
typedef void(Array_destructor_callback)(void*value);

struct T {
 IsDestroyable __destructor;
 Array_destructor *destructor;
 void(*push)(T *self, Item item);
 Item(*get)(T *self, size_t index);
 char *(*to_json)(T *self);
 size_t(*capacity)(T *self);
 size_t (*length)(T *self);
 Item** (*values)(T *self);
 char** (*keys)(T *self);
 Array_Entry** (*entries)(T *self);
 void *__private;
};

T *array_constructor(size_t initial_size);
void array_destructor(T* self);

#undef T
#endif
