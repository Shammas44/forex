#include "https.h"

#define SOCKET int
#define CLOSESOCKET(s) close(s)
#define ISVALIDSOCKET(s) ((s) >= 0)
#define GETSOCKETERRNO() (errno)

// =========================================================================="
// Prototypes functions
// =========================================================================="

void __https_cleanup(bool ignore, SSL*ssl, SOCKET sockfd);

// =========================================================================="
// Public functions
// =========================================================================="

int https_fetch(HttpsRequest *request, HttpsResponse *res){
  SSL *ssl = NULL;
  int sockfd;
  int status = httpsRequest_send(request,&ssl, &sockfd,(struct HttpsResponse*)res);
  status = httpsResponse_receive(ssl,res);
  __https_cleanup(status,ssl, sockfd);
  return status;
}

// Function to generate a random 32-bit mask
void https_generate_mask(unsigned char mask[4]) {
    RAND_bytes(mask, 4);
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
