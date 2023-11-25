#include "url.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* __url_parse_string(char *string,char *out, const char delimiter);

//@TODO handle ipv6 addresses
Url *url_constructor(char *url) {
  if(url == NULL) return NULL;
  Url *url_struct = malloc(sizeof(Url));
  int i = 0;
  int j = 0;
  int k = 0;
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

  url = __url_parse_string(url, protocol, ':');
  url+=3; //because of the :// in url

  if(strchr(url, ':') != NULL){
    url = __url_parse_string(url, host, ':');
    url++;
    url = __url_parse_string(url, port, '\0');
  }else{
    url = __url_parse_string(url, host, '/');
    url = __url_parse_string(url, path, '\0');
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
}

char* __url_parse_string(char *string,char *out, const char delimiter) {
int string_length = strlen(string);
int i = 0;
  while (*string != delimiter && i != string_length && string != NULL) {
    out[i] = *string;
    string++;
    i++;
  }
  out[i] = '\0';
  return string;
}
