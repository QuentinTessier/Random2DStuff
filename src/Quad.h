#pragma once

#include "Type.h"

typedef struct Quad Quad;

struct Quad {
    Vec2 center;
    float radius;
};

static inline Quad Quad_(Vec2 center, float radius)
{
    return (Quad){center, radius};
}

static inline Vec2 Quad_min(const Quad q)
{
    return Vec2_(q.center.x - q.radius, q.center.y - q.radius);
}

static inline Vec2 Quad_max(const Quad q)
{
    return Vec2_(q.center.x + q.radius, q.center.y + q.radius);
}

static sfBool Quad_contains(const Quad q, Vec2 point)
{
    Vec2 bmin = Vec2_(q.center.x - q.radius, q.center.y - q.radius);
    Vec2 bmax = Vec2_(q.center.x + q.radius, q.center.y + q.radius);

    return (point.x >= bmin.x && point.x <= bmax.x && point.y >= bmin.y && point.y <= bmax.y);
}

static sfBool Quad_overlap(const Quad q, const Quad q1)
{
    Vec2 bmin = Vec2_(q.center.x - q.radius, q.center.y - q.radius);
    Vec2 bmin1 = Vec2_(q1.center.x - q1.radius, q1.center.y - q1.radius);
    float size = q.radius;
    float size1 = q1.radius;

    return (bmin.x < bmin1.x + size1
            && bmin.x + size > bmin1.x
            && bmin.y < bmin1.y + size1
            && bmin.y + size > bmin1.y);
}

/**
 * @brief Without inside Quad case
 */
// static inline sfBool Quad_intersect(const Ray r, const Quad q, float tmin, float tmax, Hit *h)
// {
//     Vec2 bminv = Vec2_(q.center.x - q.radius, q.center.y - q.radius);
//     Vec2 bmaxv = Vec2_(q.center.x + q.radius, q.center.y + q.radius);
//     float dir[2] = Vec2ToFloatArray(r.d);
//     float ori[2] = Vec2ToFloatArray(r.o);
//     float bmin[2] = Vec2ToFloatArray(bminv);
//     float bmax[2] = Vec2ToFloatArray(bmaxv);
//     for (int i = 0; i < 2; ++i) {
//         if (dir[i] != 0) {
//             float t1 = (bmin[i] - ori[i]) / dir[i];
//             float t2 = (bmax[i] - ori[i]) / dir[i];

//             tmin = max(tmin, min(t1, t2));
//             tmax = min(tmax, max(t1, t2));
//         } else if (ori[i] <= bmin[i] || ori[i] >= bmax[i])
//             return sfFalse;
//     }
//     h->t = tmin < 0.0 ? tmax : tmin;
//     return tmax >= tmin && tmin > 0.0;
// }

/**
 * @brief With inside Quad case
 */
// static inline sfBool Quad_intersect2(const Ray r, const Quad q, float tmin, float tmax, Hit *h)
// {
//     Vec2 bminv = Vec2_(q.center.x - q.radius, q.center.y - q.radius);
//     Vec2 bmaxv = Vec2_(q.center.x + q.radius, q.center.y + q.radius);
//     float dir[2] = Vec2ToFloatArray(r.d);
//     float ori[2] = Vec2ToFloatArray(r.o);
//     float bmin[2] = Vec2ToFloatArray(bminv);
//     float bmax[2] = Vec2ToFloatArray(bmaxv);
//     for (int i = 0; i < 2; ++i) {
//         if (dir[i] != 0) {
//             float t1 = (bmin[i] - ori[i]) / dir[i];
//             float t2 = (bmax[i] - ori[i]) / dir[i];

//             tmin = max(tmin, min(t1, t2));
//             tmax = min(tmax, max(t1, t2));
//         } else if (ori[i] <= bmin[i] || ori[i] >= bmax[i])
//             return sfFalse;
//     }
//     h->t = tmin < 0.0 ? tmax : tmin;
//     return tmax >= tmin;
// }