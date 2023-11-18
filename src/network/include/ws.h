#ifndef WS_H
#define WS_H
#include "https.h"

int ws_establish_handshake(SSL **ssl, Request *request, Response *res);

void ws_send_message(SSL *ssl, const char *message);

#endif
