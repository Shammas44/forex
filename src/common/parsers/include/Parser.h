#ifndef PARSER_H
#define PARSER_H
#include "observer.h"
#define T Parser

typedef struct T T;

typedef void (Parser_destructor)(T *parser);
typedef void* (Parser_parse)(T *parser, char*input);

struct Parser {
  Parser_destructor *destructor;
  Parser_parse *parse;
  void * __private;
};

Parser * parser_constructor();

#undef T
#endif
