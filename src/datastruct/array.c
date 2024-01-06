#include "array.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#define T Array

typedef Array_item Entry;

typedef struct {
  Entry **array;
  size_t length;
  size_t capacity;
} Private;

static Array_item __get(T *self, size_t index);
static void __push(T *self, Array_item item);
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
  Entry **array = malloc(sizeof(Entry *) * size);

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
    Entry *entry = p->array[i];
    Array_types type = entry->type;
    void * value = entry->value;
    Hashmap* map;
    Array* array;
    switch (type) {
      case Array_types_empty:
      break;
      case Array_types_array:
        array = value;
        array->destructor(array);
      break;
      case Array_types_hashmap:
        map = value;
        map->destructor(map);
      break;
      case Array_types_default:
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

char *__to_json(T *array) {
//   Private *p = array->__private;
//   char *json = malloc(sizeof(char) * 100);
//   sprintf(json, "[");
//   for (int i = 0; i < p->length; i++) {
//     Array_types *type = NULL;
//     void *value = array->get(array, i, type);
//     char *value_json = NULL;
//     switch (*type) {
//     case Array_types_default:
//       value_json = value;
//       break;
//     case Array_types_hashmap:
//       value_json = ((Hashmap *)value)->to_json(value);
//       break;
//     case Array_types_array:
//       value_json = ((Array *)value)->to_json(value);
//       break;
//     }
//     sprintf(json, "%s%s", json, value_json);
//     if (i < p->length - 1)
//       sprintf(json, "%s,", json);
//   }
//   sprintf(json, "%s]", json);
//   return json;
  return NULL;
}

static Array_item __get(T *self, size_t index) {
  Private *p = self->__private;
  if (index >= p->length || index < 0) {
    RUNTIME_ERROR("index out of bounds", 1);
    return (Array_item){.type=Array_types_empty,.value=NULL};
  }

  Entry *entry = p->array[index];
  Array_item item = {.type=entry->type,.value=entry->value};
  return item;
}

static void __push(T *self, Array_item item) {
  Private *p = self->__private;
  Entry *entry = malloc(sizeof(Entry));
  entry->type = item.value ==NULL
  ? Array_types_empty
  :item.type;
  entry->value = item.value;
  p->array[p->length] = entry;
  p->length++;
  if (p->length == p->capacity) {
     p->capacity = p->capacity * 2;
     p->array = realloc(p->array, sizeof(Entry *) * p->capacity);
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
