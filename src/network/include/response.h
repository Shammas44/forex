#ifndef RESPONSE_H
#define RESPONSE_H

struct Response;

struct response_prefill;

typedef int (response_receive)(struct Response *res);
typedef struct Response * (response_constructor_func)(struct response_prefill prefill);
typedef int (response_destructor_func)(struct Response * response);
typedef int (response_stringify_func)(struct Response * response, char **out);
typedef void (response_print_func)(struct Response *response);
typedef void* (response_allocate_func)();

typedef struct {
  response_constructor_func *constructor_func;
  response_allocate_func *allocate_func;
  response_destructor_func *destructor_func;
  response_receive *receive_func;
  response_stringify_func *stringify_func;
  response_print_func *print_func;
  void * __private;
} Response;

Response * response_constructor(struct response_prefill prefill);

#endif 
