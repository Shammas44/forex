#include "TickParser.h"
#include "json.h"
#include "tick.h"
#include "Order.h"
#include <stdlib.h>
#define T TickParser

typedef struct Token_info {
  char type[20];
  char status[20];
  char *input;
  jsmntok_t *tokens;
  int token_num;
}Token_info;

void __tickParser_destructor(T *tickParser); 
void* __tickParser_parse(T *tickParser,char*input); 
void __new_tick(T *tickParser, Token_info info);
// void __tickParser_attach_observer(T *tickParser, Observer *observer);
// void __tickParser_detach_observer(T *tickParser, Observer *observer);
// void __tickParser_notify(T *tickParser);
// void __tickParser_set_subject_state(T*tickParser, void* newState);

T *tickParser_constructor() {
  T *tickParser = malloc(sizeof(T));
  tickParser->destructor = __tickParser_destructor;
  tickParser->parse = __tickParser_parse;
  // Subject *subject = subject_constructor(NULL);
  //TODO: handle malloc error
  // tickParser->__private = subject;
  // tickParser->attach_observer = __tickParser_attach_observer;
  // tickParser->detach_observer = __tickParser_detach_observer;
  return tickParser;
}

void __tickParser_destructor(T *tickParser){
  free(tickParser);
}

void* __tickParser_parse(T *tickParser, char*input){

  jsmntok_t *tokens = NULL;
  int token_num = json_parse(input, &tokens);

  if (token_num <= 0) {
    if (tokens) {
      free(tokens);
    }
    return NULL;
  }

  char type[20];
  char status[20];
  for (int i = 1; i < token_num; i++) {
    if (json_cmp_token_to_string(input, &tokens[i], "type") == 0) {
      json_extract_token(tokens, i, input, type, to_char);
      i++;
    }
    if (json_cmp_token_to_string(input, &tokens[i], "status") == 0) {
      json_extract_token(tokens, i, input, status, to_char);
      i++;
    }
  }

  Token_info info = {};
  sprintf(info.type, "%s", type);
  sprintf(info.status, "%s", status);
  info.tokens = tokens;
  info.token_num = token_num;
  info.input = input;
  __new_tick(tickParser, info);
  return NULL;
}

void __new_tick(T *tickParser, Token_info info){
    if (strcmp(info.type, "TICKER") == 0) {
      Tick *tick = tick_create(info.input, &info.tokens, info.token_num);
      // mtqueue_enqueue(q->ticks, tick);
      // __tickParser_set_subject_state(tickParser, tick);
    } else if (strcmp(info.type, "Market") == 0) {
      // Order order = order_create(info.input, &info.tokens, info.token_num);
      // mtqueue_enqueue(q->orders, &order);
    // __tickParser_set_subject_state(order);
    }
    free(info.tokens);
}

// void __tickParser_attach_observer(T *tickParser, Observer *observer){
//   Subject *subject = (Subject*)tickParser->__private;
//   subject->attach(subject,observer);
// }

// void __tickParser_detach_observer(T *tickParser, Observer *observer){
//   Subject *subject = (Subject*)tickParser->__private;
//   subject->detach(subject,observer);
// }

// void __tickParser_notify(T *tickParser){
//   Subject *subject = (Subject*)tickParser->__private;
//   subject->notify(subject);
// }

// void __tickParser_set_subject_state(T*tickParser, void* newState) {
//     Subject * subject = tickParser->__private;
//     subject->state = newState;
//     __tickParser_notify(tickParser);
// }

#undef T

