#include "httpsResponseBuilder.h"
#include "httpsResponse.h"

 struct HttpsResponseBuilder *__httpsResponseBuilder_build(struct HttpsResponseBuilder *builder, SSL*ssl);
 struct HttpsResponseBuilder *__httpsResponseBuilder_set_body(struct HttpsResponseBuilder *builder, char* body);
 struct HttpsResponseBuilder *__httpsResponseBuilder_set_status(struct HttpsResponseBuilder *builder, char* status);
 struct HttpsResponseBuilder *__httpsResponseBuilder_add_header(struct HttpsResponseBuilder *builder, char* header);
 Response *__httpsResponseBuilder_get(struct HttpsResponseBuilder *builder);
void __httpsResponseBuilder_destructor(struct HttpsResponseBuilder *builder);

typedef struct  {
  Response* response;
}HttpsResponseBuilder_private;

HttpsResponseBuilder *httpsResponseBuilder_constructor(){
  HttpsResponseBuilder *builder = malloc(sizeof(HttpsResponseBuilder));
  HttpsResponseBuilder_private *private_properties = malloc(sizeof(HttpsResponseBuilder_private));
  builder->build_func = __httpsResponseBuilder_build;
  builder->set_body_func = __httpsResponseBuilder_set_body;
  builder->add_header_func = __httpsResponseBuilder_add_header;
  builder->set_status_func = __httpsResponseBuilder_set_status;
  builder->get_func = __httpsResponseBuilder_get;
  builder->destructor_func = __httpsResponseBuilder_destructor;
  builder->__private = private_properties;
  return builder;
}

void __httpsResponseBuilder_destructor(struct HttpsResponseBuilder *builder){
  HttpsResponseBuilder *__builder = (HttpsResponseBuilder *)builder;
  free(__builder->__private);
  free(__builder);
}

 struct HttpsResponseBuilder *__httpsResponseBuilder_build(struct HttpsResponseBuilder *builder, SSL*ssl){
  HttpsResponseBuilder *__builder = (HttpsResponseBuilder *)builder;
  HttpsResponseBuilder_private *private_properties = (HttpsResponseBuilder_private *)__builder->__private;
  Response * response = httpsResponse_constructor(ssl);
  private_properties->response = (Response*)response;
  return builder;
}

struct HttpsResponseBuilder *__httpsResponseBuilder_set_body(struct HttpsResponseBuilder *builder, char* body){
  HttpsResponseBuilder *__builder = (HttpsResponseBuilder *)builder;
  HttpsResponseBuilder_private *private_properties = (HttpsResponseBuilder_private *)__builder->__private;
  HttpsResponse *response = (HttpsResponse *)private_properties->response;
  response->set_body_func((struct Response*)response, body);
  return builder;
}

 struct HttpsResponseBuilder *__httpsResponseBuilder_add_header(struct HttpsResponseBuilder *builder, char* header){
  HttpsResponseBuilder *__builder = (HttpsResponseBuilder *)builder;
  HttpsResponseBuilder_private *private_properties = (HttpsResponseBuilder_private *)__builder->__private;
  HttpsResponse *response = (HttpsResponse *)private_properties->response;
  response->add_header_func((struct Response*)response, header);
  return builder;
}

 struct HttpsResponseBuilder *__httpsResponseBuilder_set_status(struct HttpsResponseBuilder *builder, char* status){
  HttpsResponseBuilder *__builder = (HttpsResponseBuilder *)builder;
  HttpsResponseBuilder_private *private_properties = (HttpsResponseBuilder_private *)__builder->__private;
  HttpsResponse *response = (HttpsResponse *)private_properties->response;
  response->set_status_func((struct Response*)response, status);
  return builder;
}

Response *__httpsResponseBuilder_get(struct HttpsResponseBuilder *builder){
  HttpsResponseBuilder *__builder = (HttpsResponseBuilder *)builder;
  HttpsResponseBuilder_private *private_properties = (HttpsResponseBuilder_private *)__builder->__private;
  return private_properties->response;
}
