#ifndef REQUEST_H
#define REQUEST_H
#include "message.h"
#define T Request

typedef struct T T;

typedef struct Request_prefill Request_prefill;

typedef int (request_send)(T *res);
typedef int (request_destructor)(T * request);
typedef int (request_stringify)(T * request, char **out);
typedef void (request_print)(T *request);
typedef void* (request_get_connection)(T *request);

 struct Request {
  request_destructor *destructor_func;
  request_send *send_func;
  request_stringify *stringify_func;
  request_print *print_func;
  request_get_connection *get_connection_func;
  void * __private;
};

Request * request_constructor(Request_prefill prefill);

#undef T
#endif 
