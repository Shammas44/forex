#include "HttpsResponseParser.h"
#include "hashmap.h"
#include <criterion/criterion.h>
#define T HttpsResponseParser

T* parser = NULL;
static char *res200 = "HTTP/1.1 200 OK\n"
                           "Content-Type: text/plain\n"
                           "Content-Length: 13\n"
                           "\r\n\r\n"
                           "Hello, World!";

static void setup(void) {
  parser = httpsResponseParser_constructor();
}

// static void teardown(void) {}

Test(HttpsResponseParser_constructor, build, .init = setup ) {
  cr_assert_not_null(parser, "Result should not be NULL");
}

Test(HttpsResponseParser_parse, required_param, .init = setup ) {
  void * res = parser->parse(parser,NULL);
  cr_assert_null(res, "Result should be NULL");
}

Test(HttpsResponseParser_parse, headers, .init = setup ) {
  Hashmap * map = parser->parse(parser,res200);
  cr_assert_gt(map->length, 0, "Result should be greater");
  Hashmap * headers = map->get(map,"headers").value;
  cr_assert_gt(headers->length, 0, "Result should be greater");
  char*content_type = headers->get(headers,"Content-Type").value;
  char*content_length = headers->get(headers,"Content-Length").value;
  cr_assert_eq(strcmp(content_type, "text/plain"),0, "Not equal");
  cr_assert_eq(strcmp(content_length, "13"),0, "Not equal");
  char*body = map->get(map,"body").value;
  cr_assert_eq(strcmp(body, "Hello, World!"),0, "Not equal");
}

// Test(httpsResponse, status, .init = setup, .fini = teardown) {
//   T *response = httpsResponse_constructor(res200);
//   char *status = response->get_status(response);
//   cr_assert_eq(strcmp(status, "200"), 0, "Status should be '200'");
// }

// Test(httpsResponse, headers, .init = setup, .fini = teardown) {
//   T *response = httpsResponse_constructor(res200);
//   char *type = response->get_header(response,"Content-Type");
//   char *length = response->get_header(response,"Content-Length");
//   cr_assert_eq(strcmp(type, "text/plain"), 0, "Incorrect Content-Type");
//   cr_assert_eq(strcmp(length, "13"), 0, "Incorrect Content-Length");
// }

// Test(httpsResponse, strinfigy, .init = setup, .fini = teardown) {
//   T *response = httpsResponse_constructor(res200);
//   char*out = NULL;
//   int status = response->stringify(response,&out);
//   cr_assert_eq(status, 0, "Stringify should not fail");
//   cr_assert_eq(strcmp(out, res200), 0, "Stringify should return the same string");
// }

// Test(httpsResponse, get_body, .init = setup, .fini = teardown) {
//   T *response = httpsResponse_constructor(res200);
//   char *body = response->get_body(response);
//   cr_assert_eq(strcmp(body, "Hello, World!"), 0, "Body is not correct");
// }

#undef T
