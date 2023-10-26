#include "Exchange_p.h"
#include "guard.h"

int __exchangeTest_connect(Exchange* exchange);
int __exchangeTest_subscribe(Exchange* exchange);
int __exchangeTest_unsubscribe(Exchange* exchange);
int __exchangeTest_destructor(Exchange *exchange);

 Exchange* exchangeTest_constructor(Exchange_protocols protocols, char* url, char* auth_token){
  Exchange* exchange = malloc(sizeof(Exchange));
  OOM_GUARD(exchange); 

  exchange->auth_token = auth_token;
  exchange->url = url;
  exchange->protocols = protocols;
  exchange->connect_func = __exchangeTest_connect;
  exchange->subscribe_func = __exchangeTest_subscribe;
  exchange->unsubscribe_func = __exchangeTest_unsubscribe;
  return exchange;
}

int __exchangeTest_connect(Exchange* exchange){
  return 1;
}

int __exchangeTest_subscribe(Exchange* exchange){
  return 1;
}

int __exchangeTest_unsubscribe(Exchange* exchange){
  return 1;
}
