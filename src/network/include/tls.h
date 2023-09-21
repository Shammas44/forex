#ifndef TLS_H
#define TLS_H
#include <openssl/ssl.h>

void network_ini_openssl();

extern SSL_CTX *ctx;

int set_ssl_context(void);
#endif
