#ifndef EXCHANGE_H
#define EXCHANGE_H
#include "observer.h"
#include "Parser.h"
#include "wsHandler.h"
#include "ConfigWrapper.h"
#include "Order.h"
#define T Exchange

typedef struct T T;

struct T {
 int (*connect)(T* self);
 void (*subscribe)(T* self,char*path);
 int (*authenticate)(T* self, void *credentials);
 char* (*get_auth)(T* self);
 int (*unsubscribe)(T* self);
 void (*destructor)(T *self);
 void (*attach_observer)(T *self, Observer* observer);
 void (*dettach_observer)(T *self, Observer* observer);
 void (*send_order)(T*self,Order *order);
 void * __private;
};

T* exchange_constructor(WsHandler*ws,ConfigWrapper *config, Parser*parser);

#undef T
#endif 
