#include "ExchangeTest.h"
#include "common.h"
#include "hashmap.h"
#include "json.h"
#define T ExchangeTest

#define GET_REQUEST(exchange,route) \
({ \
  Private *_private = exchange->__private; \
  WsHandler *_ws = _private->ws; \
  Https *_https = _ws->get_https_handler(_ws); \
  HttpsRequest *_request = __build_query(exchange,route); \
  _request; \
})

typedef enum Mode {
Live,Paper
}Mode; 

typedef struct {
WsHandler *ws;
  HttpsRequestBuilder *req_builder;
  Parser*parser;
  char *email;
  char *password;
  char*token;
  Mode mode;
} Private;

static char* host = "https://127.0.0.1:443/api/";

static HttpsRequest* __build_query(T *exchange, char*route,HttpsRequest_method method);
static HttpsResponse* __send(T*exchange, char*route, HttpsRequest_method method, void*body);
static void __account_informations(T*exchange);
static int __authenticate(T*exchange,char*path);
static void __subscribe(T*exchange,char*path);

T *exchangeTest_constructor(WsHandler *handler,ConfigWrapper*config, Parser *parser){
  T *self = malloc(sizeof(T));
  if (self == NULL) return NULL;
  Private *private = malloc(sizeof(Private));
  if (private == NULL) return NULL;
  HttpsRequestBuilder *req_builder = httpsRequestBuilder_constructor();
  if (req_builder == NULL) return NULL;
  self->subscribe = __subscribe;
  private->ws = handler;
  private->parser = parser;
  private->req_builder = req_builder;
  private->email = config->get(config,"email");
  private->password = config->get(config,"password");

  bool paper = config->paper(config);
  if(paper == true) private->mode = Paper;
  else if(paper == false) private->mode = Live;
  else private->mode = Paper;
  self->__private = private;
  int status = __authenticate(self,"auth");

  return self;
}

static HttpsRequest* __build_query(T *exchange, char*route,HttpsRequest_method method){
  Private *private = exchange->__private;
  HttpsRequestBuilder *req_builder = private->req_builder;
  char *url = malloc(strlen(host) + strlen(route) + 1);
  sprintf(url, "%s%s", host, route);
  req_builder->build(req_builder,url);
  req_builder->add_header(req_builder,"Connection: keep-alive");
  req_builder->add_header(req_builder,"Accept-Encoding: gzip,deflat,br");
  req_builder->add_header(req_builder,"Content-Type: application/json");
  req_builder->add_header(req_builder,"Apca-Api-Key-Id: changeMe");
  req_builder->add_header(req_builder,"Apca-Api-Secret-Key: changeMe");
  req_builder->set_method(req_builder,method);
  HttpsRequest * req = req_builder->get(req_builder);
  return req;
}

static HttpsResponse* __send(T*exchange, char*route, HttpsRequest_method method, void*body){
  Private *private = exchange->__private; 
  WsHandler *ws = private->ws; 
  Https *https = ws->get_https_handler(ws); 
  HttpsRequest *request = __build_query(exchange,route,method);
  request->set_body(request,body);
  return https->fetch(https,request);
}

// Account
// =========================================================================="

static void __account_informations(T*exchange){
  HttpsResponse * response = __send(exchange,"account",GET, NULL);
}

static int __authenticate(T*exchange,char*path){
  Private *private = exchange->__private;
  char body_tmp[] = "{"
                    "\"email\":\"%s\","
                    "\"password\":\"%s\""
                    "}";
  char *body = malloc(strlen(body_tmp) + strlen(private->email) + strlen(private->password) + 1);
  sprintf(body, body_tmp, private->email, private->password);
  HttpsResponse * response = __send(exchange,"auth",POST, body);
  if(strcmp(response->get_status(response),"200") ==0 ){
    Hashmap*map = NULL;
    json_to_map(response->get_body(response),&map,NULL,NULL);
    private->token = map->get(map,"token",NULL);
    return 0;
  }
  return 1;
}

static void __subscribe(T*exchange,char*path){
  Private *private = exchange->__private;
  WsHandler *ws = private->ws;
  HttpsRequestBuilder *req_builder = private->req_builder;
  req_builder->build(req_builder,host);

  char*authorization = malloc(strlen(private->token) + strlen("Authorization: Bearer ") + 1);
  sprintf(authorization,"Authorization: Bearer %s",private->token);
  req_builder->add_header(req_builder,authorization);
  req_builder->add_header(req_builder,"Connection: Upgrade");
  req_builder->add_header(req_builder,"Upgrade: websocket");
  req_builder->add_header(req_builder,"Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==");
  req_builder->add_header(req_builder,"Sec-WebSocket-Version: 13");
  HttpsRequest * req = req_builder->get(req_builder);
  SSL* ssl = ws->handshake(ws,req);
  if(ssl ==NULL ) return;
  ws->listen(ws,ssl,private->parser);
}

#undef T
#undef GET_REQUEST








// #include "Exchange_p.h"
// #include "guard.h"
// #include "httpsRequest.h"
// #include "mtqueue.h"
// #include "url.h"
// #include "ws.h"
// #include "https.h"
// #include "observer.h"

// #define EXCHANGETEST_API_LOGIN_PATH "/api/auth"

// int __exchangeTest_connect(Exchange* exchange);
// int __exchangeTest_subscribe(Exchange* exchange);
// int __exchangeTest_unsubscribe(Exchange* exchange);
// int __exchangeTest_destructor(Exchange *exchange);
// int __exchangeTest_read_websocket_frames(Exchange* exchange, Mtqueue_list *q);
// int __exchangeTest_authenticate(struct Exchange* exchange, void *credentials);

//  Exchange* exchangeTest_constructor(Mtqueue_list *args, Url* url, char* auth_token){
//   Exchange* exchange = malloc(sizeof(Exchange));
//   OOM_GUARD(exchange); 

//   for (int i = 0; i < EXCHANGE_MAX_OBSERVERS; i++) {
//     exchange->observers[i] = NULL;
//   }
//   exchange->exchange = NULL;
//   exchange->numObservers = 0;
//   exchange->mtqueue_list = args;
//   exchange->connection = NULL;
//   exchange->auth_token = auth_token;
//   exchange->url = url;
//   exchange->connect_func = __exchangeTest_connect;
//   exchange->subscribe_func = __exchangeTest_subscribe;
//   exchange->unsubscribe_func = __exchangeTest_unsubscribe;
//   return exchange;
// }

// int __exchangeTest_connect(Exchange* exchange){
//   char *authorization = NULL;
//   char * credentials = malloc(sizeof(char) * 128);
//   credentials = "{\"email\":\"dakfj\"}";

//   int status = __exchangeTest_authenticate(exchange, credentials);
//   if (status) {
//     return get_error("failed to get authorization");
//   }

//   char body[] = "";
//   char *headers[5] = {
//       "Upgrade: websocket",
//       "Connection: Upgrade",
//       "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==",
//       "Sec-WebSocket-Version: 13",
//       exchange->auth_token,
//   };

//   HttpsRequest_prefill *prefill = malloc(sizeof(HttpsRequest_prefill));
//       prefill->url = exchange->url->full;
//       prefill->method = "GET";
//       prefill->body = "";
//       prefill->headers = headers;
//       prefill->header_count = 5;

//   Request *response = malloc(sizeof(Request));
//   struct Request *request = httpsRequest_constructor(prefill);

//   status = ws_establish_handshake(&exchange->connection, (Request*)request, (Response*)response);
//   if (status) {
//     get_error("failed to establish ws handshake\n");
//     return 1;
//   }

//   if (__exchangeTest_read_websocket_frames(exchange, exchange->mtqueue_list)) {
//     get_error("failed to read websocket frames");
//     return 1;
//   }
//   return 0;
// }

// int __exchangeTest_subscribe(Exchange* exchange){
//   return 1;
// }

// int __exchangeTest_unsubscribe(Exchange* exchange){
//   return 1;
// }

// int __exchangeTest_read_websocket_frames(Exchange* exchange, Mtqueue_list *q) {
//   int status = 0;

//   while (1) {
//     char *out_message = NULL;
//     //TODO: debug following function call - replace ssl by exchange->connection
//     status = wsFrame_read_single_frame(exchange->connection, &out_message);

//     if (status != 0) {
//       free(out_message);
//       break;
//     }

//     jsmntok_t *tokens = NULL;
//     int token_num;
//     status = json_parse(out_message, JSMN_OBJECT, &tokens, &token_num);

//     if (status != 0) {
//       free(out_message);
//       if (tokens) {
//         free(tokens);
//       }
//       continue;
//     }

//     char type[20];
//     char status[20];
//     for (int i = 1; i < token_num; i++) {
//       if (json_cmp_token_to_string(out_message, &tokens[i], "type") == 0) {
//         json_extract_token(tokens, i, out_message, type, to_char);
//         i++;
//       }
//       if (json_cmp_token_to_string(out_message, &tokens[i], "status") == 0) {
//         json_extract_token(tokens, i, out_message, status, to_char);
//         i++;
//       }
//     }

//     // if (strcmp(type, "TICKER") == 0) {
//     //   Tick *tick = tick_create(out_message, &tokens, token_num);
//     //   mtqueue_enqueue(q->ticks, tick);
//     // } else if (strcmp(type, "Market") == 0) {
//     //   Order order = order_create(out_message, &tokens, token_num);
//     //   mtqueue_enqueue(q->orders, &order);
//     // }

//     free(out_message);
//     free(tokens);
//   }
//   return status;
// }

// void __exchangeTest_update(Observer* observer, void * newState) {
//     printf("Observer updated\n");
// }

// int __exchangeTest_authenticate(Exchange* exchange, void *credentials){
//   char *authorization = NULL;
//   char body[] = "{\"email\":\"Admin.Admin@orif.ch\",\"password\":\"adad\"}";
//   char *headers[] = {
//       "Connection: close",
//       "Content-Type: application/json",
//   };

//   char *url = malloc(sizeof(char) * 128);
//   sprintf(url, "https://%s%s", exchange->url->host, EXCHANGETEST_API_LOGIN_PATH);

//   HttpsRequest_prefill *pre = malloc(sizeof(HttpsRequest_prefill));
//     pre->url = url;
//     pre->method = HTTP_METHODS_POST;
//     pre->body = body;
//     pre->headers = headers;
//     pre->header_count = sizeof(headers) / sizeof(headers[0]);

//   struct Request *req = httpsRequest_constructor(pre);
//   Response res;
//   // int status = https_fetch(req, &res, NULL);
//   // if (status)
//   //   return get_error("Failed to fetch");
//   // status = httpsResponse_extract_authorization(&res, &authorization);
//   // exchange->auth_token = authorization;
//   // if (status)
//   //   return get_error("failed to get authorization");
//   return 0;
// }
