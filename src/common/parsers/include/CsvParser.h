#ifndef CSVPARSER_H
#define CSVPARSER_H
#include "Parser.h"

#define T CsvParser

typedef Parser T;
typedef void (*CsvParser_on_row)(void* newState);

typedef struct CsvParser_config {
  char* delimiter;
}CsvParser_config;

T * csvParser_constructor();

#undef T
#endif
