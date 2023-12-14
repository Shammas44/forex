#include "httpsResponse.h"
#include <criterion/criterion.h>
#define T HttpsResponse

static char *res200 = "HTTP/1.1 200 OK\n"
                           "Content-Type: text/plain\n"
                           "Content-Length: 13\n"
                           "\r\n\r\n"
                           "Hello, World!";

static void setup(void) {}

static void teardown(void) {}

Test(httpsResponse, build, .init = setup, .fini = teardown) {
  T *response = httpsResponse_constructor(res200);
  cr_assert_not_null(response, "Result should not be NULL");
  cr_assert_not_null(response->destructor, "Method 'destructor' should be defined");
  cr_assert_not_null(response->set_status, "Method 'set_status' should be defined");
  cr_assert_not_null(response->set_body, "Method 'set_body' should be defined");
  cr_assert_not_null(response->add_header, "Method 'add_header' should be defined");
  cr_assert_not_null(response->stringify, "Method 'stringify' should be defined");
  cr_assert_not_null(response->print, "Method 'print' should be defined");
  cr_assert_not_null(response->get_status, "Method 'get_status' should be defined");
  cr_assert_not_null(response->get_body, "Method 'get_body' should be defined");
  cr_assert_not_null(response->get_headers, "Method 'get_headers' should be defined");
  cr_assert_not_null(response->get_header, "Method 'get_header' should be defined");
  cr_assert_not_null(response->get_content_type, "Method 'get_content_type' should be defined");
}

Test(httpsResponse, status, .init = setup, .fini = teardown) {
  T *response = httpsResponse_constructor(res200);
  char *status = response->get_status(response);
  cr_assert_eq(strcmp(status, "200"), 0, "Status should be '200'");
}

Test(httpsResponse, headers, .init = setup, .fini = teardown) {
  T *response = httpsResponse_constructor(res200);
  char *type = response->get_header(response,"Content-Type");
  char *length = response->get_header(response,"Content-Length");
  cr_assert_eq(strcmp(type, "text/plain"), 0, "Incorrect Content-Type");
  cr_assert_eq(strcmp(length, "13"), 0, "Incorrect Content-Length");
}

Test(httpsResponse, strinfigy, .init = setup, .fini = teardown) {
  T *response = httpsResponse_constructor(res200);
  char*out = NULL;
  int status = response->stringify(response,&out);
  cr_assert_eq(status, 0, "Stringify should not fail");
  cr_assert_eq(strcmp(out, res200), 0, "Stringify should return the same string");
}

Test(httpsResponse, get_body, .init = setup, .fini = teardown) {
  T *response = httpsResponse_constructor(res200);
  char *body = response->get_body(response);
  cr_assert_eq(strcmp(body, "Hello, World!"), 0, "Body is not correct");
}

#undef T
