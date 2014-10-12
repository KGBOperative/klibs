#include "defs.h"

typedef struct {
    bool e;
    union {
        void *val;
        int err;
    };
} optional;