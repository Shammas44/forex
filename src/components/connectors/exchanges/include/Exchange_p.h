#ifndef EXCHANGE_P_H
#define EXCHANGE_P_H
#include "Exchange.h"

typedef struct Exchange {
  char* auth_token;
  char *url;
  SSL *connection;
  Exchange_protocols protocols;
  Exchange_connect connect_func;
  Exchange_subscribe subscribe_func;
  Exchange_authenticate authenticate_func;
  Exchange_get_auth get_auth_func;
  Exchange_unsubscribe unsubscribe_func;
} Exchange;
#endif
