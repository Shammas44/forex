#ifndef REQUEST_H
#define REQUEST_H

#include "message.h"

struct Request;

struct Request_prefill;

typedef int (request_send)(struct Request *res);
typedef int (request_destructor_func)(struct Request * request);
typedef int (request_stringify_func)(struct Request * request, char **out);
typedef void (request_print_func)(struct Request *request);
typedef void* (request_get_connection_func)(struct Request *request);

typedef struct {
  request_destructor_func *destructor_func;
  request_send *send_func;
  request_stringify_func *stringify_func;
  request_print_func *print_func;
  request_get_connection_func *get_connection_func;
  void * __private;
} Request;

Request * request_constructor(struct Request_prefill prefill);

#endif 
