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
SSL* __https_ws_handshake(Https *https, Request *request);
void __https_destructor(Https *https);

// =========================================================================="
// Public functions
// =========================================================================="

Https * https_constructor(){
  Https * https = (Https*)malloc(sizeof(Https));
  HttpsResponseBuilder * response_builder = httpsResponseBuilder_constructor();
  https->destructor = __https_destructor;
  https->response_builder = response_builder;
  https->ws_handshake = __https_ws_handshake;
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

void __https_destructor(Https *https){
  HttpsResponseBuilder * response_builder = https->response_builder;
  response_builder->destructor(response_builder);
  free(https);
  if(ctx!=NULL) SSL_CTX_free(ctx);
}

Response* __https_fetch(Https *https, Request *request){
  Request *req = (Request*)request;
  HttpsRequest *https_req = (HttpsRequest*)req->__private;
  HttpsResponseBuilder *res_builder = https->response_builder;

  req->send(req);
  res_builder->build(res_builder, req->get_connection(req));
  Response * res = res_builder->get(res_builder);
  int error = res->receive(res);
  if(error){
    printf("SSL_get_error: %d\n", error);
  }
  https_req->cleanup(request);
  req->destructor(req);
  return res;
}

SSL* __https_ws_handshake(Https *https, Request *request){
  Request *req = (Request*)request;
  HttpsRequest *https_req = (HttpsRequest*)req->__private;
  HttpsResponseBuilder *res_builder = https->response_builder;

  https_req->set_method(request,GET);
  req->send(req);
  SSL *ssl = req->get_connection(req);
  res_builder->build(res_builder, ssl);
  Response * res = res_builder->get(res_builder);
  int error = res->receive(res);
  if(error){
    printf("SSL_get_error: %d\n", error);
  }
  char *status = res->get_status(res);

  if(strcmp("101",status) != 0) https_req->cleanup(request); 
  req->destructor(req);
  return ssl;
}

Response* __https_get(Https *https, Request *request){
  HttpsRequest *req = (HttpsRequest*)request->__private;
  req->set_method(request,GET);
  return __https_fetch(https, request);
}

Response* __https_post(Https *https, Request*request){
  HttpsRequest *req = (HttpsRequest*)request->__private;
  req->set_method(request,POST);
  return __https_fetch(https, request);
}

Response* __https_put(Https *https, Request *request){
  HttpsRequest *req = (HttpsRequest*)request->__private;
  req->set_method(request,PUT);
  return __https_fetch(https, request);
}

Response* __https_patch(Https *https, Request *request){
  HttpsRequest *req = (HttpsRequest*)request->__private;
  req->set_method(request,PATCH);
  return __https_fetch(https, request);
}

Response* __https_delete(Https *https, Request *request){
  HttpsRequest *req = (HttpsRequest*)request->__private;
  req->set_method(request,DELETE);
  return __https_fetch(https, request);
}

