#include "https.h"
#include "HttpsResponseParser.h"
#include "json.h"
#include <openssl/err.h>
#include <openssl/rand.h>

#define SOCKET int
#define CLOSESOCKET(s) close(s)
#define ISVALIDSOCKET(s) ((s) >= 0)
#define GETSOCKETERRNO() (errno)
#define ONERROR(s) ((s) < 0)
#define MAX_TIMEOUT_SECONDS 5 // Change this according to your desired timeout
#define T Https

typedef struct {
  Parser *response_parser;
  Network*network;
  SslWrapper*ssl;
} Private;

// =========================================================================="
// Prototypes functions
// =========================================================================="

static void __destructor(Https *https);
static HttpsResponse* __fetch(T*self,HttpsRequest *request);
static HttpsResponse* __get(T*self,HttpsRequest *request);
static HttpsResponse* __post(T*self,HttpsRequest*request);
static HttpsResponse* __put(T*self,HttpsRequest *request);
static HttpsResponse* __patch(T*self,HttpsRequest *request);
static HttpsResponse* __delete(T*self,HttpsRequest *request);
static SSL* __ws_handshake(T*self,HttpsRequest *request);

static int _$send_request(T*self,HttpsRequest *request, SSL **ssl, int* sockfd);
static void _$cleanup(SSL *ssl, int sockfd);
static HttpsResponse *_$receive(T*self,SSL *ssl);
static HttpsResponse * _$fetch_keep_open(T*self,HttpsRequest *request, SSL**ssl);

// =========================================================================="
// Public functions
// =========================================================================="

T * https_constructor(Network*network,SslWrapper*ssl){
  if(network == NULL) return NULL;
  if(ssl == NULL) return NULL;
  Https * https = (Https*)malloc(sizeof(Https));
  Private *private = malloc(sizeof(Private));
  private->response_parser = httpsResponseParser_constructor();
  private->network = network;
  private->ssl = ssl;
  https->__private = private;
  https->destructor = __destructor;
  https->ws_handshake = __ws_handshake;
  https->fetch = __fetch;
  https->get = __get;
  https->post = __post;
  https->put = __put;
  https->patch = __patch;
  https->delete = __delete;
  return https;
}

// =========================================================================="
// Private functions
// =========================================================================="

static void __destructor(T *self){
  Private *private = self->__private;
  JsonParser *parser = private->response_parser;
  parser->destructor(parser);
  free(private);
  free(self);
  if(ctx!=NULL) SSL_CTX_free(ctx);
}

static HttpsResponse* __fetch(T*self,HttpsRequest *request){
  SSL *ssl = NULL;
  int *sockfd = malloc(sizeof(int));

  int status = _$send_request(self,request, &ssl, sockfd);
  if(status >0) return NULL;
  HttpsResponse * response = _$receive(self,ssl);
  if(!response){
    return NULL;
  }
  _$cleanup(ssl, *sockfd);
  request->destructor(request);
  return response;
}

static HttpsResponse * _$fetch_keep_open(T*self,HttpsRequest *request, SSL**ssl){
  int *sockfd = malloc(sizeof(int));

  int status = _$send_request(self,request, ssl, sockfd);
  if(status>0) return NULL;
  HttpsResponse * response = _$receive(self,*ssl);
  if(!response){
    return NULL;
  }
  return response;
}

static SSL* __ws_handshake(T*self,HttpsRequest *request){
  SSL *ssl = NULL;
  int *sockfd = malloc(sizeof(int));

  request->set_method(request,GET);
  int status1 = _$send_request(self,request, &ssl, sockfd);
  if(status1 >0) return NULL;
  HttpsResponse * res = _$receive(self,ssl);
  if(!res){
    return NULL;
  }
  char *status = res->status(res);

  if(strcmp("101",status) != 0) _$cleanup(ssl, *sockfd);
  request->destructor(request);
  return ssl;
}

static HttpsResponse* __get(T*self,HttpsRequest *request){
  request->set_method(request,GET);
  return __fetch(self,request);
}

static HttpsResponse* __post(T*self,HttpsRequest*request){
  request->set_method(request,POST);
  return __fetch(self,request);
}

static HttpsResponse* __put(T*self,HttpsRequest *request){
  request->set_method(request,PUT);
  return __fetch(self,request);
}

static HttpsResponse* __patch(T*self,HttpsRequest *request){
  request->set_method(request,PATCH);
  return __fetch(self,request);
}

static HttpsResponse* __delete(T*self,HttpsRequest *request){
  request->set_method(request,DELETE);
  return __fetch(self,request);
}

static int _$send_request(T*self, HttpsRequest *request, SSL **ssl, int* sockfd){
  Private * private = self->__private;
  SslWrapper *sslWrapper = private->ssl;
  Network *network = private->network;
  struct addrinfo *add_info = NULL;
  Url * url = request->get_url(request);
  int status;
  char *host = url->host;
  char *port = url->port;
  int error = 0;

  while (error == 0) {

    add_info = network->adresses(network,url);
    if (ONERROR(add_info)) break;

    *sockfd = network->socket(network,add_info);
    if (!ISVALIDSOCKET(*sockfd)) break;

    status = network->connect(network,*sockfd, add_info);
    if (status) break;

    if (!ctx) break;

    *ssl = sslWrapper->new(sslWrapper,ctx);
    if (!ssl) break; 

    if (!sslWrapper->set_host(sslWrapper,*ssl, host)) break; 

    status = sslWrapper->set_fd(sslWrapper,*ssl,*sockfd);
    if(status<0) break;

    status = sslWrapper->connect(sslWrapper,*ssl);
    if(status<0) break;

    char *str_request = NULL;

    int request_len = request->stringify(request, &str_request);
    status = sslWrapper->write(sslWrapper,*ssl,str_request,request_len);

    free(str_request);
    if (status == -1) break; 
    break;
  }
  if (add_info != NULL) freeaddrinfo(add_info);
  return error;
}

static HttpsResponse *_$receive(T*self,SSL *ssl) {
  Private *private = self->__private;
  SslWrapper *sslWrapper = private->ssl;
  HttpsResponseParser *parser = private->response_parser;
  size_t response_size = 0;
  size_t response_capacity = 4096;
  char *raw_response = buffer_init(response_capacity);
  int status = sslWrapper->read(sslWrapper,ssl, &raw_response, &response_size);
  if (status)
    return NULL;

  Hashmap* map = parser->parse(parser,raw_response);
  HttpsResponse * response = httpsResponse_constructor(map);
  free(raw_response);
  return response;
}

static void _$cleanup(SSL *ssl, int sockfd){
  if(ssl!=NULL) SSL_shutdown(ssl);
  if(ISVALIDSOCKET(sockfd)) CLOSESOCKET(sockfd);
}

#undef MAX_TIMEOUT_SECONDS
#undef T
