#include "httpsRequest.h"

#define ISVALIDSOCKET(s) ((s) >= 0)
#define ONERROR(s) ((s) < 0)
#define CLOSESOCKET(s) close(s)
#define GET_MESSAGE_FROM_REQUEST(req) \
({ \
  HttpsRequest *_https_request = req->__private; \
  Message *_message = _https_request->message; \
  _message; \
})

// =========================================================================="
// Data structures
// =========================================================================="

struct Message {
    Url* url;
    char method[8]; 
    char* body;
    size_t body_length;
    char** headers;
    size_t header_count;
    SSL* ssl;
    int sockfd;
};

// =========================================================================="
// Prototypes functions
// =========================================================================="

 int __httpsRequest_send(Request *res);
 int __httpsRequest_destructor(Request * request);
 int __httpsRequest_stringify(Request * request, char **out);
 void __httpsRequest_print(Request *request);
int __httpsRequest_parse(char* raw_request, Message *request);
char *__httpsRequest_method_to_string(HttpsRequest_method method);
Request *__httpsRequest_set_method(Request *req, HttpsRequest_method method);
Request *__httpsRequest_set_body(Request *req, char* body);
Request *__httpsRequest_add_header(Request *req, char* header);
void *__httpsRequest_get_ssl(Request* request);
void __httpsRequest_cleanup(Request*request);

Request *httpsRequest_constructor(HttpsRequest_prefill prefill) {
  Request *request = malloc(sizeof(Request));
  Message *message = malloc(sizeof(Message));
  HttpsRequest *httpsRequest = malloc(sizeof(HttpsRequest));
  //request
  request->__private = httpsRequest;
  request->print = __httpsRequest_print;
  request->stringify = __httpsRequest_stringify;
  request->send = __httpsRequest_send;
  request->destructor = __httpsRequest_destructor;
  request->get_connection = __httpsRequest_get_ssl;
  //httpsRequest
  httpsRequest->set_body = __httpsRequest_set_body;
  httpsRequest->set_method = __httpsRequest_set_method;
  httpsRequest->add_header = __httpsRequest_add_header;
  httpsRequest->cleanup = __httpsRequest_cleanup;
  httpsRequest->message = message;

  //message
  message->ssl = NULL;
  //message->url
  Url *url = url_constructor(prefill.url);
  message->url = url;
  //message->method
  // message->method = __httpsRequest_method_to_string(prefill.method);
  sprintf(message->method, "%s", __httpsRequest_method_to_string(prefill.method));
  // char *method = malloc(sizeof(char)* strlen(method_as_string));
  // sprintf(method, "%s", method_as_string);
  //message->body
  char *body;
  size_t body_length = prefill.body!=NULL ? strlen(prefill.body) : 0;
  if(body_length > 0){
    body = malloc(sizeof(char)* body_length);
    sprintf(body, "%s", prefill.body);
    message->body = body;
    message->body_length = body_length;
  }else {
    message->body = NULL;
    message->body_length = 0;
  }
  //message->headers
  char **headers = malloc(sizeof(char*) * 10);
  if (headers != NULL) {
    for (int i = 0; i < 10; i++) {
      headers[i] = malloc(sizeof(char) * 200);
      if (headers[i] != NULL) {
        if (i < prefill.header_count && prefill.headers[i] != NULL) {
          strncpy(headers[i], prefill.headers[i], 127); // Copy up to 127 characters
          headers[i][127] = '\0'; // Ensure null-termination
        } else {
          headers[i][0] = '\0'; // Empty string or null if not required
        }
      }
    }
  }
  message->headers = headers;
  message->header_count = prefill.header_count;
  return request;
}

int __httpsRequest_destructor(Request *request) {
  HttpsRequest *httpsRequest = request->__private;
  Message * message = httpsRequest->message;
  // if(message->ssl!=NULL) SSL_free(message->ssl);
  url_destructor(message->url);
  for (int i = 0; i < 10; i++) {
    free(message->headers[i]);
  }
  free(message->headers);
  if(message->body_length >0)free(message->body);
  free(message);
  free(httpsRequest);
  free(request);
  return 0;
}

void __httpsRequest_print(Request *request){
  Message *message = GET_MESSAGE_FROM_REQUEST(request);
  puts("");
  printf("Method: %s\n", message->method);
  printf("Body: %s\n", message->body);
  printf("Body length: %zu\n", message->body_length);
  printf("Header count: %zu\n", message->header_count);
  printf("Headers: \n");
  for (size_t i = 0; i < message->header_count; i++) {
    printf("\t%s\n", message->headers[i]);
  }
  url_print(message->url);
}

int __httpsRequest_stringify(Request *request, char **out) {
  Message *message = GET_MESSAGE_FROM_REQUEST(request);
  char * host = message->url->host;
  char * path = message->url->path;
  // Estimate the length of the request string
  size_t length = strlen(message->method) + strlen(path) + strlen(host) + 100; // Adding 100 for additional characters

  for (size_t i = 0; i < message->header_count; i++) {
    length += strlen(message->headers[i]) +
              2; // Additional characters for newline and carriage return
  }

  if (message->body) {
    length += message->body_length +
              2; // Additional characters for newline and carriage return
  }

  *out = (char *)malloc(length);

  if (!out) {
    return -1;
  }
  // Construct the request line
  int offset = sprintf(*out, "%s %s HTTP/1.1\r\nHost: %s\r\n", message->method,
                       path, host);
  // Append the headers
  for (size_t i = 0; i < message->header_count; i++) {
    offset += sprintf((*out) + offset, "%s\r\n", message->headers[i]);
  }
  // append Content-Length header
  offset +=
      sprintf((*out) + offset, "Content-Length: %zu\r\n", message->body_length);
  // Append an extra newline before the body, if any
  offset += sprintf((*out) + offset, "\r\n");
  // Append the body, if any
  if (message->body) {
    memcpy(*out + offset, message->body, message->body_length);
    offset += message->body_length;
  }
  // Null-terminate the request string
  (*out)[offset] = '\0';
  return offset;
}

int __httpsRequest_send(Request *request){
  struct addrinfo *add_info = NULL;
  Message *message = GET_MESSAGE_FROM_REQUEST(request);
  int status;
  char *host = message->url->host;
  char *path = message->url->path;
  char *port = message->url->port;
  int domain_len = strlen(message->url->host);
  int error = 0;
  SSL **ssl = &message->ssl;
  int *sockfd = &message->sockfd;

  while (error == 0) {

    network_ini_openssl();

    status = set_ssl_context();
    if (ONERROR(status)) {
      error = get_error("SSL_CTX_new() failed.");
    }

    //disble ssl certificate verification -- only for dev
    // SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);

    status = network_get_adresses(host, port, &add_info);
    if (ONERROR(status)) {
      error = get_error("getaddrinfo() failed.");
    }

    *sockfd = network_get_socket(add_info);
    if (!ISVALIDSOCKET(sockfd)) {
      error = get_error("socket() failed.");
    }

    status = connect(*sockfd, add_info->ai_addr, add_info->ai_addrlen);
    if (status) {
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

    if (!SSL_set_tlsext_host_name(*ssl, host)) {
      error = get_error("SSL_set_tlsext_host_name() failed.");
    }

    SSL_set_fd(*ssl, *sockfd);
    if (SSL_connect(*ssl) == -1) {
      error = get_error("SSL_connect() failed.");
    }

    char *str_request = NULL;

    int request_len = __httpsRequest_stringify(request, &str_request);
    status = SSL_write(*ssl, str_request, request_len);

    free(str_request);
    if (status == -1) {
      error = get_error("ssl_write() failed.");
    }
    break;
  }
  if (add_info != NULL)
    freeaddrinfo(add_info);

  return error;
}

char* __httpsRequest_method_to_string(HttpsRequest_method method) {
    char* methods[] = {
        [GET] = "GET",
        [POST] = "POST",
        [PUT] = "PUT",
        [DELETE] = "DELETE",
        [PATCH] = "PATCH",
        [HEAD] = "HEAD",
        [OPTIONS] = "OPTIONS",
        [CONNECT] = "CONNECT",
        [TRACE] = "TRACE"
    };

    if (method >= GET && method <= TRACE) {
        return methods[method];
    } else {
        return NULL; // Invalid method
    }
}

 Request *__httpsRequest_set_method(Request *request, HttpsRequest_method method){
  Message *message = GET_MESSAGE_FROM_REQUEST(request);
  sprintf(message->method, "%s", __httpsRequest_method_to_string(method));
  return request;
}
  Request *__httpsRequest_set_body(Request *request, char* body){
  Message *message = GET_MESSAGE_FROM_REQUEST(request);
  message->body = body;
  return request;
}

Request *__httpsRequest_add_header(Request *request, char* header){
  Message *message = GET_MESSAGE_FROM_REQUEST(request);
  int header_count = message->header_count;
  if(header_count < 10){
    message->header_count ++;
    sprintf(message->headers[header_count],"%s",header); 
  }
  return request;
}

 void *__httpsRequest_get_ssl(Request* request){
  Message *message = GET_MESSAGE_FROM_REQUEST(request);
  return message->ssl;
}

void __httpsRequest_cleanup(Request*request){
  Message *message = GET_MESSAGE_FROM_REQUEST(request);
  if(message->ssl!=NULL) SSL_shutdown(message->ssl);
  if(ISVALIDSOCKET(message->sockfd)) CLOSESOCKET(message->sockfd);
}

