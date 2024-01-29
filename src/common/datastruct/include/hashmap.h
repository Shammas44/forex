#ifndef MAP_H
#define MAP_H
#include "array.h"
#include "isDestroyable.h"
#include "isIterable.h"
#include "item.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#define T Hashmap

typedef IsIterable T;

T *hashmap_constructor(size_t initial_capacity);
void hashmap_destructor(T *self);

//TODO: remove those functions
char* hashmap_get_string(T*self,const char*key);
int hashmap_get_int(T*self,const char*key);
bool hashmap_get_bool(T*self,const char*key);
double hashmap_get_double(T*self,const char*key);

#undef T
#endif
