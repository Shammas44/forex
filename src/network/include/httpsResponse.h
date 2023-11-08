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

typedef struct {
    char* status;
    char* headers;
    char* body;
} HttpsResponse;

HttpsResponse * httpsResponse_constructor(char* res);

int httpsResponse_destructor(HttpsResponse *res);

void httpsResponse_parse(char* response, HttpsResponse *res);

int httpsResponse_extract_authorization(HttpsResponse *res,char **authorization);

#endif
