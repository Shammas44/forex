#include "hashmap.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#define T Hashmap
#define MAX_CAPACITY 10000
#define LOAD_FACTOR_THRESHOLD 0.7

typedef struct {
  char *key;
  void *value;
  Item_type type;
  bool is_destroyable;
} Entry;

typedef struct {
  Entry *entries;
  size_t capacity;
  size_t length;
} Private;

static unsigned int __hash(const char *key, size_t capacity);
static void __resize(T *self);
static size_t __capacity(T *self);
static size_t __length(T *self);
static Item __get(T *self, void *key);
static void __push(T *self, const char *key, Item item);
static char *__to_json(T *self);
static char **__keys(T *self);
static Item **__values(T *self);
static IsIterable_Entry **__entries(T *self);

static int _$compareStrings(const void *a, const void *b);

T *hashmap_constructor(size_t initial_capacity) {
  if (initial_capacity > MAX_CAPACITY)
    return NULL;
  if (initial_capacity < 1)
    return NULL;

  T *self = (T *)malloc(sizeof(T));
  if (self == NULL) {
    RUNTIME_ERROR("map is NULL", 1);
    return NULL;
  }
  Private *private = (Private *)malloc(sizeof(Private));
  self->__private = private;
  self->length = __length;
  self->capacity = __capacity;
  self->destructor = hashmap_destructor;
  self->get = __get;
  self->push = __push;
  self->to_json = __to_json;
  self->keys = __keys;
  self->values = __values;
  self->entries = __entries;
  self->__destructor = (IsDestroyable){.destructor = hashmap_destructor};
  initial_capacity =
      initial_capacity + (initial_capacity * LOAD_FACTOR_THRESHOLD);
  if (initial_capacity > MAX_CAPACITY)
    initial_capacity = MAX_CAPACITY;

  if (self != NULL) {
    private->entries = (Entry *)malloc(sizeof(Entry) * initial_capacity);
    if (private->entries != NULL) {
      memset(private->entries, 0, sizeof(Entry) * initial_capacity);
      private->capacity = initial_capacity;
      private->length = 0;
      return self;
    }
    free(self);
  }
  return NULL;
}

void hashmap_destructor(T *self) {
  if (self != NULL) {
    Private *private = self->__private;
    Entry *entries = private->entries;

    for (int i = 0; i < private->capacity; i++) {
      if (entries[i].key == NULL) {
        continue;
      }
      Entry entry = entries[i];
      Item_type type = entry.type;
      bool is_destroyable = entry.is_destroyable;
      void *value = entry.value;
      if (is_destroyable) {
        IsDestroyable *element = (IsDestroyable *)value;
        void *callback = element->destructor;
        IsIterable_destructor *destructor = (IsIterable_destructor *)callback;
        destructor(value);
      } else {
        free(value);
      }
    }
    free(entries);
    free(private);
    free(self);
  }
}

static size_t __length(T *self) {
  Private *p = self->__private;
  return p->length;
}

static size_t __capacity(T *self) {
  Private *p = self->__private;
  return p->capacity;
}

static char **__keys(T *self) {
  Private *p = self->__private;
  Entry *entries = p->entries;
  size_t length = p->length;
  size_t capacity = p->capacity;
  char **output = malloc(sizeof(char *) * length);
  size_t j = 0;
  if (output == NULL)
    return NULL;

  for (int i = 0; i < capacity; i++) {
    if (entries[i].key != NULL) {
      output[j] = malloc(strlen(entries[i].key) + 1);

      if (output[j] != NULL) {
        strcpy(output[j], entries[i].key);
        j++;
      } else {
        // Handle memory allocation failure
        // Free previously allocated memory before returning NULL
        for (int k = 0; k < j; k++) {
          free(output[k]);
        }
        free(output);
        return NULL;
      }
    }
  }

  qsort(output, length, sizeof(char*), _$compareStrings);
  return output;
}

static Item **__values(T *self) {
  Private *p = self->__private;
  Entry *entries = p->entries;
  size_t length = p->length;
  size_t capacity = p->capacity;
  if(length == 0) return NULL;
  Item **output = malloc(sizeof(Item *) * length);
  if (output == NULL) return NULL;

  char**keys = __keys(self);
  for (int i=0; i < length; i++) {
    Item item = __get(self, keys[i]); 
    output[i] = malloc(sizeof(Item));
    output[i]->value = item.value;
    output[i]->type = item.type;
  }
  return output;
}

static IsIterable_Entry **__entries(T *self){
  Private *p = self->__private;
  Entry *entries = p->entries;
  size_t length = p->length;
  size_t capacity = p->capacity;
  if(length == 0) return NULL;
  IsIterable_Entry **output = malloc(sizeof(IsIterable_Entry *) * length);
  if (output == NULL) return NULL;

  char**keys = __keys(self);
  for (int i=0; i < length; i++) {
    char*key = keys[i];
    Item item = __get(self, key); 
    output[i] = malloc(sizeof(IsIterable_Entry));
    output[i]->key = key;
    output[i]->value = item.value;
    output[i]->type = item.type;
  }
  return output;
}

static char *__to_json(Hashmap *map) {
    if(map == NULL) return NULL;
    // char*json = malloc(sizeof(char)*100000);
    // strcpy(json,"{");
    // for (int i = 0; i < map->capacity; i++) {
    //   char*key = map->entries[i].key;
    //   if(key != NULL) {
    //     char*value = NULL;
    //     Hashmap_types type = map->entries[i].type;
    //     if(type == Hashmap_types_array){
    //       Array*array = map->entries[i].value;
    //       value = array->to_json(array);
    //     }else if(type == Hashmap_types_hashmap){
    //       T*map_tmp = map->entries[i].value;
    //       value = map_tmp->to_json(map_tmp);
    //     }else{
    //       value = map->entries[i].value;
    //     }
    //     char*comma = ",";
    //     if(i == map->size-1) comma = "";
    //     strcat(json,"\"");
    //     strcat(json,key);
    //     strcat(json,"\":");
    //     strcat(json,value);
    //     strcat(json,comma);
    //     free(value);
    //   }
    // }
    // strcat(json,"}");
    // return json;
  return NULL;
}

// //function to clone deeply a hashmap
// T* __clone(T*map){
//   T*clone = hashmap_constructor(map->capacity);
//   if(clone == NULL) return NULL;
//   for (int i = 0; i < map->capacity; i++) {
//     char* key = map->entries[i].key;
//     if(key != NULL) {
//       Hashmap_types type = map->entries[i].type;
//       if(type == Hashmap_types_array){
//         // Array*array = map->entries[i].value;
//         // Array*clone_array = array->clone(array);
//         // clone->push(clone,array->key,clone_array,Hashmap_types_array);
//       }else if(type == Hashmap_types_hashmap){
//         T*map_tmp = map->entries[i].value;
//         T*clone_map = __clone(map_tmp);
//         clone->push(clone,key,clone_map,Hashmap_types_hashmap);
//       }else{
//         clone->push(clone,key,map->entries[i].value,type);
//       }
//     }
//   }
//   return clone;
// }

static unsigned int __hash(const char *key, size_t capacity) {
  unsigned int hash = 0;
  for (int i = 0; key[i] != '\0'; i++) {
    hash = 31 * hash + key[i];
  }
  return hash % capacity;
}

static void __resize(T *self) {
  Private *private = self->__private;
  int new_capacity = private->capacity * 2;
  Entry *entries = private->entries;
  Entry *new_entries = (Entry *)malloc(sizeof(Entry) * new_capacity);

  if (new_entries != NULL) {
    // Initialize new_entries to NULL key
    for (int i = 0; i < new_capacity; i++) {
      new_entries[i].key = NULL;
    }

    // Rehash and move elements to the new array
    for (int i = 0; i < private->capacity; i++) {
      if (entries[i].key != NULL) {
        unsigned int index = __hash(entries[i].key, new_capacity);
        // Linear probing for collision resolution
        while (new_entries[index].key != NULL) {
          index = (index + 1) % new_capacity;
        }
        // Copy the entry to the new array
        new_entries[index] = entries[i];
      }
    }

    // Free the old entries and update hashmap properties
    free(entries);
    private->entries = new_entries;
    private->capacity = new_capacity;
  }
}

static void __push(T *self, const char *key, Item item) {
  Private *private = self->__private;
  size_t *capacity = &private->capacity;
  size_t *length = &private->length;
  Entry *entries = private->entries;
  Item_type type = item.type;
  void *value = item.value;

  unsigned int index = __hash(key, *capacity);

  bool isReplacement = false;
  if (entries[index % *capacity].key != NULL) {
    if (strcmp(entries[index % *capacity].key, key) == 0) {
      isReplacement = true;
      index = index % *capacity;
    } else {
      while (entries[index].key != NULL) {
        index = (index + 1) % *capacity;
        if (entries[index % *capacity].key != NULL &&
            strcmp(entries[index].key, key) == 0) {
          isReplacement = true;
          break;
        }
      }
    }
  }

  if (!isReplacement) {
    while (entries[index].key != NULL) {
      index = (index + 1) % *capacity;
    }
  }

  entries[index].key = strdup(key);
  entries[index].value = value;
  entries[index].type = type;
  entries[index].is_destroyable = (type != Item_default) ? true : false;

  if (!isReplacement) {
    *length += 1;
    if ((double)(*length) / *capacity > LOAD_FACTOR_THRESHOLD) {
      __resize(self);
    }
  }
}

static Item __get(T *self, void *key) {
  Private *private = self->__private;
  Entry *entries = private->entries;
  unsigned int index = __hash(key, private->capacity);
  Entry entry;
  while (entries[index].key != NULL) {
    if (strcmp(entries[index].key, key) == 0) {
      entry = entries[index];
      return (Item){.type = entry.type, .value = entry.value};
    }
    // Linear probing for collision resolution
    index = (index + 1) % private->capacity;
  }
  return (Item){.type = Item_null, .value = NULL};
}

static int _$compareStrings(const void *a, const void *b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

#undef LOAD_FACTOR_THRESHOLD
#undef T
#undef MAX_CAPACITY
