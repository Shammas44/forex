#ifndef WS_H
#define WS_H
#include "https.h"

int ws_establish_handshake(SSL **ssl, HttpsRequest *request, HttpsResponse *res);

void ws_send_message(SSL *ssl, const char *message);

#endif
