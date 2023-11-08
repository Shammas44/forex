#include "http.h"

#define SOCKET int
#define CLOSESOCKET(s) close(s)
#define ISVALIDSOCKET(s) ((s) >= 0)
#define GETSOCKETERRNO() (errno)
#define ONERROR(s) ((s) <0)
#define PROTOCOLS_IDS 2

// =========================================================================="
// Prototypes functions
// =========================================================================="

int fetch(HttpRequest *request,  HttpResponse *res, Http_request_type funcID);
int send_request(HttpRequest *request, SSL **ssl, int *sockfd,  HttpResponse *res);
int handle_http_response(SSL *ssl, HttpResponse *res);
int handle_ws_response(SSL *ssl);
int read_http_response(SSL *ssl, char **response_ptr, size_t *response_size, size_t *response_capacity);
void cleanup(bool ignore, SSL*ssl, SOCKET sockfd);

// =========================================================================="
// Public functions
// =========================================================================="

int http_fetch(HttpRequest *request, HttpResponse *res){
  SSL *ssl = NULL;
  int sockfd;
  int status = send_request(request,&ssl, &sockfd,res);
  status = handle_http_response(ssl,res);
  cleanup(status,ssl, sockfd);
  return status;
}

int ws_fetch(SSL **ssl, HttpRequest *request, HttpResponse *res){
  int sockfd;
  int status = 0;

  while(!status){
    if(send_request(request,ssl, &sockfd,res)) status = get_error("send_request() failed.");
    if(wsFrame_read_response_handshake(*ssl)) status = get_error("ws handshake failed."); 
    break;
  }
  int myBool = (status != 0);
  cleanup(!myBool,*ssl, sockfd);
  return status;
}

// Function to generate a random 32-bit mask
void generateMask(unsigned char mask[4]) {
    RAND_bytes(mask, 4);
}

// void ws_send_message(SSL *ssl, const char *message) {
//     // Create a WebSocket frame for sending a text message
//     unsigned char frame[14]; // Assuming a maximum message length of 10 bytes plus masking
//     size_t message_len = strlen(message);
//     
//     frame[0] = 0x81; // FIN bit set, opcode for text frame (0x81)
//     
//     // Determine the length of the message and set the appropriate length bits
//     if (message_len <= 125) {
//         frame[1] = (unsigned char)(message_len | 0x80); // Set the MASK bit
//         frame[2] = '\0';
//     } else if (message_len <= 65535) {
//         frame[1] = 126 | 0x80; // Set the MASK bit
//         frame[2] = (unsigned char)((message_len >> 8) & 0xFF);
//         frame[3] = (unsigned char)(message_len & 0xFF);
//     } else {
//         frame[1] = 127 | 0x80; // Set the MASK bit
//         for (int i = 0; i < 8; i++) {
//             frame[2 + i] = (unsigned char)((message_len >> ((7 - i) * 8)) & 0xFF);
//         }
//     }

//     // Generate a random 32-bit mask
//     unsigned char mask[4];
//     generateMask(mask);
//     memcpy(&frame[2 + ((frame[1] == 126) ? 2 : ((frame[1] == 127) ? 8 : 0))], mask, 4);

//     // Send the frame header
//     SSL_write(ssl, frame, 2 + ((frame[1] == 126) ? 2 : ((frame[1] == 127) ? 8 : 0)) + 4);

//     // Initialize a new message buffer with masking
//     unsigned char *masked_message = malloc(message_len);
//     if (masked_message == NULL) {
//         // Handle memory allocation error
//         return;
//     }

//     // Apply the mask to the message
//     for (size_t i = 0; i < message_len; i++) {
//         masked_message[i] = message[i] ^ mask[i % 4];
//     }
//     
//     // Send the message payload
//     SSL_write(ssl, masked_message, message_len);

//     // Free the masked_message buffer
//     free(masked_message);
//     
//     // Message sent!
// }



void parse_http_response(char* response, HttpResponse *res) {
    // Find the first occurrence of two new lines, this separates headers and body
    char* separator = strstr(response, "\r\n\r\n");
    if (separator == NULL) {
        fprintf(stderr, "Could not find separator between headers and body\n");
        exit(EXIT_FAILURE);
    }
    
    // Calculate the header length and copy headers to struct
    int header_length = separator - response;
    res->headers = malloc(header_length + 1);
    if (res->headers == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    strncpy(res->headers, response, header_length);
    res->headers[header_length] = '\0';

    // Copy body to struct
    res->body = strdup(separator + 4);

    // Extract status from headers
    char* status_start = strstr(res->headers, " ");
    char* status_end = strstr(status_start + 1, " ");
    int status_length = status_end - status_start;
    res->status = malloc(status_length + 1);
    if (res->status == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    strncpy(res->status, status_start, status_length);
    res->status[status_length] = '\0';
}

void free_http_response(HttpResponse* http_response) {
    free(http_response->status);
    free(http_response->headers);
    free(http_response->body);
}

int create_request_string(const HttpRequest* request, char **res) {
    // Estimate the length of the request string
    size_t length = strlen(request->method) + strlen(request->path) + strlen(request->host) + 100; // Adding 100 for additional characters

    for (size_t i = 0; i < request->header_count; i++) {
        length += strlen(request->headers[i]) + 2; // Additional characters for newline and carriage return
    }

    if (request->body) {
        length += request->body_length + 2; // Additional characters for newline and carriage return
    }

     *res = (char*)malloc(length);

    if (!res) {
        return -1; 
    }
    // Construct the request line
    int offset = sprintf(*res, "%s %s HTTP/1.1\r\nHost: %s\r\n", request->method, request->path, request->host);
    // Append the headers
    for (size_t i = 0; i < request->header_count; i++) {
        offset += sprintf((*res) + offset, "%s\r\n", request->headers[i]);
    }
    // append Content-Length header
    offset += sprintf((*res) + offset, "Content-Length: %zu\r\n", request->body_length);
    // Append an extra newline before the body, if any
    offset += sprintf((*res) + offset, "\r\n");
    // Append the body, if any
    if (request->body) {
        memcpy(*res + offset, request->body, request->body_length);
        offset += request->body_length;
    }
    // Null-terminate the request string
  (*res)[offset] = '\0';
  return offset;
}

int http_get_authorization_from_res(HttpResponse *res,char **authorization){
  jsmn_parser parser;
  jsmn_init(&parser);
  char *json = res->body;
  int required_tokens = jsmn_parse(&parser, json, strlen(json), NULL, 0);
  jsmntok_t *tokens = malloc(sizeof(jsmntok_t) * required_tokens);
  jsmn_init(&parser);
  int token_num = jsmn_parse(&parser, json, strlen(json), tokens, required_tokens);
  if (token_num < 0)  return get_error("Failed to parse JSON"); 
  if (token_num < 1 || tokens[0].type != JSMN_OBJECT) {
    return get_error("jsmn expected another type of data");
  }

  free_http_response(res);
  bool found = false;
  for (int i = 1; i < token_num && !found; i++) {
    if (tokens[i].type == JSMN_STRING && json_cmp_token_to_string(json, &tokens[i], "token") == 0) {
      i++; 

      int token_length = tokens[i].end - tokens[i].start;
      *authorization = malloc(
          sizeof(char) * (token_length + strlen("Authorization: Bearer ") + 1));
      if (*authorization == NULL) {
        printf("Memory allocation failed!\n");
        break; 
      }

      int t = sprintf(*authorization, "Authorization: Bearer %.*s", token_length, json + tokens[i].start);
      // *authorization[t + 1] = '\0';
      (*authorization)[t] = '\0';
      found = true;
    }
  }
  return 0;
}

// =========================================================================="
// Private functions
// =========================================================================="

int read_http_response(SSL *ssl, char **response_ptr, size_t *response_size, size_t *response_capacity) {
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

int send_request(HttpRequest *request, SSL **ssl, int *sockfd,  HttpResponse *res ){
  struct addrinfo *add_info = NULL;
  int status;
  int domain_len = strlen(request->host);
  int error = 0;

  while(error == 0){

    network_ini_openssl();

    status = set_ssl_context();
    if (ONERROR(status)){
      error = get_error("SSL_CTX_new() failed.");
    }

    status = network_get_adresses(request->host, request->port, &add_info);
    if (ONERROR(status)){
      error = get_error("getaddrinfo() failed.");
    }

    *sockfd = network_get_socket(add_info);
    if (!ISVALIDSOCKET(sockfd)){
      error = get_error("socket() failed.");
    }

    status = connect(*sockfd, add_info->ai_addr, add_info->ai_addrlen);
    if (status){
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

    if (!SSL_set_tlsext_host_name(*ssl, request->host)) {
      error = get_error("SSL_set_tlsext_host_name() failed.");
    }

    SSL_set_fd(*ssl, *sockfd);
    if (SSL_connect(*ssl) == -1) {
      error = get_error("SSL_connect() failed.");
    }

    char *str_request = NULL;
    int request_len = create_request_string(request,&str_request);
    status = SSL_write(*ssl, str_request, request_len);
    free(str_request);
    if (status == -1){
      error = get_error("ssl_write() failed.");
    }
    break;
  }
  if(add_info!=NULL) freeaddrinfo(add_info);

  return error;
}

int handle_http_response(SSL *ssl, HttpResponse *res) {
    size_t response_size = 0;
    size_t response_capacity = 4096;
    char *response = buffer_init(response_capacity);
    if (!response)  return get_error("malloc() failed."); 
    int status = read_http_response(ssl, &response, &response_size, &response_capacity);
    if (status) return status; 
    parse_http_response(response, res);
    free(response); 
    return 0;
}

void cleanup(bool ignore, SSL*ssl, SOCKET sockfd){
  if(!ignore){
  if(ssl!=NULL) SSL_shutdown(ssl);
  if(ssl!=NULL) SSL_free(ssl);
  // if(ctx!=NULL) SSL_CTX_free(ctx);
  if(ISVALIDSOCKET(sockfd)) CLOSESOCKET(sockfd);
  }
}
