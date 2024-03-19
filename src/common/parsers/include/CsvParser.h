#ifndef CSVPARSER_H
#define CSVPARSER_H
#include "Parser.h"

#define T CsvParser

typedef Parser T;
typedef void (*CsvParser_on_row)(void* newState);

typedef enum {
  CsvParser_type_file,
  CsvParser_type_string,
} CsvParser_type;

typedef struct CsvParser_config {
  char* delimiter;
  CsvParser_type type;
}CsvParser_config;

T * csvParser_constructor();

#undef T
#endif
