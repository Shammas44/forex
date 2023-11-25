#include "https.h"
#include "httpsResponseBuilder.h"
#include "httpsRequestBuilder.h"

#define SOCKET int
#define CLOSESOCKET(s) close(s)
#define ISVALIDSOCKET(s) ((s) >= 0)
#define GETSOCKETERRNO() (errno)
#define ONERROR(s) ((s) < 0)

// =========================================================================="
// Prototypes functions
// =========================================================================="

Response* __https_fetch(Https *https, Request *request);
Response* __https_get(Https *https, Request *request);
Response* __https_post(Https *https, Request*request);
Response* __https_put(Https *https, Request *request);
Response* __https_patch(Https *https, Request *request);
Response* __https_delete(Https *https, Request *request);
void __https_destructor(Https *https);

// =========================================================================="
// Public functions
// =========================================================================="

Https * https_constructor(){
  Https * https = (Https*)malloc(sizeof(Https));
  HttpsResponseBuilder * response_builder = httpsResponseBuilder_constructor();
  https->destructor = __https_destructor;
  https->response_builder = response_builder;
  https->get = __https_get;
  https->post = __https_post;
  https->put = __https_put;
  https->patch = __https_patch;
  https->delete = __https_delete;
  return https;
}

// =========================================================================="
// Private functions
// =========================================================================="

void __https_destructor(struct Https *https){
  Https *https_handler = (Https*)https;
  HttpsResponseBuilder * response_builder = https_handler->response_builder;
  response_builder->destructor((struct HttpsResponseBuilder*)response_builder);
  free(https);
  if(ctx!=NULL) SSL_CTX_free(ctx);
}

Response* __https_fetch(Https *https, Request *request){
  Request *req = (Request*)request;
  HttpsRequest *https_req = (HttpsRequest*)req->__private;
  HttpsResponseBuilder *res_builder = https->response_builder;

  req->send_func((struct Request*)req);
  res_builder->build(res_builder, req->get_connection_func((struct Request*)req));
  Response * res = res_builder->get(res_builder);
  res->receive_func((struct Response*)res);
  https_req->cleanup((struct Request*)request);
  req->destructor_func((struct Request*)req);
  return res;
}

Response* __https_get(Https *https, Request *request){
  HttpsRequest *req = (HttpsRequest*)request->__private;
  req->set_method((struct Request*)request,GET);
  return __https_fetch(https, request);
}

Response* __https_post(Https *https, Request*request){
  HttpsRequest *req = (HttpsRequest*)request;
  req->set_method((struct Request*)req,POST);
  return __https_fetch(https, request);
}

Response* __https_put(Https *https, Request *request){
  HttpsRequest *req = (HttpsRequest*)request;
  req->set_method((struct Request*)req,PUT);
  return __https_fetch(https, request);
}

Response* __https_patch(Https *https, Request *request){
  HttpsRequest *req = (HttpsRequest*)request;
  req->set_method((struct Request*)req,PATCH);
  return __https_fetch(https, request);
}

Response* __https_delete(Https *https, Request *request){
  HttpsRequest *req = (HttpsRequest*)request;
  req->set_method((struct Request*)req,DELETE);
  return __https_fetch(https, request);
}

