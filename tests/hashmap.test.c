#include "hashmap.h"
#include <criterion/criterion.h>
#define T Hashmap

T *map = NULL;

static void setup(void) { map = hashmap_constructor(10); }

static void teardown(void) { free(map); }

Test(hashmap, build, .init = setup, .fini = teardown) {
  Hashmap_entry *entries = map->entries;
  cr_assert_not_null(map, "Result should not be NULL");
  cr_assert_not_null(entries, "Result should not be NULL");
  cr_assert_eq(map->capacity, 10, "Capacity should be 10");
  cr_assert_eq(map->size, 0, "Size should be 0");
  cr_assert_not_null(map->entries, "entries should should not be NULL");
  cr_assert_not_null(&map->destructor,
                     "Destructor method should should be defined");
  cr_assert_not_null(&map->get, "Get method should should be defined");
  cr_assert_not_null(&map->push, "Push method should should be defined");
}

Test(hashmap, map_size_is_positiv, .fini = teardown) {
  map = hashmap_constructor(0);
  cr_assert_null(map, "map should be NULL");
  map = hashmap_constructor(-1);
  cr_assert_null(map, "map should be NULL");
  map = hashmap_constructor(1000000);
  cr_assert_null(map, "map should be NULL");
}

Test(hashmap, insertion, .init = setup, .fini = teardown) {
  char key1[10] = "DOLLAR", value1[10] = "$";
  map->push(map, key1, value1,0);
  cr_assert_eq(map->size, 1, "Size should be 1");
  char key2[10] = "Euro", value2[10] = "€";
  map->push(map, key2, value2,0);
  cr_assert_eq(map->size, 2, "Size should be 2");
}

Test(hashmap, retrieve, .init = setup, .fini = teardown) {
  char key[10] = "DOLLAR", value[10] = "$";
  map->push(map, key, value,0);
  char *result = map->get(map, key,NULL);
  cr_assert_eq(result, value, "Result should be equal to '$'");
}

Test(hashmap, case_sensitive, .init = setup, .fini = teardown) {
  char key[10] = "DOLLAR", value[10] = "$";
  map->push(map, key, value,0);
  char *result = map->get(map, "dollar",NULL);
  cr_assert_null(result, "Result should be NULL");
}

Test(hashmap, resize_is_triggered, .init = setup, .fini = teardown) {
  map = hashmap_constructor(10);
  char result[2];
  for (int i = 1; i <= 7; i++) {
    snprintf(result, sizeof(result), "%d", i);
    map->push(map, result, "$",0);
  }
  cr_assert_eq(map->capacity, 10, "Capacity should be 10");
  for (int i = 7; i <= 10; i++) {
    snprintf(result, sizeof(result), "%d", i);
    map->push(map, result, "$",0);
  }
  cr_assert_eq(map->capacity, 20, "Capacity should have been doubled");
}

#undef MAX_CAPACITY
#undef T
