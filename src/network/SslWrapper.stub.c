#include "SslWrapper.stub.h"
#include "tls.h"
#include "common.h"
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <stdbool.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>
#define T SslWrapperStub
#define SOCKET int
#define CLOSESOCKET(s) close(s)
#define ISVALIDSOCKET(s) ((s) >= 0)
#define GETSOCKETERRNO() (errno)
#define ONERROR(s) ((s) < 0)
#define MAX_TIMEOUT_SECONDS 5 // Change this according to your desired timeout

typedef struct {

} Private;


static void __destructor(T *self);
static int __connect(T *self, SSL *ssl);
static int __write(T *self, SSL *ssl, char *payload, int payload_length);
static int __read(T *self, SSL *ssl, char **out, int *out_length);
static int __get_last_error(T *self);
static int __cleanup(T *self);
static SSL *__new(T *self, SSL_CTX *ctx);
static long __set_host(T *self,SSL*ssl, char *host);
static int __set_fd(T *self, SSL *ssl, int fd);
static int __shutdown(T *self, SSL *ssl);

static void _$ini_openssl();
static int _$set_ssl_context(void);

T *sslWrapperStub_constructor() {
  T *self = (T *)malloc(sizeof(T));
  Private *private = (Private *)malloc(sizeof(Private));
  self->__private = private;
  self->destructor = __destructor;
  self->connect = __connect;
  self->write = __write;
  self->read = __read;
  self->new = __new;
  self->set_host = __set_host;
  self->set_fd = __set_fd;
  self->shutdown = __shutdown;
  _$ini_openssl();
  _$set_ssl_context();
  return self;
}

void __destructor(T *self) {
  Private *private = self->__private;
  free(private);
  free(self);
}

static SSL *__new(T *self, SSL_CTX *ctx) {
  return SSL_new(ctx); 
}

static long __set_host(T *self, SSL *ssl, char *host) {
  long status = SSL_set_tlsext_host_name(ssl, host);
  return status;
}

static int __set_fd(T *self, SSL *ssl, int fd) {
  return SSL_set_fd(ssl, fd); 
}

static int __connect(T *self, SSL *ssl) {
  // return SSL_connect(ssl); 
  return 0;
}

static int __write(T *self, SSL *ssl, char *payload, int payload_length) {
  // return SSL_write(ssl, payload, payload_length);
  return 0;
}

static int __shutdown(T *self, SSL *ssl) {
  int status = 0;
  if (ssl != NULL)
    status = SSL_shutdown(ssl);
  return status;
}

static int __read(T *self, SSL *ssl, char **out, int *out_length) {
  *out =  "HTTP/1.1 200 OK\n"
          "Content-Type: text/plain\n"
          "Content-Length: 13\n"
          "\r\n\r\n"
          "Hello, World!";
  *out_length = strlen(*out);
  return 0;
}

static void _$ini_openssl() {
  static bool initialized = false;
  if (!initialized) {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    initialized = true;
  }
}

static int _$set_ssl_context(void) {
  if (ctx == NULL)
    ctx = SSL_CTX_new(TLS_client_method());
  if (!ctx) {
    return 1;
  }
  return 0;
}

#undef T
#undef SOCKET
#undef CLOSESOCKET
#undef ISVALIDSOCKET
#undef GETSOCKETERRNO
#undef ONERROR
#undef MAX_TIMEOUT_SECONDS
