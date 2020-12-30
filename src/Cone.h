#pragma once

#include "Type.h"

typedef struct Cone Cone;

struct Cone {
    Vec2 origin;
    Vec2 end;
    float angle;
};

static inline Cone Cone_(Vec2 origin, Vec2 end, float angle)
{
    return (Cone){origin, end, angle};
}

static inline Cone Cone_rotate(const Cone c, float degree)
{
    return (Cone){c.origin, Vec2_rotate_around(c.end, c.origin, radian(degree)), c.angle};
}