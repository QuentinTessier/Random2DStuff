#pragma once

#include "Type.h"

typedef struct Ray Ray;

struct Ray {
    Vec2 o;
    Vec2 d;
};

static inline Ray Ray_(Vec2 origin, Vec2 direction)
{ return (Ray){origin, direction}; }

static inline Vec2 At_parameter(const Ray r, float t)
{ return Vec2_(r.o.x + t * r.d.x, r.o.y + t * r.d.y); }