#ifndef HTTPS_H
#define HTTPS_H
#include "httpsRequest.h"
#include "httpsRequestBuilder.h"
#include "httpsResponseBuilder.h"
#include "httpsResponse.h"

struct Https;

typedef Response* (https_fetch)(struct Https *https, Request *request);

typedef Response* (https_get)(struct Https *https, Request *request);

typedef Response* (https_post)(struct Https *https, Request *request);

typedef Response* (https_put)(struct Https *https, Request *request);

typedef Response* (https_patch)(struct Https *https, Request *request);

typedef Response* (https_delete)(struct Https *https, Request *request);

typedef void (https_destructor)(struct Https *https);

typedef struct Https {
  HttpsResponseBuilder *response_builder;
  https_get * get;
  https_post * post;
  https_put * put;
  https_patch * patch;
  https_delete * delete;
  https_destructor * destructor;
} Https;

Https * https_constructor();
#endif
