#include "hashmap.h"
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
  map->push(map, key0,(Item){.type=Item_default,.value=value0});
  cr_assert_eq(map->length(map), 1, "Wrong length");
  char key1[10] = "Euro", value1[10] = "€";
  map->push(map, key1,(Item){.type=Item_default,.value=value1});
  cr_assert_eq(map->length(map), 2, "Wrong length");
}

Test(hashmap_get, retrieve, .init = setup, .fini = teardown) {
  char key0[10] = "DOLLAR", value0[10] = "$";
  map->push(map, key0,(Item){.type=Item_default,.value=value0});
  char* v0 = map->get(map, key0).value;
  cr_assert_eq(strcmp(v0,value0),0, "Should be equal");
  char key1[10] = "Euro", value1[10] = "€";
  map->push(map, key1,(Item){.type=Item_default,.value=value1});
  char* v1 = map->get(map, key1).value;
  cr_assert_eq(strcmp(v1,value1),0, "Should be equal");
}

Test(hashmap_get, case_sensitive, .init = setup, .fini = teardown) {
  char key[10] = "DOLLAR", value[10] = "$";
  map->push(map, key,(Item){.type=Item_default,.value=value});
  char *result = map->get(map, "dollar").value;
  cr_assert_null(result, "Result should be NULL");
}

Test(hashmap_push, is_resized, .init=setup, .fini = teardown) {
  char key[3];
  for (int i = 0; i < 20; i++) {
    sprintf(key,"%d",i);
    map->push(map, key, (Item){.type=Item_null,.value=NULL});
  }
  cr_assert_gt(map->capacity(map), 20, "Capacity should be greater");
}

Test(hashmap_push, nested_hashmap, .init = setup, .fini = teardown) {
  T*map2 = hashmap_constructor(10);
  map2->push(map2,"DOLLAR",(Item){.type=Item_default,.value="$"});
  map->push(map, "Symbols",(Item){.type=Item_array,.value=map2});
  T*map3 = map->get(map,"Symbols").value;
  char *result = map3->get(map3, "DOLLAR").value;
  cr_assert_eq(strcmp(result,"$"),0, "Should be equal");
}

Test(hashmap_push, replacement, .init = setup, .fini = teardown) {
  map->push(map, "Symbols",(Item){.type=Item_array,.value="$"});
  map->push(map, "Symbols",(Item){.type=Item_array,.value="€"});
  char* result = map->get(map,"Symbols").value;
  cr_assert_eq(strcmp(result,"€"),0, "Should be equal");
}

Test(hashmap_destructor, is_truly_destroyed, .fini = teardown) {
  T*map = hashmap_constructor(10);
  T*map2 = hashmap_constructor(10);
  char* value = malloc(sizeof(char)*2);
  sprintf(value,"$");
  map2->push(map2,"DOLLAR",(Item){.type=Item_default,.value=value});
  map->push(map, "Symbols",(Item){.type=Item_array,.value=map2});
  map->destructor(map);
  cr_expect_neq(strcmp(value,"$"),0, "values are the same");
}

Test(hashmap_keys, correct_keys, .init=setup, .fini = teardown) {
  char key[3];
  int result = 1 + 2 + 3 + 4; 
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sprintf(key,"%d",i);
    map->push(map, key, (Item){.type=Item_null,.value=NULL});
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
    map->push(map, key, (Item){.type=Item_null,.value=value});
  }
  Item**values = map->values(map);
  for (int i = 0; i < 5; i++) {
    sum+= atoi((char*)values[i]->value);
  }
  cr_assert_eq(result,sum, "Should be equal");
}

Test(hashmap_entries, empty_map, .init=setup, .fini = teardown) {
  IsIterable_Entry**entries = map->entries(map);
  cr_assert_null(entries,"Should be null");
}

Test(hashmap_entries, simple, .init=setup, .fini = teardown) {
  map->push(map, "DOLLAR",(Item){.type=Item_string,.value="$"});
  map->push(map, "EURO",(Item){.type=Item_string,.value="€"});
  IsIterable_Entry**entries = map->entries(map);
  cr_assert_eq(strcmp(entries[0]->value,"$"),0, "Should be equal");
  cr_assert_eq(strcmp(entries[1]->value,"€"),0, "Should be equal");
}

// Test(hashmap_to_json, correct_values, .fini = teardown) {
//   map = hashmap_constructor(10);
//   map->push(map, "DOLLAR",(Item){.type=Item_string,.value="$"});
//   char *json = map->to_json(map);
//   cr_assert_eq(strcmp(json,"{\"DOLLAR\":\"$\"}"),0, "Should be equal");
// }

#undef MAX_CAPACITY
#undef T
