#ifndef EXCHANGE_H
#define EXCHANGE_H
#include "observer.h"

typedef enum Exchange_protocols {
  Exchange_protocols_HTTPS,
  Exchange_protocols_WS,
  Exchange_protocols_FIX,
} Exchange_protocols;

struct Exchange;

typedef struct Exchange* (*Exchange_constructor)(struct Exchange *exchange);
typedef int (*Exchange_connect)(struct Exchange* exchange);
typedef int (*Exchange_subscribe)(struct Exchange* exchange);
typedef int (*Exchange_authenticate)(struct Exchange* exchange, void *credentials);
typedef char* (*Exchange_get_auth)(struct Exchange* exchange);
typedef int (*Exchange_unsubscribe)(struct Exchange* exchange);
typedef void (*Exchange_destructor)(struct Exchange *exchange);

/**
 * constructor
 * @param protocols - protocols type
 * @param url - exchange base url
 * @param token - auth token
 * @return Exchange*
 */
struct Exchange* exchange_constructor(struct Exchange* exchange);

int exchange_connect(struct Exchange* exchange);

int exchange_authenticate(struct Exchange* exchange, void *credentials);

int exchange_subscribe(struct Exchange* exchange);

char* exchange_get_auth(struct Exchange* exchange);

int exchange_unsubscribe(struct Exchange* exchange);

int exchange_destructor(struct Exchange *exchange);

void exchange_attach_observer(struct Exchange* exchange, Observer* observer);

void exchange_notify_observers(struct Exchange* exchange,void * state);
#endif 
