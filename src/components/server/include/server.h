#ifndef SERVER_H
#define SERVER_H
#include "mtqueue.h"
#include "candle.h"
#include "date.h"
#include "pthread.h"
#include <libwebsockets.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *server_init(void *arg);
#endif
