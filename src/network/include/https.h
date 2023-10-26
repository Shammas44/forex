#ifndef HTTPS_H
#define HTTPS_H
#include "httpsRequest.h"
#include "httpsResponse.h"

/**
 * Make an http request
 * @param request - pointer to an http request
 * @param res - pointer to an http response
 * @param funcId - id of the function to be executed
 * @return int as status code
 */
int https_fetch(HttpsRequest *request,  HttpsResponse *res);

void https_generate_mask(unsigned char mask[4]);

#endif
