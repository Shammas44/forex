#include "wsHandler.h"
#include <criterion/criterion.h>
#define T WsHandler
T *ws = NULL;
// char path[] = "https://www.example.com/path/to/page";

static void setup(void) {
  Https *https = https_constructor();
  ws = wsHandler_constructor(https);
}

static void teardown(void){
  // free(result->full);
  // free(result);
}

Test(wsHandler, is_implemented ) {
  cr_assert_not_null(&wsHandler_constructor);
}

Test(wsHandler, return_null_on_null_param ) {
  ws = wsHandler_constructor(NULL);
  cr_assert_null(ws);
}

Test(wsHandler, basic_test, .init = setup, .fini = teardown) {
  cr_assert_not_null(&ws->destructor, "Destructor is not implemented");
  cr_assert_not_null(&ws->handshake, "Handshake is not implemented");
  cr_assert_not_null(&ws->send,  "Send is not implemented");
}

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
