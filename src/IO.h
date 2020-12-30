#pragma once

#include <stdio.h>
#include "Quad.h"
#include "Type.h"
#include "Ray.h"

static inline int pVec2_(const Vec2 v)
{ return printf("(x: %.2f, y: %.2f)", v.x, v.y); }

static inline int sVec2_(const Vec2 v, char *buffer, size_t n)
{ return snprintf(buffer, n, "(%.2f, %.2f)", v.x, v.y); }

static inline int pQuad_(const Quad q)
{
    return printf("{ center: (x: %.2f, y: %.2f), radius: %.2f }\n", q.center.x, q.center.y, q.radius);
}

static inline int sQuad_(const Quad q, char *buffer, size_t n)
{
    return snprintf(buffer, n, "{ center: (x: %.2f, y: %.2f), radius: %.2f }\n", q.center.x, q.center.y, q.radius);
}

static inline int pRay_(const Ray r)
{ return printf("{origin: (%.2f, %.2f), direction: (%.2f, %.2f)}\n", r.o.x, r.o.y, r.d.x, r.d.y); }

static inline int sRay_(const Ray r, char *buffer, size_t n)
{ return snprintf(buffer, n, "{origin: (%.2f, %.2f), direction: (%.2f, %.2f)}\n", r.o.x, r.o.y, r.d.x, r.d.y); }