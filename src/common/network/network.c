#include "network.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define T Network
#define CLOSESOCKET(s) close(s)
#define ISVALIDSOCKET(s) ((s) >= 0)

typedef struct {

} Private;

static struct addrinfo* __adresses(T*self,Url*url);
static int __socket(T*self,struct addrinfo *addresses);
static void __free(T*self, int sockfd, struct addrinfo *addresses);
static void __close_sockfd(T*self, int sockfd); 
static int __connect(T*self,int sockfd, struct addrinfo *address);
static int __address_to_human_format(T*self,struct addrinfo *addresse, char *ip, char *protocol);
static void __close_sockfd(T*self, int sockfd);
static void __destructor(T*self);

T*network_constructor(){
  T*self = (T*)malloc(sizeof(T));
  self->__destructor = (IsDestroyable){.destructor = __destructor};
  Private *private = (Private *)malloc(sizeof(Private));
  private = NULL;
  self->__private = private;
  self->adresses = __adresses;
  self->destructor = __destructor;
  self->free = __free;
  self->connect = __connect;
  self->close_sockfd = __close_sockfd;
  self->socket = __socket;
  self->address_to_human_format = __address_to_human_format;
  return self;
}

static void __destructor(T*self){
  Private *private = self->__private;
  free(private);
  free(self);
}

static void __close_sockfd(T*self, int sockfd){
  if(ISVALIDSOCKET(sockfd)) CLOSESOCKET(sockfd);
}

static struct addrinfo* __adresses(T*self,Url*url) {
  struct addrinfo *results = NULL;
  int addressSize = strlen(url->host);
  char address[addressSize + 1];
  strcpy(address, strcmp(url->host, "") != 0 ? url->host : "127.0.0.1");

  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;     // ipv4/ipv6
  hints.ai_socktype = SOCK_STREAM; // TCP

  int error = getaddrinfo(address, url->port, &hints, &results);
  if(error) RUNTIME_ERROR("getaddrinfo() failed.",1);
  return results;
}

static int __socket(T*self,struct addrinfo *addresses) {
  int sockfd = socket(addresses->ai_family,
                      addresses->ai_socktype,
                      addresses->ai_protocol);
    if (sockfd<0) {
      RUNTIME_ERROR("socket() failed.",1);
      sockfd = -1;
    }
  return sockfd;
}

static void __free(T*self, int sockfd, struct addrinfo *addresses) {
  close(sockfd);
  freeaddrinfo(addresses);
}

static int __connect(T*self,int sockfd, struct addrinfo *address) {
  return connect(sockfd, address->ai_addr, address->ai_addrlen);
}

static int __address_to_human_format(T*self,struct addrinfo *addresse, char *ip, char *protocol) {
  int status = getnameinfo(addresse->ai_addr,
                           addresse->ai_addrlen,
                           ip,
                           URL_LEN,
                           protocol,
                           PROTOCOL_LEN,
                           NI_NUMERICHOST);
  if (status != 0)
    return EXIT_FAILURE;
  return 0;
}

#undef T
#undef CLOSESOCKET
#undef ISVALIDSOCKET
