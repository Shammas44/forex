#include "httpsRequestBuilder.h"

#define T HttpsRequestBuilder

T *__httpsRequestBuilder_build(T *builder, char* url);
T *__httpsRequestBuilder_set_body(T *builder, char* body);
T *__httpsRequestBuilder_add_header(T *builder, char* header);
T *__httpsRequestBuilder_set_method(T *builder, HttpsRequest_method method);
HttpsRequest *__httpsRequestBuilder_get(T *builder);
void __httpsRequestBuilder_destructor(T *builder);

T *httpsRequestBuilder_constructor(){
  T *builder = malloc(sizeof(T));
  builder->destructor = __httpsRequestBuilder_destructor;
  builder->build = __httpsRequestBuilder_build;
  builder->set_body = __httpsRequestBuilder_set_body;
  builder->add_header = __httpsRequestBuilder_add_header;
  builder->set_method = __httpsRequestBuilder_set_method;
  builder->get = __httpsRequestBuilder_get;
  builder->__private = NULL;
  return builder;
}

 T *__httpsRequestBuilder_build(T *builder, char* url){
  HttpsRequest_prefill prefill = {};

  char *headers[] = {
      "Connection: close",
  };

  prefill.url = url;
  prefill.method = GET;
  prefill.body = NULL;
  prefill.headers = NULL;
  prefill.header_count = 0;
  prefill.headers = headers;

  HttpsRequest *request = httpsRequest_constructor(prefill);
  builder->__private = request;
  return builder;
}

void __httpsRequestBuilder_destructor(T *builder){
  HttpsRequest *request = builder->__private;
  if(request != NULL){
    request->destructor(request);
  }
  free(builder);
}

T *__httpsRequestBuilder_set_body(T *builder, char* body){
  HttpsRequest *request = builder->__private;
  request->set_body(request, body);
  return builder;
}

 T *__httpsRequestBuilder_add_header(T *builder, char* header){
  HttpsRequest *request = builder->__private;
  request->add_header(request, header);
  return builder;
}

 T *__httpsRequestBuilder_set_method(T *builder, HttpsRequest_method method){
  HttpsRequest *request = builder->__private;
  request->set_method(request, method);
  return builder;
}

 HttpsRequest *__httpsRequestBuilder_get(T *builder){
  HttpsRequest *request = builder->__private;
  builder->__private = NULL;
  return request;
}
#undef T
