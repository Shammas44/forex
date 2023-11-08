#ifndef HTTPS_H
#define HTTPS_H
#include "httpsRequest.h"
#include "httpsResponse.h"

#define HTTP_METHODS_GET "GET"
#define HTTP_METHODS_POST "POST"
#define HTTP_METHODS_DELETE "DELETE"
#define HTTP_METHODS_UPDATE "UPDATE"
#define HTTP_METHODS_PATCH "PATCH"

/**
 * Make an http request
 * @param request - pointer to an http request
 * @param res - pointer to an http response
 * @param funcId - id of the function to be executed
 * @return int as status code
 */
int https_fetch(HttpsRequest *request,  HttpsResponse *res, SSL *ssl);

int https_get(HttpsRequest *request,  HttpsResponse *res, SSL *ssl);

int https_post(HttpsRequest *request,  HttpsResponse *res, SSL *ssl);

int https_delete(HttpsRequest *request,  HttpsResponse *res, SSL *ssl);

int https_update(HttpsRequest *request,  HttpsResponse *res, SSL *ssl);

int https_patch(HttpsRequest *request,  HttpsResponse *res, SSL *ssl);

int https_send_request(HttpsRequest *request,SSL **ssl, int *sockfd, HttpsResponse*res);

#endif
