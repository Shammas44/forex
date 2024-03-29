#include "json.h"
#include "RuntimeError.h"
#include "common.h"
#include "item.h"
#include "Array.h"
#include "Hashmap.h"
#define T JsonParser

enum { KEY = 1, VALUE = 0 };
// =========================================================================="
// Prototypes functions
// =========================================================================="

void handle_ticks(char *json, jsmntok_t **tokens, int token_num);
Hashmap *__json_parse_object(char *json, jsmntok_t *tokens, int *token_num);
Hashmap* __json_to(char *json, jsmntok_t *tokens, int token_num);
void *__json_extract_string(char*json,jsmntok_t*token);
void __jsonParser_destructor(T *parser);
void* __jsonParser_parse(T *parser,char*input);

// =========================================================================="
// Public functions
// =========================================================================="


T * jsonParser_constructor(){
  T *self = malloc(sizeof(T));
  self->destructor = __jsonParser_destructor;
  self->parse = __jsonParser_parse;
  return self;
}

void __jsonParser_destructor(T *parser){
  free(parser);
}

void* __jsonParser_parse(T *parser,char*input){
  jsmntok_t *tokens = NULL;
  int token_num = json_parse(input, &tokens);
  void*value =NULL;
  if(token_num <= 1) return NULL;
  jsmntype_t type = tokens[0].type;
  switch (type) {
    case JSMN_OBJECT:
      json_to_map(input, (Hashmap**)&value,NULL,0);
    break;
    case JSMN_ARRAY:
      json_to_array(input, (Array**)&value,NULL,0);
    break;
    case JSMN_STRING:
      value = NULL;
    break;
    case JSMN_PRIMITIVE:
      value = NULL;
    break;
    case JSMN_UNDEFINED:
      value = NULL;
    break;
  }
  return value;
}

int json_cmp_token_to_string(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

void json_print_token(const char *key, jsmntok_t *tokens, int i, char *json) {
  int token_start = tokens[i + 1].start;
  int token_end = tokens[i + 1].end;
  int token_length = token_end - token_start;
  char *token_string = json + token_start;
  printf("%s: %.*s\n", key, token_length, token_string);
}

void json_extract_token(jsmntok_t *tokens, int i, char *json, void *res,
                        Convert_callback callback) {
  int token_start = tokens[i + 1].start;
  int token_end = tokens[i + 1].end;
  int token_length = token_end - token_start;
  char *token_string = json + token_start;
  callback(res, token_length, token_string);
}

void to_char(void *res, int token_length, char *token_string) {
  char *char_res = (char *)res;
  for (int i = 0; i < token_length; i++) {
    char_res[i] = token_string[i];
  }
  char_res[token_length] = '\0';
}

void to_float(void *res, int token_length, char *token_string) {
  float *float_res = (float *)res;
  *float_res = atof(token_string);
}

void to_double(void *res, int token_length, char *token_string) {
  double *double_res = (double *)res;
  *double_res = atof(token_string);
}

void to_int(void *res, int token_length, char *token_string) {
  int *int_res = (int *)res;
  *int_res = atoi(token_string);
}

void to_bool(void *res, int token_length, char *token_string) {
  bool *bool_res = (bool *)res;
  char value = token_string[0];
  if (value == '1') {
    *bool_res = true;
  } else {
    *bool_res = false;
  }
}

void to_time(void *res, int token_length, char *token_string) {
  time_t *time_res = (time_t *)res;
  char timestamp[25];
  if (token_length < sizeof(timestamp)) { // Ensure we don't overrun our buffer
    to_char(timestamp, token_length, token_string);
    *time_res = iso8601_to_epoch(timestamp);
  } else {
    // Handle error, token string is too long
    *time_res = 0; // or some error value
  }
}

int json_parse(char *json, jsmntok_t **tokens) {
  int token_num = 0;
  jsmn_parser parser;
  jsmn_init(&parser);

  int required_tokens = jsmn_parse(&parser, json, strlen(json), NULL, 0);

  *tokens = malloc(sizeof(jsmntok_t) * required_tokens);
  if (!*tokens) {
    RUNTIME_ERROR("Memory allocation failed",1);
    return 1;
  }

  jsmn_init(&parser);
  token_num = jsmn_parse(&parser, json, strlen(json), *tokens, required_tokens);

  if (token_num < 0) {
    free(*tokens);
    *tokens = NULL;
    RUNTIME_ERROR("Failed to parse JSON",1);
    return 1;
  }

  if (token_num < 1 ) {
    free(*tokens);
    *tokens = NULL;
    RUNTIME_ERROR("Nothing to parse",1);
    return 1;
  }

  return token_num;
}

int json_to_map(char *json, Hashmap**map,jsmntok_t *tokens, int token_num) {
  char *key, *value;
  if(tokens ==NULL && token_num == 0){
   tokens = NULL;
   token_num = json_parse(json, &tokens);
  }
  if(tokens[0].type != JSMN_OBJECT) return 0;
  if (token_num <= 0) return -1;
  *map = __json_to(json, tokens, token_num);
  return token_num;
}

int json_to_array(char *json, Array**array,jsmntok_t *tokens, int token_num){
  char *key;
  void* value = NULL;
  if(tokens ==NULL && token_num == 0){
   tokens = NULL;
   token_num = json_parse(json, &tokens);
  }
  if(tokens[0].type != JSMN_ARRAY) return 0;
  if (token_num <= 0) return 0;
  *array = array_constructor(token_num);
  if(array == NULL) return 0;
  for (int i = 1; i < token_num; i++) {
    int inner_token_num = 0;
    char*string = __json_extract_string(json,&tokens[i]);

    if(string == NULL) return 0;

    if(tokens[i].type == JSMN_OBJECT){
      inner_token_num = json_to_map(string, (Hashmap**)&value,NULL,0);
      inner_token_num--;
    }
    else if(tokens[i].type == JSMN_ARRAY){
      inner_token_num = json_to_array(string, (Array**)&value,NULL,0);
      inner_token_num--;
    }else {
      value = string;
    }
    (*array)->push(*array,(Item){.type=Item_string,.value=value});
    i+=inner_token_num;
  }
  return token_num;
}

void *__json_extract_string(char*json,jsmntok_t*token){
      int token_start = token->start;
      int token_end = token->end;
      int token_length = token_end - token_start;
      if (token_length <= 0)  return NULL; 
      char *token_string = json + token_start;
      char *string = malloc(token_length + 1);
      if (string == NULL)  return NULL; 
      strncpy(string, token_string, token_length);
      string[token_length] = '\0';
      return string;
}

Hashmap* __json_to(char *json, jsmntok_t *tokens, int token_num) {
  Hashmap *map = hashmap_constructor(100);
  Hashmap*inner_map = NULL; 
  Array*inner_array = NULL; 
  void*value = NULL;
  char *key = NULL;
  if (map == NULL) return NULL;

  for (int i = 1; i < token_num; i++) {
    int inner_token_num = 0;
      char*x = __json_extract_string(json,&tokens[i]);
    if (tokens[i].type == JSMN_STRING && tokens[i + 1].type != JSMN_UNDEFINED) {
      // If the current token is a string and the next token exists
      // Then it's a key-value pair
      char*key = __json_extract_string(json,&tokens[i]);
      i++;
      char *value = __json_extract_string(json,&tokens[i]);
      // printf("%s: %s\n", key, value);

      Item item;
      void *primitive;
      Hashmap_Entry primitive_entry = (Hashmap_Entry){.key=key};
      if(tokens[i].type == JSMN_PRIMITIVE){
        if(strcasecmp(value, "true") == 0 || strcasecmp(value, "false") == 0){
          primitive = malloc(sizeof(bool));
          *(bool*)primitive = strcasecmp(value, "true") == 0 ? true : false;
          primitive_entry.type = Item_bool;
          primitive_entry.value = primitive;
        }else if(strcasecmp(value, "null") == 0){
          primitive = malloc(sizeof(char*) *5);
          sprintf(primitive,"%s",value);
          primitive_entry.type = Item_null;
          primitive_entry.value = primitive;
        }else {
          primitive = malloc(sizeof(double));
          *(double*) primitive = atof(value);
          primitive_entry.type = Item_double;
          primitive_entry.value = primitive;
        }
        free(value);
      }

      switch (tokens[i].type) {
        case JSMN_STRING:
          map->push(map,(Hashmap_Entry){.key=key,.type=Item_string,.value=value});
          break;
        case JSMN_PRIMITIVE:
          map->push(map,primitive_entry);
          break;
        case JSMN_OBJECT:
          inner_token_num = json_to_map(value, &inner_map,NULL,0);
          inner_token_num--;
          map->push(map,(Hashmap_Entry){.key=key,.type=Item_map,.value=inner_map});
          break;
        case JSMN_ARRAY:
          inner_token_num = json_to_array(value,&inner_array,NULL,0);
          inner_token_num--;
          map->push(map,(Hashmap_Entry){.key=key,.type=Item_array,.value=inner_array});
          break;
        default:
          RUNTIME_ERROR("Unhandled type",1);
          break;
      }
      i+=inner_token_num;
    }
  }
  return map;
}

// =========================================================================="
// Private functions
// =========================================================================="
