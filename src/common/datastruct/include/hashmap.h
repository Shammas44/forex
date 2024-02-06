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

#undef T
#endif
