#include "ws.h"
#include "https.h"

#define SOCKET int
#define CLOSESOCKET(s) close(s)
#define ISVALIDSOCKET(s) ((s) >= 0)
#define GETSOCKETERRNO() (errno)
#define ONERROR(s) ((s) <0)
#define PROTOCOLS_IDS 2

// =========================================================================="
// Prototypes functions
// =========================================================================="

void __ws_generateMask(unsigned char mask[4]);
void __ws_cleanup(bool ignore, SSL*ssl, SOCKET sockfd);

// =========================================================================="
// Public functions
// =========================================================================="

int ws_establish_handshake(SSL **ssl, Request *request, Response *res) {
  // int sockfd;
  // int status = 0;

  // while(!status){
  //   if(https_send_request(request,ssl, &sockfd,res)) status = get_error("send_request() failed.");
  //   if(wsFrame_read_response_handshake(*ssl)) status = get_error("ws handshake failed."); 
  //   break;
  // }
  // int myBool = (status != 0);
  // __ws_cleanup(!myBool,*ssl, sockfd);
  // return status;
  return 0;
}

void ws_send_message(SSL *ssl, const char *message) {
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
    __ws_generateMask(mask);
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

// =========================================================================="
// Private functions
// =========================================================================="

// Function to generate a random 32-bit mask
void __ws_generateMask(unsigned char mask[4]) {
    RAND_bytes(mask, 4);
}

void __ws_cleanup(bool ignore, SSL*ssl, SOCKET sockfd){
  if(!ignore){
  if(ssl!=NULL) SSL_shutdown(ssl);
  if(ssl!=NULL) SSL_free(ssl);
  // if(ctx!=NULL) SSL_CTX_free(ctx);
  if(ISVALIDSOCKET(sockfd)) CLOSESOCKET(sockfd);
  }
}
