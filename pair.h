#include "defs.h"

typedef struct pair pair;

// initialize the pair struct with values 'a' and 'b'
extern pair *pair_init(void *key, void *val);

// return the key from the pair
extern void *key(pair *t);

// return the value from the pair
extern void *val(pair *t);