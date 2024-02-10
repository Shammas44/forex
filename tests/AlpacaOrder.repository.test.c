#include "AlpacaOrder.repository.h"
#include "SslWrapper.stub.h"
#include "NetworkWrapperStub.h"
#include <criterion/criterion.h>
#define T AlpacaOrderRepository

static Https*https = NULL;
static T *repo = NULL;

static void setup(void) { 
  char *host = "https://127.0.0.1";
  HttpsRequestBuilder *builder = httpsRequestBuilder_constructor();
  Network* network = networkWrapperStub_constructor();
  JsonParser* parser = jsonParser_constructor();
  SslWrapper *ssl = sslWrapperStub_constructor("Hello");
  https = https_constructor(network,ssl);
  repo = alpacaOrderRepository_constructor((OrderRepositoryArgs){
      .host=host,
      .https=https,
      .req_builder=builder,
      .response_body_parser=parser
    });
}

static void teardown(void) { }

Test(alpacaOrderRepository_constructor, isValid_object, .init = setup, .fini = teardown) {
  cr_assert_not_null(https, "should not be NULL");
}


#undef T
