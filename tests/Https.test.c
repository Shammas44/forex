#include "https.h"
#include "NetworkWrapperStub.h"
#include "SslWrapper.stub.h"
#include "httpsRequestBuilder.h"
#include <criterion/criterion.h>
#define T Https

T*https = NULL;
HttpsRequestBuilder *builder = NULL;

static void setup(void) { 
  builder = httpsRequestBuilder_constructor();
  builder->build(builder, "https://127.0.0.1");
  Network* network = networkWrapperStub_constructor();
  SslWrapper *ssl = sslWrapperStub_constructor();
  https = https_constructor(network,ssl);
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
