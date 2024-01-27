#include "Message.h"
#include <criterion/criterion.h>
#define T Message

T*message = NULL;
static void setup(void) {
  Item item = {.value = "Hello World", .type = Item_string};
  Msg_args args = (Msg_args){.item=item, .code=Msg_stop};
  message = message_constructor(args);
}

static void teardown(void) {}

Test(message_constructor, build, .init = setup, .fini = teardown) {
  cr_expect_not_null(message);
}

Test(message_code, retrieve_value, .init = setup, .fini = teardown) {
  cr_expect_not_null(message);
  Msg_type type = message->code(message,READ,0);
  cr_expect_eq(type, Msg_stop);
}

Test(message_value, retrieve_value, .init = setup, .fini = teardown) {
  cr_expect_not_null(message);
  Item item = message->value(message,READ,(Item){});
  cr_expect_eq(item.type, Item_string);
  cr_expect_eq(strcmp(item.value, "Hello World"), 0);
}

Test(message_code, write, .init = setup, .fini = teardown) {
  cr_expect_not_null(message);
  Msg_type new = Msg_candle;
  Msg_type type = message->code(message,WRITE,new);
  cr_expect_eq(type, Msg_candle);
}

Test(message_value, write, .init= setup, .fini = teardown) {
  cr_expect_not_null(message);
  Item new = {.value = "Hello", .type = Item_string};
  Item item = message->value(message,WRITE,new);
  cr_expect_eq(item.type, Item_string);
  cr_expect_eq(strcmp(item.value, "Hello"), 0);
}

#undef T
