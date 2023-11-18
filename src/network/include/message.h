#ifndef MESSAGE_H
#define MESSAGE_H

struct Message;

struct message_prefill;

typedef struct Message * (message_constructor_func)(struct message_prefill prefill);
typedef int (message_destructor_func)(struct Message * msg);
typedef int (message_stringify_func)(struct Message * msg, char **res);
typedef void (message_print_func)(struct Message *msg);

struct Message * message_constructor(struct message_prefill prefill);

int message_destructor(struct Message * msg);

int message_stringify(struct Message * msg, char **res);

void message_print(struct Message *msg);

#endif 
