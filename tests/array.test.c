#include "array.h"
#include "hashmap.h"
#include "RuntimeErrorImp.h"
#include <criterion/criterion.h>
#define T Array
#define DEFAULT Array_types_default
#define HASHMAP Array_types_hashmap
#define ARRAY Array_types_array
#define EMPTY Array_types_empty
T*a = NULL;

static void setup(void) {
  runtimeError_reset();
  runtimeerrors_mode = RuntimeErrors_mode_silent;
}

static void teardown(void) { free(a); }

Test(array_constructor, param_check, .fini = teardown, .init = setup) {
  T*a = array_constructor(0);
  cr_expect_null(a, "array is not null");
  cr_expect_neq(strcmp(runtimeError_get_last_error().message,"No error"),0);
}

Test(array_constructor, default_length, .fini = teardown, .init = setup) {
  T*a = array_constructor(10);
  cr_expect_not_null(a, "array is null");
  cr_expect_eq(a->length(a), 0);
}

Test(array_push, correct_length, .fini = teardown, .init = setup) {
  T*a = array_constructor(10);
  char* value = malloc(sizeof(char)*10);
  sprintf(value,"test");
  a->push(a,(Array_item){.type=DEFAULT,.value=value});
  cr_expect_eq(a->length(a), 1);
}

Test(array_push, correct_resizing, .fini = teardown, .init = setup) {
  size_t size = 5;
  T*a = array_constructor(size);
  for (size_t i = 0; i < size+1; i++) {
    char* value = malloc(sizeof(char)*10);
    sprintf(value,"test");
    a->push(a,(Array_item){.type=DEFAULT,.value=value});
  }
  cr_expect_eq(a->length(a), size+1);
  cr_expect_gt(a->capacity(a), size);
}

Test(array_push, get_item_after_resize, .fini = teardown, .init = setup) {
  size_t size = 2;
  T*a = array_constructor(size);
  for (size_t i = 0; i < size+1; i++) {
    char* value = malloc(sizeof(char)*10);
    sprintf(value,"test%zu",i);
    a->push(a,(Array_item){.type=DEFAULT,.value=value});
  }
  char* item0 = a->get(a,0).value;
  char* item1 = a->get(a,1).value;
  char* item2 = a->get(a,2).value;
  cr_expect_eq(strcmp(item0,"test0"),0,"values are not equal");
  cr_expect_eq(strcmp(item1,"test1"),0,"values are not equal");
  cr_expect_eq(strcmp(item2,"test2"),0,"values are not equal");
}

Test(array_get, retrieve_items, .fini = teardown, .init = setup) {
  size_t size = 3;
  T*a = array_constructor(size);
  for (size_t i = 0; i < size+1; i++) {
    char* value = malloc(sizeof(char)*10);
    sprintf(value,"test %zu",i);
    a->push(a,(Array_item){.type=DEFAULT,.value=value});
  }
  Array_item item0 = a->get(a,0);
  cr_expect_eq(strcmp(item0.value,"test 0"),0);
  Array_item item1 = a->get(a,1);
  cr_expect_eq(strcmp(item1.value,"test 1"),0);
  Array_item item2 = a->get(a,2);
  cr_expect_eq(strcmp(item2.value,"test 2"),0);
}

Test(array_get, outOfBounds_check, .fini = teardown, .init = setup) {
  size_t size = 3;
  T*a = array_constructor(size);
  for (size_t i = 0; i < size+1; i++) {
    char* value = malloc(sizeof(char)*10);
    sprintf(value,"test %zu",i);
    a->push(a,(Array_item){.type=DEFAULT,.value=value});
  }
  Array_item item0 = a->get(a,4);
  cr_expect_eq(item0.type,EMPTY);
  cr_expect_eq(item0.value,NULL);
  Array_item item1 = a->get(a,-1);
  cr_expect_eq(item1.type,EMPTY);
  cr_expect_eq(item1.value,NULL);
}

Test(array_destructor, is_truly_destroyed, .fini = teardown, .init = setup) {
  size_t size = 3;
  T*a = array_constructor(size);
  char* value0 = malloc(sizeof(char)*10);
  sprintf(value0,"test 0");
  a->push(a,(Array_item){.type=DEFAULT,.value=value0});
  char* value1 = malloc(sizeof(char)*10);
  sprintf(value1,"test 1");
  a->push(a,(Array_item){.type=DEFAULT,.value=value1});
  a->destructor(a);
  cr_expect_neq(strcmp(value0,"value0"),0, "values are the same");
  cr_expect_neq(strcmp(value1,"value1"),0, "values are the same");
}

Test(array_destructor, deep_array, .fini = teardown, .init = setup) {
  T*a = array_constructor(3);
  T*b = array_constructor(3);
  char* value0 = malloc(sizeof(char)*10);
  sprintf(value0,"test0");
  char* value1 = malloc(sizeof(char)*10);
  sprintf(value1,"test1");
  b->push(b,(Array_item){.type=DEFAULT,.value=value0});
  b->push(b,(Array_item){.type=DEFAULT,.value=value1});
  a->push(a,(Array_item){.type=ARRAY,.value=b});
  Array_item items = a->get(a,0);
  Array* c = items.value;
  Array_item item0 = c->get(c,0);
  Array_item item1 = c->get(c,1);
  a->destructor(a);
  cr_expect_neq(strcmp(item0.value,"test0"),0, "values are the same");
  cr_expect_neq(strcmp(item1.value,"test1"),0, "values are the same");
}

Test(array, retrieve_array, .fini = teardown, .init = setup) {
  T*a = array_constructor(3);
  T*b = array_constructor(3);
  char* value0 = malloc(sizeof(char)*10);
  sprintf(value0,"test0");
  char* value1 = malloc(sizeof(char)*10);
  sprintf(value1,"test1");
  b->push(b,(Array_item){.type=DEFAULT,.value=value0});
  b->push(b,(Array_item){.type=DEFAULT,.value=value1});
  a->push(a,(Array_item){.type=ARRAY,.value=b});
  Array_item items = a->get(a,0);
  Array* c = items.value;
  Array_item item0 = c->get(c,0);
  Array_item item1 = c->get(c,1);
  cr_expect_eq(strcmp(item0.value,"test0"),0, "values are not equal");
  cr_expect_eq(strcmp(item1.value,"test1"),0, "values are not equal");
}

#undef DEFAULT
#undef HASHMAP
#undef ARRAY
#undef EMPTY
