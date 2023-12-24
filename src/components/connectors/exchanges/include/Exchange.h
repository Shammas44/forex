#ifndef EXCHANGE_H
#define EXCHANGE_H
#include "observer.h"
#include "parser.h"
#include "wsHandler.h"
#include "configWrapper.h"
#define T Exchange

typedef struct T T;

typedef int (Exchange_connect)(T* exchange);
typedef void (Exchange_subscribe)(T* exchange,char*path);
typedef int (Exchange_authenticate)(T* exchange, void *credentials);
typedef char* (Exchange_get_auth)(T* exchange);
typedef int (Exchange_unsubscribe)(T* exchange);
typedef void (Exchange_destructor)(T *exchange);

typedef struct T {
Exchange_destructor *destructor;
Exchange_connect *connect;
Exchange_subscribe *subscribe;
Exchange_authenticate *authenticate;
Exchange_get_auth *get_auth;
Exchange_unsubscribe *unsubscribe;
void * __private;
}T;

T* exchange_constructor(WsHandler*ws,ConfigWrapper *config, Parser*parser);

// void exchange_attach_observer(struct Exchange* exchange, Observer* observer);

// void exchange_notify_observers(struct Exchange* exchange,void * state);
#undef T
#endif 
