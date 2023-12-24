#include "httpsRequest.h"

#define T HttpsRequest
#define ISVALIDSOCKET(s) ((s) >= 0)
#define ONERROR(s) ((s) < 0)
#define CLOSESOCKET(s) close(s)
#define GET_MESSAGE_FROM_REQUEST(req) \
({ \
  Message *_message = (Message *)req->__private; \
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
};

// =========================================================================="
// Prototypes functions
// =========================================================================="

void __httpsRequest_destructor(T * request);
int __httpsRequest_stringify(T * request, char **out);
void __httpsRequest_print(T *request);
int __httpsRequest_parse(char* raw_request, Message *request);
char *__httpsRequest_method_to_string(HttpsRequest_method method);
T *__httpsRequest_set_method(T *req, HttpsRequest_method method);
T *__httpsRequest_set_body(T *req, char* body);
T *__httpsRequest_add_header(T *req, char* header);
Url *__httpsRequest_get_url(T* request);

T *httpsRequest_constructor(HttpsRequest_prefill prefill) {
  Message *message = malloc(sizeof(Message));
  T *request = malloc(sizeof(T));
  request->print = __httpsRequest_print;
  request->stringify = __httpsRequest_stringify;
  request->destructor = __httpsRequest_destructor;
  request->set_body = __httpsRequest_set_body;
  request->set_method = __httpsRequest_set_method;
  request->add_header = __httpsRequest_add_header;
  request->get_url = __httpsRequest_get_url;
  request->__private = message;

  //message->url
  Url *url = url_constructor(prefill.url);
  message->url = url;
  sprintf(message->method, "%s", __httpsRequest_method_to_string(prefill.method));
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

void __httpsRequest_destructor(T *request) {
  Message * message = GET_MESSAGE_FROM_REQUEST(request);
  // if(message->ssl!=NULL) SSL_free(message->ssl);
  url_destructor(message->url);
  for (int i = 0; i < 10; i++) {
    free(message->headers[i]);
  }
  free(message->headers);
  if(message->body_length >0)free(message->body);
  free(message);
  free(request);
}

void __httpsRequest_print(T *request){
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

int __httpsRequest_stringify(T *request, char **out) {
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

 T *__httpsRequest_set_method(T *request, HttpsRequest_method method){
  Message *message = GET_MESSAGE_FROM_REQUEST(request);
  sprintf(message->method, "%s", __httpsRequest_method_to_string(method));
  return request;
}
  T *__httpsRequest_set_body(T *request, char* body){
  Message *message = GET_MESSAGE_FROM_REQUEST(request);
  message->body = body;
  message->body_length = strlen(body);
  return request;
}

T *__httpsRequest_add_header(T *request, char* header){
  Message *message = GET_MESSAGE_FROM_REQUEST(request);
  int header_count = message->header_count;
  if(header_count < 10){
    message->header_count ++;
    sprintf(message->headers[header_count],"%s",header); 
  }
  return request;
}

 Url *__httpsRequest_get_url(T* request){
  Message *message = GET_MESSAGE_FROM_REQUEST(request);
  return message->url;
}


#undef T
