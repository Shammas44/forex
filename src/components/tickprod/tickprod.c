#include "tickprod.h"

void *connectors_produce_tick(void *arg) {
  Mtqueue_list *queues = (Mtqueue_list *)arg;

  if (connectors_establish_ws_handshake(&queues->ssl)) {
    get_error("failed to establish ws handshake\n");
    return NULL;
  }

  if (connectors_read_websocket_frames(queues)) {
    get_error("failed to read websocket frames");
    return NULL;
  }

  return NULL;
}
