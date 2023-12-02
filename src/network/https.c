#include "https.h"
#include "httpsResponseBuilder.h"
#include <openssl/err.h>
#include <openssl/rand.h>

#define SOCKET int
#define CLOSESOCKET(s) close(s)
#define ISVALIDSOCKET(s) ((s) >= 0)
#define GETSOCKETERRNO() (errno)
#define ONERROR(s) ((s) < 0)
#define MAX_TIMEOUT_SECONDS 5 // Change this according to your desired timeout

// =========================================================================="
// Prototypes functions
// =========================================================================="

HttpsResponse* __https_fetch(Https *https, HttpsRequest *request);
HttpsResponse* __https_get(Https *https, HttpsRequest *request);
HttpsResponse* __https_post(Https *https, HttpsRequest*request);
HttpsResponse* __https_put(Https *https, HttpsRequest *request);
HttpsResponse* __https_patch(Https *https, HttpsRequest *request);
HttpsResponse* __https_delete(Https *https, HttpsRequest *request);
SSL* __https_ws_handshake(Https *https, HttpsRequest *request);
void __https_destructor(Https *https);
int __https_send_request(HttpsRequest *request, SSL **ssl, int* sockfd);
void __https_cleanup(SSL *ssl, int sockfd);
int __https_read_response_bytes(SSL *ssl, char **response_ptr, size_t *response_size, size_t *response_capacity);
HttpsResponse *__https_receive(SSL *ssl);

// =========================================================================="
// Public functions
// =========================================================================="

Https * https_constructor(){
  Https * https = (Https*)malloc(sizeof(Https));
  HttpsResponseBuilder * response_builder = httpsResponseBuilder_constructor();
  https->destructor = __https_destructor;
  https->response_builder = response_builder;
  https->ws_handshake = __https_ws_handshake;
  https->get = __https_get;
  https->post = __https_post;
  https->put = __https_put;
  https->patch = __https_patch;
  https->delete = __https_delete;
  return https;
}

// =========================================================================="
// Private functions
// =========================================================================="

void __https_destructor(Https *https){
  HttpsResponseBuilder * response_builder = https->response_builder;
  response_builder->destructor(response_builder);
  free(https);
  if(ctx!=NULL) SSL_CTX_free(ctx);
}

HttpsResponse* __https_fetch(Https *https, HttpsRequest *request){
  HttpsResponseBuilder *res_builder = https->response_builder;
  SSL *ssl = NULL;
  int *sockfd = malloc(sizeof(int));

  int status = __https_send_request(request, &ssl, sockfd);
  HttpsResponse * response = __https_receive(ssl);
  if(!response){
    return NULL;
  }
  __https_cleanup(ssl, *sockfd);
  request->destructor(request);
  return response;
}

SSL* __https_ws_handshake(Https *https, HttpsRequest *request){
  SSL *ssl = NULL;
  int *sockfd = malloc(sizeof(int));

  request->set_method(request,GET);
  int status1 = __https_send_request(request, &ssl, sockfd);
  HttpsResponse * res = __https_receive(ssl);
  if(!res){
    return NULL;
  }
  char *status = res->get_status(res);

  if(strcmp("101",status) != 0) __https_cleanup(ssl, *sockfd);
  request->destructor(request);
  return ssl;
}

HttpsResponse* __https_get(Https *https, HttpsRequest *request){
  request->set_method(request,GET);
  return __https_fetch(https, request);
}

HttpsResponse* __https_post(Https *https, HttpsRequest*request){
  request->set_method(request,POST);
  return __https_fetch(https, request);
}

HttpsResponse* __https_put(Https *https, HttpsRequest *request){
  request->set_method(request,PUT);
  return __https_fetch(https, request);
}

HttpsResponse* __https_patch(Https *https, HttpsRequest *request){
  request->set_method(request,PATCH);
  return __https_fetch(https, request);
}

HttpsResponse* __https_delete(Https *https, HttpsRequest *request){
  request->set_method(request,DELETE);
  return __https_fetch(https, request);
}

int __https_send_request(HttpsRequest *request, SSL **ssl, int* sockfd){
  struct addrinfo *add_info = NULL;
  Message *message = request->__private;
  Url * url = request->get_url(request);
  int status;
  char *host = url->host;
  char *path = url->path;
  char *port = url->port;
  int domain_len = strlen(url->host);
  int error = 0;

  while (error == 0) {

    network_ini_openssl();

    status = set_ssl_context();
    if (ONERROR(status)) {
      error = get_error("SSL_CTX_new() failed.");
    }

    //disble ssl certificate verification -- only for dev
    // SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);

    status = network_get_adresses(host, port, &add_info);
    if (ONERROR(status)) {
      error = get_error("getaddrinfo() failed.");
    }

    *sockfd = network_get_socket(add_info);
    if (!ISVALIDSOCKET(*sockfd)) {
      error = get_error("socket() failed.");
    }

    status = connect(*sockfd, add_info->ai_addr, add_info->ai_addrlen);
    if (status) {
      error = get_error("connect() failed.");
    }

    if (!ctx) {
      error = get_error("SSL_CTX is NULL.");
    } else {
      *ssl = SSL_new(ctx);
      if (!ssl) {
        error = get_error("SSL_new() failed.");
      }
    }

    if (!SSL_set_tlsext_host_name(*ssl, host)) {
      error = get_error("SSL_set_tlsext_host_name() failed.");
    }

    SSL_set_fd(*ssl, *sockfd);
    if (SSL_connect(*ssl) == -1) {
      error = get_error("SSL_connect() failed.");
    }

    char *str_request = NULL;

    int request_len = request->stringify(request, &str_request);
    status = SSL_write(*ssl, str_request, request_len);

    free(str_request);
    if (status == -1) {
      error = get_error("ssl_write() failed.");
    }
    break;
  }
  if (add_info != NULL)
    freeaddrinfo(add_info);

  return error;
}

HttpsResponse *__https_receive(SSL *ssl) {
  size_t response_size = 0;
  size_t response_capacity = 4096;
  char *raw_response = buffer_init(response_capacity);
  int status = __https_read_response_bytes(
      ssl, &raw_response, &response_size, &response_capacity);
  if (status)
    return NULL;
  HttpsResponse * response = httpsResponse_constructor(raw_response);
  free(raw_response);
  return response;
}

int __https_read_response_bytes(SSL *ssl, char **response_ptr, size_t *response_size, size_t *response_capacity) {
    ssize_t bytes_received;
    int socket_fd = SSL_get_fd(ssl);

    //TODO: invesiagte why does the following code interrupt data flow in ws listening
    // Set socket to non-blocking mode
    // if (setSocketNonBlocking(socket_fd) == -1) {
    //     return -1;
    // }

    int total_wait_seconds = 0;

    while (total_wait_seconds < MAX_TIMEOUT_SECONDS) {
        bytes_received = SSL_read(ssl, *response_ptr + *response_size, *response_capacity - *response_size);
        if (bytes_received > 0) {
            *response_size += bytes_received;
            if (*response_size == *response_capacity) {
                *response_capacity *= 2; // Double the capacity
                char *new_response = realloc(*response_ptr, *response_capacity);
                if (!new_response) {
                    free(*response_ptr);
                    return -1; // Adjust this error handling according to your needs
                }
                *response_ptr = new_response;
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
                printf("SSL read timeout occurred after %d seconds.\n", MAX_TIMEOUT_SECONDS);
                return -1; // Timeout error
            } else {
                // Other error occurred
                fprintf(stderr, "SSL read error: %s\n", ERR_reason_error_string(ERR_get_error()));
                return -1; // Adjust error handling as needed
            }
        }
    }

    // Timeout occurred
    printf("SSL read timeout occurred after %d seconds.\n", MAX_TIMEOUT_SECONDS);
    return -1; // Timeout error
}

void __https_cleanup(SSL *ssl, int sockfd){
  if(ssl!=NULL) SSL_shutdown(ssl);
  if(ISVALIDSOCKET(sockfd)) CLOSESOCKET(sockfd);
}

#undef MAX_TIMEOUT_SECONDS
