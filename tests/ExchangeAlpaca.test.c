#include "ExchangeAlpaca.h"
#include <criterion/criterion.h>
#define T ExchangeAlpaca
// Url *result = NULL;
// char path[] = "https://www.example.com/path/to/page";
Parser *parser = NULL;
WsHandler *handler = NULL;
ConfigWrapper*config = NULL;

char json[] = "{"
              "\"paper\":true,"
              "\"symbol\":\"EUR/USD\","
              "}";

static void setup(void) {
  Parser p = {};
  p.destructor = NULL;
  p.parse = NULL;
  p.__private = NULL;
  parser = &p;

  WsHandler h = {};
  h.destructor = NULL;
  h.handshake = NULL;
  h.listen = NULL;
  h.send = NULL;
  h.__private = NULL;
  handler = &h;

  Hashmap*c = hashmap_constructor(10);
  c->push(c,"paper","true",0);
  c->push(c,"symbol","EUR/USD",0);
  ConfigWrapper *wrapper = configWrapper_constructor(c);
  config = wrapper;
}
//  struct T {
//   WsHandler_destructor *destructor;
//   WsHandler_handshake *handshake;
//   WsHandler_listen *listen;
//   WsHandler_send *send;
//   void * __private;
// };

// static void teardown(void){
//   free(result->full);
//   free(result);
// }

Test(ExchangeAlpaca_constructor, is_implemented, .init = setup ) {
  cr_assert_not_null(&exchangeAlpaca_constructor, "constructor not implemented");
  T * exchange = exchangeAlpaca_constructor(handler, NULL,parser);
  cr_assert_not_null(exchange, "Exchange shouldn't be NULL");
  // cr_assert_not_null(exchange->destructor, "Exchange destructor not implemented");
  // cr_assert_not_null(exchange->connect, "Exchange connect not implemented");
  // cr_assert_not_null(exchange->subscribe, "Exchange subscribe not implemented");
  // cr_assert_not_null(exchange->authenticate, "Exchange authenticate not implemented)");
  // cr_assert_not_null(exchange->get_auth, "Exchange get_auth not implemented");
  // cr_assert_not_null(exchange->unsubscribe, "Exchange unsubscribe not implemented");
}

// Test(url_constructor, return_null_on_null_param ) {
//   Url *out = url_constructor(NULL);
//   cr_assert_null(out);
// }

// Test(url_constructor, basic_test, .init = setup, .fini = teardown) {
//   result = url_constructor(path);
//   cr_expect_str_eq(result->full, path, "Full URL mismatch");
//   cr_expect_str_eq(result->protocol, "https", "Protocol mismatch");
//   cr_expect_str_eq(result->host, "www.example.com", "Host mismatch");
//   cr_expect_str_eq(result->path, "/path/to/page", "Path mismatch");
//   cr_expect_str_eq(result->port, "443", "Port mismatch");
// }

// Test(url_constructor, port_is_provided, .init = setup, .fini = teardown) {
//   result = url_constructor("https://www.example.com:8080/path/to/page");
//   cr_expect_str_eq(result->full, "https://www.example.com:8080/path/to/page", "Full URL mismatch");
//   cr_expect_str_eq(result->protocol, "https", "Protocol mismatch");
//   cr_expect_str_eq(result->host, "www.example.com", "Host mismatch");
//   cr_expect_str_eq(result->path, "/path/to/page", "Path mismatch"); 
//   cr_expect_str_eq(result->port, "8080", "Port mismatch");
// }

// Test(url_constructor, empty_path_test, .init = setup, .fini = teardown) {
//     // Test for URL without a path
//     char input_url[] = "https://www.example.com";
//     result = url_constructor(input_url);

//     cr_assert_not_null(result, "Result should not be NULL");
//     cr_expect_str_eq(result->full, "https://www.example.com", "Full URL mismatch");
//     cr_expect_str_eq(result->protocol, "https", "Protocol mismatch");
//     cr_expect_str_eq(result->host, "www.example.com", "Host mismatch");
//     cr_expect_str_eq(result->path, "/", "Path mismatch"); // Expect default path "/"
//     cr_expect_str_eq(result->port, "443", "Port mismatch");
// }

#undef T
