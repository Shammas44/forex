#ifndef RESPONSE_H
#define RESPONSE_H

#define T Response
typedef struct T T;

typedef struct Response_prefill Response_prefill;

typedef int (Response_receive)(T *res);
typedef T * (Response_constructor)(Response_prefill prefill);
typedef int (Response_destructor)(T * response);
typedef int (Response_stringify)(T * response, char **out);
typedef void (Response_print)(T *response);

 struct T {
  Response_constructor *constructor_func;
  Response_destructor *destructor_func;
  Response_receive *receive_func;
  Response_stringify *stringify_func;
  Response_print *print_func;
  void * __private;
};

T * response_constructor(Response_prefill prefill);

#undef T
#endif 
