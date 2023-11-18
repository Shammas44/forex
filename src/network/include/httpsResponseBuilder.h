#ifndef HTTPSRESPONSEBUILDER_H
#define HTTPSRESPONSEBUILDER_H
#include "httpsResponse.h"

struct HttpsResponseBuilder;

typedef struct HttpsResponseBuilder *(HttpsResponseBuilder_build)(struct HttpsResponseBuilder *builder, SSL*ssl);
typedef struct HttpsResponseBuilder *(HttpsResponseBuilder_set_body)(struct HttpsResponseBuilder *builder, char* body);
typedef struct HttpsResponseBuilder *(HttpsResponseBuilder_add_header)(struct HttpsResponseBuilder *builder, char* header);
typedef struct HttpsResponseBuilder *(HttpsResponseBuilder_set_status)(struct HttpsResponseBuilder *builder, char* status);
typedef Response *(HttpsResponseBuilder_get)(struct HttpsResponseBuilder *builder);
typedef void (httpsResponseBuilder_destructor)(struct HttpsResponseBuilder *builder);

typedef struct {
  HttpsResponseBuilder_build *build_func;
  HttpsResponseBuilder_set_body *set_body_func;
  HttpsResponseBuilder_add_header *add_header_func;
  HttpsResponseBuilder_set_status *set_status_func;
  HttpsResponseBuilder_get *get_func;
  httpsResponseBuilder_destructor *destructor_func;
  void * __private;
} HttpsResponseBuilder;

HttpsResponseBuilder *httpsResponseBuilder_constructor();

#endif 
