#include "url.h"
#include <criterion/criterion.h>
Url *result = NULL;
char path[] = "https://www.example.com/path/to/page";

static void setup(void) {
    // puts("Runs before the test");
}

static void teardown(void){
  free(result->full);
  free(result);
}

Test(url_constructor, is_implemented ) {
  cr_assert_not_null(&url_constructor);
}

Test(url_constructor, return_null_on_null_param ) {
  Url *out = url_constructor(NULL);
  cr_assert_null(out);
}

Test(url_constructor, basic_test, .init = setup, .fini = teardown) {
  result = url_constructor(path);
  cr_expect_str_eq(result->full, path, "Full URL mismatch");
  cr_expect_str_eq(result->protocol, "https", "Protocol mismatch");
  cr_expect_str_eq(result->host, "www.example.com", "Host mismatch");
  cr_expect_str_eq(result->path, "/path/to/page", "Path mismatch");
  cr_expect_str_eq(result->port, "443", "Port mismatch");
}

Test(url_constructor, port_is_provided, .init = setup, .fini = teardown) {
  result = url_constructor("https://www.example.com:8080/path/to/page");
  cr_expect_str_eq(result->full, "https://www.example.com:8080/path/to/page", "Full URL mismatch");
  cr_expect_str_eq(result->protocol, "https", "Protocol mismatch");
  cr_expect_str_eq(result->host, "www.example.com", "Host mismatch");
  cr_expect_str_eq(result->path, "/path/to/page", "Path mismatch"); 
  cr_expect_str_eq(result->port, "8080", "Port mismatch");
}

Test(url_constructor, empty_path_test, .init = setup, .fini = teardown) {
    // Test for URL without a path
    char input_url[] = "https://www.example.com";
    result = url_constructor(input_url);

    cr_assert_not_null(result, "Result should not be NULL");
    cr_expect_str_eq(result->full, "https://www.example.com", "Full URL mismatch");
    cr_expect_str_eq(result->protocol, "https", "Protocol mismatch");
    cr_expect_str_eq(result->host, "www.example.com", "Host mismatch");
    cr_expect_str_eq(result->path, "/", "Path mismatch"); // Expect default path "/"
    cr_expect_str_eq(result->port, "443", "Port mismatch");
}

