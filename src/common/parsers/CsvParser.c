#include "CsvParser.h"
#include "Array.h"
#include "Message.h"
#include "Hashmap.h"
#include <stdio.h>
#include <string.h>
#define T CsvParser

static void __destructor(T *csvParser);
static int __parse_stream(T *parser, void *file_path,void*caller, Parser_on_data on_data);
static void __config(T *parser, CsvParser_config *config);

typedef struct {
  char*delimiter;
} Private;

T *csvParser_constructor() {
  T *self = malloc(sizeof(T));
  self->destructor = __destructor;
  self->parse_stream = __parse_stream;
  self->config = (Parser_config*)__config;
  self->__private = malloc(sizeof(Private));
  return self;
}

static void __destructor(T *csvParser) { free(csvParser); }

static void __config(T *csvParser, CsvParser_config *config){
  Private * private = csvParser->__private;
  private->delimiter = strdup(config->delimiter);
}

static int __parse_stream(T *parser, void *file_path,void*caller, Parser_on_data on_data){
  char*separator = ((Private*)parser->__private)->delimiter;
  if(separator == NULL) separator = ",";
  FILE *fp = fopen(file_path, "r");
  char line[1024];
  Array *array = array_constructor(20);

  int columns = 0;

  if (!fp) {
    perror("Unable to open file");
    return 1;
  }

  if (fgets(line, sizeof(line), fp) != NULL) {
    char *token = strtok(line, separator);

    while (token != NULL) {
      columns++;
      char *key = malloc(sizeof(char) * strlen(token));
      if ('\n' == token[strlen(token) - 1]) {
        strncpy(key, token, strlen(token) - 2);
        array->push(array, (Item){.type=Item_string,.value=key});
        break;
      }
      strcpy(key, token);
        array->push(array, (Item){.type=Item_string,.value=key});
      token = strtok(NULL, separator);
    }
  }

  Hashmap *map = hashmap_constructor(columns*2);

  while (fgets(line, sizeof(line), fp) != NULL) {
    if (map == NULL) return 1;
    char *token = strtok(line, separator);
    int i = 0;

    while (token != NULL && i < columns) {
      char *key = array->get(array, i).value;
      char *value = malloc(sizeof(char) * strlen(token));

      if ('\n' == token[strlen(token) - 1]) {
        strncpy(value, token, strlen(token) - 2);
        Hashmap_Entry item_value ={.key=key,.type=Item_string,.value=value};
        map->push(map,item_value);
        Item item = {.value = map, .type = Item_map};
        Message *message = message_constructor((Msg_args){.item=item, .code=Msg_unknown});
        on_data(caller,message);
        map = hashmap_constructor(columns*2);
        break;
      }
      strcpy(value, token);
      Hashmap_Entry item2 ={.key=key,.type=Item_string,.value=value};
      map->push(map,item2);
      i++;
      token = strtok(NULL, separator);
    }
  }

  Item item = {.value = NULL, .type = Item_null};
  Message *message = message_constructor((Msg_args){.item={}, .code=Msg_stop});
  on_data(caller,message);
  array->destructor(array);
  fclose(fp);
  return 0;
}

#undef T
