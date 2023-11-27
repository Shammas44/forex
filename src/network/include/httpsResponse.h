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

typedef Response *(HttpsResponse_set_status)(Response *res, char* status);
typedef Response *(HttpsResponse_set_body)(Response *res, char* body);
typedef Response *(HttpsResponse_add_header)(Response *res, char* header);

struct T {
    Message *message;
    HttpsResponse_set_status *set_status;
    HttpsResponse_set_body *set_body;
    HttpsResponse_add_header *add_header;
};

Response * httpsResponse_constructor(SSL*ssl);
#undef T
#endif
