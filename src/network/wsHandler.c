#include "wsHandler.h"
#include "https.h"
#define T WsHandler
#define x128 0x80
#define x127 0x7f
#define x126 0x7e
#define x125 0x7d


enum err {
SSL_READ_FAILED,
};

static char* errors[50] = { 
  "SSL_read() failed.",
};

void __wsHandler_destructor(T * ws);
SSL* __wsHandler_handshake(T * ws, HttpsRequest * request);
void __wsHandler_send(T * ws, SSL *ssl, const char *message);
int __wsHandler_listen(T * ws, SSL *ssl,void* caller,Wshandler_on_frame_receive update );
int __wsHandler_get_frame_length(SSL *ssl,char second_byte, int *bytes_received, int *res);
int __wsHandler_create_frame(SSL *ssl, char **out_message);
Https* __wsHandler_get_https_handler(T*ws);

typedef struct Private {
  Https *https;
}Private;

T *wsHandler_constructor(Https * https){
  if(!https) return NULL;
  T * ws = malloc(sizeof(T));
  ws->destructor = __wsHandler_destructor;
  ws->handshake = __wsHandler_handshake;
  ws->send = __wsHandler_send;
  ws->listen = __wsHandler_listen;
  ws->get_https_handler = __wsHandler_get_https_handler;
  Private * private = malloc(sizeof(Private));
  private->https = https;
  ws->__private = private;
  return ws;
}

void __wsHandler_destructor(T * ws){ }

SSL* __wsHandler_handshake(T * ws, HttpsRequest * request){
  Private * private = (Private*)ws->__private;
  Https * https = private->https;
  return https->ws_handshake(request);
}

void __wsHandler_send(T * ws, SSL *ssl, const char *message){}

int __wsHandler_listen(T * ws, SSL *ssl,void* caller,Wshandler_on_frame_receive update ){
  int status = 0;
  while (1) {
  char *out_message = NULL;
    status = __wsHandler_create_frame(ssl, &out_message);

    if (status != 0) {
      free(out_message);
      break;
    }
    // void * value = parser->parse(parser,out_message);
    // printf("%s\n", out_message);
    update(caller,out_message);
    free(out_message);
    // subject->notify(subject);
  }
  return status;
}

int __wsHandler_create_frame(SSL *ssl, char **out_message) {
  if(!ssl) return 1;


  unsigned char header[2];
  //TODO: SSL_read is not working properly
  int bytes_received = SSL_read(ssl, header, 2);  // Read minimal frame header
  if (bytes_received <= 0) {
    int ssl_error = SSL_get_error(ssl, bytes_received);
    printf("SSL_get_error: %d\n", ssl_error);
    switch (ssl_error) {
    case SSL_ERROR_WANT_READ:
      printf("SSL_ERROR_WANT_READ\n");
      return 0;
      break; 
    }
    return get_error("SSL_read() failed.");
  }

  char fin = (header[0] & x128) >> 7;
  char opcode = header[0] & x127;

  if (opcode != 1) {
    return get_error("opcode != 1");
  }

  unsigned char masked = (header[1] & x128) >> 7;
  int payload_length = 0;
  if(__wsHandler_get_frame_length(ssl, header[1], &bytes_received,  &payload_length)){
    return get_error("wsframe_get_length() failed.");
  }

  unsigned char mask_key[4];
  if (masked) {
    bytes_received = SSL_read(ssl, mask_key, 4);
    if (bytes_received != 4) {
      return get_error("SSL_read() failed for mask.");
    }
  }

  *out_message = (char *) malloc(payload_length + 1);
  if (!*out_message) {
    return get_error("malloc() failed.");
  }

  bytes_received = SSL_read(ssl, *out_message, payload_length);
  if (bytes_received != payload_length) {
    return get_error("SSL_read() failed for payload.");
  }

  (*out_message)[payload_length] = '\0';
  return 0;
}

int __wsHandler_get_frame_length(SSL *ssl,char second_byte, int *bytes_received, int *res){
    unsigned char payload_length_indicator = second_byte & x127;

     *res = payload_length_indicator;
    if (payload_length_indicator == x126) {
        unsigned char extended_payload[2];
        *bytes_received = SSL_read(ssl, extended_payload, 2);
        if (*bytes_received != 2) {
            return get_error("SSL_read() failed for extended payload length.");
        }
        *res = (extended_payload[0] << 8) + extended_payload[1];
    } else if (payload_length_indicator == x127) {
        unsigned char extended_payload[8];
        *bytes_received = SSL_read(ssl, extended_payload, 8);
        if (*bytes_received != 8) {
            return get_error("SSL_read() failed for extended payload length.");
        }
        *res = 
            ((int64_t)extended_payload[0] << 56) + 
            ((int64_t)extended_payload[1] << 48) +
            ((int64_t)extended_payload[2] << 40) + 
            ((int64_t)extended_payload[3] << 32) +
            ((int64_t)extended_payload[4] << 24) +
            ((int64_t)extended_payload[5] << 16) +
            ((int64_t)extended_payload[6] << 8)  +
            (int64_t)extended_payload[7];
    }
  return 0;
}

Https* __wsHandler_get_https_handler(T*ws){
  return ((Private*)ws->__private)->https;
}

#undef T
