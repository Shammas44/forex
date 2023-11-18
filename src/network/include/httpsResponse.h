#ifndef HTTPSRESPONSE_H
#define HTTPSRESPONSE_H
#include <stddef.h>
#include <openssl/err.h>
#include "buffer.h"
#include "wsframe.h"
#include "tls.h"
#include "error.h"
#include "network.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <openssl/rand.h>
#include "response.h"

struct httpsResponse_prefill;

typedef struct Response *(HttpsResponse_set_status)(struct Response *res, char* status);
typedef struct Response *(HttpsResponse_set_body)(struct Response *res, char* body);
typedef struct Response *(HttpsResponse_add_header)(struct Response *res, char* header);

typedef struct HttpsResponse {
    struct Message *message;
    HttpsResponse_set_status *set_status_func;
    HttpsResponse_set_body *set_body_func;
    HttpsResponse_add_header *add_header_func;
} HttpsResponse;

Response * httpsResponse_constructor(SSL*ssl);

#endif
