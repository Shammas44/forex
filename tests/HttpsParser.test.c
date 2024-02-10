#include "HttpsParser.h"
#include "httpsRequestBuilder.h"
#include "httpsResponse.keys.h"
#include "Hashmap.h"
#include <criterion/criterion.h>
#define T HttpsParser

T* parser = NULL;
static char *res200 = "HTTPS/1.1 200 OK\r\n"
                           "Content-Type: text/plain\r\n"
                           "Content-Length: 13\r\n"
                           "\r\n\r\n"
                           "Hello, World!";

static void response_parser(void) {
  parser = httpsParser_constructor();
  HttpsParser_config config = {.type = HttpsParser_response,.jsonBody=false};
  Parser_config_obj *c = (Parser_config_obj *) &config;
  parser->config(parser, c);
}

// static void teardown(void) {}

Test(HttpsParser_constructor, build, .init = response_parser ) {
  cr_assert_not_null(parser, "Result should not be NULL");
}

Test(HttpsParser_parse, required_param, .init = response_parser ) {
  void * res = parser->parse(parser,NULL);
  cr_assert_null(res, "Result should be NULL");
}

Test(HttpsParser_parse, headers, .init = response_parser ) {
  Hashmap * map = parser->parse(parser,res200);
  cr_assert_gt(map->length, 0, "Result should be greater");
  Hashmap * headers = map->get(map,KEY(Headers)).value;
  cr_assert_gt(headers->length, 0, "Result should be greater");
  char*content_type = headers->get(headers,KEY(Content_Type)).value;
  char*content_length = headers->get(headers,KEY(Content_Length)).value;
  cr_assert_eq(strcmp(content_type, "text/plain"),0, "Content-Type is wrong");
  cr_assert_eq(strcmp(content_length, "13"),0, "Content-Length is wrong");
  char*body = map->get(map,KEY(Body)).value;
  cr_assert_eq(strcmp(body, "Hello, World!"),0, "Body is wrong");
}

Test(HttpsParser_parse, status, .init = response_parser) {
  Hashmap * map = parser->parse(parser,res200);
  char*status_code = map->get(map,KEY(Status_Code)).value;
  char*status_msg = map->get(map,KEY(Status_Message)).value;
  char*protocol_name = map->get(map,KEY(Protocol_Name)).value;
  char*protocol_version = map->get(map,KEY(Protocol_Version)).value;
  cr_assert_eq(strcmp(status_code, "200"),0, "Status code is wrong");
  cr_assert_eq(strcmp(status_msg, "OK"),0, "Status message is wrong");
  cr_assert_eq(strcmp(protocol_name, "HTTPS"),0, "Protocol is wrong");
  cr_assert_eq(strcmp(protocol_version, "1.1"),0, "Protocol version is wrong");
}

Test(HttpsParser_parse, from_app_request, .init = response_parser) {
  HttpsRequestBuilder *builder = httpsRequestBuilder_constructor();

  builder->build(builder,"https://127.0.0.1");
  builder->add_header(builder,"Connection: keep-alive");
  builder->add_header(builder,"Accept-Encoding: gzip,deflat,br");
  builder->add_header(builder,"Content-Type: application/json");
  builder->set_method(builder,GET);
  HttpsRequest * req = builder->get(builder);
  char*string;
  req->stringify(req, &string);
  parser->config(parser, (Parser_config_obj *) &((HttpsParser_config){.type = HttpsParser_request,.jsonBody=false}));
  Hashmap * map = parser->parse(parser,string);
  Hashmap * headers = map->get(map,KEY(Headers)).value;
  char*content_type = headers->get(headers,KEY(Content_Type)).value;
  char*connection = headers->get(headers,KEY(Connection)).value;
  char*encoding = headers->get(headers,"Accept-Encoding").value;
  cr_assert_eq(strcmp(connection, "keep-alive"),0, "Connection is wrong");
  cr_assert_eq(strcmp(encoding, "gzip,deflat,br"),0, "Accept-Encoding is wrong");
  cr_assert_eq(strcmp(content_type, "application/json"),0, "Content-Type is wrong");
}

#undef T
