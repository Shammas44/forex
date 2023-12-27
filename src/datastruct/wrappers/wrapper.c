#include "wrapper.h"
#include "hashmap.h"
#include <string.h>
#define T Wrapper

char* wrapper_get_string(Hashmap*map,const char*key){
  if(key == NULL) return NULL;
  char* value = HASHMAP_GET_STRING(map, key);
  size_t len = strlen(value);
  char* out = malloc(sizeof(char) * len +1);
  strcpy(out,value);
  return out;
}

int wrapper_get_int(Hashmap*map,const char*key){
  if(key == NULL) return 0;
  char*value = HASHMAP_GET_STRING(map, key);
  int out = atoi(value);
  free(value);
  return out;
}

bool wrapper_get_bool(Hashmap*map,const char*key){
  if(key == NULL) return 0;
  char*value = HASHMAP_GET_STRING(map, key);
  bool out = NULL;
  if(strcmp(value,"true") == 0) out = true;
  if(strcmp(value,"false") == 0) out = false;
  free(value);
  return out;
}

double wrapper_get_double(Hashmap*map,const char*key){
  if(key == NULL) return 0;
  char*value = HASHMAP_GET_STRING(map, key);
  int out = atof(value);
  free(value);
  return out;
}
