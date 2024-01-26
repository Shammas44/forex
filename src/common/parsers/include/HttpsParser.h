#ifndef HTTPSRESPONSEPARSER_H
#define HTTPSRESPONSEPARSER_H
#include "Parser.h"
#include <stdbool.h>

#define T HttpsParser

typedef enum {
  HttpsParser_request,
  HttpsParser_response,
} HttpsParser_type;

typedef Parser T;

typedef struct HttpsParser_config {
  bool jsonBody;
  HttpsParser_type type;
} HttpsParser_config;

T *httpsParser_constructor();

#undef T
#endif
