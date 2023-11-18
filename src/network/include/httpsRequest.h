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
#include "wsframe.h"
#include "message.h"

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

typedef struct Request *(HttpsRequest_set_method)(struct Request *req, HttpsRequest_method method);
typedef struct Request *(HttpsRequest_set_body)(struct Request *req, char* body);
typedef struct Request *(HttpsRequest_add_header)(struct Request *req, char* header);
typedef void (HttpsRequest_destructor)(struct Request*);
typedef void (HttpsRequest_cleanup)(struct Request*);

typedef struct {
  struct Message *message;
  HttpsRequest_destructor *destroy_func;
  HttpsRequest_set_method *set_method_func;
  HttpsRequest_set_body *set_body_func;
  HttpsRequest_add_header *add_header_func;
  HttpsRequest_cleanup *cleanup_func;
} HttpsRequest;

struct Request *httpsRequest_constructor(HttpsRequest_prefill prefill);

#endif
