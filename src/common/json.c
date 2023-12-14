#include "json.h"

// =========================================================================="
// Prototypes functions
// =========================================================================="

void handle_ticks(char *json, jsmntok_t **tokens, int token_num);

// =========================================================================="
// Public functions
// =========================================================================="

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

void json_extract_token(jsmntok_t *tokens, int i, char *json, void*res, Convert_callback callback) {
  int token_start = tokens[i + 1].start;
  int token_end = tokens[i + 1].end;
  int token_length = token_end - token_start;
  char *token_string = json + token_start;
  callback(res, token_length, token_string);
}

void to_char(void* res, int token_length, char* token_string) {
    char* char_res = (char*) res;  
    for (int i = 0; i < token_length; i++) {
        char_res[i] = token_string[i];
    } 
    char_res[token_length] = '\0';  
}

void to_float(void* res, int token_length ,char* token_string) {
    float* float_res = (float*) res;
    *float_res = atof(token_string);
}

void to_double(void* res, int token_length ,char* token_string) {
    double* double_res = (double*) res;
    *double_res = atof(token_string);
}

void to_int(void* res, int token_length ,char* token_string) {
    int* int_res = (int*) res;
    *int_res = atoi(token_string);
}

void to_bool(void* res, int token_length ,char* token_string) {
    bool* bool_res = (bool*) res;
    char value = token_string[0];
    if (value == '1') {
        *bool_res = true;
    } else {
        *bool_res = false;
    }
}

void to_time(void* res, int token_length ,char* token_string) {
    time_t* time_res = (time_t*) res; 
    char timestamp[25];  
    if (token_length < sizeof(timestamp)) {  // Ensure we don't overrun our buffer
        to_char(timestamp, token_length, token_string);
        *time_res = iso8601_to_epoch(timestamp);
    } else {
        // Handle error, token string is too long
        *time_res = 0;  // or some error value
    }
}

int json_parse(char *json, jsmntype_t expected_type, jsmntok_t **tokens, int *token_num) {
    jsmn_parser parser;
    jsmn_init(&parser);
    
    int required_tokens = jsmn_parse(&parser, json, strlen(json), NULL, 0);
    
    *tokens = malloc(sizeof(jsmntok_t) * required_tokens);
    if (!*tokens) {
        return get_error("Memory allocation failed");
    }

    jsmn_init(&parser);
    *token_num = jsmn_parse(&parser, json, strlen(json), *tokens, required_tokens);

    if (token_num < 0) {
        free(*tokens);
        *tokens = NULL;
        return get_error("Failed to parse JSON");
    }

    if (*token_num < 1 || (*tokens)[0].type != expected_type) {
        free(*tokens);
        *tokens = NULL;
        return get_error("jsmn expected another type of data");
    }

    return 0;
}

void* json_create_struct(char* json, size_t size) {
    int token_num;
    jsmntok_t *tokens = NULL;
    int status = json_parse(json, JSMN_OBJECT, &tokens, &token_num);
    int j = 0;
    if (status != 0) return NULL;

    void* out = malloc(size);
    if (out == NULL) return NULL;

    enum { VALUE = 1 };

    for (int i = 0; i < token_num; i++) {
        int token_start = tokens[i + 1].start;
        int token_end = tokens[i + 1].end;
        int token_length = token_end - token_start;
        char* token_string = json + token_start;
        char* string = malloc(token_length + 1);
        strncpy(string, token_string, token_length);
        string[token_length] = '\0'; // Null-terminate the string

        if (i % 2 == VALUE){
            char* p = (char*)out + j * sizeof(char*);
            memcpy(p, &string, sizeof(char*)); // Copy the string pointer to the out+i location
            j++;
        }
    }

    return out;
}

// =========================================================================="
// Private functions
// =========================================================================="

