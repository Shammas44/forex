#include "configparser.h"
#include "tsmetadata.h"

void to_strategy(void* res, int token_length ,char* token_string);
void to_mode(void* res, int token_length ,char* token_string);

void configparser_init(const char *path, Tsmetadata *m){
  char *file_content = file_read(path);
  jsmntok_t *tokens = NULL;
  int token_num = json_parse(file_content, &tokens);

  if (token_num <= 0) {
    free(file_content);
    if (tokens) {
      free(tokens);
    }
  }

  tsmetadata_init(m);

  for (int i = 1; i < token_num; i++) {
    if (json_cmp_token_to_string(file_content, &tokens[i], "mode") == 0) {
      json_extract_token(tokens, i, file_content, &m->mode, to_mode);
      i++;
    }
    if (json_cmp_token_to_string(file_content, &tokens[i], "strategy") == 0) {
      json_extract_token(tokens, i, file_content, &m->strategy, to_strategy);
      json_extract_token(tokens, i, file_content, &m->strategy_name, to_char);
      i++;
    }
    if (json_cmp_token_to_string(file_content, &tokens[i], "spread") == 0) {
      json_extract_token(tokens, i, file_content, &m->spread, to_double);
      i++;
    }
    if (json_cmp_token_to_string(file_content, &tokens[i], "capital") == 0) {
      json_extract_token(tokens, i, file_content, &m->capital, to_double);
      i++;
    }
    if (json_cmp_token_to_string(file_content, &tokens[i], "leverage") == 0) {
      json_extract_token(tokens, i, file_content, &m->leverage, to_int);
      i++;
    }
    if (json_cmp_token_to_string(file_content, &tokens[i], "backtest_data") == 0) {
      json_extract_token(tokens, i, file_content, &m->backtest_data, to_char);
      i++;
    }
    if (json_cmp_token_to_string(file_content, &tokens[i], "broker_commision_dpm") == 0) {
      json_extract_token(tokens, i, file_content, &m->broker_commision_dpm, to_double);
      i++;
    }
    if (json_cmp_token_to_string(file_content, &tokens[i], "symbol") == 0) {
      json_extract_token(tokens, i, file_content, &m->symbol, to_char);
      i++;
    }
  }
  free(file_content);
  free(tokens);
}

void to_strategy(void* res, int token_length ,char* token_string){
  int *final_res = (int*) res;
  char char_res[50];
  if(token_length > 50){
    printf("Error: token_length > 50\n");
    exit(EXIT_FAILURE); 
  }

  for (int i = 0; i < token_length; i++) {
    char_res[i] = token_string[i];
  } 
  char_res[token_length] = '\0';  

  if(strcmp(char_res, "STRATEGY_TEST") == 0){
    *final_res = STRATEGY_TEST;
  }
  else if (strcmp(char_res, "STRATEGY_TREND_FOLLOWING") == 0){
    *final_res = STRATEGY_TREND_FOLLOWING;
  }else{
    printf("Error: strategy not found\n");
    exit(EXIT_FAILURE);
  }
}

void to_mode(void* res, int token_length ,char* token_string){
  int *final_res = (int*) res;
  char char_res[50];
  if(token_length > 50){
    printf("Error: token_length > 50\n");
    exit(EXIT_FAILURE); 
  }

  for (int i = 0; i < token_length; i++) {
    char_res[i] = token_string[i];
  } 
  char_res[token_length] = '\0';  

  if(strcmp(char_res, "BACKTEST") == 0){
    *final_res = TSMETADATA_MODE_BACKTEST;
  }
  else if (strcmp(char_res, "REAL") == 0){
    *final_res = TSMETADATA_MODE_REAL;
  }else{
    printf("Error: mode not found\n");
    exit(EXIT_FAILURE);
  }
}
