#ifndef HTTPSREQUEST_H
#define HTTPSREQUEST_H
#include <openssl/err.h>
#include <openssl/rand.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "buffer.h"
#include "error.h"
#include "network.h"
#include "request.h"
#include "tls.h"
#include "url.h"
#include "wsframe.old.h"
#include "message.h"
#define T HttpsRequest

typedef struct T T;
typedef enum {
  GET,
  POST,
  PUT,
  PATCH,
  DELETE,
  HEAD,
  OPTIONS,
  TRACE,
  CONNECT
} HttpsRequest_method;

typedef struct {
  char *url;
  HttpsRequest_method method;
  char *body;
  char **headers;
  size_t header_count;
} HttpsRequest_prefill;

typedef Request *(HttpsRequest_set_method)(Request *req, HttpsRequest_method method);
typedef Request *(HttpsRequest_set_body)(Request *req, char* body);
typedef Request *(HttpsRequest_add_header)(Request *req, char* header);
typedef void (HttpsRequest_destructor)(Request*req);
typedef void (HttpsRequest_cleanup)(Request*req);

struct T {
  Message *message;
  HttpsRequest_destructor *destructor;
  HttpsRequest_set_method *set_method;
  HttpsRequest_set_body *set_body;
  HttpsRequest_add_header *add_header;
  HttpsRequest_cleanup *cleanup;
};

Request *httpsRequest_constructor(HttpsRequest_prefill prefill);

#undef T
#endif
