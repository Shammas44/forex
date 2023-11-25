#include "hashmap.h"
#include <stdio.h>
#include <string.h>
#define T Hashmap
#define MAX_CAPACITY 10000
#define LOAD_FACTOR_THRESHOLD 0.7

static unsigned int __hashmap_hash(const char *key, int capacity);
static void __hashmap_resize(T *map);
static void __hashmap_push(T *map, const char *key, void *value);
static void __hashmap_destructor(T *map);
static void *__hashmap_get(T *map, const char *key);

T *hashmap_constructor(size_t initial_capacity) {
  if (initial_capacity > MAX_CAPACITY)
    return NULL;
  if (initial_capacity < 1)
    return NULL;

  T *map = (T *)malloc(sizeof(T));
  map->destructor = __hashmap_destructor;
  map->get = __hashmap_get;
  map->push = __hashmap_push;

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

static unsigned int __hashmap_hash(const char *key, int capacity) {
  unsigned int hash = 0;
  for (int i = 0; key[i] != '\0'; i++) {
    hash = 31 * hash + key[i];
  }
  return hash % capacity;
}

static void __hashmap_resize(T *map) {
  int new_capacity = map->capacity * 2;
  Hashmap_entry *new_entries =
      (Hashmap_entry *)malloc(sizeof(Hashmap_entry) * new_capacity);
  if (new_entries != NULL) {
    // Rehash all elements to the new array
    for (int i = 0; i < map->capacity; i++) {
      if (map->entries[i].key != NULL) {
        unsigned int index = __hashmap_hash(map->entries[i].key, new_capacity);
        new_entries[index] = map->entries[i];
      }
    }
    free(map->entries);
    map->entries = new_entries;
    map->capacity = new_capacity;
  }
}

static void __hashmap_push(T *map, const char *key, void *value) {
  if ((double)(map->size + 1) / map->capacity > LOAD_FACTOR_THRESHOLD) {
    __hashmap_resize(map);
  }

  unsigned int index = __hashmap_hash(key, map->capacity);
  while (map->entries[index].key != NULL) {
    index =
        (index + 1) % map->capacity; // Linear probing for collision resolution
  }

  map->entries[index].key = strdup(key);
  map->entries[index].value = value;
  map->size++;
}

static void *__hashmap_get(T *map, const char *key) {
  unsigned int index = __hashmap_hash(key, map->capacity);
  while (map->entries[index].key != NULL) {
    if (strcmp(map->entries[index].key, key) == 0) {
      return map->entries[index].value;
    }
    index =
        (index + 1) % map->capacity; // Linear probing for collision resolution
  }
  return NULL; // Not found
}

static void __hashmap_destructor(T *map) {
    if (map != NULL) {
        for (int i = 0; i < map->capacity; i++) {
            free(map->entries[i].key);
        }
        free(map->entries);
        free(map);
        map = NULL;
    }
}

#undef LOAD_FACTOR_THRESHOLD
#undef T
#undef MAX_CAPACITY
