#include "httpsRequest.h"
#include <stdio.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define ONERROR(s) ((s) < 0)

struct HttpsResponse;

HttpsRequest *httpsRequest_constructor(HttpsRequest_prerequest prefill) {
  HttpsRequest *request = malloc(sizeof(HttpsRequest));
  Url *url = url_constructor(prefill.url);
  request->method = prefill.method;
  request->body = prefill.body;
  request->url = url;
  request->headers = prefill.headers;
  request->body_length = strlen(prefill.body);
  request->header_count = prefill.header_count;
  return request;
}

int httpsRequest_destructor(HttpsRequest *request) {
  free(request->method);
  free(request->url);
  free(request->headers);
  free(request->body);
  free(request);
  return 0;
}

void httpsRequest_print(HttpsRequest *request){
  puts("");
  printf("Method: %s\n", request->method);
  printf("Body: %s\n", request->body);
  printf("Body length: %zu\n", request->body_length);
  printf("Header count: %zu\n", request->header_count);
  printf("Headers: \n");
  for (size_t i = 0; i < request->header_count; i++) {
    printf("%s\n", request->headers[i]);
  }
  url_print(request->url);
}

int httpsRequest_stringify(const HttpsRequest *request, char **res) {
  // Estimate the length of the request string
  char * host = request->url->host;
  char * path = request->url->path;
  size_t length = strlen(request->method) + strlen(path) + strlen(host) + 100; // Adding 100 for additional characters

  for (size_t i = 0; i < request->header_count; i++) {
    length += strlen(request->headers[i]) +
              2; // Additional characters for newline and carriage return
  }

  if (request->body) {
    length += request->body_length +
              2; // Additional characters for newline and carriage return
  }

  *res = (char *)malloc(length);

  if (!res) {
    return -1;
  }
  // Construct the request line
  int offset = sprintf(*res, "%s %s HTTP/1.1\r\nHost: %s\r\n", request->method,
                       path, host);
  // Append the headers
  for (size_t i = 0; i < request->header_count; i++) {
    offset += sprintf((*res) + offset, "%s\r\n", request->headers[i]);
  }
  // append Content-Length header
  offset +=
      sprintf((*res) + offset, "Content-Length: %zu\r\n", request->body_length);
  // Append an extra newline before the body, if any
  offset += sprintf((*res) + offset, "\r\n");
  // Append the body, if any
  if (request->body) {
    memcpy(*res + offset, request->body, request->body_length);
    offset += request->body_length;
  }
  // Null-terminate the request string
  (*res)[offset] = '\0';
  return offset;
}
