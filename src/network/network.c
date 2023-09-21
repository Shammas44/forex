#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int network_get_adresses(const char *url, const char *port, struct addrinfo **results) {
  int addressSize = strlen(url);
  char address[addressSize + 1];
  strcpy(address, strcmp(url, "") != 0 ? url : "127.0.0.1");

  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;     // ipv4/ipv6
  hints.ai_socktype = SOCK_STREAM; // TCP

  if (getaddrinfo(address, port, &hints, results))
    return EXIT_FAILURE;
  return 0;
}

int network_get_socket(struct addrinfo *addresses) {
  int sockfd = socket(addresses->ai_family, addresses->ai_socktype,
                      addresses->ai_protocol);
  return sockfd;
}

void network_free(const char *error_msg, int sockfd, struct addrinfo *addresses) {
  if (strcmp(error_msg, "") != 0){
    perror(error_msg);
  }
  close(sockfd);
  freeaddrinfo(addresses);
}

int network_socket_connect(int sockfd, struct addrinfo *address) {
  return connect(sockfd, address->ai_addr, address->ai_addrlen);
}

int network_address_to_human_format(struct addrinfo *addresse, char *ip, char *protocol) {
  int status = getnameinfo(addresse->ai_addr, addresse->ai_addrlen, ip, URL_LEN,
                           protocol, PROTOCOL_LEN, NI_NUMERICHOST);
  if (status != 0)
    return EXIT_FAILURE;
  return 0;
}
