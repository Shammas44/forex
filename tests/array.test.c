// #include <criterion/criterion.h>
// #include <stdio.h>
// #include <string.h>
// #include "array.h"
// the 3 laws of tdd
// 1. your are not allowed to write production code unless it make a falling test pass
// 2. your are not allowed to any more of a unit test that is sufficient to fail
// 3. your are not allowed to write any more production code than is sufficient to pass the one failing unit test

// void print_int(const void *current_value, const int index, const size_t array_size) {
//   int val = *(int *)current_value;
//   printf("array[%d] = %i; array-size = %zu\n", index, val,array_size);
// }

// int truc(char *str){
//   size_t length = 0;
//   while (str[length] != '\0') {
//     length++;
//   }
//   return length;
// }


// Test(foreach, is_defined){
//   int int_array[] = {1, 2, 3};
//   size_t int_array_size = sizeof(int_array) / sizeof(int);
//   foreach(int_array, int_array_size, sizeof(int), print_int);
// }

// Test(str_len, is_defined){
//   int res = truc("");
//   cr_assert_eq(0,res);
// }
