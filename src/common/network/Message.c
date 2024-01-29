#include "Message.h"
#include "common.h"
#include <stdlib.h>
#define T Message

typedef struct {
  Item value;
  Msg_type code;
} Private;

static void __destructor(T *self);
static Item __value(T *self,Access_mode mode,Item new);
static Msg_type __code(T *self, Access_mode mode,Msg_type new);

T *message_constructor(Msg_args args) {
  T *self = (T *)malloc(sizeof(T));
  Private *private = (Private *)malloc(sizeof(Private));
  private->value = args.item;
  private->code = args.code;
  self->__private = private;
  self->destructor = __destructor;
  self->__destructor = (IsDestroyable){.destructor = __destructor};
  self->value = __value;
  self->code = __code;
  return self;
}

static void __destructor(T *self) {
  Private *private = self->__private;
  free(private);
  free(self);
}

static Item __value(T *self,Access_mode mode,Item new){
  Private *private = self->__private;
  if(mode == WRITE) private->value = new;
  return private->value;
}

static Msg_type __code(T *self, Access_mode mode,Msg_type new){
  Private *private = self->__private;
  if(mode == WRITE) private->code = new;
  return private->code;
}

#undef T
