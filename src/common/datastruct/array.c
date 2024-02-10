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
static Item** __values(T *self);
static char** __keys(T *self);
static Array_Entry** __entries(T *self);

static char* _$convertToTrimmedString(double num);

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
  self->__destructor = (IsDestroyable){.destructor=__destructor};
  self->destructor = __destructor;
  self->get = __get;
  self->length = __length;
  self->capacity = __capacity;
  self->push = __push;
  self->to_json = __to_json;
  self->keys = __keys;
  self->entries = __entries;
  self->values = __values;
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

static char* __to_json(T* self) {
    if (self == NULL) {
        return NULL;
    }

    const size_t initialCapacity = 100000;
    char* json = malloc(sizeof(char) * initialCapacity);
    if (json == NULL) {
        return NULL;  // Memory allocation failure
    }

    strcpy(json, "[");

    Private* p = self->__private;
    size_t length = p->length;
    Item** values = __values(self);

    for (size_t i = 0; i < length; i++) {
        char* value = NULL;
        char* tick = "\"";
        char* comma = (i == length - 1) ? "" : ",";
        Hashmap*map;

        switch (values[i]->type) {
            case Item_string:
            case Item_default:
                value = values[i]->value;
                break;

            case Item_double:
                value = _$convertToTrimmedString(*(double*)values[i]->value);
                tick = "";
                break;

            case Item_int:
                value = malloc(sizeof(char) * 100);
                if (value != NULL) {
                    sprintf(value, "%d", *(int*)values[i]->value);
                    tick = "";
                } else {
                    free(json);
                    return NULL;  // Memory allocation failure
                }
                break;

            case Item_bool:
                value = *(bool*)values[i]->value ? "true" : "false";
                tick = "";
                break;

            case Item_null:
                value = "null";
                tick = "";
                break;

            case Item_array:
                value = __to_json(values[i]->value);
                tick = "";
                break;
            case Item_map:
                map = values[i]->value;
                value = map->to_json(map);
                tick = "";
                break;

            default:
                // Handle unsupported type or error
                free(json);
                return NULL;
        }

        strcat(json, tick);
        strcat(json, value);
        strcat(json, tick);
        strcat(json, comma);

        // Free dynamically allocated memory for value (if any)
        if (values[i]->type == Item_int) {
            free(value);
        } else if (values[i]->type == Item_double || values[i]->type == Item_map) {
            free(value);
        }
    }

    strcat(json, "]");
    return json;
}

static char* _$convertToTrimmedString(double num) {
    char buffer[50];  // Adjust the size based on your needs
    sprintf(buffer, "%.15f", num);  // Use a precision that fits your requirements

    // Remove trailing zeroes
    int len = strlen(buffer);
    while (len > 1 && buffer[len - 1] == '0') {
        buffer[--len] = '\0';
    }

    // Remove trailing decimal point, if present
    if (buffer[len - 1] == '.') {
        buffer[--len] = '\0';
    }

    // Allocate memory for the final string and copy the result
    char *result = strdup(buffer);

    return result;
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

static Item** __values(T *self){
  Private *p = self->__private;
  Item **array = p->array;
  size_t length = p->length;
  size_t capacity = p->capacity;
  if(length == 0) return NULL;
  Item **output = malloc(sizeof(Item *) * length);
  if (output == NULL) return NULL;

  for (int i=0; i < length; i++) {
    Item item = __get(self, i); 
    output[i] = malloc(sizeof(Item));
    output[i]->value = item.value;
    output[i]->type = item.type;
  }
  return output;
}

static char** __keys(T *self){
  Private *p = self->__private;
  Item **array = p->array;
  size_t length = p->length;
  size_t capacity = p->capacity;
  char **output = malloc(sizeof(char *) * length);
  size_t j = 0;
  if (output == NULL) return NULL;

  for (int i = 0; i < capacity; i++) {
      output[j] = malloc(sizeof(char*) * 16); 
      if (output[j] != NULL) {
        sprintf(output[j],"%d",i);
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
  return output;
}

static Array_Entry** __entries(T *self){
  Private *p = self->__private;
  Item **array = p->array;
  size_t length = p->length;
  size_t capacity = p->capacity;
  Array_Entry **output = malloc(sizeof(Array_Entry *) * length);
  size_t j = 0;
  if (output == NULL) return NULL;

  for (int i=0; i < length; i++) {
    char*key = malloc(sizeof(char*) * 16);
    sprintf(key,"%d",i);
    Item item = __get(self, i); 
    output[i] = malloc(sizeof(Hashmap_Entry));
    output[i]->key = key;
    output[i]->value = item.value;
    output[i]->type = item.type;
  }
  return output;
}

#undef T
