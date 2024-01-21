#include "tls.h"
#include <stdbool.h>
#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/err.h>

// void network_ini_openssl(){
//   static bool initialized = false;
//   if(!initialized){
//     SSL_library_init();
//     OpenSSL_add_all_algorithms();
//     SSL_load_error_strings();
//     initialized = true;
//   }
// }

SSL_CTX *ctx = NULL;

// int set_ssl_context(void){
//   if(ctx==NULL) ctx = SSL_CTX_new(TLS_client_method());
//   if (!ctx) {
//     return 1;
//   }
//   return 0;
// }

