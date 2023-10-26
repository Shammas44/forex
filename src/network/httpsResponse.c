#include "httpsResponse.h"

// =========================================================================="
// Prototypes functions
// =========================================================================="

int __httpsResponse_read_bytes(SSL *ssl, char **response_ptr, size_t *response_size, size_t *response_capacity);

// =========================================================================="
// Public functions
// =========================================================================="

void httpsResponse_parse(char* response, HttpsResponse *res) {
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

void httpsResponse_free(HttpsResponse* http_response) {
    free(http_response->status);
    free(http_response->headers);
    free(http_response->body);
}

int httpsResponse_extract_authorization(HttpsResponse *res,char **authorization){
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

  httpsResponse_free(res);
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

int httpsResponse_receive(SSL *ssl, HttpsResponse *res) {
    size_t response_size = 0;
    size_t response_capacity = 4096;
    char *response = buffer_init(response_capacity);
    if (!response)  return get_error("malloc() failed."); 
    int status = __httpsResponse_read_bytes(ssl, &response, &response_size, &response_capacity);
    if (status) return status; 
    httpsResponse_parse(response, res);
    free(response); 
    return 0;
}

// =========================================================================="
// Private functions
// =========================================================================="

int __httpsResponse_read_bytes(SSL *ssl, char **response_ptr, size_t *response_size, size_t *response_capacity) {
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



