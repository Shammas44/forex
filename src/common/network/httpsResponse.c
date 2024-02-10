#include <stdio.h>
#include <fcntl.h>
#include "httpsResponse.h"
#include "httpsResponse.keys.h"
#include "common.h"
#include "HttpsParser.h"
#include "Hashmap.h"
#define T HttpsResponse

// =========================================================================="
// Data structures
// =========================================================================="

typedef struct {
  Hashmap *map;
} Private;

// =========================================================================="
// Prototypes functions
// =========================================================================="

static void __destructor(T *self);
static char* __stringify(T *self);
static char* __status(T *self);
static int __read_response_bytes(SSL *ssl, char **response_ptr, size_t *response_size, size_t *response_capacity);
static Hashmap * __headers(T *self);
static Item __header(T *self, char*key);
static char * __body(T *self);
static char *__content_type(T *self);

static void _$cleanup(int headers_count, char **keys, Item **values);

// =========================================================================="
// Public functions
// =========================================================================="

T *httpsResponse_constructor(Hashmap*map) {
  T *self = malloc(sizeof(T));
  if(map == NULL){
    RUNTIME_ERROR("Hashmap is NULL",1);
    return NULL;
  }
  Hashmap *headers = map->get(map,KEY(Headers)).value;
  char*status = map->get(map,KEY(Status_Code)).value;

  if(strcmp(status,"101") != 0){
    if(headers->get(headers,KEY(Content_Length)).value == NULL){
      RUNTIME_ERROR("Invalid Hashmap",1);
      return NULL;
    };
    if(headers->get(headers,KEY(Content_Type)).value == NULL){
      RUNTIME_ERROR("Invalid Hashmap",1);
      return NULL;
    };
  }
  Private *private = malloc(sizeof(Private));
  private->map = map;

  self->__destructor = (IsDestroyable){.destructor = __destructor};
  self->destructor = __destructor;
  self->stringify = __stringify;
  self->status = __status;
  self->body = __body;
  self->headers = __headers;
  self->header = __header;
  self->content_type = __content_type;
  self->__private = private;
  return self;
}

// =========================================================================="
// Private functions
// =========================================================================="

static void __destructor(T *self) {
  Private *private = self->__private;
  Hashmap*map = private->map;
  map->destructor(map);
  free(private);
  free(self);
}

static char* __stringify(T *self) {
  Private *private = self->__private;
  Hashmap*map = private->map;
  Hashmap*headers = map->get(map,KEY(Headers)).value;
  char**keys = headers->keys(headers);
  Item**values = headers->values(headers);
  char *out = NULL;

  char*protocol_name = map->get(map,KEY(Protocol_Name)).value;
  char*protocol_version = map->get(map,KEY(Protocol_Version)).value;
  char*status_code = map->get(map,KEY(Status_Code)).value;
  char*status_message = map->get(map,KEY(Status_Message)).value;
  size_t headers_count = headers->length(headers);

    // + 5 for '/', ' ', ' ', '\r', '\n'
    size_t status_length = 
      + strlen(protocol_name)
      + strlen(protocol_version)
      + strlen(status_code)
      + strlen(status_message)+ 5;

  char * status = malloc(status_length * sizeof(char));
  snprintf(status, status_length, "%s/%s %s %s\r", protocol_name, protocol_version,status_code, status_message);

    size_t headers_length = 0;

    for (int i = 0; i < headers_count; i++) {
        // +4 for '\n', ' ', ':', '\r,
        headers_length += strlen(keys[i]) + strlen(values[i]->value) + 4; 
    }
    // Allocate memory for headers with newlines and null terminator
    char *raw_headers = malloc(headers_length + 1);
    if (raw_headers == NULL) {
        free(status);
        goto clean;
    }

    char *p = raw_headers;

    for (int i = 0; i < headers_count; i++) {
        // +5 for ': ', '\r', '\n', ' ', null terminator
        size_t header_length = strlen(keys[i]) + strlen(values[i]->value) +5;
        snprintf(p, header_length, "%s: %s\r\n", keys[i],(char*)values[i]->value); 
        p += header_length -1; // Move the pointer forward to the null terminator
    }
    *p = '\0'; // Null-terminate the string

    // Calculate total length including headers, body, and extra newline characters
    char *body = __body(self);
    size_t body_length = body != NULL ? strlen(body) : 0;
    size_t total_length = status_length + headers_length + body_length +4 ; // 2 newlines + 2 carriage returns

    // Allocate memory for the final output string
    out = malloc(total_length + 1); // +1 for null terminator
    if (out == NULL) {
        goto clean;
    }

    // Construct the final output string
    snprintf(out, total_length + 1, "%s\n%s\r\n\r\n%s", status, raw_headers, (body != NULL) ? body : "");

    // Free allocated memory
    free(raw_headers);
    free(status);

    // Clean up
    _$cleanup(headers_count, keys, values);

    return out;

clean:
    _$cleanup(headers_count, keys, values);
    free(out);
    return NULL;
}

static void _$cleanup(int headers_count, char **keys, Item **values) {
    for (int i = 0; i < headers_count; i++) {
        free(values[i]);
        free(keys[i]);
    }
}

static Item __header(T *self, char*key){
  Private *private = self->__private;
  Hashmap *map = private->map;
  Hashmap *headers = map->get(map,KEY(Headers)).value;
  Item item = headers->get(headers,key);
  return item;
}

static char *__status(T *self) {
  Private *private = self->__private;
  Hashmap *map = private->map;
  return map->get(map,KEY(Status_Code)).value;
}

static char *__body(T *self) {
  Private *private = self->__private;
  Hashmap *map = private->map;
  return map->get(map,KEY(Body)).value;
}

static Hashmap *__headers(T *self) {
  Private *private = self->__private;
  Hashmap *map = private->map;
  Hashmap *headers = map->get(map,KEY(Headers)).value;
  return headers;
}

static char *__content_type(T *self) {
  Private *private = self->__private;
  Hashmap *map = private->map;
  Hashmap *headers = map->get(map,KEY(Headers)).value;
  return headers->get(headers,KEY(Content_Type)).value;
}

#undef T
