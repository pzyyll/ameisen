//
// Created by czl17 on 2017-12-18.
//

#ifndef SVR_AM_MALLOC_H
#define SVR_AM_MALLOC_H

#include <cstddef>

void *AM_malloc(size_t size);
void AM_free(void *ptr);
void *AM_realloc(void *ptr, size_t size);
void *AM_calloc(size_t nmemb, size_t size);

#endif //SVR_AM_MALLOC_H
