#include "CsvParser.h"
#include "Array.h"
#include "Hashmap.h"
#include "Message.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define T CsvParser

static void __destructor(T *csvParser);
static int __parse_stream(T *parser, void *file_path, void *caller,
                          Parser_on_data on_data);
static void __config(T *parser, CsvParser_config *config);
static void *__parse(T *self, char *input);

static bool _$can_be_double(char *p);
static Hashmap_Entry _$create_entry(char *key, char *value);
static int _$row_number(char *input);
static Array *_$headers(char *row, char *separator, int *columns);
static char *_$delimiter(T *self);
static void* _$parse_file(T*self,char*filePath);
static void *_$parse_string(T *self, char *input);
static Message* _$message_constructor(Hashmap*value);
static Hashmap *_$parse_row(char *token, Array *array, char *separator,
                            int columns);

typedef struct {
  char *delimiter;
  CsvParser_type type;
} Private;

T *csvParser_constructor() {
  T *self = malloc(sizeof(T));
  self->destructor = __destructor;
  self->parse_stream = __parse_stream;
  self->parse = __parse;
  self->config = (Parser_config *)__config;
  Private *private = malloc(sizeof(Private));
  self->__private = private;
  return self;
}

static void __destructor(T *self) { free(self); }

static void __config(T *self, CsvParser_config *config) {
  Private *private = self->__private;
  private->delimiter = strdup(config->delimiter);
  private->type = config->type;
}

static char *_$delimiter(T *self) {
  char *separator = ((Private *)self->__private)->delimiter;
  char *value = separator == NULL ? "," : separator;
  return value;
}

static void *__parse(T *self, char *input) {
  CsvParser_type type = ((Private *)self->__private)->type;
  switch (type) {
    case CsvParser_type_string:
      return _$parse_string(self, input);
    case CsvParser_type_file:
      return _$parse_file(self, input);
  }
}

static void *_$parse_string(T *self, char *input) {
  char *separator = _$delimiter(self);
  int row_number = _$row_number(input);

  Array *array = array_constructor(row_number);
  int columns = 0;

  Array *headers = _$headers(input, separator, &columns);
  Hashmap *map = hashmap_constructor(columns * 2);
  char* row_p = NULL;

  char *row = strtok_r(input, "\n", &row_p);
  while (row != NULL) {
    row = strtok_r(NULL, "\n",&row_p);
    if (row != NULL ) {
      long len = strlen(row);
      char *substring = malloc(sizeof(char) *len);
      sprintf(substring, "%.*s", (int)len, row);
      Hashmap *map = _$parse_row(substring, headers, separator, columns);
      array->push(array,(Item){.type=Item_map,.value=map});
      free(substring);
    }
  }
  free(headers);
  return array;
}

static void* _$parse_file(T*self,char*filePath){
  char *separator = _$delimiter(self);
  FILE *fp = fopen(filePath, "r");
  char line[1024];
  Array *headers = NULL;
  int columns = 0;

  if (!fp) {
    RUNTIME_ERROR("Unable to open file", 1);
    return NULL;
  }

  if (fgets(line, sizeof(line), fp) != NULL) {
    headers = _$headers(line, separator, &columns);
  }

  Array*array = array_constructor(columns);

  while (fgets(line, sizeof(line), fp) != NULL) {
    Hashmap *map = _$parse_row(line, headers, separator, columns);
    array->push(array,(Item){.type=Item_map,.value=map});
  }

  headers->destructor(headers);
  fclose(fp);
  return array;
}

static Array *_$headers(char *string, char *separator, int *columns) {
  Array *array = array_constructor(20);
  char *copy = strdup(string);
  char *token = strtok(copy, "\n");
  token = strtok(token, separator);
  while (token != NULL) {
    (*columns)++;
    char *key =
        malloc(sizeof(char) *
               (strlen(token) + 1)); // Allocate memory for null terminator
    if (key == NULL) {
      RUNTIME_ERROR("Memory allocation failed", 1);
      free(copy);
      return NULL;
    }
    if ('\n' == token[strlen(token) - 1]) {
      strncpy(key, token, strlen(token) - 1); // Exclude newline character
    } else {
      strcpy(key, token);
    }
    array->push(array, (Item){.type = Item_string, .value = key});
    token = strtok(NULL, separator);
  }
  free(copy);
  return array;
}

static int _$row_number(char *input) {
  int size = 0;
  char *copy = strdup(input);
  if (copy == NULL)
    return size;
  char *row = strtok(copy, "\n");
  while (row != NULL) {
    size++;
    row = strtok(NULL, "\n");
  }
  free(copy);
  return size - 1;
}

static Hashmap *_$parse_row(char *row, Array *array, char *separator, int columns) {
  Hashmap *map = hashmap_constructor(columns * 2);
  int i = 0;
  char *token = strtok(row, separator);
  while (token != NULL) {
    char *key = array->get(array, i).value;
    char *value = malloc(sizeof(char) * strlen(token));

    if (i + 1 == columns) {
      strncpy(value, token, strlen(token));
      Hashmap_Entry item_value = _$create_entry(key, value);
      map->push(map, item_value);
      return map;
    }
    strcpy(value, token);
    Hashmap_Entry item2 = _$create_entry(key, value);
    map->push(map, item2);
    i++;
    token = strtok(NULL, separator);
  }
  return NULL;
}

static Message* _$message_constructor(Hashmap*value){
  Msg_type code = value == NULL ? Msg_stop :Msg_unknown;
  Item_type type = value == NULL ? Item_null : Item_map;
  Item item = {.value = value, .type = type};
  return message_constructor((Msg_args){.item = item, .code = code});
}

static int __parse_stream(T *self, void *file_path, void *caller, Parser_on_data on_data) {
  char *separator = _$delimiter(self);
  FILE *fp = fopen(file_path, "r");
  char line[1024];
  Array *array = NULL;
  int columns = 0;

  if (!fp) {
    RUNTIME_ERROR("Unable to open file", 1);
    return 1;
  }

  if (fgets(line, sizeof(line), fp) != NULL) {
    array = _$headers(line, separator, &columns);
  }

  while (fgets(line, sizeof(line), fp) != NULL) {
    Hashmap *map = _$parse_row(line, array, separator, columns);
    on_data(caller, _$message_constructor(map));
  }

  on_data(caller, _$message_constructor(NULL));
  array->destructor(array);
  fclose(fp);
  return 0;
}

static bool _$can_be_double(char *value) {
  char *endptr; // Pointer to character after the last converted digit
  double num;
  num = strtod(value, &endptr);
  if (errno == ERANGE || endptr == value || *endptr != '\0')
    return false;
  return true;
}

static Hashmap_Entry _$create_entry(char *key, char *value) {
  bool can_be_double = _$can_be_double(value);
  Hashmap_Entry entry;
  if (can_be_double) {
    double rawValue = atof(value);
    double *p = malloc(sizeof(double));
    *p = rawValue;
    entry = (Hashmap_Entry){.key = key, .type = Item_double, .value = p};
    free(value);
  } else {
    entry = (Hashmap_Entry){.key = key, .type = Item_string, .value = value};
  }
  return entry;
}

#undef T
