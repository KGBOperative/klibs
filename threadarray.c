#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "defs.h"
#include "threadarray.h"

struct threadarray {
    const int8_t t;
    size_t m;
    size_t n;
    pthread_mutex_t lock;
    pthread_cond_t notify;
    void **as;
};

threadarray *tharr_init(size_t m) {
    threadarray *arr;
    if ((arr = malloc(sizeof(threadarray))) == null) {
        return null;
    }

    if ((arr->as = malloc(m * sizeof(void *))) == null) {
        free(arr);
        return null;
    }

    if ((pthread_cond_init(&arr->notify, null) != 0) ||
            pthread_mutex_init(&arr->lock, null) != 0) {
        free(arr->as);
        free(arr);
        return null;
    }

    *((int8_t *) arr) = 6;
    memset(arr->as, 0, m * sizeof(void *));
    arr->m = m;
    arr->n = 0;

    return arr;
}

threadarray *tharr_copy(threadarray *arr, size_t m) {
    threadarray *new_arr;
    if ((new_arr = malloc(sizeof(threadarray))) == null) {
        return null;
    }

    if ((new_arr->as = malloc(m * sizeof(void *))) == null) {
        free(new_arr);
        return null;
    }
    new_arr->m = m;

    if (pthread_mutex_lock(&arr->lock) == 0) {
        new_arr->n = arr->n < m ? arr->n : m;
        memcpy(new_arr->as, arr->as, new_arr->n * sizeof(void *));
        pthread_mutex_unlock(&arr->lock);
    } else {
        free(new_arr->as);
        free(new_arr);
        return null;
    }

    return new_arr;
}

void tharr_free(threadarray *arr) {
    pthread_mutex_lock(&arr->lock);

    free(arr->as);

    pthread_cond_destroy(&arr->notify);
    pthread_mutex_destroy(&arr->lock);

    free(arr);
}

void *tharr_peek(threadarray *arr) {
    if (pthread_mutex_lock(&arr->lock) == 0) {
        void *a = arr->as[0];

        pthread_mutex_unlock(&arr->lock);
        return a;
    } else {
        return null;
    }
}

void *tharr_pop(threadarray *arr) {
    if (pthread_mutex_lock(&arr->lock) == 0 && arr->n > 0) {
        void *a = arr->as[0];
        memmove(arr->as, &arr->as[1], (arr->n--) * sizeof(void *));

        pthread_mutex_unlock(&arr->lock);
        return a;
    } else {
        return null;
    }
}

bool tharr_push(threadarray *arr, void *a) {
    if (pthread_mutex_lock(&arr->lock) == 0) {
        if (arr->n == arr->m) {
            size_t new_size = arr->m * 2;
            void **new_as;
            if ((new_as = malloc(new_size * sizeof(void *))) == null) {
                pthread_mutex_unlock(&arr->lock);
                return false;
            }
            memcpy(new_as, arr->as, arr->m * sizeof(void *));
            free(arr->as);
            arr->as = new_as;
            arr->m = new_size;
        }

        arr->as[arr->n++] = a;
        pthread_mutex_unlock(&arr->lock);

        return true;
    } else {
        return false;
    }
}

bool tharr_concat(threadarray *dest, threadarray *src) {

    if (dest->n + src->n >= dest->m) {
        size_t new_size = dest->m + src->m;
        void **new_as;
        if ((new_as = malloc(new_size * sizeof(void *))) == null) {
            return false;
        }
        memcpy(new_as, dest->as, dest->n * sizeof(void *));
        free(dest->as);
        dest->as = new_as;
    }

    memcpy(&dest->as[dest->n], src->as, src->n * sizeof(void *));
    dest->n += src->n;
    tharr_free(src);
    return true;
}

void tharr_foreach(threadarray *arr, void *(*func)(void *)) {
    int i;
    for (i = 0; i < arr->n; i++) {
        arr->as[i] = func(arr->as[i]);
    }
}

size_t tharr_reduce(threadarray *arr, bool (*func)(void *)) {
    size_t i, c = 0;
    for (i = 0; i < arr->n && func(arr->as[i]); i++, c++) {
    }

    if (c < arr->n) {
        for (i++; i < arr->n; i++) {
            if (func(arr->as[i])) {
                arr->as[c++] = arr->as[i];
            }
        }

        arr->n = c;
    }

    return arr->n;
}
