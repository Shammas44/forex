#include "wsHandler.h"
#include "common.h"
#include "https.h"
#define T WsHandler
#define x128 0x80
#define x127 0x7f
#define x126 0x7e
#define x125 0x7d

typedef enum Opcode {
  CONTINUATION=0x0,
  TEXT=0x1,
  BINARY=0x2,
  FIN=0x8,
  PING=0x9,
  PONG=0xA,
}Opcode;

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
void __wsHandler_sendClose(T*ws, SSL *ssl);

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
  ws->close = __wsHandler_sendClose;
  ws->get_https_handler = __wsHandler_get_https_handler;
  Private * private = malloc(sizeof(Private));
  private->https = https;
  ws->__private = private;
  return ws;
}

void __wsHandler_destructor(T * ws){ }

static void __wsHandler_generateMask(unsigned char mask[4]) {
    RAND_bytes(mask, 4);
}

SSL* __wsHandler_handshake(T * ws, HttpsRequest * request){
  Private * private = (Private*)ws->__private;
  Https * https = private->https;
  return https->ws_handshake(request);
}

void __wsHandler_send(T * ws, SSL *ssl, const char *message){
    // Create a WebSocket frame for sending a text message
    unsigned char frame[14]; // Assuming a maximum message length of 10 bytes plus masking
    size_t message_len = strlen(message);
    
    frame[0] = 0x81; // FIN bit set, opcode for text frame (0x81)
    
    // Determine the length of the message and set the appropriate length bits
    if (message_len <= 125) {
        frame[1] = (unsigned char)(message_len | 0x80); // Set the MASK bit
        frame[2] = '\0';
    } else if (message_len <= 65535) {
        frame[1] = 126 | 0x80; // Set the MASK bit
        frame[2] = (unsigned char)((message_len >> 8) & 0xFF);
        frame[3] = (unsigned char)(message_len & 0xFF);
    } else {
        frame[1] = 127 | 0x80; // Set the MASK bit
        for (int i = 0; i < 8; i++) {
            frame[2 + i] = (unsigned char)((message_len >> ((7 - i) * 8)) & 0xFF);
        }
    }

    // Generate a random 32-bit mask
    unsigned char mask[4];
    __wsHandler_generateMask(mask);
    memcpy(&frame[2 + ((frame[1] == 126) ? 2 : ((frame[1] == 127) ? 8 : 0))], mask, 4);

    // Send the frame header
    SSL_write(ssl, frame, 2 + ((frame[1] == 126) ? 2 : ((frame[1] == 127) ? 8 : 0)) + 4);

    // Initialize a new message buffer with masking
    unsigned char *masked_message = malloc(message_len);
    if (masked_message == NULL) {
        // Handle memory allocation error
        return;
    }

    // Apply the mask to the message
    for (size_t i = 0; i < message_len; i++) {
        masked_message[i] = message[i] ^ mask[i % 4];
    }
    
    // Send the message payload
    SSL_write(ssl, masked_message, message_len);

    // Free the masked_message buffer
    free(masked_message);
    
    // Message sent!
}

void __wsHandler_sendClose(T*ws, SSL *ssl) {
    // uint8_t mask[4];
    // __wsHandler_generateMask(mask);
    uint8_t closeFrame[] = {
        0x88, 0x80 | 0x02, // FIN bit set, Opcode for Close frame (0x88), Payload length with MASK bit
        0x00, 0x00, 0x00, 0x00, // Masking key
        0x03, 0xE8 // Status code 1000 (Normal closure)
    };
    SSL_write(ssl, closeFrame, sizeof(closeFrame));
}


int __wsHandler_listen(T * ws, SSL *ssl,void* caller,Wshandler_on_frame_receive update ){
  int status = 0;
  while (1) {
  char *out_message = NULL;
    status = __wsHandler_create_frame(ssl, &out_message);

    if (status != 0) {
      free(out_message);
      break;
    }
    update(caller,out_message);
    free(out_message);
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

  switch ((int)opcode) {
    case FIN:
      //TODO: return a status code instead of an error number
      return 1;
    case BINARY:
      return RUNTIME_ERROR("Binary format not handled by wsHandler",1);
    default:
      break;
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
