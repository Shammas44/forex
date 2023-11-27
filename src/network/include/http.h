#ifndef HTTP_H
#define HTTP_H
#include <stddef.h>
#include <openssl/err.h>
#include "buffer.h"
#include "wsframe.old.h"
#include "tls.h"
#include "error.h"
#include "network.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <openssl/rand.h>

typedef struct {
    char* host;
    char* port;
    char* path;
    char* method; 
    char* body;
    size_t body_length;
    char** headers;
    size_t header_count;
} HttpRequest;

typedef struct {
    char* status;
    char* headers;
    char* body;
} HttpResponse;

typedef enum Http_request_type {
 HTTP = 0,
 WS = 1,
} Http_request_type;

typedef int (*Handle_response)(SSL *ssl, HttpResponse *res);

int http_get_authorization_from_res(HttpResponse *res,char **authorization);

/**
 * Make an http request
 * @param request - pointer to an http request
 * @param res - pointer to an http response
 * @param funcId - id of the function to be executed
 * @return int as status code
 */
int http_fetch(HttpRequest *request,  HttpResponse *res);

/**
 * Make an ws request
 * @param request - pointer to an http request
 * @param res - pointer to an http response
 * @param funcId - id of the function to be executed
 * @return int as status code
 */
int ws_fetch(SSL **ssl,HttpRequest *request,  HttpResponse *res);

/**
 * parse http response
 * @param response - pointer to the input response
 * @param res - pointer to the output response
 * @return void
 */
void parse_http_response(char* response, HttpResponse *res); 

/**
 * Free an http response
 * @param http_response - pointer to an http response
 * @return void
 */
void free_http_response(HttpResponse* http_response); 

/**
 * Iterate over an array and execute a callback for each element of the array
 * @param request - pointer to an http request
 * @param res - pointer to a response as string
 * @return int as status code
 */
int create_request_string(const HttpRequest* request, char **res);

void ws_send_message(SSL *ssl, const char *message);

#endif
