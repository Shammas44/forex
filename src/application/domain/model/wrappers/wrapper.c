#include "Wrapper.h"
#include "Hashmap.h"
#include <string.h>
#define T Wrapper

typedef void(__destructor_callback)(void*value);

typedef struct {
  const char *type;
  void *content;
} Private;

static void __destructor(T *self);
static const char* __get_type(T*self);
static Object __get_content(T*self);

T* wrapper_constructor(const char*type, void* object){
  T*self = malloc(sizeof(T));
  if(self == NULL) return NULL;
  self->destructor = __destructor;
  self->content = __get_content;
  self->type = __get_type;
  Private *private = malloc(sizeof(Private));
  private->type = type;
  private->content = object;
  self->__private = private;
  return self;
}

static void __destructor(T *self){
  Private*private = self->__private;
  // __destructor_callback *destructor = private->content;
  // destructor(private->content);
  free(private);
  free(self);
}

static const char* __get_type(T*self){
  Private*private = self->__private;
  return private->type;
}

static Object __get_content(T*self){
  Private*private = self->__private;
  return private->content;
}

#undef T
