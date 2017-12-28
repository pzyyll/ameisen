//
// Created by czl17 on 2017-12-26.
//
#include "am_malloc.h"

#include <cstdlib>

#ifdef JEMALLOC
#include "jemalloc/jemalloc.h"
#define malloc je_malloc
#define free je_free
#define realloc je_realloc
#define calloc je_calloc
#endif //JEMALLOC

void *AM_malloc(size_t size) {
    return malloc(size);
}

void AM_free(void *ptr) {
    return free(ptr);
}

void *AM_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

void *AM_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}



