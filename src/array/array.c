#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "array.h"

void *__array_grow(void **array, uint32_t element_size, uint32_t required_elements)
{
    uint32_t new_cap = (required_elements > __ARRAY_START_CAP ?
                    required_elements : __ARRAY_START_CAP) - 1;
    new_cap |= new_cap >> 1;
    new_cap |= new_cap >> 2;
    new_cap |= new_cap >> 4;
    new_cap |= new_cap >> 8;
    new_cap |= new_cap >> 16;
    new_cap += 1;

    *array = (void *)((uint32_t *)realloc(__ARRAY_RAW(*array),
        new_cap * element_size + 2 * sizeof(uint32_t)) + 2);
    if (*array == 0)
        return 0;
    *(__ARRAY_RAW(*array) + __ARRAY_CAP) = new_cap;
    return *array;
}

int __array_insert(void **array, void *element, uint32_t element_size,
    uint32_t pos)
{
    int array_null = !*array;

    if (ArrayCapacity(*array) < ArraySize(*array) + 1) {
        if (__array_grow(array, element_size, ArraySize(*array) + 1) == 0)
            return 0;
    }
    if (array_null)
        __ARRAY_RAW(*array)[0] = 0;
    if (ArraySize(*array) - pos > 0) {
        memmove(((char *)(*array) + (element_size * (pos + 1))),
                ((char *)(*array) + (element_size * pos)),
                element_size * (ArraySize(*array) - pos));
    }
    memcpy(((char *)(*array)) + (element_size * pos), element, element_size);
    __ARRAY_RAW(*array)[__ARRAY_SIZE] += 1;
    return 1;
}

int __array_concat(void **dest, void **src, uint32_t element_size)
{
    if (__array_grow(dest, element_size,
        ArraySize(*dest) + ArraySize(*src)) == 0)
        return 0;
    memcpy((char *)(*dest) + element_size * ArraySize(*dest), *src,
            element_size * ArraySize(*src));
    __ARRAY_RAW(*dest)[__ARRAY_SIZE] += ArraySize(*src);
    return 1;
}

int __array_erase_shift(void **array, uint32_t element_size, uint32_t pos)
{
    if (ArraySize(*array) == 0)
        return 0;
    if (ArraySize(*array) > 1) {
        memmove(((char *)(*array) + (element_size * pos)),
                ((char *)(*array) + (element_size * (pos + 1))),
                element_size * (ArraySize(*array) - pos - 1));
    }
    __ARRAY_RAW(*array)[__ARRAY_SIZE] -= 1;
    return 1;
}

int __array_erase_swap(void **array, uint32_t element_size, uint32_t pos)
{
    if (ArraySize(*array) == 0)
        return 0;
    if (ArraySize(*array) > 1) {
        memcpy(((char *)(*array) + (element_size * pos)),
                ((char *)(*array) + (element_size * (ArraySize(*array) - 1))),
                element_size);
    }
    __ARRAY_RAW(*array)[__ARRAY_SIZE] -= 1;
    return 1;
}