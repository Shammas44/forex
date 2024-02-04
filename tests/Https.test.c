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
  SslWrapper *ssl = sslWrapperStub_constructor(NULL);
  https = https_constructor(network,ssl);
}

static void setup_custom(void) { 
  builder = httpsRequestBuilder_constructor();
  builder->build(builder, "https://127.0.0.1");
  Network* network = networkWrapperStub_constructor();
  SslWrapper *ssl = sslWrapperStub_constructor("Hello");
  https = https_constructor(network,ssl);
}

static void teardown(void) { }

Test(https_constructor, isValid_object, .init = setup, .fini = teardown) {
  cr_assert_not_null(https, "should not be NULL");
}

Test(https_fetch, isValid_object, .init = setup, .fini = teardown){
  HttpsRequest *request = builder->get(builder);
  HttpsResponse * response = https->fetch(https, request);
  char* status = response->status(response);
  char* type = response->content_type(response);
  cr_assert_eq(strcmp(status, "200"), 0, "Status should be '200'");
  cr_assert_eq(strcmp(type, "text/plain"), 0, "Wrong Content-Type");
}

Test(https_fetch, custom_constructor, .init = setup_custom, .fini = teardown){
  HttpsRequest *request = builder->get(builder);
  HttpsResponse * response = https->fetch(https, request);
  char* status = response->status(response);
  char* type = response->content_type(response);
  char* body = response->body(response);
  cr_assert_eq(strcmp(status, "200"), 0, "Status should be '200'");
  cr_assert_eq(strcmp(type, "text/plain"), 0, "Wrong Content-Type");
  cr_assert_eq(strcmp(body, "Hello"), 0, "Wrong body");
}

Test(https_get, isValid_object, .init = setup, .fini = teardown){
  builder->set_method(builder,GET);
  HttpsRequest *request = builder->get(builder);
  HttpsResponse * response = https->fetch(https, request);
  char* status = response->status(response);
  char* type = response->content_type(response);
  cr_assert_eq(strcmp(status, "200"), 0, "Status should be '200'");
  cr_assert_eq(strcmp(type, "text/plain"), 0, "Wrong Content-Type");
}

Test(https_post, isValid_object, .init = setup, .fini = teardown){
  builder->set_method(builder,POST);
  HttpsRequest *request = builder->get(builder);
  HttpsResponse * response = https->fetch(https, request);
  char* status = response->status(response);
  char* type = response->content_type(response);
  cr_assert_eq(strcmp(status, "200"), 0, "Status should be '200'");
  cr_assert_eq(strcmp(type, "text/plain"), 0, "Wrong Content-Type");
}

Test(https_patch, isValid_object, .init = setup, .fini = teardown){
  builder->set_method(builder,PATCH);
  HttpsRequest *request = builder->get(builder);
  HttpsResponse * response = https->fetch(https, request);
  char* status = response->status(response);
  char* type = response->content_type(response);
  cr_assert_eq(strcmp(status, "200"), 0, "Status should be '200'");
  cr_assert_eq(strcmp(type, "text/plain"), 0, "Wrong Content-Type");
}

Test(https_put, isValid_object, .init = setup, .fini = teardown){
  builder->set_method(builder,PUT);
  HttpsRequest *request = builder->get(builder);
  HttpsResponse * response = https->fetch(https, request);
  char* status = response->status(response);
  char* type = response->content_type(response);
  cr_assert_eq(strcmp(status, "200"), 0, "Status should be '200'");
  cr_assert_eq(strcmp(type, "text/plain"), 0, "Wrong Content-Type");
}

Test(https_delete, isValid_object, .init = setup, .fini = teardown){
  builder->set_method(builder,DELETE);
  HttpsRequest *request = builder->get(builder);
  HttpsResponse * response = https->fetch(https, request);
  char* status = response->status(response);
  char* type = response->content_type(response);
  cr_assert_eq(strcmp(status, "200"), 0, "Status should be '200'");
  cr_assert_eq(strcmp(type, "text/plain"), 0, "Wrong Content-Type");
}

Test(https_ws_handshake, isValid_object, .init = setup, .fini = teardown){
  builder->set_method(builder,GET);
  builder->add_header(builder,"Connection: Upgrade");
  builder->add_header(builder,"Upgrade: websocket");
  builder->add_header(builder,"Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==");
  builder->add_header(builder,"Sec-WebSocket-Version: 13");
  HttpsRequest *request = builder->get(builder);
  SSL* ssl = https->ws_handshake(https, request);
  cr_assert_not_null(ssl, "ssl should not be NULL");
}

#undef T
