#ifndef MESSAGE_H
#define MESSAGE_H
#define T Message
typedef struct T T;

typedef struct Message_prefill Message_prefill;

typedef T * (Message_constructor)(Message_prefill prefill);
typedef int (Message_destructor)(T * msg);
typedef int (Message_stringify)(T * msg, char **res);
typedef void (Message_print)(T *msg);

T * message_constructor(Message_prefill prefill);

int message_destructor(T * msg);

int message_stringify(T * msg, char **res);

void message_print(T *msg);
#undef T
#endif 
