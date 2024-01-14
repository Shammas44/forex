#ifndef HTTPS_H
#define HTTPS_H
#include "httpsRequest.h"
#include "httpsRequestBuilder.h"
#include "httpsResponseBuilder.h"
#include "httpsResponse.h"

#define T Https

typedef struct T T;

typedef HttpsResponse* (https_fetch)(T*self,HttpsRequest *request);
typedef HttpsResponse* (https_get)(T*self,HttpsRequest *request);
typedef HttpsResponse* (https_post)(T*self,HttpsRequest *request);
typedef HttpsResponse* (https_put)(T*self,HttpsRequest *request);
typedef HttpsResponse* (https_patch)(T*self,HttpsRequest *request);
typedef HttpsResponse* (https_delete)(T*self,HttpsRequest *request);
typedef SSL* (https_ws_handshake)(T*self,HttpsRequest *request);
typedef void (https_destructor)(T *https);

typedef struct T {
  https_destructor * destructor;
  https_get * get;
  https_post * post;
  https_put * put;
  https_patch * patch;
  https_delete * delete;
  https_fetch * fetch;
  https_ws_handshake * ws_handshake;
  void* __private;
} T;

T * https_constructor();

#undef T
#endif
