#ifndef LIST
#define LIST

#include <stddef.h>
#include "defs.h"

typedef struct list list;

// initialize the item list with max length of m
extern list *list_init();

// create and return a new copy of the item list with size 'm', init new item list if 'l' is null
extern list *list_copy(list *l, size_t m);

// free the memory used by the item list
extern void list_free(list *l);

// reset the state of the list to init
extern void list_reset(list *l);

// insert a item into the item list, return true if successful
extern bool list_push(list *l, void *a);

// concatenate the src item list onto the end of the dest item list
extern bool list_concat(list *dest, list *src);

// peek at the head of the item list
extern void *list_peek(list *l);

// pop the head of the item list
extern void *list_pop(list *l);

// apply the function to each item in the list
extern void list_foreach(list *l, void *(*func)(void *));

// remove the items marked with false by the function, returns the new size of the list
extern size_t list_reduce(list *l, bool (*func)(void *));

#endif // LIST

