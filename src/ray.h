#pragma once

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <SFML/System/Vector2.h>
#include <SFML/Graphics/Color.h>

static const int randi(int min, int max)
{
    return min + rand() * (max - min);
}

static inline float min(float a, float b)
{ return a < b ? a : b; }

static inline float max(float a, float b)
{ return a > b ? a : b; }

typedef struct Hit {
    float t;
    sfColor color;
} Hit;

typedef struct Quad {
    sfVector2f center;
    float radius;
} Quad;

typedef struct Quadc {
    Quad q;
    sfColor c;
} Quadc;

static inline Quad Quad_(sfVector2f center, float radius)
{
    return (Quad){center, radius};
}


static void pQuad(const Quad q)
{
    sfVector2f bmin = (sfVector2f){q.center.x - q.radius, q.center.y - q.radius};
    sfVector2f bmax = (sfVector2f){q.center.x + q.radius, q.center.y + q.radius};
    printf("{(%.2f, %.2f), %.2f} :: {(%.2f, %.2f), (%.2f, %.2f)}\n", q.center.x, q.center.y, q.radius, bmin.x, bmin.y, bmax.x, bmax.y);
}

static sfBool Quad_contains(const Quad q, sfVector2f point)
{
    sfVector2f bmin = (sfVector2f){q.center.x - q.radius, q.center.y - q.radius};
    sfVector2f bmax = (sfVector2f){q.center.x + q.radius, q.center.y + q.radius};

    return (point.x >= bmin.x && point.x <= bmax.x && point.y >= bmin.y && point.y <= bmax.y);
}

static sfBool Quad_overlap(const Quad q, const Quad q1)
{
    sfVector2f bmin = (sfVector2f){q.center.x - q.radius, q.center.y - q.radius};
    sfVector2f bmin1 = (sfVector2f){q1.center.x - q1.radius, q1.center.y - q1.radius};
    float size = q.radius;
    float size1 = q1.radius;

    return (bmin.x < bmin1.x + size1
            && bmin.x + size > bmin1.x
            && bmin.y < bmin1.y + size1
            && bmin.y + size > bmin1.y);
}

typedef struct Ray {
    sfVector2f o;
    sfVector2f d;
} Ray;

static inline Ray Ray_(sfVector2f o, sfVector2f d)
{ return (Ray){o, d}; }

static inline sfVector2f Ray_at_parameter(const Ray r, float t)
{
    return (sfVector2f){ r.o.x + t * r.d.x, r.o.y + t * r.d.y };
}


static inline float dot(sfVector2f a, sfVector2f b)
{
    return a.x * b.x + a.y * b.y;
}

static inline sfBool Circle_intersect(const Ray r, const Quad q, float tmin, float tmax, Hit *h)
{
    float t0, t1;
    sfVector2f co = { q.center.x - r.o.x, q.center.y - r.o.y };
    float tca = dot(co, r.d);
    if (tca < 0.f) return sfFalse;
    float d2 = dot(co, co) - tca * tca;
    if (d2 > q.radius * q.radius) return sfFalse;
    float thc = sqrtf(q.radius * q.radius - d2);
    t0 = tca - thc;
    t1 = thc - tca;
    if (t0 > t1) {
        float tmp = t0;
        t0 = t1;
        t1 = tmp;
    }
    if (t0 < 0 && t1 < 0) return sfFalse;
    h->t = (t0 < 0) ? t1 : t0;
    return sfTrue;
}