#ifndef HTTPSREQUEST_H
#define HTTPSREQUEST_H
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

struct HttpsResponse;

typedef struct {
    char* host;
    char* port;
    char* path;
    char* method; 
    char* body;
    size_t body_length;
    char** headers;
    size_t header_count;
} HttpsRequest;

typedef struct {
    char* host;
    char* port;
    char* path;
    char* method; 
    char* body;
    char** headers;
    size_t header_count;
} HttpsRequest_prerequest;

HttpsRequest * httpsRequest_constructor(HttpsRequest_prerequest prefill);

int httpsRequest_destructor(HttpsRequest *request);

/**
 * Iterate over an array and execute a callback for each element of the array
 * @param request - pointer to an http request
 * @param res - pointer to a response as string
 * @return int as status code
 */
int httpsRequest_stringify(const HttpsRequest* request, char **res);

int httpsRequest_send(HttpsRequest *request, SSL **ssl, int *sockfd,  struct HttpsResponse *res );

#endif
