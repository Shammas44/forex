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
typedef char* (Response_get_status)(T *response);

 struct T {
  Response_constructor *constructor;
  Response_destructor *destructor;
  Response_receive *receive;
  Response_stringify *stringify;
  Response_print *print;
  Response_get_status *get_status;
  void * __private;
};

T * response_constructor(Response_prefill prefill);

#undef T
#endif 
