#ifndef MAP_H
#define MAP_H
#include "array.h"
#include "isDestroyable.h"
#include "item.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#define T Hashmap

typedef struct T T;

typedef void(Hashmap_destructor_callback)(void*value);
typedef void(Hashmap_push)(T *self, const char *key, Item item);
typedef void(Hashmap_destructor)(T *self);
typedef Item(Hashmap_get)(T *self, const char *key);
typedef char *(Hashmap_to_json)(T *self);
typedef size_t (Hashmap_capacity)(T *self);
typedef size_t (Hashmap_length)(T *self);

typedef struct T {
  IsDestroyable __destructor;
  Hashmap_destructor *destructor;
  Hashmap_push *push;
  Hashmap_get *get;
  Hashmap_to_json *to_json;
  Hashmap_capacity *capacity;
  Hashmap_length *length;
  void *__private;
}T;

T *hashmap_constructor(size_t initial_capacity);
void hashmap_destructor(T *self);

//TODO: remove those functions
char* hashmap_get_string(T*self,const char*key);
int hashmap_get_int(T*self,const char*key);
bool hashmap_get_bool(T*self,const char*key);
double hashmap_get_double(T*self,const char*key);

#undef T
#endif
