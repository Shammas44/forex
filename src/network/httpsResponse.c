#include <stdio.h>
#include <fcntl.h>
#include "httpsResponse.h"
#include "hashmap.h"
#define T HttpsResponse
#define MAX_TIMEOUT_SECONDS 5 // Change this according to your desired timeout

// =========================================================================="
// Data structures
// =========================================================================="

struct Message {
  char *status;
  char *status_message;
  char *protocol;
  Hashmap *headers;
  char *body;
  size_t body_length;
  char *content_type;
};

// =========================================================================="
// Prototypes functions
// =========================================================================="

int __httpsResponse_destructor(T *response);
int __httpsResponse_stringify(T *response, char **out);
void __httpsResponse_print(T *response);
char* __httpsResponse_get_status(T *response);
int __httpsResponse_parse(char *raw_response, Message *response);
int __httpsResponse_read_response_bytes(SSL *ssl, char **response_ptr, size_t *response_size, size_t *response_capacity);
T *__httpsResponse_set_status(T *res, char *status);
T *__httpsResponse_set_body(T *res, char *body);
T *__httpsResponse_add_header(T *res, char *header);
Hashmap* __httpsResponse_extract_headers(const char *rawResponse);
Hashmap * __httpsResponse_get_headers(T *response);
char * __httpsResponse_get_header(T *response,const char*);
char * __httpsResponse_get_body(T *response);
char *__httpsResponse_get_content_type(T *response);
int __httpsResponse_parse_status(char *raw_response, Message*response);
int __httpsResponse_parse_content_type(Hashmap*headers, Message*response);
int __httpsResponse_parse_status_message(char *raw_response, Message*response);
int __httpsResponse_parse_protocol(char *raw_response, Message*response);

// =========================================================================="
// Public functions
// =========================================================================="

T *httpsResponse_constructor(char *raw_response) {
  T *response = malloc(sizeof(T));
  Message *message = malloc(sizeof(Message));
  response->destructor = __httpsResponse_destructor;
  response->stringify = __httpsResponse_stringify;
  response->print = __httpsResponse_print;
  response->get_status = __httpsResponse_get_status;
  response->set_status = __httpsResponse_set_status;
  response->set_body = __httpsResponse_set_body;
  response->add_header = __httpsResponse_add_header;
  response->get_body = __httpsResponse_get_body;
  response->get_headers = __httpsResponse_get_headers;
  response->get_header = __httpsResponse_get_header;
  response->get_content_type = __httpsResponse_get_content_type;
  response->__private = message;
  message->body = NULL;
  message->headers = NULL;
  message->status = NULL;
  message->body_length = 0;
  message->content_type = NULL;
  message->protocol = NULL;
  message->status_message = NULL;
  int status = __httpsResponse_parse(raw_response, message);
  return response;
}

// =========================================================================="
// Private functions
// =========================================================================="

char *__copy_str(char *start, char separator) {
  char *end = strchr(start, separator);
  if (end != NULL) {
    size_t length = end - start;
    char *copy = (char *)malloc((length + 1) * sizeof(char));
    if (copy != NULL) {
      strncpy(copy, start, length);
      copy[length] = '\0';
    } else {
      printf("Memory allocation failed.\n");
    }
    return copy;
  }
  return NULL;
}

static char** __hashmap_entries(Hashmap *map){

  if (map == NULL || map->entries == NULL) {
    return NULL;
  }
  char ** out = malloc(map->size * sizeof(char*));
  size_t j = 0;

  for (size_t i = 0; i < map->capacity; ++i) {
    Hashmap_entry entry = map->entries[i];
    if(entry.key == NULL)continue;
    const char* key = entry.key;
    void* value = entry.value;
    if (key != NULL) {
      char * key_copy = entry.key;
      size_t key_length = strlen(entry.key);
      size_t value_length = strlen(entry.value);
      size_t length = key_length + value_length + 3;
      char *key = entry.key;
      char *value = entry.value;
      char *header = malloc(length * sizeof(char));
      snprintf(header, length , "%s: %s", key, value);
      out[j] = header;
      j++;
    }
  }
  return out;
}

Hashmap* __httpsResponse_extract_headers(const char *rawResponse) {
  Hashmap *headers = hashmap_constructor(25);
  char *token;
  char *rest = strdup(rawResponse);
  int index = 0;
  int max_headers = 50;
  bool firstLine = true;

  while ((token = strtok_r(rest, "\n", &rest)) != NULL && index < max_headers) {
    if (firstLine) {
    firstLine = false;
      continue;
    }
    if(token[0] == '\r')break;

    char *colon = strchr(token, ':');
    if (colon != NULL) {
      char *header = __copy_str(token, ':');
      char *value = __copy_str(colon + 2, '\0');
      headers->push(headers, header, value);
      index++;
      free(header);
    }
  }
  return headers;
}

int __httpsResponse_parse(char *raw_response, Message *response) {
  int status = 0;
  // Find the first occurrence of two new lines, this separates headers and body
  char *separator = strstr(raw_response, "\r\n\r\n");
  if (separator == NULL) {
    fprintf(stderr, "Could not find separator between headers and body\n");
    return -1;
  }

  Hashmap *headers = __httpsResponse_extract_headers(raw_response);
  status = __httpsResponse_parse_content_type(headers, response);
  status = __httpsResponse_parse_protocol(raw_response, response);
  status = __httpsResponse_parse_status(raw_response, response);
  status = __httpsResponse_parse_status_message(raw_response, response);
  response->headers = headers;
  response->body = strdup(separator + 4);
  return status;
}

int __httpsResponse_parse_content_type(Hashmap*headers, Message*response){
  char *content_type = headers->get(headers, "Content-Type");
  if (content_type == NULL) {
    const char default_type[] = "None/unspecified";
    size_t default_length = strlen(default_type);
    response->content_type = malloc(default_length + 1);
    //TODO: handle memory allocation failure
    strcpy(response->content_type, default_type);
  } else {
    size_t content_type_length = strlen(content_type);
    response->content_type = malloc(content_type_length + 1);
    //TODO: handle memory allocation failure
    strcpy(response->content_type, content_type);
  }
  return 0;
}

int __httpsResponse_parse_status(char *raw_response, Message*response){
  char *status_start = strstr(raw_response, " ");
  status_start++; //to skip space in front of status code
  char *status_end = strstr(status_start + 1, " ");
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

int __httpsResponse_parse_status_message(char *raw_response, Message*response){
  char *status_start = strstr(raw_response, " ");
  status_start+=5;
  char *status_end = strstr(status_start, "\n");
  int status_length = status_end - status_start;
  response->status_message = malloc(status_length + 1);
  if (response->status_message == NULL) {
    fprintf(stderr, "Memory allocation error\n");
    return -1;
  }
  strncpy(response->status_message, status_start, status_length);
  response->status_message[status_length] = '\0';
  return 0;
}

int __httpsResponse_parse_protocol(char *raw_response, Message*response){
  char *status_start = raw_response;
  char *status_end = strstr(status_start + 1, " ");
  int status_length = status_end - status_start;
  response->protocol = malloc(status_length + 1);
  if (response->protocol == NULL) {
    fprintf(stderr, "Memory allocation error\n");
    return -1;
  }
  strncpy(response->protocol, status_start, status_length);
  response->protocol[status_length] = '\0';
  return 0;
}

int __httpsResponse_destructor(T *response) {
  Message *message = response->__private;
  Hashmap *headers = message->headers;
  if(message->status) free(message->status);
  if(message->headers) message->headers->destructor(headers);
  if(message->body) free(message->body);
  if(message->content_type) free(message->content_type);
  free(message);
  free(response);
  return 0;
}

int __httpsResponse_stringify(T *response, char **out) {
    Message *message = response->__private;

    size_t status_length = 
      + strlen(message->status)
      + strlen(message->status_message)
      + strlen(message->protocol) + 3;

  char * status = malloc(status_length * sizeof(char));
  snprintf(status, status_length, "%s %s %s", message->protocol, message->status, message->status_message);

    char **headers = __hashmap_entries(message->headers);
    size_t headers_length = 0;

    for (int i = 0; i < message->headers->size; i++) {
        headers_length += strlen(headers[i]) + 1; // +1 for newline character
    }

    // Allocate memory for headers with newlines and null terminator
    char *raw_headers = malloc(headers_length + 1);
    if (raw_headers == NULL) {
        // Handle allocation failure
        return -1;
    }

    char *p = raw_headers;

    for (int i = 0; i < message->headers->size; i++) {
        size_t header_length = strlen(headers[i]);
        snprintf(p, header_length + 2, "%s\n", headers[i]); // +2 for newline and null terminator
        p += header_length + 1; // Move the pointer forward
    }
    *p = '\0'; // Null-terminate the string

    // Calculate total length including headers, body, and extra newline characters
    size_t body_length = (message->body != NULL) ? strlen(message->body) : 0;
    size_t total_length = status_length + headers_length + body_length +4 ; // 2 newlines + 2 carriage returns

    // Allocate memory for the final output string
    *out = malloc(total_length + 1); // +1 for null terminator
    if (*out == NULL) {
        free(raw_headers); // Free allocated memory if out allocation fails
        return -1;
    }

    // Construct the final output string
    snprintf(*out, total_length + 1, "%s\n%s\r\n\r\n%s", status, raw_headers, (message->body != NULL) ? message->body : "");

    // Free allocated memory and cleanup
    for (int i = 0; i < message->headers->size; i++) {
        free(headers[i]);
    }
    free(raw_headers);
    free(status);
    return 0;
}

void __httpsResponse_print(T *response) {
  char *string;
  Message *message = response->__private;
  int status = __httpsResponse_stringify(response, &string);
  if (status) {
    printf("Response is empty\n");
  } else {
    printf("%s\n", string);
    free(string);
  }
}

char * __httpsResponse_get_header(T *response,const char*header){
  Hashmap* headers = response->get_headers(response);
  return headers->get(headers,header);
}

T *__httpsResponse_set_status(T *response, char *status){
  return NULL;
}
T *__httpsResponse_set_body(T *response, char *body){
  return NULL;
}
T *__httpsResponse_add_header(T *response, char *header){
return NULL;
}

char *__httpsResponse_get_status(T *response) {
  Message *message = response->__private;
  return message->status;
}

char *__httpsResponse_get_body(T *response) {
  Message *message = response->__private;
  return message->body;
}

Hashmap *__httpsResponse_get_headers(T *response) {
  Message *message = response->__private;
  return message->headers;
}

char *__httpsResponse_get_content_type(T *response) {
  Message *message = response->__private;
  return message->content_type;
}

// int __httpsResponse_extract_authorization(Response *response, char **authorization) {
//   HttpsResponse *private = response->__private;
//   Message *message = private->message;
//   jsmn_parser parser;
//   jsmn_init(&parser);
//   char *json = message->body;
//   int required_tokens = jsmn_parse(&parser, json, strlen(json), NULL, 0);
//   jsmntok_t *tokens = malloc(sizeof(jsmntok_t) * required_tokens);
//   jsmn_init(&parser);
//   int token_num =
//       jsmn_parse(&parser, json, strlen(json), tokens, required_tokens);
//   if (token_num < 0)
//     return get_error("Failed to parse JSON");
//   if (token_num < 1 || tokens[0].type != JSMN_OBJECT) {
//     return get_error("jsmn expected another type of data");
//   }

//   bool found = false;
//   for (int i = 1; i < token_num && !found; i++) {
//     if (tokens[i].type == JSMN_STRING &&
//         json_cmp_token_to_string(json, &tokens[i], "token") == 0) {
//       i++;

//       int token_length = tokens[i].end - tokens[i].start;
//       *authorization = malloc(
//           sizeof(char) * (token_length + strlen("Authorization: Bearer ") + 1));
//       if (*authorization == NULL) {
//         printf("Memory allocation failed!\n");
//         break;
//       }

//       int t = sprintf(*authorization, "Authorization: Bearer %.*s",
//                       token_length, json + tokens[i].start);
//       // *authorization[t + 1] = '\0';
//       (*authorization)[t] = '\0';
//       found = true;
//     }
//   }
//   return 0;
// }

#undef MAX_TIMEOUT_SECONDS 
#undef T
