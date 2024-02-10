#include "NetworkWrapperStub.h"
#include "Hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define T NetworkWrapperStub

typedef struct {
  Hashmap*sockfd;
} Private;

static struct addrinfo* __adresses(T*self,Url*url);
static int __socket(T*self,struct addrinfo *addresses);
static void __free(T*self, int sockfd, struct addrinfo *addresses);
static int __connect(T*self,int sockfd, struct addrinfo *address);
static int __address_to_human_format(T*self,struct addrinfo *addresse, char *ip, char *protocol);
static void __close_sockfd(T*self, int sockfd);
static void __destructor(T*self);

T*networkWrapperStub_constructor(){
  T*self = (T*)malloc(sizeof(T));
  self->__destructor = (IsDestroyable){.destructor = __destructor};
  Private *private = (Private *)malloc(sizeof(Private));
  private->sockfd = hashmap_constructor(10);
  self->__private = private;
  self->adresses = __adresses;
  self->socket = __socket;
  self->close_sockfd = __close_sockfd;
  self->destructor = __destructor;
  self->connect = __connect;
  self->free = __free;
  return self;
}

static void __destructor(T*self){
  Private *private = self->__private;
  Hashmap*sockfd = private->sockfd;
  sockfd->destructor(sockfd);
  free(private);
  free(self);
}

static void __close_sockfd(T*self, int sockfd){
  return;
}

static struct addrinfo* __adresses(T*self,Url*url) {
    struct addrinfo *mock_addrinfo = malloc(sizeof(struct addrinfo));
    memset(mock_addrinfo, 0, sizeof(struct addrinfo));

    mock_addrinfo->ai_family = AF_UNSPEC;
    mock_addrinfo->ai_socktype = SOCK_STREAM;
    mock_addrinfo->ai_protocol = IPPROTO_TCP;
    mock_addrinfo->ai_next = NULL;
    mock_addrinfo->ai_canonname = NULL;
    return mock_addrinfo;
}

static int __socket(T*self,struct addrinfo *addresses) {
  Private *private = (Private *)self->__private;
  Hashmap *sockets = private->sockfd;
  size_t length = sockets->length(sockets);
  int *index = malloc(sizeof(int));
  *index = length +10;
  char*key = malloc(sizeof(char)*12);
  sprintf(key, "%d", *index);
  Hashmap_Entry item ={.key=key,.type=Item_int, .value=index};
  sockets->push(sockets, item);
  return *index;
}

static void __free(T*self, int sockfd, struct addrinfo *addresses) {
  Private *private = (Private *)self->__private;
  Hashmap *sockets = private->sockfd;
  // sockets->delete(sockets, sockfd);
  freeaddrinfo(addresses);
}

static int __connect(T*self,int sockfd, struct addrinfo *address) {
  return 0;
}

static int __address_to_human_format(T*self,struct addrinfo *addresse, char *ip, char *protocol) {
  int status = getnameinfo(addresse->ai_addr, addresse->ai_addrlen, ip, URL_LEN,
                           protocol, PROTOCOL_LEN, NI_NUMERICHOST);
  if (status != 0)
    return EXIT_FAILURE;
  return 0;
}

#undef T
