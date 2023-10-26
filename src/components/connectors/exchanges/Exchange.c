#include "Exchange.h"
#include "Exchange_p.h"
#include "guard.h"

Exchange* exchange_constructor(Exchange implementation){
  Exchange* exchange = malloc(sizeof(Exchange));
  OOM_GUARD(exchange); 

  exchange->auth_token = implementation.auth_token;
  exchange->url = implementation.url;
  exchange->connection = implementation.connection;
  exchange->connect_func  = implementation.connect_func;
  exchange->authenticate_func  = implementation.authenticate_func;
  exchange->subscribe_func = implementation.subscribe_func;
  exchange->get_auth_func  = implementation.get_auth_func;
  exchange->unsubscribe_func  = implementation.unsubscribe_func;
  return exchange;
}

int exchange_connect(Exchange* exchange){
  int status = exchange->connect_func(exchange);
  return status;
}

int exchange_subscribe(Exchange* exchange){
  int status = exchange->subscribe_func(exchange);
  return status;
}

int exchange_authenticate(struct Exchange* exchange, void *credentials, char* token_res){
  int status = exchange->authenticate_func(exchange, credentials,token_res);
  return status;
}

char* exchange_get_auth(Exchange* exchange){
  char* auth = exchange->auth_token;
  return auth;
}

int exchange_unsubscribe(Exchange* exchange){
  int status = exchange->unsubscribe_func(exchange);
  return status;
}

int exchange_destructor(Exchange *exchange){
  free(exchange->auth_token);
  free(exchange->url);
  //@TODO clean SSL
  free(exchange);
  return 0;
}
