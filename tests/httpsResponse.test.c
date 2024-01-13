#include "httpsResponse.h"
#include "httpsResponse.keys.h"
#include "RuntimeErrorImp.h"
#include <criterion/criterion.h>
#include "HttpsResponseParser.h"
#define T HttpsResponse

static char *res200 = "HTTP/1.1 200 OK\n"
                           "Content-Type: text/plain\n"
                           "Content-Length: 13\n"
                           "\r\n\r\n"
                           "Hello, World!";


static char *noType = "HTTP/1.1 200 OK\n"
                           "Content-Length: 13\n"
                           "\r\n\r\n"
                           "Hello, World!";

static char *noLength = "HTTP/1.1 200 OK\n"
                           "Content-Type: text/plain\n"
                           "\r\n\r\n"
                           "Hello, World!";

Hashmap *response_map = NULL;

static HttpsResponseParser *parser = NULL;

static void setup(void) {
  runtimeError_reset();
  runtimeerrors_mode = RuntimeErrors_mode_silent;
  parser = httpsResponseParser_constructor();
  response_map = parser->parse(parser,res200);
}

static void teardown(void) {}

Test(httpsResponse, build, .init = setup, .fini = teardown) {
  T *response = httpsResponse_constructor(response_map);
  cr_assert_not_null(response, "Result should not be NULL");
}

Test(httpsResponse, build_param_check, .init=setup, .fini = teardown) {
  parser = httpsResponseParser_constructor();
  Hashmap* map1 = parser->parse(parser,noType);
  T *res1 = httpsResponse_constructor(map1);
  cr_assert_null(res1, "Result should be NULL");
  Hashmap* map2 = parser->parse(parser,noLength);
  T *res2 = httpsResponse_constructor(map2);
  cr_assert_null(res2, "Result should be NULL");
}

Test(httpsResponse, status, .init = setup, .fini = teardown) {
  T *response = httpsResponse_constructor(response_map);
  char *status = response->status(response);
  cr_assert_eq(strcmp(status, "200"), 0, "Status should be '200'");
}

Test(httpsResponse, headers, .init = setup, .fini = teardown) {
  T *response = httpsResponse_constructor(response_map);
  Hashmap *headers = response->headers(response);
  cr_assert_not_null(headers, "Headers should not be NULL");
  char *type = headers->get(headers,KEY(Content_Type)).value;
  char *length = headers->get(headers,KEY(Content_Length)).value;
  cr_assert_eq(strcmp(type, "text/plain"), 0, "Incorrect Content-Type");
  cr_assert_eq(strcmp(length, "13"), 0, "Incorrect Content-Length");
}

Test(httpsResponse, header, .init = setup, .fini = teardown) {
  T *response = httpsResponse_constructor(response_map);
  char*type = response->header(response,KEY(Content_Type)).value;
  char *length = response->header(response,KEY(Content_Length)).value;
  cr_assert_eq(strcmp(type, "text/plain"), 0, "Incorrect Content-Type");
  cr_assert_eq(strcmp(length, "13"), 0, "Incorrect Content-Length");
}

Test(httpsResponse, get_body, .init = setup, .fini = teardown) {
  T *response = httpsResponse_constructor(response_map);
  char *body = response->body(response);
  cr_assert_eq(strcmp(body, "Hello, World!"), 0, "Body is not correct");
}

Test(httpsResponse, content_type, .init = setup, .fini = teardown) {
  T *response = httpsResponse_constructor(response_map);
  char *body = response->content_type(response);
  cr_assert_eq(strcmp(body, "text/plain"), 0, "Incorrect Content-Type");
}

Test(httpsResponse, strinfigy, .init = setup, .fini = teardown) {
  T *response = httpsResponse_constructor(response_map);
  char*out = response->stringify(response);
  printf("%s\n",out);
  cr_assert_eq(strcmp(out, res200), 0, "Stringify should return the same string");
}

#undef T
