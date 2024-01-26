#include "HttpsParser.h"
#include "httpsResponse.keys.h"
#include "array.h"
#include "common.h"
#include "hashmap.h"
#include <stdio.h>
#include <string.h>
#define T HttpsParser

typedef struct {
  bool jsonBody;
  HttpsParser_type type;
} Private;

typedef struct {
  char protocol_name[10];
  char protocol_version[10];
  char status_code[4];
  char status_message[50];
} Response_firstLine;

typedef struct {
char method[10];
char path[50];
char protocol_name[10];
char protocol_version[10];
} Request_firstLine;

static void __destructor(T *self);
static int __parse_stream(T *self, void *file_path,void*caller, Parser_on_data on_data);
static void __config(T *self, HttpsParser_config *config);
static void* __parse(T *self, char*input);

static Hashmap* _$extract_headers(const char *rawResponse);
static char *_$copy_str(char *start, char separator);
static Response_firstLine _$extract_response_status(const char*response);
static Request_firstLine _$extract_request_status(const char*request);
static void* _$parse_request(T *self, char*input);
static void* _$parse_response(T *self, char*input);

T *httpsParser_constructor() {
  T *self = malloc(sizeof(T));
  self->destructor = __destructor;
  self->parse_stream = __parse_stream;
  self->parse = __parse;
  self->config = (Parser_config*)__config;
  Private *private = malloc(sizeof(Private));
  private->jsonBody = false;
  private->type = HttpsParser_response;
  self->__private = private;
  return self;
}

static void __destructor(T *self) { free(self); }

static void __config(T *self, HttpsParser_config *config){
  Private * private = self->__private;
  if(config->jsonBody){
    private->jsonBody = config->jsonBody;
  }
  if(config->type){
    private->type = config->type;
  }
}

static void* __parse(T *self, char*input){
  if(input == NULL){
    return NULL;
  }
  Private * private = self->__private;
  HttpsParser_type type = private->type;
  switch (type) {
    case HttpsParser_request:
      return _$parse_request(self, input);
    case HttpsParser_response:
      return _$parse_response(self, input);
  }
}

static int __parse_stream(T *self, void *file_path,void*caller, Parser_on_data on_data){
  RUNTIME_ERROR("Not implemented",1);
  return 1;
}

static Hashmap* _$extract_headers(const char *rawResponse) {
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
      char *header = _$copy_str(token, ':');
      char *value = _$copy_str(colon + 2, '\r');
      headers->push(headers, header,(Item){.type=Item_default,.value=value});
      index++;
      free(header);
    }
  }
  return headers;
}

char *_$copy_str(char *start, char separator) {
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

static Response_firstLine _$extract_response_status(const char*response){
    Response_firstLine out = {0};
    const char *newlinePos = strchr(response, '\r');
    
    char *firstLine;
    if (newlinePos != NULL) {
        size_t firstLineLength = newlinePos - response;
        firstLine = malloc(firstLineLength + 1); // +1 for null terminator
        if (firstLine != NULL) {
            strncpy(firstLine, response, firstLineLength);
            firstLine[firstLineLength] = '\0'; // Null-terminate the string
        } else {
            printf("Memory allocation failed\n");
            return out;
        }
    } else {
        // Handle if there's no newline character in the response
        printf("No newline character found\n");
        return out;
    }

    sscanf(firstLine, "%9[^/]/%9s %9s %49[^\n]", out.protocol_name,
           out.protocol_version,
           out.status_code,
           out.status_message
           );

  free(firstLine);

    return out;
}

static Request_firstLine _$extract_request_status(const char*request){
    Request_firstLine out = {0};
    const char *newlinePos = strchr(request, '\r');
    
    char *firstLine;
    if (newlinePos != NULL) {
        size_t firstLineLength = newlinePos - request;
        firstLine = malloc(firstLineLength + 1); // +1 for null terminator
        if (firstLine != NULL) {
            strncpy(firstLine, request, firstLineLength);
            firstLine[firstLineLength] = '\0'; // Null-terminate the string
        } else {
            printf("Memory allocation failed\n");
            return out;
        }
    } else {
        // Handle if there's no newline character in the response
        printf("No newline character found\n");
        return out;
    }

  int result = sscanf(
    firstLine,
    "%9s %49s %9[^/]/%9s",
    out.method,
    out.path,
    out.protocol_name,
    out.protocol_version
  );

  free(firstLine);
  return out;
}
static void* _$parse_response(T *self, char*response){
  int status = 0;
  // Find the first occurrence of two new lines, this separates headers and body
  char *separator = strstr(response, "\r\n\r\n");
  if (separator == NULL) {
    RUNTIME_ERROR("Could not find separator between headers and body\n",1);
    return NULL;
  }

  Hashmap *map = hashmap_constructor(25);
  Hashmap *headers = _$extract_headers(response);
  Response_firstLine firstLine = _$extract_response_status(response);
  map->push(map, KEY(Protocol_Name),(Item){.type=Item_default,.value=strdup(firstLine.protocol_name)});
  map->push(map, KEY(Protocol_Version),(Item){.type=Item_default,.value=strdup(firstLine.protocol_version)});
  map->push(map, KEY(Status_Code),(Item){.type=Item_default,.value=strdup(firstLine.status_code)});
  map->push(map, KEY(Status_Message),(Item){.type=Item_default,.value=strdup(firstLine.status_message)});
  map->push(map, KEY(Headers),(Item){.type=Item_map,.value=headers});
  //TODO: parse body
  map->push(map, KEY(Body),(Item){.type=Item_default,.value=strdup(separator + 6)});
  return map;
}

static void* _$parse_request(T *self, char*request){
  int status = 0;
  // Find the first occurrence of two new lines, this separates headers and body
  char *separator = strstr(request, "\r\n\r\n");
  if (separator == NULL) {
    RUNTIME_ERROR("Could not find separator between headers and body\n",1);
    return NULL;
  }

  Hashmap *map = hashmap_constructor(25);
  Hashmap *headers = _$extract_headers(request);
  Request_firstLine firstLine = _$extract_request_status(request);
  map->push(map, KEY(Protocol_Name),(Item){.type=Item_default,.value=strdup(firstLine.protocol_name)});
  map->push(map, KEY(Protocol_Version),(Item){.type=Item_default,.value=strdup(firstLine.protocol_version)});
  map->push(map, KEY(Method_Name),(Item){.type=Item_default,.value=strdup(firstLine.method)});
  map->push(map, KEY(Path),(Item){.type=Item_default,.value=strdup(firstLine.path)});
  map->push(map, KEY(Headers),(Item){.type=Item_map,.value=headers});
  //TODO: parse body
  map->push(map, KEY(Body),(Item){.type=Item_default,.value=strdup(separator + 4)});
  return map;
}

#undef T
