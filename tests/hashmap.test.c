#include "Hashmap.h"
#include <criterion/criterion.h>
#define T Hashmap

T *map = NULL;

static void setup(void) { map = hashmap_constructor(10); }

static void teardown(void) { free(map); }

Test(hashmap_constructor, build, .init = setup, .fini = teardown) {
  cr_assert_not_null(map, "should not be NULL");
  cr_assert_gt(map->capacity(map), 10, "Capacity should be greater");
  cr_assert_eq(map->length(map), 0, "Length should be 0");
}

Test(hashmap_constructor, param_check, .fini = teardown) {
  map = hashmap_constructor(0);
  cr_assert_null(map, "should be NULL");
  map = hashmap_constructor(-1);
  cr_assert_null(map, "should be NULL");
  map = hashmap_constructor(1000000);
  cr_assert_null(map, "map should be NULL");
}

Test(hashmap_push, insertion, .init = setup, .fini = teardown) {
  char key0[10] = "DOLLAR", value0[10] = "$";
  Hashmap_Entry item ={.key=key0,.type=Item_default,.value=value0};
  map->push(map ,item);
  cr_assert_eq(map->length(map), 1, "Wrong length");
  char key1[10] = "Euro", value1[10] = "€";
  Hashmap_Entry item2 ={.key=key1,.type=Item_default,.value=value1};
  map->push(map ,item2);
  cr_assert_eq(map->length(map), 2, "Wrong length");
}

Test(hashmap_get, retrieve, .init = setup, .fini = teardown) {
  char key0[10] = "DOLLAR", value0[10] = "$";
  Hashmap_Entry item = {.key=key0,.type=Item_default,.value=value0};
  map->push(map, item);
  char* v0 = map->get(map,key0 ).value;
  cr_assert_eq(strcmp(v0,value0),0, "Should be equal");
  char key1[10] = "Euro", value1[10] = "€";
  Hashmap_Entry item2 = {.key=key1,.type=Item_default,.value=value1};
  map->push(map, item2);
  char* v1 = map->get(map, key1).value;
  cr_assert_eq(strcmp(v1,value1),0, "Should be equal");
}

Test(hashmap_get, case_sensitive, .init = setup, .fini = teardown) {
  char key[10] = "DOLLAR", value[10] = "$";
  Hashmap_Entry item ={.key=key,.type=Item_default,.value=value};
  map->push(map, item);
  char *result = map->get(map, "dollar").value;
  cr_assert_null(result, "Result should be NULL");
}

Test(hashmap_push, is_resized, .init=setup, .fini = teardown) {
  char key[3];
  for (int i = 0; i < 20; i++) {
    sprintf(key,"%d",i);
    Hashmap_Entry item ={.key=key,.type=Item_null,.value=NULL};
    map->push(map, item );
  }
  cr_assert_gt(map->capacity(map), 20, "Capacity should be greater");
}

Test(hashmap_push, nested_hashmap, .init = setup, .fini = teardown) {
  T*map2 = hashmap_constructor(10);
  Hashmap_Entry item ={.key="DOLLAR",.type=Item_default,.value="$"};
  map2->push(map2,item);
  Hashmap_Entry item2 ={.key="Symbols",.type=Item_array,.value=map2};
  map->push(map, item2);
  T*map3 = map->get(map,"Symbols").value;
  char *result = map3->get(map3, "DOLLAR").value;
  cr_assert_eq(strcmp(result,"$"),0, "Should be equal");
}

Test(hashmap_push, replacement, .init = setup, .fini = teardown) {
  Hashmap_Entry item = {.key="Symbols",.type=Item_array,.value="$"};
  map->push(map, item);
  Hashmap_Entry item2 = {.key="Symbols",.type=Item_array,.value="€"};
  map->push(map, item2);
  char* result = map->get(map,"Symbols").value;
  cr_assert_eq(strcmp(result,"€"),0, "Should be equal");
}

Test(hashmap_destructor, is_truly_destroyed, .fini = teardown) {
  T*map = hashmap_constructor(10);
  T*map2 = hashmap_constructor(10);
  char* value = malloc(sizeof(char)*2);
  sprintf(value,"$");
  Hashmap_Entry item = {.key="DOLLAR",.type=Item_default,.value=value};
  map2->push(map2,item);
  Hashmap_Entry item2 = {.key="Symbols",.type=Item_array,.value=map2};
  map->push(map, item2);
  map->destructor(map);
  cr_expect_neq(strcmp(value,"$"),0, "values are the same");
}

Test(hashmap_keys, correct_keys, .init=setup, .fini = teardown) {
  char key[3];
  int result = 1 + 2 + 3 + 4; 
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sprintf(key,"%d",i);
    Hashmap_Entry item = {.key=key,.type=Item_null,.value=NULL};
    map->push(map, item);
  }
  char**keys = map->keys(map);
  for (int i = 0; i < 5; i++) {
    sum+= atoi(keys[i]);
  }
  cr_assert_eq(result,sum, "Should be equal");
}

Test(hashmap_values, correct_values, .init=setup, .fini = teardown) {
  char key[3];
  int result = 1 + 2 + 3 + 4; 
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sprintf(key,"%d",i);
    char* value = malloc(sizeof(char)*3);
    sprintf(value,"%d",i);
    Hashmap_Entry item = {.key=key,.type=Item_null,.value=value};
    map->push(map, item);
  }
  Item**values = map->values(map);
  for (int i = 0; i < 5; i++) {
    sum+= atoi((char*)values[i]->value);
  }
  cr_assert_eq(result,sum, "Should be equal");
}

Test(hashmap_entries, empty_map, .init=setup, .fini = teardown) {
  Hashmap_Entry**entries = map->entries(map);
  cr_assert_null(entries,"Should be null");
}

Test(hashmap_entries, simple, .init=setup, .fini = teardown) {
  Hashmap_Entry item ={.key="DOLLAR",.type=Item_string,.value="$"};
  map->push(map, item);
  Hashmap_Entry item2 = {.key="EURO",.type=Item_string,.value="€"};
  map->push(map, item2);
  Hashmap_Entry**entries = map->entries(map);
  cr_assert_eq(strcmp(entries[0]->value,"$"),0, "Should be equal");
  cr_assert_eq(strcmp(entries[1]->value,"€"),0, "Should be equal");
}

Test(hashmap_to_json, correct_values, .fini = teardown) {
  map = hashmap_constructor(10);
  char *res = "{\"DOLLAR\":\"$\"}";
  Hashmap_Entry item ={.key="DOLLAR",.type=Item_string,.value="$"};
  map->push(map, item);
  char *json = map->to_json(map);
  cr_assert_eq(strcmp(json,res),0, "Should be equal");
}

Test(hashmap_to_json, double_keys, .fini = teardown) {
  map = hashmap_constructor(10);
  char *res = "{\"DOLLAR\":\"$\",\"EURO\":\"€\"}";
  Hashmap_Entry item ={.key="DOLLAR",.type=Item_string,.value="$"};
  map->push(map ,item);
  Hashmap_Entry item2 ={.key="EURO",.type=Item_string,.value="€"};
  map->push(map ,item2);
  char *json = map->to_json(map);
  cr_assert_eq(strcmp(json,res),0, "Should be equal");
}

Test(hashmap_to_json, double, .fini = teardown) {
  map = hashmap_constructor(10);
  char *res = "{\"amount\":2,\"price\":2.5}";
  double price = 2.5;
  double amount = 2;
  Hashmap_Entry item ={.key="price",.type=Item_double,.value=&price};
  map->push(map ,item);
  Hashmap_Entry item2 ={.key="amount",.type=Item_double,.value=&amount};
  map->push(map ,item2);
  char *json = map->to_json(map);
  cr_assert_eq(strcmp(json,res),0, "Should be equal");
}

Test(hashmap_to_json, int, .fini = teardown) {
  map = hashmap_constructor(10);
  char *res = "{\"amount\":2,\"price\":2}";
  int price = 2;
  int amount = 2;
  Hashmap_Entry item ={.key="price",.type=Item_int,.value=&price};
  map->push(map ,item);
  Hashmap_Entry item2 ={.key="amount",.type=Item_int,.value=&amount};
  map->push(map ,item2);
  char *json = map->to_json(map);
  cr_assert_eq(strcmp(json,res),0, "Should be equal");
}

Test(hashmap_to_json, boolean, .fini = teardown) {
  map = hashmap_constructor(10);
  char *res = "{\"amount\":true,\"price\":false}";
  bool price = false;
  bool amount = true;
  Hashmap_Entry item ={.key="price",.type=Item_bool,.value=&price};
  map->push(map ,item);
  Hashmap_Entry item2 ={.key="amount",.type=Item_bool,.value=&amount};
  map->push(map ,item2);
  char *json = map->to_json(map);
  cr_assert_eq(strcmp(json,res),0, "Should be equal");
}

Test(hashmap_to_json, null, .fini = teardown) {
  map = hashmap_constructor(10);
  char *res = "{\"amount\":null,\"price\":null}";
  char* price = NULL;
  char* amount = NULL;
  Hashmap_Entry item ={.key="price",.type=Item_null,.value=&price};
  map->push(map ,item);
  Hashmap_Entry item2 ={.key="amount",.type=Item_null,.value=&amount};
  map->push(map ,item2);
  char *json = map->to_json(map);
  cr_assert_eq(strcmp(json,res),0, "Should be equal");
}

Test(hashmap_to_json, hashmap, .fini = teardown) {
  map = hashmap_constructor(10);
  char *res = "{\"amount\":{\"value\":\"$\"},\"price\":{\"value\":\"£\"}}";
  Hashmap* price = hashmap_constructor(1);
  Hashmap_Entry item ={.key="value",.type=Item_string,.value="£"};
  price->push(price ,item);
  Hashmap* amount = hashmap_constructor(1);
  Hashmap_Entry item2 ={.key="value",.type=Item_string,.value="$"};
  amount->push(amount ,item2);
  Hashmap_Entry item3 ={.key="price",.type=Item_map,.value=price};
  map->push(map ,item3);
  Hashmap_Entry item4 ={.key="amount",.type=Item_map,.value=amount};
  map->push(map ,item4);
  char *json = map->to_json(map);
  printf("%s\n",json);
  cr_assert_eq(strcmp(json,res),0, "Should be equal");
}

#undef MAX_CAPACITY
#undef T
