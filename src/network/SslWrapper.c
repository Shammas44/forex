#include "SslWrapper.h"
#include "common.h"
#include "tls.h"
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <stdbool.h>
#include <sys/errno.h>
#include <unistd.h>
#define T SslWrapper
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
static int __read(T *self, SSL *ssl, char **out, size_t *out_length);
static int __get_last_error(T *self);
static int __cleanup(T *self);
static SSL *__new(T *self, SSL_CTX *ctx);
static long __set_host(T *self,SSL*ssl, char *host);
static int __set_fd(T *self, SSL *ssl, int fd);
static int __shutdown(T *self, SSL *ssl);

static void _$ini_openssl();
static int _$set_ssl_context(void);

T *sslWrapper_constructor() {
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

static SSL *__new(T *self, SSL_CTX *ctx) { return SSL_new(ctx); }

static long __set_host(T *self, SSL *ssl, char *host) {
  long status = SSL_set_tlsext_host_name(ssl, host);
  return status;
}

static int __set_fd(T *self, SSL *ssl, int fd) { return SSL_set_fd(ssl, fd); }

static int __connect(T *self, SSL *ssl) { return SSL_connect(ssl); }

static int __write(T *self, SSL *ssl, char *payload, int payload_length) {
  return SSL_write(ssl, payload, payload_length);
}

static int __shutdown(T *self, SSL *ssl) {
  int status = 0;
  if (ssl != NULL)
    status = SSL_shutdown(ssl);
  return status;
}

static int __read(T *self, SSL *ssl, char **out, size_t *out_length) {
  ssize_t bytes_received;
  int response_capacity = 4096;

  int total_wait_seconds = 0;

  while (total_wait_seconds < MAX_TIMEOUT_SECONDS) {
    bytes_received =
        SSL_read(ssl, *out + *out_length, response_capacity - *out_length);
    if (bytes_received > 0) {
      *out_length += bytes_received;
      if (*out_length == response_capacity) {
        response_capacity *= 2; // Double the capacity
        char *new_response = realloc(*out, response_capacity);
        if (!new_response) {
          free(*out);
          return -1; // Adjust this error handling according to your needs
        }
        *out = new_response;
      }
      return 0; // Successful read
    } else if (bytes_received == 0) {
      // No more data
      break;
    } else {
      int err = SSL_get_error(ssl, bytes_received);
      if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
        // Retry if more data is expected
        sleep(1); // Adjust sleep time as needed
        total_wait_seconds++;
        continue;
      } else if (err == SSL_ERROR_SYSCALL && errno == EAGAIN) {
        // Timeout occurred
        char msg[100];
        sprintf(msg, "SSL read timeout occurred after %d seconds.\n",
                MAX_TIMEOUT_SECONDS);
        RUNTIME_ERROR(msg, -1);
        return -1; // Timeout error
      } else {
        // Other error occurred
        char msg[100];
        sprintf(msg, "SSL read error: %s\n",
                ERR_reason_error_string(ERR_get_error()));
        return -1; // Adjust error handling as needed
      }
    }
  }

  char msg[100];
  sprintf(msg, "SSL read timeout occurred after %d seconds.\n",
          MAX_TIMEOUT_SECONDS);
  // Timeout occurred
  return -1; // Timeout error
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
  int status = 0;
  if (ctx == NULL) ctx = SSL_CTX_new(TLS_client_method());
  if (!ctx)  status = 1; 
  if (ONERROR(status)) {
    RUNTIME_ERROR("SSL_CTX_new() failed.",1);
  }
  return status;
}

#undef T
#undef SOCKET
#undef CLOSESOCKET
#undef ISVALIDSOCKET
#undef GETSOCKETERRNO
#undef ONERROR
#undef MAX_TIMEOUT_SECONDS
