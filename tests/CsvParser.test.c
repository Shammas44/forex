#include "CsvParser.h"
#include "Hashmap.h"
#include <criterion/criterion.h>
#define T CsvParser

static T *parser = NULL;
char csv[] =
    "Date,Time,Open,UpVolume\n"
    "1/21/2015,13:29:00,1.21111,200\n"
    "1/22/2015,13:29:00,1.22222,300\n"
    "1/23/2015,13:29:00,1.23333,400\n"
    "1/24/2015,13:29:00,1.24444,500\n"
    "1/25/2015,13:29:00,1.25555,600";

static void teardown(void) {
  parser->destructor(parser);
}

static void init(void) {
  parser = csvParser_constructor();
  CsvParser_config config = {.delimiter = ","};
  Parser_config_obj *c = (Parser_config_obj *)&config;
  parser->config(parser, c);
}

Test(T, parse, .init = init ,.fini=teardown) {
  Array* array = parser->parse(parser, csv);
  cr_assert_eq(array->length(array), 5, "Array length should be 5");
  Hashmap* map0 = array->get(array,0).value;
  cr_assert_eq(*(double*)map0->get(map0,"Open").value, 1.21111);
  cr_assert_eq(*(double*)map0->get(map0,"UpVolume").value, 200);
  cr_assert_eq(strcmp(map0->get(map0,"Date").value, "1/21/2015"),0);
  cr_assert_eq(strcmp(map0->get(map0,"Time").value, "13:29:00"),0);
}

#undef T
