#include "httpsRequest.h"

#define ISVALIDSOCKET(s) ((s) >= 0)
#define ONERROR(s) ((s) <0)

struct HttpsResponse;

HttpsRequest * httpsRequest_constructor(HttpsRequest_prerequest prefill){
HttpsRequest *request = malloc(sizeof(HttpsRequest));
  request->method = prefill.method;
  request->body = prefill.body;
  request->path = prefill.path;
  request->host = prefill.host;
  request->port = prefill.port;
  request->headers = prefill.headers;
  request->body_length = sizeof(request->body) > 1 ? sizeof(prefill.body) - 1 : 0;
  request->header_count = prefill.header_count;
  return request;
}

int httpsRequest_destructor(HttpsRequest *request){
  free(request->method);
  free(request->path);
  free(request->host);
  free(request->headers);
  free(request->body);
  free(request->port);
  free(request);
  return 0;
}

int httpsRequest_send(HttpsRequest *request, SSL **ssl, int *sockfd,  struct HttpsResponse *res ){
  struct addrinfo *add_info = NULL;
  int status;
  int domain_len = strlen(request->host);
  int error = 0;

  while(error == 0){

    network_ini_openssl();

    status = set_ssl_context();
    if (ONERROR(status)){
      error = get_error("SSL_CTX_new() failed.");
    }

    status = network_get_adresses(request->host, request->port, &add_info);
    if (ONERROR(status)){
      error = get_error("getaddrinfo() failed.");
    }

    *sockfd = network_get_socket(add_info);
    if (!ISVALIDSOCKET(sockfd)){
      error = get_error("socket() failed.");
    }

    status = connect(*sockfd, add_info->ai_addr, add_info->ai_addrlen);
    if (status){
      error = get_error("connect() failed.");
    }

    if (!ctx) {
      error = get_error("SSL_CTX is NULL.");
    } else {
      *ssl = SSL_new(ctx);
      if (!ssl) {
        error = get_error("SSL_new() failed.");
      }
    }

    if (!SSL_set_tlsext_host_name(*ssl, request->host)) {
      error = get_error("SSL_set_tlsext_host_name() failed.");
    }

    SSL_set_fd(*ssl, *sockfd);
    if (SSL_connect(*ssl) == -1) {
      error = get_error("SSL_connect() failed.");
    }

    char *str_request = NULL;

    int request_len = httpsRequest_stringify(request,&str_request);
    status = SSL_write(*ssl, str_request, request_len);
    free(str_request);
    if (status == -1){
      error = get_error("ssl_write() failed.");
    }
    break;
  }
  if(add_info!=NULL) freeaddrinfo(add_info);

  return error;
}

int httpsRequest_stringify(const HttpsRequest* request, char **res){
    // Estimate the length of the request string
    size_t length = strlen(request->method) + strlen(request->path) + strlen(request->host) + 100; // Adding 100 for additional characters

    for (size_t i = 0; i < request->header_count; i++) {
        length += strlen(request->headers[i]) + 2; // Additional characters for newline and carriage return
    }

    if (request->body) {
        length += request->body_length + 2; // Additional characters for newline and carriage return
    }

     *res = (char*)malloc(length);

    if (!res) {
        return -1; 
    }
    // Construct the request line
    int offset = sprintf(*res, "%s %s HTTP/1.1\r\nHost: %s\r\n", request->method, request->path, request->host);
    // Append the headers
    for (size_t i = 0; i < request->header_count; i++) {
        offset += sprintf((*res) + offset, "%s\r\n", request->headers[i]);
    }
    // append Content-Length header
    offset += sprintf((*res) + offset, "Content-Length: %zu\r\n", request->body_length);
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
