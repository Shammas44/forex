#ifndef HTTPSRESPONSEPARSER_H
#define HTTPSRESPONSEPARSER_H
#include "Parser.h"
#include <stdbool.h>

#define T HttpsResponseParser

typedef Parser T;

typedef struct HttpsResponseParser_config {
  bool jsonBody;
}HttpsResponseParser_config;

T * httpsResponseParser_constructor();

#undef T
#endif

