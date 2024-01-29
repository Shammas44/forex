#ifndef WRAPPER_H
#define WRAPPER_H
#include "hashmap.h"

#define T Wrapper

typedef struct T T;
typedef void* Object;

typedef void (Wrapper_destructor)(T*self);
typedef const char* (Wrapper_get_type)(T*self);
typedef Object (Wrapper_get_content)(T*self);

typedef struct T {
  Wrapper_destructor *destructor;
  Wrapper_get_type *type;
  Wrapper_get_content *content;
  void* __private;
} T;

T* wrapper_constructor(const char*type,Object object);

#undef T
#endif
