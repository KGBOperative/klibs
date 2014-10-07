#ifndef BSA
#define BSA

#include <stddef.h>
#include "defs.h"

typedef struct bsa bsa;

// initialize the bsa (binary search array)
extern bsa *bsa_init(int (*cmp)(const void *, const void *));

// insert an item into the bsa
extern bool bsa_push(bsa *b, void *a);

// remove and return the least item out of the tree
extern void *bsa_pop(bsa *b);

// return the least item in the tree
extern void *bsa_peek(bsa *b);

// returns whether the tree has the item
extern bool bsa_has(bsa *b, void *a);

// returns whether the tree is empty
extern bool bsa_empty(bsa *b);

// applies the function to each of the elements in the bsa
extern void bsa_foreach(bsa *b, void *(*func)(void *));

// remove the items marked as false by the function, returns the new size of the bsa
extern size_t bsa_reduce(bsa *b, bool (*func)(void *));

#endif
