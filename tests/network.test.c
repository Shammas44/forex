#include "network.h"
#include <criterion/criterion.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Test(fetch, is_defined){
//   int error = fetch("google.com","","");
// }

// Test(fetch, truc){
//   int error = fetch(" ","","");
//   cr_assert_eq(1,error);
// }

Test(get_ip_adresses, is_defined) {
  struct addrinfo *res = NULL;
  int status = network_get_adresses("", "", &res);
  if (!status && res != NULL)
    freeaddrinfo(res);
}

Test(get_ip_adresses, is_valid_address) {
  struct addrinfo *res = NULL;
  int status = network_get_adresses("google.com", "8080", &res);
  cr_assert_eq(0, status);
  if (!status && res != NULL)
    freeaddrinfo(res);
}

Test(get_ip_adresses, is_not_valid_address) {
  struct addrinfo *res = NULL;
  int status = network_get_adresses("   ", "8080", &res);
  cr_assert_eq(1, status);
  if (!status && res != NULL)
    freeaddrinfo(res);
}

Test(get_first_ip_address, is_not_valid_address) {
  char url[URL_LEN];
  char protocol[PROTOCOL_LEN];
  struct addrinfo *res = NULL;
  int status1 = network_get_adresses("", "8080", &res);
  int status2 = network_address_to_human_format(res, url, protocol);
  // printf("\t%s %s\n", url, protocol);
  // cr_assert_eq(1, status);
  if (!status2 && res != NULL)
    freeaddrinfo(res);
}

Test(get_first_ip_address, fetch) {
  char* headers[] = {
    // "Content-Type: application/json",
    "Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiJhZGFkIiwiZXhwIjoxNjkxNzA1MDI2LCJzY29wZSI6InVzZXIiLCJpYXQiOjE2OTExMDAyMjZ9.bQXpJ438dZK7O61e_146A_SvAsqacISzEzvqb8OaqAc",
    // "Accept-Encoding: gzip",
  };
    // char body[] = "{\"key\":\"value\"}";
    char body[] = "";
    HttpRequest postrequest = {
        .host = "localhost",
        .path = "/api/cda",
        .method = "GET",
        .port = "3000",
        .body = "",
        .body_length = sizeof(body) > 1 ? sizeof(body) - 1:0, // Not including the null terminator
        .headers = headers,
        .header_count = sizeof(headers) / sizeof(headers[0]),
    };
  HttpResponse res;  
  fetch(&postrequest,&res);
  // printf("Status: %s\n", res.status);
  // printf("Body: %s\n", res.headers);
  // printf("Status: %s\n", res.status);
  free_http_response(&res);
}

