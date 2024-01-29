#include "server.h"
#define MAX_CLIENTS 32

static struct lws *clients[MAX_CLIENTS];
static struct lws_context *context;
static char *json_candle;

static void broadcast_message();
static int callback_function(struct lws *wsi, enum lws_callback_reasons reason,
                             void *user, void *in, size_t len);

static void broadcast_message() {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i]) {
      lws_callback_on_writable(clients[i]);
    }
  }
}

static int callback_function(struct lws *wsi, enum lws_callback_reasons reason,
                             void *user, void *in, size_t len) {
  switch (reason) {
  case LWS_CALLBACK_ESTABLISHED:
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (!clients[i]) {
        clients[i] = wsi;
        break;
      }
    }
    break;

  case LWS_CALLBACK_SERVER_WRITEABLE: {
    unsigned char buffer[LWS_SEND_BUFFER_PRE_PADDING + 512 +
                         LWS_SEND_BUFFER_POST_PADDING];
    unsigned char *p = &buffer[LWS_SEND_BUFFER_PRE_PADDING];
    // size_t message_length = sprintf((char *)p, "Hello!");
    size_t message_length = sprintf((char *)p, "%s", json_candle);
    lws_write(wsi, p, message_length, LWS_WRITE_TEXT);
    break;
  }

  case LWS_CALLBACK_CLOSED:
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (clients[i] == wsi) {
        clients[i] = NULL;
        break;
      }
    }
    break;

  default:
    break;
  }
  return 0;
}

static struct lws_protocols protocols[] = {
    {
        "my-protocol",
        callback_function,
        0,
        0,
    },
    {NULL, NULL, 0, 0} // terminator
};

void *server_init(void *arg) {
  Mtqueue *q = (Mtqueue *)arg;
  struct lws_context_creation_info info;
  memset(&info, 0, sizeof(info));

  info.port = 9000;
  info.protocols = protocols;
  info.gid = -1;
  info.uid = -1;

  context = lws_create_context(&info);
  if (!context) {
    fprintf(stderr, "Failed to create context.\n");
    return NULL;
  }

  printf("WebSocket server started on port 9000\n");

  while (1) {
    lws_service(context, 50);
    void *data = mtqueue_dequeue(q);
    Candle *candle = (Candle *)data;
    json_candle = candle_to_json(candle);
    printf("\n%s\n", json_candle);
    if(json_candle) broadcast_message();
  }

  lws_context_destroy(context);
  return NULL;
}
