#pragma once

#include <stdlib.h>
#include <stdint.h>

#define Array(type)         type *

#define __ARRAY_START_CAP   32

#define __ARRAY_SIZE        0
#define __ARRAY_CAP         1
#define __ARRAY_RAW(a)      ((a) ? ((uint32_t *)(a)) - 2 : 0)

#define ArraySize(a)        ((a) ? __ARRAY_RAW(a)[__ARRAY_SIZE] : 0)
#define ArrayCapacity(a)    ((a) ? __ARRAY_RAW(a)[__ARRAY_CAP] : 0)

void *__array_grow(void **array, uint32_t element_size, uint32_t required_elements);
int __array_insert(void **array, void *element, uint32_t element_size, uint32_t pos);
int __array_concat(void **dst, void **src, uint32_t element_size);
int __array_erase_swap(void **array, uint32_t element_size, uint32_t pos);
int __array_erase_shift(void **array, uint32_t element_size, uint32_t pos);

#define Array_push(a, e)                                                            \
    {                                                                               \
        typeof(e) __anon = e;                                                       \
        __array_insert((void **)&(a), (void *)&(__anon), sizeof(e), ArraySize(a));  \
    }

#define Array_insert(a, e, i)                                                       \
    typeof(e) __anon = e;                                                           \
    __array_insert((void **)&(a), (void *)&(__anon), sizeof(e), i)

#define Array_pop(a)                                                                \
    ((__ARRAY_RAW(a)) ? __ARRAY_RAW(a)[__ARRAY_SIZE] -= 1, 1 : 0)

#define Array_erase_swap(a, i)                                                      \
    ((__ARRAY_RAW(a)) ? __array_erase_swap((void **)&(a), sizeof((a)[0]), i) : 0)

#define Array_erase_shift(a, i)                                                     \
    ((__ARRAY_RAW(a)) ? __array_erase_shift((void **)&(a), sizeof((a)[0]), i) : 0)

#define Array_clear(a)                                                              \
    ((__ARRAY_RAW(a)) ? __ARRAY_RAW(a)[__ARRAY_SIZE] = 0, 1 : 0)

#define Array_free(a)                                                               \
    ((__ARRAY_RAW(a)) ? free(__ARRAY_RAW(a)), a = 0, 1 : 0)

#define Array_foreach(a, ite, code)                                                 \
    for (uint32_t const_ite = 0; const_ite < ArraySize(a); ++const_ite) {           \
        typeof(a) ite = &(a)[const_ite];                                            \
        code                                                                        \
    }
