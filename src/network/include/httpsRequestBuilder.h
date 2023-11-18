#ifndef HTTPSREQUESTBUILDER_H
#define HTTPSREQUESTBUILDER_H
#include "httpsRequest.h"
#include "url.h"

struct HttpsRequestBuilder;

typedef struct HttpsRequestBuilder *(HttpsRequestBuilder_build)(struct HttpsRequestBuilder *builder, char* url);
typedef struct HttpsRequestBuilder *(HttpsRequestBuilder_set_body)(struct HttpsRequestBuilder *builder, char* body);
typedef struct HttpsRequestBuilder *(HttpsRequestBuilder_add_header)(struct HttpsRequestBuilder *builder, char* header);
typedef struct HttpsRequestBuilder *(HttpsRequestBuilder_set_method)(struct HttpsRequestBuilder *builder, HttpsRequest_method method);
typedef Request *(HttpsRequestBuilder_get)(struct HttpsRequestBuilder *builder);
typedef void (HttpsRequestBuilder_destructor)(struct HttpsRequestBuilder *builder);

typedef struct {
  HttpsRequestBuilder_destructor *destructor;
  HttpsRequestBuilder_build *build;
  HttpsRequestBuilder_set_body *set_body;
  HttpsRequestBuilder_add_header *add_header;
  HttpsRequestBuilder_set_method *set_method;
  HttpsRequestBuilder_get *get;
  void * __private;
} HttpsRequestBuilder;

HttpsRequestBuilder *httpsRequestBuilder_constructor();

#endif 
