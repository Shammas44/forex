#include "Array.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define T Array

typedef struct {
  Item **array;
  size_t length;
  size_t capacity;
} Private;

static Item __get(T *self, size_t index);
static void __push(T *self, Item item);
static char *__to_json(T *self);
static size_t __length(T *self);
static size_t __capacity(T *self);
static void __destructor(T *self);

T *array_constructor(size_t size) {
  T *self = malloc(sizeof(T));
  if(size <= 0){
    RUNTIME_ERROR("Array size should be greater than 0",1);
    return NULL;
  }
  self->length = 0;
  self->__private = malloc(sizeof(Private));
  Item **array = malloc(sizeof(Item *) * size);

  for (int i = 0; i < size; i++) {
    array[i] = NULL;
  }

  Private *p = self->__private;
  p->array = array;
  p->length = 0;
  p->capacity = size;
  self->get = __get;
  self->length = __length;
  self->capacity = __capacity;
  self->push = __push;
  self->to_json = __to_json;
  self->destructor = __destructor;
  return self;
}

static void __destructor(T *self) {
  if(self == NULL){
    return;
  }
  Private *p = self->__private;
  size_t capacity = p->capacity;
  size_t length = p->length;
  for (size_t i=0;i<length;i++) {
    Item *entry = p->array[i];
    Item_type type = entry->type;
    void * value = entry->value;
    Hashmap* map;
    Array* array;
    switch (type) {
      case Item_null:
      break;
      case Item_array:
        array = value;
        array->destructor(array);
      break;
      case Item_map:
        map = value;
        map->destructor(map);
      break;
      default: 
      free(value);
      break;
    
    }
    free(entry);
  }
  free(p->array);
  free(self->__private);
  free(self);
}

static size_t __length(T *self) {
  Private *p = self->__private;
  return p->length;
}

static size_t __capacity(T *self){
  Private *p = self->__private;
  return p->capacity;
}

char *__to_json(T *self) {
    return NULL;
    // if (self == NULL) {
    //     return NULL;
    // }

    // const size_t initialCapacity = 100000;
    // char* json = malloc(sizeof(char) * initialCapacity);
    // if (json == NULL) {
    //     return NULL;  // Memory allocation failure
    // }

    // strcpy(json, "[");

    // Private* p = self->__private;
    // size_t length = p->length;
    // IsIterable_Entry** entries = __entries(self);

    // for (size_t i = 0; i < length; i++) {
    //     char* key = entries[i]->key;
    //     char* value = NULL;
    //     char* tick = "\"";
    //     char* comma = (i == length - 1) ? "" : ",";

    //     switch (entries[i]->type) {
    //         case Item_string:
    //         case Item_default:
    //             value = entries[i]->value;
    //             break;

    //         case Item_double:
    //             value = _$convertToTrimmedString(*(double*)entries[i]->value);
    //             tick = "";
    //             break;

    //         case Item_int:
    //             value = malloc(sizeof(char) * 100);
    //             if (value != NULL) {
    //                 sprintf(value, "%d", *(int*)entries[i]->value);
    //                 tick = "";
    //             } else {
    //                 free(json);
    //                 return NULL;  // Memory allocation failure
    //             }
    //             break;

    //         case Item_bool:
    //             value = *(bool*)entries[i]->value ? "true" : "false";
    //             tick = "";
    //             break;

    //         case Item_null:
    //             value = "null";
    //             tick = "";
    //             break;

    //         case Item_map:
    //             value = __to_json(entries[i]->value);
    //             tick = "";
    //             break;

    //         default:
    //             // Handle unsupported type or error
    //             free(json);
    //             return NULL;
    //     }

    //     strcat(json, "\"");
    //     strcat(json, key);
    //     strcat(json, "\":");
    //     strcat(json, tick);
    //     strcat(json, value);
    //     strcat(json, tick);
    //     strcat(json, comma);

    //     // Free dynamically allocated memory for value (if any)
    //     if (entries[i]->type == Item_int) {
    //         free(value);
    //     } else if (entries[i]->type == Item_double || entries[i]->type == Item_map) {
    //         free(value);
    //     }
    // }

    // strcat(json, "]");
    // return json;
}

static Item __get(T *self, size_t index) {
  Private *p = self->__private;
  if (index >= p->length || index < 0) {
    RUNTIME_ERROR("index out of bounds", 1);
    return (Item){.type=Item_null,.value=NULL};
  }

  Item *entry = p->array[index];
  Item item = {.type=entry->type,.value=entry->value};
  return item;
}

static void __push(T *self, Item item) {
  Private *p = self->__private;
  Item *entry = malloc(sizeof(Item));
  entry->type = item.value ==NULL
  ? Item_null
  :item.type;
  entry->value = item.value;
  p->array[p->length] = entry;
  p->length++;
  if (p->length == p->capacity) {
     p->capacity = p->capacity * 2;
     p->array = realloc(p->array, sizeof(Item *) * p->capacity);
  }
}

// T* __array_clone(T* darray){
//   T* clone = array_constructor(darray->length);
//   for (int i = 0; i < darray->length; i++) {
//     void*value = darray->get(darray,i);
//     void*copy = malloc(sizeof(*value));
//     *copy = value;
//     clone->push(clone,darray->get(darray,i));
//   }
//   return clone;
// }

#undef T
