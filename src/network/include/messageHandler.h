#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "response.h"
#include "request.h"

struct messageHandler_Request;
struct messageHandler_Response;


struct MessageHandler;
typedef Request * (messageHandler_request_constructor_func)();
typedef Response * (messageHandler_response_constructor_func)();

typedef struct MessageHandler {
  messageHandler_request_constructor_func * request_constructor;
  messageHandler_request_constructor_func * response_constructor;
}MessageHandler;


MessageHandler * messageHandler_constructor(messageHandler_request_constructor_func * request_constructor, messageHandler_response_constructor_func * response_constructor);

void messageHandler_destructor(MessageHandler * messageHandler);

int messageHandler_fetch(struct messageHandler_Request *request,  struct messageHandler_Response *res);

#endif 
