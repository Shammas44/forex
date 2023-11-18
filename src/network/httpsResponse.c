#include "httpsResponse.h"
#include <stdio.h>

// =========================================================================="
// Data structures
// =========================================================================="

typedef struct Message {
    char* status;
    char* headers;
    size_t header_count;
    char* body;
    size_t body_length;
    SSL* ssl;
} Message;

// =========================================================================="
// Prototypes functions
// =========================================================================="

int __httpsResponse_receive(struct Response *res);
int __httpsResponse_destructor(struct Response * response);
int __httpsResponse_stringify(struct Response * response, char **out);
void __httpsResponse_print(struct Response *response);
int __httpsResponse_parse(char* raw_response, struct Message *response);
void * __httpsResponse_allocate();
int __httpsResponse_read_response_bytes(SSL *ssl, char **response_ptr, size_t *response_size, size_t *response_capacity);
struct Response *__httpsResponse_set_status(struct Response *res, char* status);
struct Response *__httpsResponse_set_body(struct Response *res, char* body);
struct Response *__httpsResponse_add_header(struct Response *res, char* header);

// =========================================================================="
// Public functions
// =========================================================================="

Response * httpsResponse_constructor(SSL*ssl){
  Response *response = __httpsResponse_allocate();
  HttpsResponse *httpsResponse = malloc(sizeof(HttpsResponse));
  response->receive_func = __httpsResponse_receive;
  response->allocate_func = __httpsResponse_allocate;
  response->destructor_func = __httpsResponse_destructor;
  response->stringify_func = __httpsResponse_stringify; 
  response->print_func = __httpsResponse_print;
  response->__private = httpsResponse;
  HttpsResponse *https_response = (HttpsResponse *)response->__private;
  Message *message = malloc(sizeof(Message));
  https_response->message = message;
  message->ssl = ssl;
  message->body = NULL;
  message->headers = NULL;
  message->status = NULL;
  message->body_length = 0;
  message->header_count = 0;
  return response;
}

// =========================================================================="
// Private functions
// =========================================================================="

void * __httpsResponse_allocate(){
  void *response = malloc(sizeof(Response));
  return response;
}

int __httpsResponse_parse(char* raw_response, Message *response){
  // Find the first occurrence of two new lines, this separates headers and body
  char* separator = strstr(raw_response, "\r\n\r\n");
  if (separator == NULL) {
    fprintf(stderr, "Could not find separator between headers and body\n");
    return -1;
  }

  // Calculate the header length and copy headers to struct
  int header_length = separator - raw_response;
  response->headers = malloc((header_length + 1)*sizeof(char));
  if (response->headers == NULL) {
    fprintf(stderr, "Memory allocation error\n");
    return -1;
  }
  strncpy(response->headers, raw_response, header_length);
  response->headers[header_length] = '\0';

  // Copy body to struct
  response->body = strdup(separator + 4);

  // Extract status from headers
  char* status_start = strstr(response->headers, " ");
  char* status_end = strstr(status_start + 1, " ");
  int status_length = status_end - status_start;
  response->status = malloc(status_length + 1);
  if (response->status == NULL) {
    fprintf(stderr, "Memory allocation error\n");
    return -1;
  }
  strncpy(response->status, status_start, status_length);
  response->status[status_length] = '\0';
  return 0;
}

int __httpsResponse_destructor(struct Response *response){
  Response *res = (Response *)response;
  HttpsResponse *private = (HttpsResponse *)res->__private;
  Message *message = (Message *)private->message;
  free(message->status);
  free(message->headers);
  free(message->body);
  free(message);
  free(res);
  free(private);
  return 0;
}

int __httpsResponse_stringify(struct Response *response, char **out) {
    Response *res = (Response *)response;
    HttpsResponse *httpsResponse = (HttpsResponse *)res->__private;
    Message *msg = (Message *)httpsResponse->message;
    // Assuming the size needed for the string is the sum of the lengths of headers and body + 4 (for two newlines)
    size_t headers_length = msg->headers != NULL ? strlen(msg->headers) :0;
    size_t body_length = msg->body != NULL ? strlen(msg->body) :0;
    size_t length = headers_length + body_length + 4;
    if(length == 4) return -1;

    *out = malloc(length * sizeof(char));
    if (*out == NULL) {
        return -1;
    }
    // Use snprintf to prevent buffer overflow
    snprintf(*out, length, "%s\n\n%s\n", msg->headers, msg->body);
    return 0;
}

 void __httpsResponse_print(struct Response *response){
  char *string;
  Response *res = (Response *)response;
  HttpsResponse *httpsResponse = (HttpsResponse *)res->__private;
  Message * msg = (Message*)httpsResponse->message;
  int status = __httpsResponse_stringify(response, &string);
  if(status){
    printf("Response is empty\n");
  }else {
    printf("%s\n", string);
    free(string);
  }
}

int __httpsResponse_extract_authorization(Response *res,char **authorization){
  HttpsResponse *private = (HttpsResponse *)res->__private;
  Message *message = (Message *)private->message;
  jsmn_parser parser;
  jsmn_init(&parser);
  char *json = message->body;
  int required_tokens = jsmn_parse(&parser, json, strlen(json), NULL, 0);
  jsmntok_t *tokens = malloc(sizeof(jsmntok_t) * required_tokens);
  jsmn_init(&parser);
  int token_num = jsmn_parse(&parser, json, strlen(json), tokens, required_tokens);
  if (token_num < 0)  return get_error("Failed to parse JSON"); 
  if (token_num < 1 || tokens[0].type != JSMN_OBJECT) {
    return get_error("jsmn expected another type of data");
  }

  bool found = false;
  for (int i = 1; i < token_num && !found; i++) {
    if (tokens[i].type == JSMN_STRING && json_cmp_token_to_string(json, &tokens[i], "token") == 0) {
      i++; 

      int token_length = tokens[i].end - tokens[i].start;
      *authorization = malloc(
          sizeof(char) * (token_length + strlen("Authorization: Bearer ") + 1));
      if (*authorization == NULL) {
        printf("Memory allocation failed!\n");
        break; 
      }

      int t = sprintf(*authorization, "Authorization: Bearer %.*s", token_length, json + tokens[i].start);
      // *authorization[t + 1] = '\0';
      (*authorization)[t] = '\0';
      found = true;
    }
  }
  return 0;
}

 struct Response *__httpsResponse_set_status(struct Response *res, char* status);
 struct Response *__httpsResponse_set_body(struct Response *res, char* body);
 struct Response *__httpsResponse_add_header(struct Response *res, char* header);

int __httpsResponse_receive(struct Response * res) {
  Response * response = (Response*)res;
  HttpsResponse *private = (HttpsResponse *)response->__private;
  Message *message = (Message *)private->message;
  SSL *ssl = message->ssl;
  size_t response_size = 0;
  size_t response_capacity = 4096;
  char *raw_response = buffer_init(response_capacity);
  if (!response)  return get_error("malloc() failed."); 
  int status = __httpsResponse_read_response_bytes(ssl, &raw_response, &response_size, &response_capacity);
  if (status) return status; 
  __httpsResponse_parse(raw_response, message);
  free(raw_response);
  return 0;
}

int __httpsResponse_read_response_bytes(SSL *ssl, char **response_ptr, size_t *response_size, size_t *response_capacity) {
    ssize_t bytes_received;

    while ((bytes_received = SSL_read(ssl, *response_ptr + *response_size, *response_capacity - *response_size)) > 0) {
        // system("clear");
        // printf("%s\n", *response_ptr);
        *response_size += bytes_received;
        if (*response_size == *response_capacity) {
            *response_capacity *= 2; // Double the capacity
            char *new_response = realloc(*response_ptr, *response_capacity);
            if (!new_response) {
                free(*response_ptr);
                return get_error("realloc() failed.");
            }
            *response_ptr = new_response;
        }
    }
    
    if (bytes_received < 0) {
        free(*response_ptr);
        return get_error("SSL_read() failed.");
    }
    
    (*response_ptr)[*response_size] = '\0';
    return 0;
}
