#ifndef WSHANDLER_H
#define WSHANDLER_H
#include <openssl/ssl.h>
#include "https.h"

#define T WsHandler
typedef struct T T;

typedef void (WsHandler_destructor)(T * ws);
typedef int (WsHandler_handshake)(T * ws, Request * request);
typedef void (WsHandler_send)(T * ws, SSL *ssl, const char *message);
typedef int (WsHandler_listen)(T * ws, SSL *ssl);

 struct T {
  WsHandler_destructor *destructor;
  WsHandler_handshake *handshake;
  WsHandler_listen *listen;
  WsHandler_send *send;
  void * __private;
};

T * wsHandler_constructor(Https * https);

#undef T
#endif 
