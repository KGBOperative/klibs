#include "bsa.h"
#include <stdlib.h>
#include <string.h>

struct bsa {
    const int8_t t;
    size_t n;
    size_t m;

    int (*cmp)(const void *, const void *);

    void **as;
};

bsa *bsa_init(int (*cmp)(const void *, const void *)) {
    bsa *b = malloc(sizeof(bsa));
    *((int8_t *) b) = 1;
    b->m = 100;
    b->n = 0;
    b->cmp = cmp;
    b->as = malloc(b->m * sizeof(void *));

    return b;
}

bool bsa_push(bsa *b, void *a) {
    b->n++;
    if (b->n == b->m) {
        size_t new_n = b->m << 2;
        void **new_as;
        if ((new_as = malloc(new_n * sizeof(void *))) == null) {
            return false;
        }
        memcpy(new_as, b->as, b->m * sizeof(void *));
        free(b->as);
        b->as = new_as;
        b->m = new_n;
    }

    size_t n = b->n - 1;
    size_t m = 0;

    size_t i = 0;
    while (m < n) {
        i = (m + n) / 2;
        int cmp = b->cmp(a, b->as[i]);
        if (cmp < 0) {
            n = i - 1;
        } else if (cmp > 0) {
            m = i + 1;
        } else {
            break;
        }
    }

    memmove(&b->as[i + 1], &b->as[i], (b->n - i) * sizeof(void *));
    b->as[i] = a;

    return true;
}

void *bsa_pop(bsa *b) {
    return b->n > 0 ? b->as[0] : null;
}

void *bsa_peek(bsa *b) {
    void *a = null;
    if (b->n > 0) {
        b->n--;
        a = b->as[0];
        memmove(&b->as[0], &b->as[1], b->n * sizeof(void *));
    }

    return a;
}

bool bsa_has(bsa *b, void *a) {
    size_t n = b->n - 1;
    size_t m = 0;

    size_t i = 0;
    while (m < n) {
        i = (m + n) / 2;
        int cmp = b->cmp(a, b->as[i]);
        if (cmp < 0) {
            n = i - 1;
        } else if (cmp > 0) {
            m = i + 1;
        } else {
            return true;
        }
    }

    return false;
}

bool bsa_empty(bsa *b) {
    return b->n == 0;
}

void bsa_foreach(bsa *b, void *(*func)(void *)) {
    size_t i;
    for (i = 0; i < b->n; i++) {
        b->as[i] = (func)(b->as[i]);
    }
}

size_t bsa_reduce(bsa *b, bool (*func)(void *)) {
    size_t i, c = 0;
    for (i = 0; i < b->n && (func)(b->as[i]); i++, c++) {
    }

    if (c < b->n) {
        for (i++; i < b->n; i++) {
            if ((func)(b->as[i])) {
                b->as[c++] = b->as[i];
            }
        }

        b->n = c;
    }

    return b->n;
}
