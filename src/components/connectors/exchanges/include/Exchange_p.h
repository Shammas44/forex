#ifndef EXCHANGE_P_H
#define EXCHANGE_P_H
#define EXCHANGE_MAX_OBSERVERS 10
#include "Exchange.h"
#include "observer.h"
#include "mtqueue.h"
#include "url.h"

typedef struct Exchange {
  struct Exchange *exchange;
  Mtqueue_list *mtqueue_list;
  char* auth_token;
  Url *url;
  SSL *connection;
  Observer* observers[EXCHANGE_MAX_OBSERVERS]; 
  int numObservers; 
  Exchange_connect connect_func;
  Exchange_subscribe subscribe_func;
  Exchange_authenticate authenticate_func;
  // Observer_update_func notify_observers_func;
  Exchange_get_auth get_auth_func;
  Exchange_unsubscribe unsubscribe_func;
} Exchange;
#endif
