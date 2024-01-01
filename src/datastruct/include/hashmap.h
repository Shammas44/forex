#ifndef MAP_H
#define MAP_H
#include "array.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#define T Hashmap
#define HASHMAP_GET_STRING(map,key) \
({ \
  Hashmap_types* _type; \
  char* _value = (char*)map->get(map,key,&_type); \
  _value; \
})

#define HASHMAP_GET_OBJECT(map,key) \
({ \
  Hashmap_types* _type; \
  Hashmap* _value = (Hashmap*)map->get(map,key,&_type); \
  _value; \
})

#define HASHMAP_GET_ARRAY(map,key) \
({ \
  Hashmap_types* _type; \
  Array* _value = (Array*)map->get(map,key,&_type); \
  _value; \
})

typedef struct T T;

typedef enum {
Hashmap_types_default,
Hashmap_types_hashmap,
Hashmap_types_array,
} Hashmap_types;

typedef void(Hashmap_destructor_callback)(void*value);
typedef void(Hashmap_push)(T *map, const char *key, void *value, Hashmap_types type);
typedef void(Hashmap_destructor)(T *map);
typedef void *(Hashmap_get)(T *map, const char *key, Hashmap_types**type);

typedef struct {
  char *key;
  void *value;
  Hashmap_types type;
  Hashmap_destructor_callback *destructor;
} Hashmap_entry;

struct T {
  Hashmap_destructor *destructor;
  Hashmap_push *push;
  Hashmap_get *get;
  Hashmap_entry *entries;
  int capacity;
  int size;
};

T *hashmap_constructor(size_t initial_capacity);
void hashmap_destructor(T *map);
char* hashmap_get_string(Hashmap*map,const char*key);
int hashmap_get_int(Hashmap*map,const char*key);
bool hashmap_get_bool(Hashmap*map,const char*key);
double hashmap_get_double(Hashmap*map,const char*key);

#undef T
#endif
