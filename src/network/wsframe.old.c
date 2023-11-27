#include "wsframe.old.h"
#define x128 0x80
#define x127 0x7f
#define x126 0x7e
#define x125 0x7d

// =========================================================================="
// Prototypes functions
// =========================================================================="

int wsframe_read_single_frame(SSL *ssl, char **out_message);
int wsframe_get_frame_length(SSL *ssl,char second_byte, int *bytes_received, int *res);

// =========================================================================="
// Public functions
// =========================================================================="

int wsframe_read_response_handshake(SSL *ssl) {
  int response_capacity = 4096;
  int response_size = 0;
  int bytes_received;
  char *out_message = buffer_init(response_capacity);
  if(!out_message) {
    return get_error("buffer_init() failed.");
  }

  char *write_ptr = out_message;  // Pointer to keep track of where to write the next chunk

  while ((bytes_received = SSL_read(ssl, write_ptr, response_capacity - response_size)) > 0) {
    response_size += bytes_received;
    write_ptr += bytes_received;
    printf("%s\n", out_message);

    if (response_size == response_capacity) {
      response_capacity *= 2; // Double the capacity
      char *new_response = realloc(out_message, response_capacity);
      if (!new_response) {
        free(out_message);
        out_message = NULL;
        return get_error("realloc() failed.");
      }
      write_ptr = new_response + response_size;  // Adjust write_ptr to new memory location
      out_message = new_response;
    }

    if (strstr(out_message, "\r\n\r\n")) {
      break;
    }
  }

  if (!bytes_received && SSL_get_error(ssl, bytes_received) != SSL_ERROR_ZERO_RETURN) {
    free(out_message);
    return get_error("SSL_read() failed.");
  }

  free(out_message);
  return 0;
}

int wsframe_read_single_frame(SSL *ssl, char **out_message) {
  unsigned char header[2];
  //TODO: SSL_read is not working properly
  int bytes_received = SSL_read(ssl, header, 2);  // Read minimal frame header
  if (bytes_received != 2) {
    return get_error("SSL_read() failed.");
  }

  char fin = (header[0] & x128) >> 7;
  char opcode = header[0] & x127;

  if (opcode != 1) {
    return get_error("opcode != 1");
  }

  unsigned char masked = (header[1] & x128) >> 7;
  int payload_length = 0;
  if(wsframe_get_frame_length(ssl, header[1], &bytes_received,  &payload_length)){
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
    free(*out_message);
    return get_error("SSL_read() failed for payload.");
  }

  (*out_message)[payload_length] = '\0';
  return 0;
}

// =========================================================================="
// Private functions
// =========================================================================="

int wsframe_get_frame_length(SSL *ssl,char second_byte, int *bytes_received, int *res){
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
