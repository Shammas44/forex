#include "array.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#define T Array

typedef struct {
  void**array;
}__private;

typedef struct {
  Array_types type; 
  void* value;
} Entry;

static void* __get(T* self, size_t index, Array_types **type);
static void __push(T* self, void* element, Array_types type);
static char* __to_json(T* self);
static void __destructor(T* self);

T* array_constructor(size_t size) {
  T* self = malloc(sizeof(T));
  self->length = 0;
  self->__private = malloc(sizeof(__private));
  void **array = malloc(sizeof(void*) * size);
  __private *p = self->__private;
  p->array = array;
  self->get = __get;
  self->push = __push;
  self->to_json = __to_json;
  self->pop = NULL;
  self->destructor = __destructor;
  return self;
}

static void __destructor(T* self) {
  __private *p = self->__private;
  free(p->array);
  free(self->__private);
  free(self);
}

char* __to_json(T* array){
  __private *p = array->__private;
  char *json = malloc(sizeof(char) * 100);
  sprintf(json, "[");
  for (int i = 0; i < array->length; i++) {
    Array_types *type = NULL;
    void*value = array->get(array,i, &type);
    char*value_json = NULL;
    switch(*type){
      case Array_types_default:
        value_json = value;
        break;
      case Array_types_hashmap:
        value_json = ((Hashmap*)value)->to_json(value);
        break;
      case Array_types_array:
        value_json = ((Array*)value)->to_json(value);
        break;
    }
    sprintf(json, "%s%s", json, value_json);
    if(i < array->length - 1) sprintf(json, "%s,", json);
  }
  sprintf(json, "%s]", json);
  return json;
}

static void* __get(T* self, size_t index, Array_types **type) {
  __private *p = self->__private;
  if(index >= self->length || index < 0){
    RUNTIME_ERROR("index out of bounds",1);
    return NULL;
  } 
  Entry *entry = p->array[index];
  *type = &entry->type;
  return entry->value;
}

static void __push(T* self, void* element, Array_types type) {
  __private *p = self->__private;
  Entry *entry = malloc(sizeof(Entry));
  entry->type = type;
  entry->value = element;
  p->array[self->length] = entry;
  self->length++;
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
