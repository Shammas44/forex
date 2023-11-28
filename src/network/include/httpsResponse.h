#ifndef HTTPSRESPONSE_H
#define HTTPSRESPONSE_H
#include <stddef.h>
#include <openssl/err.h>
#include "buffer.h"
#include "wsFrame.h"
#include "tls.h"
#include "error.h"
#include "network.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <openssl/rand.h>
#include "response.h"
#include "message.h"

#define T HttpsResponse

typedef struct T T;
typedef struct HttpsResponse_prefill HttpsResponse_prefill;

typedef T *(HttpsResponse_set_status)(T *res, char* status);
typedef T *(HttpsResponse_set_body)(T *res, char* body);
typedef T *(HttpsResponse_add_header)(T *res, char* header);
typedef int (HttpsResponse_receive)(T *res);
// typedef T * (Response_constructor)(Response_prefill prefill);
typedef int (HttpsResponse_destructor)(T * response);
typedef int (HttpsResponse_stringify)(T * response, char **out);
typedef void (HttpsResponse_print)(T *response);
typedef char* (HttpsResponse_get_status)(T *response);

struct T {
    HttpsResponse_destructor *destructor;
    HttpsResponse_set_status *set_status;
    HttpsResponse_set_body *set_body;
    HttpsResponse_add_header *add_header;
    HttpsResponse_receive *receive;
    HttpsResponse_stringify *stringify;
    HttpsResponse_print *print;
    HttpsResponse_get_status *get_status;
    void * __private;
};

T * httpsResponse_constructor(SSL*ssl);
#undef T
#endif
