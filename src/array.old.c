// #include "array.old.h"
// #include "guard.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// void foreach (const void *array, size_t array_size, size_t elem_size, Foreach_callback callback) {
//   // we use char as a type but it could be anything else
//   char *p = (char *)array;
//   for (size_t i = 0; i < array_size; i++) {
//     callback((void *)p, i, array_size);
//     p += elem_size;
//   }
// }

// void reduce(void *accumulator, void *array, size_t array_size, size_t elem_size, Reduce_callback callback) {
//   char *p = (char *)array;
//   for (size_t i = 0; i < array_size; i++) {
//     callback(accumulator, (void *)p, i, array_size);
//     p += elem_size;
//   }
// }

// void *filter(void *array, size_t array_size, size_t elem_size, size_t *result_size, Condition_callback callback) {
//   char *p_result = malloc(array_size * elem_size);
//   if (p_result == NULL) {
//     *result_size = 0;
//     return NULL;
//   }

//   char *p = (char *)array;
//   int size = 0;
//   for (size_t i = 0; i < array_size; i++) {
//     if (callback((void *)p, i, array_size)) {
//       // Use pointer arithmetic to calculate the address to store the element
//       char *p_filtered = p_result + (size * elem_size);
//       for (size_t j = 0; j < elem_size; j++) {
//         p_filtered[j] = p[j];
//       }
//       size++;
//     }
//     p += elem_size;
//   }

//   // Resize the memory to the exact size required for the filtered elements
//   p_result = realloc(p_result, size * elem_size);
//   *result_size = size;
//   return p_result;
// }

// bool every(void *array, size_t array_size, size_t elem_size, Condition_callback callback) {
//   char *p = (char *)array;
//   for (size_t i = 0; i < array_size; i++) {
//     bool result = callback((void *)p, i, array_size);
//     if (!result) return false;
//     p += elem_size;
//   }
//   return true;
// }

// int find_index(void *array, size_t array_size, size_t elem_size, Condition_callback callback) {
//   char *p = (char *)array;
//   for (size_t i = 0; i < array_size; i++) {
//     bool result = callback((void *)p, i, array_size);
//     if (result) return i;
//     p += elem_size;
//   }
//   return -1;
// }

// bool some(void *array, size_t array_size, size_t elem_size, Condition_callback callback) {
//   char *p = (char *)array;
//   bool result = false;
//   for (size_t i = 0; i < array_size; i++) {
//     result = callback((void *)p, i, array_size);
//     p += elem_size;
//   }
//   return result;
// }

// size_t size_of_2d_array(const char **array) {
//   if (array == NULL)
//     return 0;
//   size_t sum = 0;
//   for (size_t i = 0; array[i] != NULL; i++) {
//     sum += strlen(array[i]);
//   }
//   return sum;
// }

// char *join(const char **array, size_t array_size, const char *separator) {
//   size_t sum = size_of_2d_array(array);
//   size_t separator_length = strlen(separator);
//   size_t all_separators_length = separator_length * (array_size - 1);
//   size_t total_length = sum + all_separators_length;
//   char *result = malloc(total_length + 1);
//   size_t x = 0;
//   for (size_t i = 0; array[i] != NULL; i++) {
//     for (size_t j = 0; j < strlen(array[i]); j++) {
//       result[x++] = array[i][j];
//     }
//     if (i < array_size - 1) {
//       for (size_t j = 0; j < separator_length; j++) {
//         result[x++] = separator[j];
//       }
//     }
//   }
//   result[x] = '\0';
//   return result;
// }
