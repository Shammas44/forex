#ifndef HTTPSREQUESTBUILDER_H
#define HTTPSREQUESTBUILDER_H
#include "httpsRequest.h"

#define T HttpsRequestBuilder

typedef struct T T;

typedef T *(HttpsRequestBuilder_build)(T *builder, char* url);
typedef T *(HttpsRequestBuilder_set_body)(T *builder, char* body);
typedef T *(HttpsRequestBuilder_add_header)(T *builder, char* header);
typedef T *(HttpsRequestBuilder_set_method)(T *builder, HttpsRequest_method method);
typedef Request *(HttpsRequestBuilder_get)(T *builder);
typedef void (HttpsRequestBuilder_destructor)(T *builder);

struct T {
  HttpsRequestBuilder_destructor *destructor;
  HttpsRequestBuilder_build *build;
  HttpsRequestBuilder_set_body *set_body;
  HttpsRequestBuilder_add_header *add_header;
  HttpsRequestBuilder_set_method *set_method;
  HttpsRequestBuilder_get *get;
  void * __private;
};

T *httpsRequestBuilder_constructor();
#undef T
#endif 
