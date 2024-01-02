#include "hashmap.h"
#include "common.h"
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
static char* __to_json(T *map);

T *hashmap_constructor(size_t initial_capacity) {
  if (initial_capacity > MAX_CAPACITY)
    return NULL;
  if (initial_capacity < 1)
    return NULL;

  T *map = (T *)malloc(sizeof(T));
  if(map == NULL){
    RUNTIME_ERROR("map is NULL",1);
    return NULL;
  } 
  map->destructor = hashmap_destructor;
  map->get = __get;
  map->push = __push;
  map->to_json = __to_json;
  initial_capacity = initial_capacity + (initial_capacity * LOAD_FACTOR_THRESHOLD);
  if(initial_capacity > MAX_CAPACITY) initial_capacity = MAX_CAPACITY;

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
          if(map->entries[i].key != NULL) {
            continue;
          }
          Hashmap_types type = map->entries[i].type;
          Hashmap_destructor_callback *destructor = map->entries[i].destructor;
          void*value = map->entries[i].value;
          //TODO: debug this
          if(destructor != NULL) destructor(value);
          else free(value); 
        }
        free(map->entries);
        free(map);
        map = NULL;
    }
}

static char* __to_json(Hashmap*map){
  if(map == NULL) return NULL;
  char*json = malloc(sizeof(char)*100000);
  strcpy(json,"{");
  for (int i = 0; i < map->capacity; i++) {
    char*key = map->entries[i].key;
    if(key != NULL) {
      char*value = NULL;
      Hashmap_types type = map->entries[i].type;
      if(type == Hashmap_types_array){
        Array*array = map->entries[i].value;
        value = array->to_json(array);
      }else if(type == Hashmap_types_hashmap){
        T*map_tmp = map->entries[i].value;
        value = map_tmp->to_json(map_tmp);
      }else{
        value = map->entries[i].value;
      }
      char*comma = ",";
      if(i == map->size-1) comma = "";
      strcat(json,"\"");
      strcat(json,key);
      strcat(json,"\":");
      strcat(json,value);
      strcat(json,comma);
      free(value);
    }
  }
  strcat(json,"}");
  return json;
}

char* hashmap_get_string(Hashmap*map,const char*key){
  if(key == NULL) return NULL;
  char* value = HASHMAP_GET_STRING(map, key);
  if(value == NULL) return NULL;
  size_t len = strlen(value);
  char* out = malloc(sizeof(char) * len +1);
  strcpy(out,value);
  return out;
}

int hashmap_get_int(Hashmap*map,const char*key){
  if(key == NULL) return -1;
  char*value = HASHMAP_GET_STRING(map, key);
  if(value == NULL) return -1;
  return atoi(value);
}

bool hashmap_get_bool(Hashmap*map,const char*key){
  if(key == NULL) return NULL;
  char*value = HASHMAP_GET_STRING(map, key);
  if(value == NULL) return NULL;
  bool out = NULL;
  if(strcmp(value,"true") == 0) out = true;
  if(strcmp(value,"false") == 0) out = false;
  free(value);
  return out;
}

double hashmap_get_double(Hashmap*map,const char*key){
  if(key == NULL) return -1;
  char*value = HASHMAP_GET_STRING(map, key);
  if(value == NULL) return -1;
  return atof(value);
}

//function to clone deeply a hashmap
T* __clone(T*map){
  T*clone = hashmap_constructor(map->capacity);
  if(clone == NULL) return NULL;
  for (int i = 0; i < map->capacity; i++) {
    char* key = map->entries[i].key;
    if(key != NULL) {
      Hashmap_types type = map->entries[i].type;
      if(type == Hashmap_types_array){
        // Array*array = map->entries[i].value;
        // Array*clone_array = array->clone(array);
        // clone->push(clone,array->key,clone_array,Hashmap_types_array);
      }else if(type == Hashmap_types_hashmap){
        T*map_tmp = map->entries[i].value;
        T*clone_map = __clone(map_tmp);
        clone->push(clone,key,clone_map,Hashmap_types_hashmap);
      }else{
        clone->push(clone,key,map->entries[i].value,type);
      }
    }
  }
  return clone;
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
    Hashmap_entry *new_entries = (Hashmap_entry *)malloc(sizeof(Hashmap_entry) * new_capacity);
    
    if (new_entries != NULL) {
        // Initialize new_entries to NULL key
        for (int i = 0; i < new_capacity; i++) {
            new_entries[i].key = NULL;
        }
        
        // Rehash and move elements to the new array
        for (int i = 0; i < map->capacity; i++) {
            if (map->entries[i].key != NULL) {
                unsigned int index = __hash(map->entries[i].key, new_capacity);
                
                // Linear probing for collision resolution
                while (new_entries[index].key != NULL) {
                    index = (index + 1) % new_capacity; 
                }
                
                // Copy the entry to the new array
                new_entries[index] = map->entries[i];
            }
        }
        
        // Free the old entries and update hashmap properties
        free(map->entries);
        map->entries = new_entries;
        map->capacity = new_capacity;
    }
}

static void __push_value(T *map, const char *key, void *value, Hashmap_types type ){
  bool isReplacement = false;
  unsigned int index = __hash(key, map->capacity);

  if(map->entries[index %map->capacity].key != NULL) {
    if(strcmp(map->entries[index %map->capacity].key,key) == 0){
      isReplacement = true;
      index = (index) % map->capacity; 
      goto push;
    }
  }

  while (map->entries[index].key != NULL ) {
    // Linear probing for collision resolution
    index = (index + 1) % map->capacity; 
    if(map->entries[index %map->capacity].key != NULL) {
      if(strcmp(map->entries[index].key,key) == 0){
        goto push;
      }
    }
  }

  map->entries[index].key = strdup(key);
push:
  map->entries[index].value = value;
  map->entries[index].type = type;
  map->entries[index].destructor = NULL;
  if(type != Hashmap_types_default) map->entries[index].destructor = value;

  if(!isReplacement){
    if ((double)(map->size + 1) / map->capacity > LOAD_FACTOR_THRESHOLD) {
      __resize(map);
    }
    map->size++;
  }
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
