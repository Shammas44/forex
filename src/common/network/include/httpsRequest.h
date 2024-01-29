#ifndef HTTPSREQUEST_H
#define HTTPSREQUEST_H
#include "buffer.h"
#include "error.h"
#include "network.h"
#include "tls.h"
#include "url.h"
#include <openssl/err.h>
#include <openssl/rand.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
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

typedef T *(HttpsRequest_set_method)(T *request, HttpsRequest_method method);
typedef T *(HttpsRequest_set_body)(T *request, char *body);
typedef T *(HttpsRequest_add_header)(T *request, char *header);
typedef T *(HttpsRequest_set_url)(T *request, char *url);
typedef void(HttpsRequest_destructor)(T *request);
typedef int(HttpsRequest_stringify)(T *request, char **out);
typedef void(HttpsRequest_print)(T *request);
typedef SSL *(HttpsRequest_get_connection)(T *request);
typedef Url *(HttpsRequest_get_url)(T *request);

struct T {
  HttpsRequest_destructor *destructor;
  HttpsRequest_set_method *set_method;
  HttpsRequest_set_body *set_body;
  HttpsRequest_set_url *set_url;
  HttpsRequest_add_header *add_header;
  HttpsRequest_stringify *stringify;
  HttpsRequest_print *print;
  HttpsRequest_get_connection *get_connection;
  HttpsRequest_get_url *get_url;
  void *__private;
};

T *httpsRequest_constructor(HttpsRequest_prefill prefill);

#undef T
#endif
