#ifndef ISITERABLE_H
#define ISITERABLE_H
#include "item.h"
#include "isDestroyable.h"
#include <stddef.h>
#define T IsIterable

typedef struct T T;

typedef struct {
  char *key;
  Item_type type;
  void*value;
} IsIterable_Entry;

typedef void(IsIterable_destructor)(T *self);
typedef void(IsIterable_destructor_callback)(void*value);

typedef struct T {
 IsDestroyable __destructor;
 IsIterable_destructor *destructor;
 void(*push)(T *self, const char *key, Item item);
 Item(*get)(T *self, void *key);
 char *(*to_json)(T *self);
 size_t(*capacity)(T *self);
 size_t (*length)(T *self);
 Item** (*values)(T *self);
 char** (*keys)(T *self);
 IsIterable_Entry** (*entries)(T *self);
 void *__private;
}T;

#undef T
#endif
