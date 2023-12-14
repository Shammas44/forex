#ifndef JSON_H
#define JSON_H
#include "jsmn.h"
#include "date.h"
#include "error.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

typedef void (*Convert_callback)(void*res, int token_length, char* token_string); 

int json_cmp_token_to_string(const char *json, jsmntok_t *tok, const char *s); 
void json_print_token(const char *key, jsmntok_t *tokens, int i, char *json); 
int json_parse(char*json, jsmntype_t expected_type,jsmntok_t **tokens, int *token_num);
void json_extract_token(jsmntok_t *tokens, int i, char *json, void*res, Convert_callback callback); 
void to_char(void* res, int token_length, char* token_string);
void to_float(void* res, int token_length ,char* token_string);
void to_time(void* res, int token_length ,char* token_string);
void to_bool(void* res, int token_length ,char* token_string);
void to_int(void* res, int token_length ,char* token_string);
void to_double(void* res, int token_length ,char* token_string);
void* json_create_struct(char*json,size_t size);

#endif 
