#pragma once

#include "Type.h"

typedef struct Line Line;

struct Line {
    Vec2 a;
    Vec2 b;
};

static inline Line Line_(Vec2 a, Vec2 b)
{
    return (Line){a, b};
}