#include "HttpsResponseParser.h"
#include "array.h"
#include "common.h"
#include "hashmap.h"
#include <stdio.h>
#include <string.h>
#define T HttpsResponseParser

typedef struct {
  bool jsonBody;
} Private;

typedef struct {
  char protocol_name[10];
  char protocol_version[10];
  char status_code[4];
  char status_message[50];
} FirstLine;

static void __destructor(T *self);
static int __parse_stream(T *self, void *file_path,void*caller, Parser_on_data on_data);
static void __config(T *self, HttpsResponseParser_config *config);
static void* __parse(T *self, char*input);

static Hashmap* _$extract_headers(const char *rawResponse);
static char *_$copy_str(char *start, char separator);
static FirstLine _$extract_status(const char*response);

T *httpsResponseParser_constructor() {
  T *self = malloc(sizeof(T));
  self->destructor = __destructor;
  self->parse_stream = __parse_stream;
  self->parse = __parse;
  self->config = (Parser_config*)__config;
  self->__private = malloc(sizeof(Private));
  return self;
}

static void __destructor(T *self) { free(self); }

static void __config(T *self, HttpsResponseParser_config *config){
  Private * private = self->__private;
  private->jsonBody = config->jsonBody;
}

static void* __parse(T *self, char*input){
  if(input == NULL){
    return NULL;
  }
  int status = 0;
  // Find the first occurrence of two new lines, this separates headers and body
  char *separator = strstr(input, "\r\n\r\n");
  if (separator == NULL) {
    RUNTIME_ERROR("Could not find separator between headers and body\n",1);
    return NULL;
  }

  Hashmap *map = hashmap_constructor(25);
  Hashmap *headers = _$extract_headers(input);
  FirstLine firstLine = _$extract_status(input);
  map->push(map, "protocol_name",(Item){.type=Item_default,.value=strdup(firstLine.protocol_name)});
  map->push(map, "protocol_version",(Item){.type=Item_default,.value=strdup(firstLine.protocol_version)});
  map->push(map, "status_code",(Item){.type=Item_default,.value=strdup(firstLine.status_code)});
  map->push(map, "status_message",(Item){.type=Item_default,.value=strdup(firstLine.status_message)});
  map->push(map, "headers",(Item){.type=Item_map,.value=headers});
  //TODO: parse body
  map->push(map, "body",(Item){.type=Item_default,.value=strdup(separator + 4)});
  return map;
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
      char *value = _$copy_str(colon + 2, '\0');
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

static FirstLine _$extract_status(const char*response){
    FirstLine out = {0};
    const char *newlinePos = strchr(response, '\n');
    
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
  char protocol_name[10];
  char protocol_version[10];
  char status_code[10];
  char status_message[50];

    sscanf(firstLine, "%9s %9s %9s %49[^\n]", out.protocol_name,
           out.protocol_name,
           out.status_code,
           out.status_message
           );

  free(firstLine);

    return out;
}

#undef T
