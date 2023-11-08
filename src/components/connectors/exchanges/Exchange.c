#include "Exchange.h"
#include "Exchange_p.h"
#include "guard.h"

Exchange* exchange_constructor(Exchange *implementation){
  Exchange* exchange = malloc(sizeof(Exchange));
  OOM_GUARD(exchange); 

  exchange->exchange = (struct Exchange*)implementation;
  // exchange->auth_token = implementation->auth_token;
  // exchange->url = implementation.url;
  // exchange->numObservers = implementation.numObservers;
  // for (int i = 0; i < EXCHANGE_MAX_OBSERVERS; i++) {
  //   exchange->observers[i] = implementation.observers[i];
  // }
  // exchange->connection = implementation.connection;
  // exchange->connect_func  = implementation.connect_func;
  // exchange->authenticate_func  = implementation.authenticate_func;
  // exchange->subscribe_func = implementation.subscribe_func;
  // exchange->get_auth_func  = implementation.get_auth_func;
  // exchange->unsubscribe_func  = implementation.unsubscribe_func;
  return exchange;
}

int exchange_connect(Exchange* exchange){
  int status = exchange->exchange->connect_func(exchange->exchange);
  return status;
}

int exchange_subscribe(Exchange* exchange){
  int status = exchange->exchange->subscribe_func(exchange);
  return status;
}

int exchange_authenticate(struct Exchange* exchange, void *credentials){
  int status = exchange->exchange->authenticate_func(exchange, credentials);
  return status;
}

char* exchange_get_auth(Exchange* exchange){
  char* auth = exchange->exchange->auth_token;
  return auth;
}

int exchange_unsubscribe(Exchange* exchange){
  int status = exchange->exchange->unsubscribe_func(exchange);
  return status;
}

void exchange_attach_observer(Exchange* exchange, Observer* observer) {
    if (exchange->exchange->numObservers < EXCHANGE_MAX_OBSERVERS) {
        exchange->exchange->observers[exchange->exchange->numObservers++] = observer;
    }
}

void exchange_notify_observers(Exchange* exchange, void *state) {
    for (int i = 0; i < exchange->numObservers; i++) {
        exchange->observers[i]->update(exchange->observers[i], state);
    }
}

int exchange_destructor(Exchange *exchange){
  // free(exchange->auth_token);
  // free(exchange->url);
  //@TODO clean SSL
  free(exchange->exchange);
  free(exchange);
  return 0;
}
