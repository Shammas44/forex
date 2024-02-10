#ifndef MESSAGE_H
#define MESSAGE_H
#include "IsDestroyable.h"
#include "item.h"
#include "common.h"
#define T Message
typedef struct T T;

typedef enum { Msg_empty,Msg_candle,Msg_stop,Msg_unknown } Msg_type;

typedef struct {
  Item item;
  Msg_type code;
} Msg_args;

struct T {
  IsDestroyable __destructor;
  void (*destructor)(T *self);
  Item (*value)(T *self,Access_mode mode, Item new);
  Msg_type (*code)(T *self,Access_mode mode,Msg_type code);
  void *__private;
};

T *message_constructor(Msg_args args);
#undef T
#endif
