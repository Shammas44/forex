#ifndef HTTPSRESPONSE_H
#define HTTPSRESPONSE_H
#include "buffer.h"
#include "error.h"
#include "hashmap.h"
#include "message.h"
#include "network.h"
#include "response.h"
#include "tls.h"
#include "wsFrame.h"
#include <openssl/err.h>
#include <openssl/rand.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define T HttpsResponse

typedef struct T T;

typedef T *(HttpsResponse_set_status)(T *res, char *status);
typedef T *(HttpsResponse_set_body)(T *res, char *body);
typedef T *(HttpsResponse_add_header)(T *res, char *header);
typedef int(HttpsResponse_destructor)(T *response);
typedef int(HttpsResponse_stringify)(T *response, char **out);
typedef void(HttpsResponse_print)(T *response);
typedef char *(HttpsResponse_get_status)(T *response);
typedef char *(HttpsResponse_get_body)(T *response);
typedef Hashmap *(HttpsResponse_get_headers)(T *response);

struct T {
  HttpsResponse_destructor *destructor;
  HttpsResponse_set_status *set_status;
  HttpsResponse_set_body *set_body;
  HttpsResponse_add_header *add_header;
  HttpsResponse_stringify *stringify;
  HttpsResponse_print *print;
  HttpsResponse_get_status *get_status;
  HttpsResponse_get_body *get_body;
  HttpsResponse_get_headers *get_headers;
  void *__private;
};

T *httpsResponse_constructor(char *raw_response);
#undef T
#endif
