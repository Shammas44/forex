#ifndef HTTPSRESPONSEBUILDER_H
#define HTTPSRESPONSEBUILDER_H
#include "httpsResponse.h"

#define T HttpsResponseBuilder 
typedef struct T T;

typedef T *(HttpsResponseBuilder_build)(T *builder, SSL*ssl);
typedef T *(HttpsResponseBuilder_set_body)(T *builder, char* body);
typedef T *(HttpsResponseBuilder_add_header)(T *builder, char* header);
typedef T *(HttpsResponseBuilder_set_status)(T *builder, char* status);
typedef HttpsResponse *(HttpsResponseBuilder_get)(T *builder);
typedef void (HttpsResponseBuilder_destructor)(T *builder);

struct T {
  HttpsResponseBuilder_build *build;
  HttpsResponseBuilder_set_body *set_body;
  HttpsResponseBuilder_add_header *add_header;
  HttpsResponseBuilder_set_status *set_status;
  HttpsResponseBuilder_destructor *destructor;
  HttpsResponseBuilder_get *get;
  void *__private;
};

T *httpsResponseBuilder_constructor();
#undef T
#endif 
