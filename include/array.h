#include <stddef.h>
#include <stdbool.h>
#ifndef ARRAY_H
#define ARRAY_H

typedef void (*Foreach_callback)(const void *value, const int index, const size_t array_size);
typedef void (*Reduce_callback)(void *accumulator, const void *current_value, const int index, const size_t array_size); 
typedef bool (*Condition_callback)(const void *current_value, const int index, const size_t array_size); 

/**
 * Iterate over an array and execute a callback for each element of the array
 * @param array - pointer to an array to iterate over
 * @param array_size - size of the array
 * @param elem_size - size of each element of the array
 * @param callback - function to execute on each element
 * @return void
 */
void foreach(const void *array, size_t array_size, size_t elem_size, Foreach_callback callback);

/**
 * Iterate over an array and reduce it to a single value
 * @param accumulator - pointer to a variable storing the end result, should be initilized with a default value
 * @param array - adresse of an array to iterate over
 * @param array_size - size of the array
 * @param callback - function to execute on each element
 * @return void
 */
void reduce(void *accumulator, void *array, size_t array_size, size_t elem_size,Reduce_callback callback);

/**
 * Iterate over an array and filter it according to certain condition(s)
 * @param array - pointer to an array to iterate over
 * @param array_size - size of the array
 * @param result_size - pointer to the result array size
 * @param callback - function testing a condition
 * @return a pointer to the dynamic result array
 */
void* filter(void *array, size_t array_size, size_t elem_size,size_t* result_size,Condition_callback callback );

/**
 * Tests whether all elements in the array pass the test implemented by the provided function
 * @param array - pointer to an array to iterate over
 * @param array_size - size of the array
 * @param result_size - pointer to the result array size
 * @param callback - function testing a condition
 * @return void
 */
bool every(void *array, size_t array_size, size_t elem_size, Condition_callback callback);

/**
 * Tests whether at least one elements in the array pass the test implemented by the provided function
 * @param array - pointer to an array to iterate over
 * @param array_size - size of the array
 * @param result_size - pointer to the result array size
 * @param callback - function testing a condition
 * @return void
 */
bool some(void *array, size_t array_size, size_t elem_size, Condition_callback callback);

/**
 * Creates and returns a pointer to a string by concatenating all of the elements in an array
 * @param array - pointer to an array to iterate over
 * @param array_size - size of the array
 * @param separator - string to separate each pair of adjacent elements of the array
 * @return a pointer to the dynamic array (string)
 */
char* join(const char**array,size_t array_size,const char *separator);

/**
 * Return the size of a dynamic array of string 
 * @param array - pointer to a dynamic array of string 
 * @return the size of the array
 */
size_t size_of_2d_array(const char**array);

/**
 * returns the index of the first element in an array that satisfies the provided testing function
 * @param array - pointer to an array to iterate over
 * @param array_size - size of the array
 * @param result_size - pointer to the result array size
 * @param callback - function testing a condition
 * @return matching index from array or -1 if no element matches
 */
int find_index(void *array, size_t array_size, size_t elem_size, Condition_callback callback);

#endif
