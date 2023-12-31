#ifndef WSHANDLER_H
#define WSHANDLER_H
#include <openssl/ssl.h>
#include "https.h"
#include "Parser.h"

#define T WsHandler

typedef void (*Wshandler_on_frame_receive)(void*caller,void* newState);
typedef struct T T;

typedef void (WsHandler_destructor)(T * ws);
typedef SSL* (WsHandler_handshake)(T * ws, HttpsRequest * request);
typedef void (WsHandler_send)(T * ws, SSL *ssl, const char *message);
typedef int (WsHandler_listen)(T * ws, SSL *ssl, void*caller, Wshandler_on_frame_receive update);
typedef Https* (WsHandler_get_https_handler)(T * ws);
typedef void (WsHandler_sendClose)(T*ws,SSL *ssl);

 struct T {
  WsHandler_destructor *destructor;
  WsHandler_handshake *handshake;
  WsHandler_listen *listen;
  WsHandler_send *send;
  WsHandler_sendClose *close;
  WsHandler_get_https_handler *get_https_handler;
  void * __private;
};

T * wsHandler_constructor(Https * https);

#undef T
#endif 
