#include <stdio.h>
#include "httpsResponse.h"
#include "hashmap.h"

// =========================================================================="
// Data structures
// =========================================================================="

struct Message {
  char *status;
  Hashmap *headers;
  char *body;
  size_t body_length;
  SSL *ssl;
};

// =========================================================================="
// Prototypes functions
// =========================================================================="

int __httpsResponse_receive(Response *res);
int __httpsResponse_destructor(Response *response);
int __httpsResponse_stringify(Response *response, char **out);
void __httpsResponse_print(Response *response);
int __httpsResponse_parse(char *raw_response, Message *response);
int __httpsResponse_read_response_bytes(SSL *ssl, char **response_ptr, size_t *response_size, size_t *response_capacity);
Response *__httpsResponse_set_status(Response *res, char *status);
Response *__httpsResponse_set_body(Response *res, char *body);
Response *__httpsResponse_add_header(Response *res, char *header);
Hashmap* __httpsResponse_extract_headers(const char *rawResponse);

// =========================================================================="
// Public functions
// =========================================================================="

Response *httpsResponse_constructor(SSL *ssl) {
  Response *response = malloc(sizeof(Response));
  HttpsResponse *httpsResponse = malloc(sizeof(HttpsResponse));
  response->receive_func = __httpsResponse_receive;
  response->destructor_func = __httpsResponse_destructor;
  response->stringify_func = __httpsResponse_stringify;
  response->print_func = __httpsResponse_print;
  response->__private = httpsResponse;
  HttpsResponse *https_response = response->__private;
  Message *message = malloc(sizeof(Message));
  https_response->message = message;
  message->ssl = ssl;
  message->body = NULL;
  message->headers = NULL;
  message->status = NULL;
  message->body_length = 0;
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
  // Find the first occurrence of two new lines, this separates headers and body
  char *separator = strstr(raw_response, "\r\n\r\n");
  if (separator == NULL) {
    fprintf(stderr, "Could not find separator between headers and body\n");
    return -1;
  }
  Hashmap *headers = __httpsResponse_extract_headers(raw_response);
  response->headers = headers;

  response->body = strdup(separator + 4);

  char *status_start = strstr(raw_response, " ");
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

int __httpsResponse_destructor(Response *response) {
  HttpsResponse *private = response->__private;
  Message *message = private->message;
  Hashmap *headers = message->headers;
  free(message->status);
  message->headers->destructor(headers);
  free(message->body);
  free(message);
  free(response);
  free(private);
  return 0;
}

int __httpsResponse_stringify(Response *response, char **out) {
    HttpsResponse *httpsResponse = response->__private;
    Message *msg = httpsResponse->message;

    char **headers = __hashmap_entries(msg->headers);
    size_t headers_length = 0;

    for (int i = 0; i < msg->headers->size; i++) {
        headers_length += strlen(headers[i]) + 1; // +1 for newline character
    }

    // Allocate memory for headers with newlines and null terminator
    char *raw_headers = malloc(headers_length + 1);
    if (raw_headers == NULL) {
        // Handle allocation failure
        return -1;
    }

    char *p = raw_headers;

    for (int i = 0; i < msg->headers->size; i++) {
        size_t header_length = strlen(headers[i]);
        snprintf(p, header_length + 2, "%s\n", headers[i]); // +2 for newline and null terminator
        p += header_length + 1; // Move the pointer forward
    }
    *p = '\0'; // Null-terminate the string

    // Calculate total length including headers, body, and extra newline characters
    size_t body_length = (msg->body != NULL) ? strlen(msg->body) : 0;
    size_t total_length = headers_length + body_length + 2; // +2 for two newlines

    // Allocate memory for the final output string
    *out = malloc(total_length + 1); // +1 for null terminator
    if (*out == NULL) {
        free(raw_headers); // Free allocated memory if out allocation fails
        return -1;
    }

    // Construct the final output string
    snprintf(*out, total_length + 1, "%s\n\n%s", raw_headers, (msg->body != NULL) ? msg->body : "");

    // Free allocated memory and cleanup
    for (int i = 0; i < msg->headers->size; i++) {
        free(headers[i]);
    }
    free(raw_headers);

    return 0;
}

void __httpsResponse_print(Response *response) {
  char *string;
  HttpsResponse *httpsResponse = response->__private;
  Message *msg = httpsResponse->message;
  int status = __httpsResponse_stringify(response, &string);
  if (status) {
    printf("Response is empty\n");
  } else {
    printf("%s\n", string);
    free(string);
  }
}

int __httpsResponse_extract_authorization(Response *response, char **authorization) {
  HttpsResponse *private = response->__private;
  Message *message = private->message;
  jsmn_parser parser;
  jsmn_init(&parser);
  char *json = message->body;
  int required_tokens = jsmn_parse(&parser, json, strlen(json), NULL, 0);
  jsmntok_t *tokens = malloc(sizeof(jsmntok_t) * required_tokens);
  jsmn_init(&parser);
  int token_num =
      jsmn_parse(&parser, json, strlen(json), tokens, required_tokens);
  if (token_num < 0)
    return get_error("Failed to parse JSON");
  if (token_num < 1 || tokens[0].type != JSMN_OBJECT) {
    return get_error("jsmn expected another type of data");
  }

  bool found = false;
  for (int i = 1; i < token_num && !found; i++) {
    if (tokens[i].type == JSMN_STRING &&
        json_cmp_token_to_string(json, &tokens[i], "token") == 0) {
      i++;

      int token_length = tokens[i].end - tokens[i].start;
      *authorization = malloc(
          sizeof(char) * (token_length + strlen("Authorization: Bearer ") + 1));
      if (*authorization == NULL) {
        printf("Memory allocation failed!\n");
        break;
      }

      int t = sprintf(*authorization, "Authorization: Bearer %.*s",
                      token_length, json + tokens[i].start);
      // *authorization[t + 1] = '\0';
      (*authorization)[t] = '\0';
      found = true;
    }
  }
  return 0;
}

Response *__httpsResponse_set_status(Response *res, char *status);
Response *__httpsResponse_set_body(Response *res, char *body);
Response *__httpsResponse_add_header(Response *res, char *header);

int __httpsResponse_receive(Response *response) {
  HttpsResponse *private = response->__private;
  Message *message = private->message;
  SSL *ssl = message->ssl;
  size_t response_size = 0;
  size_t response_capacity = 4096;
  char *raw_response = buffer_init(response_capacity);
  if (!response)
    return get_error("malloc() failed.");
  int status = __httpsResponse_read_response_bytes(
      ssl, &raw_response, &response_size, &response_capacity);
  if (status)
    return status;
  __httpsResponse_parse(raw_response, message);
  free(raw_response);
  return 0;
}

int __httpsResponse_read_response_bytes(SSL *ssl, char **response_ptr, size_t *response_size, size_t *response_capacity) {
  ssize_t bytes_received;

  while ((bytes_received = SSL_read(ssl, *response_ptr + *response_size,
                                    *response_capacity - *response_size)) > 0) {
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
