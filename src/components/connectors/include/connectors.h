#ifndef CONNECTORS_H
#define CONNECTORS_H
#include "http.h"
#include "error.h"
#include "mtqueue.h"
#include "date.h"
#include "tick.h"
#include "order.h"
#include "csv.h"
#include "tick.h"
#include "wsframe.old.h"
#include "candle.h"
#include <unistd.h>

typedef Mtqueue *MtqueuePtr[2];

int connectors_establish_ws_handshake(SSL **ssl);

int connectors_get_auth(char **authorization); 

int connectors_read_websocket_frames(Mtqueue_list *q);

#endif 
