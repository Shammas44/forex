#include "hashmap.h"
#include <stdio.h>
#include <string.h>
#define T Hashmap
#define MAX_CAPACITY 10000
#define LOAD_FACTOR_THRESHOLD 0.7

static unsigned int __hash(const char *key, int capacity);
static void __resize(T *map);
static void *__get(T *map, const char *key, Hashmap_types** type);
static void __push_value(T *map, const char *key, void *value, Hashmap_types type);
static void __push(T *map, const char *key, void *value, Hashmap_types type);
static void __push_map(T *map, const char *key, T *value);
static void __push_array(T *map, const char *key, Array *value);

T *hashmap_constructor(size_t initial_capacity) {
  if (initial_capacity > MAX_CAPACITY)
    return NULL;
  if (initial_capacity < 1)
    return NULL;

  T *map = (T *)malloc(sizeof(T));
  map->destructor = hashmap_destructor;
  map->get = __get;
  map->push = __push;

  if (map != NULL) {
    map->entries =
        (Hashmap_entry *)malloc(sizeof(Hashmap_entry) * initial_capacity);
    if (map->entries != NULL) {
      memset(map->entries, 0, sizeof(Hashmap_entry) * initial_capacity);
      map->capacity = initial_capacity;
      map->size = 0;
      return map;
    }
    free(map);
  }
  return NULL;
}

void hashmap_destructor(T *map) {
    Array*array = NULL;
    if (map != NULL) {
        for (int i = 0; i < map->capacity; i++) {
          Hashmap_types type = map->entries[i].type;
          Hashmap_destructor_callback *destructor = map->entries[i].destructor;
          void*value = map->entries[i].value;
          if(destructor != NULL) destructor(value);
          else free(value); 
        }
        free(map->entries);
        free(map);
        map = NULL;
    }
}

static unsigned int __hash(const char *key, int capacity) {
  unsigned int hash = 0;
  for (int i = 0; key[i] != '\0'; i++) {
    hash = 31 * hash + key[i];
  }
  return hash % capacity;
}

static void __resize(T *map) {
  int new_capacity = map->capacity * 2;
  Hashmap_entry *new_entries =
      (Hashmap_entry *)malloc(sizeof(Hashmap_entry) * new_capacity);
  if (new_entries != NULL) {
    // Rehash all elements to the new array
    for (int i = 0; i < map->capacity; i++) {
      if (map->entries[i].key != NULL) {
        unsigned int index = __hash(map->entries[i].key, new_capacity);
        new_entries[index] = map->entries[i];
      }
    }
    free(map->entries);
    map->entries = new_entries;
    map->capacity = new_capacity;
  }
}

static void __push_value(T *map, const char *key, void *value, Hashmap_types type ){
  if ((double)(map->size + 1) / map->capacity > LOAD_FACTOR_THRESHOLD) {
    __resize(map);
  }

  unsigned int index = __hash(key, map->capacity);
  while (map->entries[index].key != NULL) {
    index =
        (index + 1) % map->capacity; // Linear probing for collision resolution
  }

  map->entries[index].key = strdup(key);
  map->entries[index].value = value;
  map->entries[index].type = type;
  map->entries[index].destructor = NULL;
  if(type != Hashmap_types_default) map->entries[index].destructor = value;
  map->size++;
}

static void __push(T *map, const char *key, void *value, Hashmap_types type){
  if(type <= 0) type = Hashmap_types_default;
  __push_value(map,key,value,type);
}

static void *__get(T *map, const char *key, Hashmap_types** type) {
  unsigned int index = __hash(key, map->capacity);
  while (map->entries[index].key != NULL) {
    if (strcmp(map->entries[index].key, key) == 0) {
      return map->entries[index].value;
    }
    index =
        (index + 1) % map->capacity; // Linear probing for collision resolution
  }
  return NULL; // Not found
}


#undef LOAD_FACTOR_THRESHOLD
#undef T
#undef MAX_CAPACITY
