#ifndef MAP_H
#define MAP_H
#include "Array.h"
#include "IsDestroyable.h"
#include "item.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#define T Hashmap

typedef struct T T;

typedef struct {
  char *key;
  Item_type type;
  void*value;
} Hashmap_Entry;

typedef void(Hashmap_destructor)(T *self);
typedef void(Hashmap_destructor_callback)(void*value);

typedef struct T {
 IsDestroyable __destructor;
 Hashmap_destructor *destructor;
 void(*push)(T *self, Hashmap_Entry entry);
 Item(*get)(T *self, char*key);
 char *(*to_json)(T *self);
 size_t(*capacity)(T *self);
 size_t (*length)(T *self);
 Item** (*values)(T *self);
 char** (*keys)(T *self);
 Hashmap_Entry** (*entries)(T *self);
 void *__private;
}T;

T *hashmap_constructor(size_t initial_capacity);
void hashmap_destructor(T *self);

#undef T
#endif
