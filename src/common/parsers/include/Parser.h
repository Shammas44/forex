#ifndef PARSER_H
#define PARSER_H
#include "observer.h"
#define T Parser

typedef struct T T;

typedef struct Parser_config_obj Parser_config_obj;

typedef void (*Parser_on_data)(void*caller,void* newState);

typedef void (Parser_destructor)(T *parser);
typedef void* (Parser_parse)(T *parser, char*input);
typedef int (Parser_parse_stream)(T *parser, void*stream, void*caller, Parser_on_data on_data);
typedef void (Parser_config)(T *parser, Parser_config_obj *config);

struct Parser {
  Parser_destructor *destructor;
  Parser_parse *parse;
  Parser_parse_stream *parse_stream;
  Parser_on_data *on_data;
  Parser_config *config;
  void * __private;
};

Parser * parser_constructor();

#undef T
#endif
