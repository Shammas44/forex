#include <netdb.h>
#ifndef NETWORK_H
#define NETWORK_H

#define URL_LEN 17
#define PROTOCOL_LEN 10

int network_get_adresses(const char *url, const char *port, struct addrinfo **results); 

int network_get_socket(struct addrinfo *addresses); 

void network_free(const char *error_msg, int sockfd, struct addrinfo *addresses); 

int network_socket_connect(int sockfd, struct addrinfo *address);

int network_address_to_human_format(struct addrinfo *addresse, char *ip, char *protocol);

#endif
