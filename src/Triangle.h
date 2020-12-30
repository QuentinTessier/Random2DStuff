#pragma once

#include "Type.h"
#include "array/array.h"

typedef struct Triangle Triangle;

struct Triangle {
    Vec2 a;
    Vec2 b;
    Vec2 c;
};

static inline Triangle Triangle_(Vec2 a, Vec2 b, Vec2 c)
{
    return (Triangle){a, b, c};
}

Array(Triangle) generate_mesh(size_t n, Vec2 origin, float scale);
Array(Triangle) generate_grid(size_t width, size_t height, Vec2 origin, float scale);