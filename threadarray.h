#ifndef THREADARRAY
#define THREADARRAY

#include "defs.h"
#include <stddef.h>

typedef struct threadarray threadarray;

// initialize the item array with max length of m
extern threadarray *tharr_init(size_t m);

// create and return a new copy of the item array with size 'm', init new item array if 'arr' is null
extern threadarray *tharr_copy(threadarray *arr, size_t m);

// free the memory used by the item array
extern void tharr_free(threadarray *arr);

// insert a item into the item array, return true if successful
extern bool tharr_push(threadarray *arr, void *a);

// concatenate the src item array onto the end of the dest item array
extern bool tharr_concat(threadarray *dest, threadarray *src);

// peek at the head of the item array
extern void *tharr_peek(threadarray *arr);

// pop the head of the item array
extern void *tharr_pop(threadarray *arr);

// apply the function to each item in the array
extern void tharr_foreach(threadarray *arr, void *(*func)(void *));

// remove the items marked with false by the function, returns the new size of the array
extern size_t tharr_reduce(threadarray *arr, bool (*func)(void *));

#endif // THREADARRAY
