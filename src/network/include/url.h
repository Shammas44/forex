#ifndef URL_H
#define URL_H

typedef struct Url {
  char * full;
  char * host;
  char * port;
  char * path;
  char * protocol;
} Url;

Url *url_constructor(char*url);

int url_destructor(Url*url);

void url_print(Url*url);

#endif 
