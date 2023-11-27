#ifndef HTTPS_H
#define HTTPS_H
#include "httpsRequest.h"
#include "httpsRequestBuilder.h"
#include "httpsResponseBuilder.h"
#include "httpsResponse.h"

#define T Https

typedef struct T T;

typedef Response* (https_fetch)(T *https, HttpsRequest *request);
typedef Response* (https_get)(T *https, HttpsRequest *request);
typedef Response* (https_post)(T *https, HttpsRequest *request);
typedef Response* (https_put)(T *https, HttpsRequest *request);
typedef Response* (https_patch)(T *https, HttpsRequest *request);
typedef Response* (https_delete)(T *https, HttpsRequest *request);
typedef SSL* (https_ws_handshake)(T *https, HttpsRequest *request);
typedef void (https_destructor)(T *https);

typedef struct T {
  https_destructor * destructor;
  https_get * get;
  https_post * post;
  https_put * put;
  https_patch * patch;
  https_delete * delete;
  https_ws_handshake * ws_handshake;
  HttpsResponseBuilder * response_builder;
} T;

T * https_constructor();

#undef T
#endif
