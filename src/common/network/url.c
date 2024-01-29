#include "url.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* __url_parse_string(char *string,char *out, const char* delimiters, size_t delimiters_len);

//@TODO handle ipv6 addresses
Url *url_constructor(char *url) {
  if(url == NULL) return NULL;
  Url *url_struct = malloc(sizeof(Url));
  char *host = malloc(sizeof(char) * 128);
  char *protocol = malloc(sizeof(char) * 8);
  char *path = malloc(sizeof(char) * 128);
  char *full = malloc(sizeof(char) *(strlen(url) + 1));
  char *port = malloc(sizeof(char) * 8);
  sprintf(full, "%s", url);
  sprintf(port, "%s", "443");
  url_struct->full = full;
  url_struct->protocol = protocol;
  url_struct->host = host;
  url_struct->path = path;
  url_struct->port = port;

  const char tok[] = {':'};
  url = __url_parse_string(url, protocol, tok,1);
  url+=3; //because of the :// in url

  if(strchr(url, ':') != NULL){
    url = __url_parse_string(url, host,(const char[]){':'}, 1);
    url++;
    url = __url_parse_string(url, port, (const char[]){'\0','/'},2);
    url = __url_parse_string(url, path, (const char[]){'\0'},1);
  }else{
    url = __url_parse_string(url, host, (const char[]){'/'},1);
    url = __url_parse_string(url, path, (const char[]){'\0'},1);
  }
  if (strlen(path) == 0) sprintf(path, "/");

  return url_struct;
}

int url_destructor(Url *url) {
  free(url->full);
  free(url->port);
  free(url->host);
  free(url->path);
  free(url->protocol);
  free(url);
  return 0;
}

void url_print(Url*url){
  puts("");
  printf("full: %s\n", url->full);
  printf("protocol: %s\n", url->protocol);
  printf("host: %s\n", url->host);
  printf("path: %s\n", url->path);
  printf("port: %s\n", url->port);
  printf("\n");
}

static bool __is_char_present(const char array[], char target, size_t array_length) {
    for (size_t i = 0; i < array_length; i++) {
        if (array[i] == target) {
            return 1; // Character found in the array
        }
    }
    return 0; // Character not found in the array
}

char* __url_parse_string(char *string,char *out, const char delimiters[], size_t delimiters_len) {
int string_length = strlen(string);
int i = 0;
  while (!__is_char_present(delimiters,string[0],delimiters_len) && i != string_length && string != NULL) {
    out[i] = *string;
    string++;
    i++;
  }
  out[i] = '\0';
  return string;
}

