#include "https.h"
#include "httpsRequestBuilder.h"
#include <criterion/criterion.h>
#define T Https

T*https = NULL;
HttpsRequestBuilder *builder = NULL;

static void setup(void) { 
  builder = httpsRequestBuilder_constructor();
  builder->build(builder, "https://127.0.0.1");
  https = https_constructor();
}

static void teardown(void) {  }

Test(https_constructor, isValid_object, .init = setup, .fini = teardown) {
  cr_assert_not_null(https, "should not be NULL");
}

// Test(https_fetch, isValid_object, .init = setup, .fini = teardown) {
//   HttpsRequest *request = builder->get(builder);
//   HttpsResponse * response = https->fetch(https, request);
//   char*string =response->stringify(response);
//   printf("%s\n",string);
//   cr_assert_not_null(https, "should not be NULL");
// }

#undef T
