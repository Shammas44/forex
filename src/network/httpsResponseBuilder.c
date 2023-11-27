#include "httpsResponseBuilder.h"
#include "httpsResponse.h"
#include <stdlib.h>
#define T HttpsResponseBuilder
#define GET_HTTPS_RESPONSE_FROM_RESPONSE(builder) \
({ \
  Response *_response = builder->__private; \
  HttpsResponse *_https_response = (HttpsResponse*)_response->__private; \
  _https_response; \
})


 T *__httpsResponseBuilder_build(T *builder, SSL*ssl);
 T *__httpsResponseBuilder_set_body(T *builder, char* body);
 T *__httpsResponseBuilder_set_status(T *builder, char* status);
 T *__httpsResponseBuilder_add_header(T *builder, char* header);
void __httpsResponseBuilder_destructor(T *builder);
Response *__httpsResponseBuilder_get(T *builder);

T *httpsResponseBuilder_constructor(){
  T *builder = malloc(sizeof(T));
  builder->__private = malloc(sizeof(Response));
  builder->build = __httpsResponseBuilder_build;
  builder->set_body = __httpsResponseBuilder_set_body;
  builder->add_header = __httpsResponseBuilder_add_header;
  builder->set_status = __httpsResponseBuilder_set_status;
  builder->destructor = __httpsResponseBuilder_destructor;
  builder->get = __httpsResponseBuilder_get;
  return builder;
}

void __httpsResponseBuilder_destructor(T *builder){
  Response *response = builder->__private;
  if(response != NULL){
    response->destructor(response);
  }
  free(builder);
}

 T *__httpsResponseBuilder_build(T *builder, SSL*ssl){
  builder->__private = httpsResponse_constructor(ssl);
  return builder;
}

T *__httpsResponseBuilder_set_body(T *builder, char* body){
  Response * response = builder->__private;
  HttpsResponse *https_response = (HttpsResponse *)response->__private;
  https_response->set_body(response, body);
  return builder;
}

 T *__httpsResponseBuilder_add_header(T *builder, char* header){
  Response * response = builder->__private;
  HttpsResponse *https_response = (HttpsResponse *)response->__private;
  https_response->add_header(response, header);
  return builder;
}

 T *__httpsResponseBuilder_set_status(T *builder, char* status){
  Response * response = builder->__private;
  HttpsResponse *https_response = (HttpsResponse *)response->__private;
  https_response->set_status(response, status);
  return builder;
}

Response *__httpsResponseBuilder_get(T *builder){
  Response * response = builder->__private;
  builder->__private = NULL;
  return response;
}
#undef T
