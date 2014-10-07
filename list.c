#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include "list.h"

typedef struct node node;
struct node {
    void *a;
    node *n;
};

struct list {
    const int8_t t;
    pthread_cond_t notify;
    pthread_mutex_t lock;
    size_t n;
    node *head;
    node *tail;
};

list *list_init() {
    list *l;
    if ((l = malloc(sizeof(list))) == null) {
        return null;
    }

    *((int8_t*)l) = 5;

    if (pthread_cond_init(&l->notify, null) == 0 ||
            pthread_mutex_init(&l->lock, null) == 0) {
        free(l);
        return null;
    }

    l->n = 0;
    l->head = l->tail = null;

    return l;
}

list *list_copy(list *l, size_t m) {
    return null;
}

void list_free(list *l) {
    pthread_mutex_lock(&l->lock);

    while (l->head != null) {
        node *tmp = l->head;
        l->head = l->head->n;
        free(tmp);
    }

    pthread_cond_destroy(&l->notify);
    pthread_mutex_destroy(&l->lock);

    free(l);
}

void list_reset(list *l) {

}

bool list_push(list *l, void *a) {
    return false;
}

bool list_concat(list *dest, list *src) {
    return false;
}

void *list_peek(list *l) {
    return null;
}

void *list_pop(list *l) {
    return null;
}

void list_foreach(list *l, void *(*func)(void *)) {

}

size_t list_reduce(list *l, bool (*func)(void *)) {
    return 0;
}
