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

Response* __https_fetch(Https *https, HttpsRequest *request);
Response* __https_get(Https *https, HttpsRequest *request);
Response* __https_post(Https *https, HttpsRequest*request);
Response* __https_put(Https *https, HttpsRequest *request);
Response* __https_patch(Https *https, HttpsRequest *request);
Response* __https_delete(Https *https, HttpsRequest *request);
SSL* __https_ws_handshake(Https *https, HttpsRequest *request);
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

Response* __https_fetch(Https *https, HttpsRequest *request){
  HttpsResponseBuilder *res_builder = https->response_builder;

  request->send(request);
  res_builder->build(res_builder, request->get_connection(request));
  Response * res = res_builder->get(res_builder);
  int error = res->receive(res);
  if(error){
    printf("SSL_get_error: %d\n", error);
  }
  request->cleanup(request);
  request->destructor(request);
  return res;
}

SSL* __https_ws_handshake(Https *https, HttpsRequest *request){
  HttpsResponseBuilder *res_builder = https->response_builder;

  request->set_method(request,GET);
  request->send(request);
  SSL *ssl = request->get_connection(request);
  res_builder->build(res_builder, ssl);
  Response * res = res_builder->get(res_builder);
  int error = res->receive(res);
  if(error){
    printf("SSL_get_error: %d\n", error);
  }
  char *status = res->get_status(res);

  if(strcmp("101",status) != 0) request->cleanup(request); 
  request->destructor(request);
  return ssl;
}

Response* __https_get(Https *https, HttpsRequest *request){
  request->set_method(request,GET);
  return __https_fetch(https, request);
}

Response* __https_post(Https *https, HttpsRequest*request){
  request->set_method(request,POST);
  return __https_fetch(https, request);
}

Response* __https_put(Https *https, HttpsRequest *request){
  request->set_method(request,PUT);
  return __https_fetch(https, request);
}

Response* __https_patch(Https *https, HttpsRequest *request){
  request->set_method(request,PATCH);
  return __https_fetch(https, request);
}

Response* __https_delete(Https *https, HttpsRequest *request){
  request->set_method(request,DELETE);
  return __https_fetch(https, request);
}

