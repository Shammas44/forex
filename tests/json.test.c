#include "json.h"
#include <criterion/criterion.h>
Hashmap *h;
Darray *a;
char json01[] = "{"
                "\"nom\":\"Doe\","
                "\"prénom\":\"John\","
                "\"age\":\"20\","
                "\"children\":[\"Louis\",\"Sam\",\"Antoine\"],"
                "\"job\":{"
                "\"title\":\"Administrator system IT\","
                "\"company\":\"XYZ Corp.\""
                "}"
                "}";

char json02[] = "{"
                "\"nom\":\"Doe\","
                "\"prénom\":\"John\","
                "\"age\":20"
                "}";

char json03[] = "{"
                "\"nom\":\"Doe\","
                "\"prénom\":\"John\","
                "\"age\":20,"
                "\"job\":{"
                "\"title\":\"Administrator system IT\","
                "\"company\":\"XYZ Corp.\""
                "}"
                "}";

char json04[] = "{"
                "\"nom\":\"Doe\","
                "\"prénom\":\"John\","
                "\"job\":{"
                "\"company\":{"
                "\"name\":\"XYZ Corp.\","
                "\"type\":\"Sa\""
                "}"
                "}"
                "}";

char json05[] = "{"
                "\"isAdmin\":false,"
                "\"isWorking\":true,"
                "\"company\":null"
                "}";

char json06[] = "{"
                "\"isAdmin\":\r,"
                "\"isWorking\":true,"
                "\"company\":null"
                "}";

char json07[] = "{"
                "\"nom\":\"Doe\","
                "\"prénom\":\"John\","
                "\"companies\":["
                "{"
                "\"name\":\"X Corp.\","
                "\"type\":\"Sa\""
                "},"
                "{"
                "\"name\":\"Y Corp.\","
                "\"type\":\"Sa\""
                "},"
                "{"
                "\"name\":\"Z Corp.\","
                "\"type\":\"Sa\""
                "}"
                "]"
                "}";

char json08[] = "["
                "\"Sam\",\"Doe\",\"John\""
                "]";

char json09[] = "["
                "{"
                "\"name\":\"X Corp.\","
                "\"type\":\"Sa\""
                "},"
                "{"
                "\"name\":\"Y Corp.\","
                "\"type\":\"Sa\""
                "},"
                "{"
                "\"name\":\"Z Corp.\","
                "\"type\":\"Sa\""
                "}"
                "]";

char json10[] = "["
                "[\"Sam\",\"Doe\",\"John\"],"
                "[1,2,3],"
                "[true,false,null]"
                "]";

// static void setup(void) {
//   // puts("Runs before the test");
// }

static void teardown(void) { free(h); }

Test(json_to_map, get_string, .fini = teardown) {
  json_to_map(json02,&h);
  cr_expect_str_eq(HASHMAP_GET_STRING(h, "nom"), "Doe", "Wrong value");
  cr_expect_str_eq(HASHMAP_GET_STRING(h, "prénom"), "John", "Wrong value");
  cr_expect_str_eq(HASHMAP_GET_STRING(h, "age"), "20", "Wrong value");
  cr_expect_eq(h->size, 3, "Wrong size");
}

Test(json_to_map, get_inner_object, .fini = teardown) {
  json_to_map(json03,&h);
  Hashmap *job = h->get(h, "job");
  cr_expect_not_null(job, "job is null");
  cr_expect_str_eq(HASHMAP_GET_STRING(job, "title"), "Administrator system IT", "wrong value");
  cr_expect_str_eq(HASHMAP_GET_STRING(job, "company"), "XYZ Corp.", "wrong value");
  cr_expect_eq(h->size, 4, "Wrong size");
}

Test(json_to_map, get_inner_inner_object, .fini = teardown) {
  json_to_map(json04,&h);
  Hashmap *job = h->get(h, "job");
  cr_expect_not_null(job, "job is null");
  Hashmap *company = job->get(job, "company");
  cr_expect_not_null(company, "company is null");
  cr_expect_str_eq(HASHMAP_GET_STRING(company, "name"), "XYZ Corp.");
  cr_expect_str_eq(HASHMAP_GET_STRING(company, "type"), "Sa");
  cr_expect_eq(h->size, 3, "Wrong size");
  cr_expect_eq(company->size, 2, "Wrong size");
}

Test(json_to_map, get_primitive, .fini = teardown) {
  json_to_map(json05,&h);
  cr_expect_str_eq(HASHMAP_GET_STRING(h, "isAdmin"), "false", "wrong value");
  cr_expect_str_eq(HASHMAP_GET_STRING(h, "isWorking"), "true", "wrong value");
  cr_expect_str_eq(HASHMAP_GET_STRING(h, "company"), "null", "wrong value");
  cr_expect_eq(h->size, 3, "Wrong size");
}

Test(json_to_map, get_array, .fini = teardown) {
  json_to_map(json01,&h);
  cr_expect_str_eq(HASHMAP_GET_STRING(h, "nom"), "Doe", "Wrong value");
  cr_expect_str_eq(HASHMAP_GET_STRING(h, "prénom"), "John", "Wrong value");
  cr_expect_str_eq(HASHMAP_GET_STRING(h, "age"), "20", "Wrong value");
  cr_expect_eq(h->size, 5, "Wrong size");
  Darray *children = h->get(h, "children");
  cr_expect_eq(children->length, 3, "Wrong size");
  cr_expect_not_null(children, "children is null");
  cr_expect_str_eq(children->get(children,0), "Louis", "Wrong value");
  cr_expect_str_eq(children->get(children,1), "Sam", "Wrong value");
  cr_expect_str_eq(children->get(children,2), "Antoine", "Wrong value");
}

Test(json_to_map, get_array_of_objects, .fini = teardown) {
  json_to_map(json07,&h);
  Darray *companies = h->get(h, "companies");
  cr_expect_not_null(companies, "companies is null");

  Hashmap*c1 = companies->get(companies,0);
  cr_expect_not_null(c1, "company is null");
  cr_expect_str_eq(HASHMAP_GET_STRING(c1, "name"), "X Corp.", "Wrong value");
  cr_expect_str_eq(HASHMAP_GET_STRING(c1, "type"), "Sa", "Wrong value");

  Hashmap*c2 = companies->get(companies,1);
  cr_expect_not_null(c2, "company is null");
  // printf("length: %zu\n", companies->length);
  // printf("%s\n", HASHMAP_GET_STRING(c2, "name"));
  // printf("%s\n", HASHMAP_GET_STRING(c2, "type"));
  cr_expect_str_eq(HASHMAP_GET_STRING(c2, "name"), "Y Corp.", "Wrong value");
  cr_expect_str_eq(HASHMAP_GET_STRING(c2, "type"), "Sa", "Wrong value");

  Hashmap*c3 = companies->get(companies,2);
  cr_expect_not_null(c3, "company is null");
  cr_expect_str_eq(HASHMAP_GET_STRING(c3, "name"), "Z Corp.", "Wrong value");
  cr_expect_str_eq(HASHMAP_GET_STRING(c3, "type"), "Sa", "Wrong value");
}

Test(json_to_array, get_array, .fini = teardown) {
  json_to_array(json08,&a);
  cr_expect_str_eq(a->get(a,0), "Sam",  "Wrong value");
  cr_expect_str_eq(a->get(a,1), "Doe",  "Wrong value");
  cr_expect_str_eq(a->get(a,2), "John",  "Wrong value");
}

Test(json_to_array, get_array_of_objects, .fini = teardown) {
  json_to_array(json09,&a);
  Hashmap*c1 = a->get(a,0);
  cr_expect_not_null(c1, "company is null");
  cr_expect_str_eq(HASHMAP_GET_STRING(c1, "name"), "X Corp.", "Wrong value");
  cr_expect_str_eq(HASHMAP_GET_STRING(c1, "type"), "Sa", "Wrong value");
  Hashmap*c2 = a->get(a,1);
  cr_expect_not_null(c2, "company is null");
  cr_expect_str_eq(HASHMAP_GET_STRING(c2, "name"), "Y Corp.", "Wrong value");
  cr_expect_str_eq(HASHMAP_GET_STRING(c2, "type"), "Sa", "Wrong value");
  Hashmap*c3 = a->get(a,2);
  cr_expect_not_null(c3, "company is null");
  cr_expect_str_eq(HASHMAP_GET_STRING(c3, "name"), "Z Corp.", "Wrong value");
  cr_expect_str_eq(HASHMAP_GET_STRING(c3, "type"), "Sa", "Wrong value");
}

Test(json_to_array, get_array_of_array, .fini = teardown) {
  json_to_array(json10,&a);
  Darray*a1 = a->get(a,0);
  cr_expect_not_null(a1, "array is null");
  cr_expect_str_eq(a1->get(a1,0), "Sam", "Wrong value");
  cr_expect_str_eq(a1->get(a1,1), "Doe", "Wrong value");
  cr_expect_str_eq(a1->get(a1,2), "John", "Wrong value");
  Darray*a2 = a->get(a,1);
  cr_expect_not_null(a2, "array is null");
  cr_expect_str_eq(a2->get(a2,0), "1", "Wrong value");
  cr_expect_str_eq(a2->get(a2,1), "2", "Wrong value");
  cr_expect_str_eq(a2->get(a2,2), "3", "Wrong value");
  Darray*a3 = a->get(a,2);
  cr_expect_not_null(a3, "array is null");
  cr_expect_str_eq(a3->get(a3,0), "true", "Wrong value");
  cr_expect_str_eq(a3->get(a3,1), "false", "Wrong value");
  cr_expect_str_eq(a3->get(a3,2), "null", "Wrong value");
}