#include "httpsRequestBuilder.h"
#include "httpsRequest.h"

 struct HttpsRequestBuilder *__httpsRequestBuilder_build(struct HttpsRequestBuilder *builder, char* url);
 struct HttpsRequestBuilder *__httpsRequestBuilder_set_body(struct HttpsRequestBuilder *builder, char* body);
 struct HttpsRequestBuilder *__httpsRequestBuilder_add_header(struct HttpsRequestBuilder *builder, char* header);
 struct HttpsRequestBuilder *__httpsRequestBuilder_set_method(struct HttpsRequestBuilder *builder, HttpsRequest_method method);
void __httpsRequestBuilder_destructor(struct HttpsRequestBuilder *builder);

typedef struct  {
  Request* request;
}HttpsRequestBuilder_private;

HttpsRequestBuilder *httpsRequestBuilder_constructor(){
  HttpsRequestBuilder *builder = malloc(sizeof(HttpsRequestBuilder));
  builder->destructor = __httpsRequestBuilder_destructor;
  builder->build = __httpsRequestBuilder_build;
  builder->set_body = __httpsRequestBuilder_set_body;
  builder->add_header = __httpsRequestBuilder_add_header;
  builder->set_method = __httpsRequestBuilder_set_method;
  builder->request = NULL;
  return builder;
}

 struct HttpsRequestBuilder *__httpsRequestBuilder_build(struct HttpsRequestBuilder *builder, char* url){
  HttpsRequestBuilder *__builder = (HttpsRequestBuilder *)builder;
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

  struct Request *request = httpsRequest_constructor(prefill);
  __builder->request = (Request*)request;
  return builder;
}

void __httpsRequestBuilder_destructor(struct HttpsRequestBuilder *builder){
  HttpsRequestBuilder *__builder = (HttpsRequestBuilder *)builder;
  free(__builder);
}

struct HttpsRequestBuilder *__httpsRequestBuilder_set_body(struct HttpsRequestBuilder *builder, char* body){
  HttpsRequestBuilder *__builder = (HttpsRequestBuilder *)builder;
  HttpsRequest *httpsRequest = (HttpsRequest *)__builder->request->__private;
  httpsRequest->set_body_func((struct Request*)httpsRequest, body);
  return builder;
}

 struct HttpsRequestBuilder *__httpsRequestBuilder_add_header(struct HttpsRequestBuilder *builder, char* header){
  HttpsRequestBuilder *__builder = (HttpsRequestBuilder *)builder;
  HttpsRequest *httpsRequest = (HttpsRequest *)__builder->request->__private;
  httpsRequest->add_header_func((struct Request*)httpsRequest, header);
  return builder;
}

 struct HttpsRequestBuilder *__httpsRequestBuilder_set_method(struct HttpsRequestBuilder *builder, HttpsRequest_method method){
  HttpsRequestBuilder *__builder = (HttpsRequestBuilder *)builder;
  HttpsRequest *httpsRequest = (HttpsRequest *)__builder->request->__private;
  httpsRequest->set_method_func((struct Request*)httpsRequest, method);
  return builder;
}
