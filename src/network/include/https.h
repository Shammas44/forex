#ifndef HTTPS_H
#define HTTPS_H
#include "httpsRequest.h"
#include "httpsRequestBuilder.h"
#include "httpsResponseBuilder.h"
#include "httpsResponse.h"

#define T Https

typedef struct T T;

typedef HttpsResponse* (https_fetch)(HttpsRequest *request);
typedef HttpsResponse* (https_get)(HttpsRequest *request);
typedef HttpsResponse* (https_post)(HttpsRequest *request);
typedef HttpsResponse* (https_put)(HttpsRequest *request);
typedef HttpsResponse* (https_patch)(HttpsRequest *request);
typedef HttpsResponse* (https_delete)(HttpsRequest *request);
typedef SSL* (https_ws_handshake)(HttpsRequest *request);
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
} T;

T * https_constructor();

#undef T
#endif
