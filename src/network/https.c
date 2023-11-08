#include "https.h"

#define SOCKET int
#define CLOSESOCKET(s) close(s)
#define ISVALIDSOCKET(s) ((s) >= 0)
#define GETSOCKETERRNO() (errno)
#define ONERROR(s) ((s) < 0)

// =========================================================================="
// Prototypes functions
// =========================================================================="

void __https_cleanup(bool ignore, SSL*ssl, SOCKET sockfd);
int __https_read_response_bytes(SSL *ssl, char **response_ptr, size_t *response_size, size_t *response_capacity);
int __https_receive_response(SSL *ssl, HttpsResponse *res);

// =========================================================================="
// Public functions
// =========================================================================="

int https_fetch(HttpsRequest *request, HttpsResponse *res , SSL *ssl){
  int sockfd;
  int status = https_send_request(request,&ssl, &sockfd, res);
  status = __https_receive_response(ssl,res);
  __https_cleanup(status,ssl, sockfd);
  return status;
}

int https_get(HttpsRequest *request, HttpsResponse *res, SSL *ssl){
  request->method = HTTP_METHODS_GET;
  return https_fetch(request, res, ssl);
}

int https_post(HttpsRequest *request, HttpsResponse *res , SSL *ssl){
  request->method = HTTP_METHODS_POST;
  return https_fetch(request, res, ssl);
}

int https_update(HttpsRequest *request, HttpsResponse *res , SSL *ssl){
  request->method = HTTP_METHODS_UPDATE;
  return https_fetch(request, res, ssl);
}

int https_patch(HttpsRequest *request, HttpsResponse *res , SSL *ssl){
  request->method = HTTP_METHODS_PATCH;
  return https_fetch(request, res, ssl);
}

int https_delete(HttpsRequest *request, HttpsResponse *res , SSL *ssl){
  request->method = HTTP_METHODS_DELETE;
  return https_fetch(request, res, ssl);
}

int https_send_request(HttpsRequest *request,SSL **ssl, SOCKET *sockfd, HttpsResponse*res){
  struct addrinfo *add_info = NULL;
  int status;
  char *host = request->url->host;
  char *path = request->url->path;
  char *port = request->url->port;
  int domain_len = strlen(request->url->host);
  int error = 0;

  while (error == 0) {

    network_ini_openssl();

    status = set_ssl_context();
    if (ONERROR(status)) {
      error = get_error("SSL_CTX_new() failed.");
    }

    status = network_get_adresses(host, port, &add_info);
    if (ONERROR(status)) {
      error = get_error("getaddrinfo() failed.");
    }

    *sockfd = network_get_socket(add_info);
    if (!ISVALIDSOCKET(sockfd)) {
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

    int request_len = httpsRequest_stringify(request, &str_request);
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

// =========================================================================="
// Private functions
// =========================================================================="

void __https_cleanup(bool ignore, SSL*ssl, SOCKET sockfd){
  if(!ignore){
  if(ssl!=NULL) SSL_shutdown(ssl);
  if(ssl!=NULL) SSL_free(ssl);
  // if(ctx!=NULL) SSL_CTX_free(ctx);
  if(ISVALIDSOCKET(sockfd)) CLOSESOCKET(sockfd);
  }
}

int __https_receive_response(SSL *ssl, HttpsResponse *res) {
    size_t response_size = 0;
    size_t response_capacity = 4096;
    char *response = buffer_init(response_capacity);
    if (!response)  return get_error("malloc() failed."); 
    int status = __https_read_response_bytes(ssl, &response, &response_size, &response_capacity);
    if (status) return status; 
    httpsResponse_parse(response, res);
    free(response); 
    return 0;
}

int __https_read_response_bytes(SSL *ssl, char **response_ptr, size_t *response_size, size_t *response_capacity) {
    ssize_t bytes_received;

    while ((bytes_received = SSL_read(ssl, *response_ptr + *response_size, *response_capacity - *response_size)) > 0) {
        // system("clear");
        // printf("%s\n", *response_ptr);
        *response_size += bytes_received;
        if (*response_size == *response_capacity) {
            *response_capacity *= 2; // Double the capacity
            char *new_response = realloc(*response_ptr, *response_capacity);
            if (!new_response) {
                free(*response_ptr);
                return get_error("realloc() failed.");
            }
            *response_ptr = new_response;
        }
    }
    
    if (bytes_received < 0) {
        free(*response_ptr);
        return get_error("SSL_read() failed.");
    }
    
    (*response_ptr)[*response_size] = '\0';
    return 0;
}
