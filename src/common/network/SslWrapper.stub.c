#include "SslWrapper.stub.h"
#include "httpsResponse.keys.h"
#include "HttpsParser.h"
#include "common.h"
#include "tls.h"
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
  SSL *ssl;
  Hashmap*last_request;
  HttpsParser *parser;
  char*response_body;
} Private;

static void __destructor(T *self);
static int __connect(T *self);
static int __write(T *self, char *payload, int payload_length);
static int __read(T *self, char **out, size_t *out_length);
static int __get_last_error(T *self);
static int __cleanup(T *self);
static SSL *__new(T *self);
static int __set_host(T *self, char *host);
static int __set_fd(T *self, int fd);
static int __shutdown(T *self);
static SSL *__get(T *self);

static void _$ini_openssl();
static int _$set_ssl_context(void);

T *sslWrapperStub_constructor(char*response_body) {
  T *self = (T *)malloc(sizeof(T));
  Private *private = (Private *)malloc(sizeof(Private));
  memset(private, 0, sizeof(Private));
  if(response_body){
    private->response_body = response_body;
  }
  HttpsParser *parser =  httpsParser_constructor();
  parser = httpsParser_constructor();
  HttpsParser_config config = {.type = HttpsParser_request,.jsonBody=false};
  Parser_config_obj *c = (Parser_config_obj *) &config;
  parser->config(parser,c);
  private->parser = parser;
  self->__private = private;
  self->destructor = __destructor;
  self->connect = __connect;
  self->write = __write;
  self->read = __read;
  self->new = __new;
  self->set_host = __set_host;
  self->set_fd = __set_fd;
  self->shutdown = __shutdown;
  self->get = __get;
  _$ini_openssl();
  _$set_ssl_context();
  return self;
}

void __destructor(T *self) {
  Private *private = self->__private;
  HttpsParser *parser = private->parser;
  parser->destructor(parser);
  Hashmap *last_request = private->last_request;
  last_request->destructor(last_request);
  free(private);
  free(self);
}

static SSL *__get(T *self) {
  Private *private = self->__private;
  return private->ssl;
}

static SSL *__new(T *self) {
  Private *private = self->__private;
  SSL* ssl = SSL_new(ctx);
  private->ssl = ssl;
  return ssl;
}

static int __set_host(T *self, char *host) {
  return 0;
}

static int __set_fd(T *self, int fd) {
  return 0;
}

static int __connect(T *self) {
  return 0;
}

static int __write(T *self, char *payload, int payload_length) {
  Private *private = self->__private;
  HttpsParser *parser = private->parser;
  Hashmap * request = parser->parse(parser,payload);
  private->last_request = request;
  return 0;
}

static int __shutdown(T *self) {
  int status = 0;
  SSL *ssl = __get(self);
  if (ssl != NULL)
    status = SSL_shutdown(ssl);
  return status;
}

static int __read(T *self, char **out, size_t *out_length) {
  Private *private = self->__private;
  Hashmap * request = private->last_request;
  Hashmap * headers = (Hashmap*)request->get(request,KEY(Headers)).value;
  char*upgrade = (char*)headers->get(headers,KEY(Upgrade)).value;
  upgrade = upgrade==NULL?"":upgrade;
  char *res;
  char*body = "";

  if(strcmp(upgrade,"websocket")==0){
    res ="HTTPS/1.1 101 Switching Protocols\r\n"
                "Upgrade: websocket\r\n"
                "Connection: Upgrade\r\n"
                "Content-Length: %d\r\n"
                "Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\n"
                "\r\n\r\n"
                "%s";
  }else{
    body = private->response_body ? private->response_body : "Hello, World!";
    res = "HTTPS/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: %d\r\n"
                "\r\n\r\n"
                "%s";
  }
  size_t body_length = strlen(body);
  char* content_str_length = malloc(sizeof(char)*body_length);
  sprintf(content_str_length,"%zu",body_length);
  size_t length = strlen(res) + 1 + strlen(content_str_length) + 1 + body_length;
  size_t size = length * sizeof(char);
  char *new_response = realloc(*out, size);
  snprintf(new_response, size, res, length, body);
  *out_length = length;
  *out = new_response;
  free(content_str_length);
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
