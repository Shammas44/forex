#ifndef HTTPSRESPONSE_H
#define HTTPSRESPONSE_H
#include "buffer.h"
#include "error.h"
#include "hashmap.h"
#include "network.h"
#include "response.h"
#include "tls.h"
#include "common.h"
#include <openssl/err.h>
#include <openssl/rand.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define T HttpsResponse

typedef struct T T;

typedef void(HttpsResponse_destructor)(T *self);
typedef char*(HttpsResponse_stringify)(T *self);
typedef char* (HttpsResponse_body)(T *self);
typedef char *(HttpsResponse_status)(T *self);
typedef Hashmap *(HttpsResponse_headers)(T *self);
typedef Item (HttpsResponse_header)(T *self, char* key);
typedef char *(HttpsResponse_content_type)(T *self);

struct T {
  IsDestroyable __destructor;
  HttpsResponse_destructor *destructor;
  HttpsResponse_body *body;
  HttpsResponse_stringify *stringify;
  HttpsResponse_headers *headers;
  HttpsResponse_header *header;
  HttpsResponse_status *status;
  HttpsResponse_content_type *content_type;
  void *__private;
};

T *httpsResponse_constructor(Hashmap *map);
#undef T
#endif
