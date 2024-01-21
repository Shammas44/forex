#include <netdb.h>
#include "url.h"
#include "isDestroyable.h"
#ifndef NETWORK_H
#define NETWORK_H

#define URL_LEN 17
#define PROTOCOL_LEN 10
#define T Network

typedef struct T T;

int network_get_adresses(const char *url, const char *port, struct addrinfo **results); 
int network_get_socket(struct addrinfo *addresses); 
void network_free(const char *error_msg, int sockfd, struct addrinfo *addresses); 
int network_socket_connect(int sockfd, struct addrinfo *address);
int network_address_to_human_format(struct addrinfo *addresse, char *ip, char *protocol);

struct T {
    IsDestroyable __destructor;
    void (*destructor)(T*self);
    struct addrinfo* (*adresses)(T*self,Url*url); 
    int (*socket)(T*self,struct addrinfo *addresses); 
    void (*free)(T*self, int sockfd, struct addrinfo *addresses); 
    int (*connect)(T*self,int sockfd, struct addrinfo *address);
    int (*address_to_human_format)(T*self,struct addrinfo *addresse, char *ip, char *protocol);
    void*__private;
};

T* network_constructor();
#undef T
#endif
