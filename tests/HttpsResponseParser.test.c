#include "HttpsResponseParser.h"
#include "httpsResponse.keys.h"
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
  Hashmap * headers = map->get(map,KEY(Headers)).value;
  cr_assert_gt(headers->length, 0, "Result should be greater");
  char*content_type = headers->get(headers,KEY(Content_Type)).value;
  char*content_length = headers->get(headers,KEY(Content_Length)).value;
  cr_assert_eq(strcmp(content_type, "text/plain"),0, "Not equal");
  cr_assert_eq(strcmp(content_length, "13"),0, "Not equal");
  char*body = map->get(map,KEY(Body)).value;
  cr_assert_eq(strcmp(body, "Hello, World!"),0, "Not equal");
}

Test(HttpsResponseParser_parse, status, .init = setup) {
  Hashmap * map = parser->parse(parser,res200);
  char*status_code = map->get(map,KEY(Status_Code)).value;
  char*status_msg = map->get(map,KEY(Status_Message)).value;
  char*protocol_name = map->get(map,KEY(Protocol_Name)).value;
  char*protocol_version = map->get(map,KEY(Protocol_Version)).value;
  cr_assert_eq(strcmp(status_code, "200"),0, "Not equal");
  cr_assert_eq(strcmp(status_msg, "OK"),0, "Not equal");
  cr_assert_eq(strcmp(protocol_name, "HTTP"),0, "Not equal");
  cr_assert_eq(strcmp(protocol_version, "1.1"),0, "Not equal");
}

#undef T
