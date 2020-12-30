#pragma once

#include "Type.h"
#include "Quad.h"
#include "Ray.h"
#include "Line.h"
#include "Triangle.h"
#include "Cone.h"

typedef struct Record {
    float t;
    Color c;
} Record;

static inline Bool Intersect_Quad(const Ray r, const Quad q, float tmin, float tmax, Record *h)
{
    Vec2 bminv = Vec2_(q.center.x - q.radius, q.center.y - q.radius);
    Vec2 bmaxv = Vec2_(q.center.x + q.radius, q.center.y + q.radius);
    float dir[2] = Vec2ToFloatArray(r.d);
    float ori[2] = Vec2ToFloatArray(r.o);
    float bmin[2] = Vec2ToFloatArray(bminv);
    float bmax[2] = Vec2ToFloatArray(bmaxv);
    for (int i = 0; i < 2; ++i) {
        if (dir[i] != 0) {
            float t1 = (bmin[i] - ori[i]) / dir[i];
            float t2 = (bmax[i] - ori[i]) / dir[i];

            tmin = max(tmin, min(t1, t2));
            tmax = min(tmax, max(t1, t2));
        } else if (ori[i] <= bmin[i] || ori[i] >= bmax[i])
            return sfFalse;
    }
    h->t = tmin < 0.0 ? tmax : tmin;
    return tmax >= tmin && tmin > 0.0;
}

static inline Bool Intersect_Quad2(const Ray r, const Quad q, float tmin, float tmax, Record *h)
{
    Vec2 bminv = Vec2_(q.center.x - q.radius, q.center.y - q.radius);
    Vec2 bmaxv = Vec2_(q.center.x + q.radius, q.center.y + q.radius);
    float dir[2] = Vec2ToFloatArray(r.d);
    float ori[2] = Vec2ToFloatArray(r.o);
    float bmin[2] = Vec2ToFloatArray(bminv);
    float bmax[2] = Vec2ToFloatArray(bmaxv);
    for (int i = 0; i < 2; ++i) {
        if (dir[i] != 0) {
            float t1 = (bmin[i] - ori[i]) / dir[i];
            float t2 = (bmax[i] - ori[i]) / dir[i];

            tmin = max(tmin, min(t1, t2));
            tmax = min(tmax, max(t1, t2));
        } else if (ori[i] <= bmin[i] || ori[i] >= bmax[i])
            return sfFalse;
    }
    h->t = tmin < 0.0 ? tmax : tmin;
    return tmax >= tmin;
}

static inline Bool Intersect_Segment(const Ray r, const Line l, float tmin, float tmax, Record *h)
{
    Vec2 s1 = Vec2_(r.o.x - l.a.x, r.o.y - l.a.y);
    Vec2 s2 = Vec2_(l.b.x - l.a.x, l.b.y - l.a.y);
    Vec2 s3 = Vec2_(-r.d.y, r.d.x);

    float d = dot(s2, s3);
    if (abs(d) < 0.000001f)
        return False;
    float t1 = length(cross(VVec3_(s2), VVec3_(s1))) / d;
    float t2 = dot(s1, s3) / d;

    h->t = t1;
    return t1 >= 0.0f && t1 < tmax && (t2 >= 0.0 && t2 <= 1.0);
}

static inline Bool Intersect_Triangle(const Ray r, const Triangle t, float tmin, float tmax, Record *h)
{
    Line lines[3] = {
        Line_(t.a, t.b),
        Line_(t.b, t.c),
        Line_(t.c, t.a)
    };

    float closest_so_far = tmax;
    Record th;
    Bool hit = False;
    for (int i = 0; i < 3; ++i) {
        if (Intersect_Segment(r, lines[i], tmin, closest_so_far, &th)) {
            closest_so_far = th.t;
            *h = th;
            hit = True;
        }
    }
    return hit;
}

static float sign (Vec2 p1, Vec2 p2, Vec2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

static inline Bool Point_in_Triangle(const Triangle t, Vec2 p)
{
    Bool d1, d2, d3;
    Bool has_neg, has_pos;

    d1 = sign(p, t.a, t.b) < 0.0f;
    d2 = sign(p, t.b, t.c) < 0.0f;
    d3 = sign(p, t.c, t.a) < 0.0f;

    return (d1 == d2) && (d2 == d3);
}

static inline Bool Point_in_cone(const Cone c, Vec2 p)
{
    Triangle t1 = Triangle_(c.origin, c.end, Vec2_rotate_around(c.end, c.origin, radian(c.angle / 2.f)));
    Triangle t2 = Triangle_(c.origin, c.end, Vec2_rotate_around(c.end, c.origin, -radian(c.angle / 2.f)));

    return (Point_in_Triangle(t1, p) || Point_in_Triangle(t2, p));
}

int tri_tri_overlap_test_2d(float p1[2], float q1[2], float r1[2],
          float p2[2], float q2[2], float r2[2]);

static inline Bool Triangle_Triangle(const Triangle a, const Triangle b)
{
    float a1[2] = Vec2ToFloatArray(a.a);
    float a2[2] = Vec2ToFloatArray(a.b);
    float a3[2] = Vec2ToFloatArray(a.c);
    float b1[2] = Vec2ToFloatArray(b.a);
    float b2[2] = Vec2ToFloatArray(b.b);
    float b3[2] = Vec2ToFloatArray(b.c);
    return tri_tri_overlap_test_2d(a1, a2, a3, b1, b2, b3);
}

static inline Bool Triangle_in_cone(const Cone c, const Triangle t)
{
    Triangle t1 = Triangle_(c.origin, c.end, Vec2_rotate_around(c.end, c.origin, radian(c.angle / 2.f)));
    Triangle t2 = Triangle_(c.origin, c.end, Vec2_rotate_around(c.end, c.origin, -radian(c.angle / 2.f)));

    return Triangle_Triangle(t1, t) || Triangle_Triangle(t2, t);
}

static inline Bool Triangle_in_Cone(const Cone c, const Triangle t, size_t res)
{
    float delta = (c.angle / 2.f) / (float)res;
    Vec2 end1 = c.end;
    Vec2 end2 = c.end;

    for (size_t i = 0; i < res; ++i) {
        Vec2 nend1 = Vec2_rotate_around(end1, c.origin, radian(delta));
        Vec2 nend2 = Vec2_rotate_around(end2, c.origin, -radian(delta));
        Triangle t1 = Triangle_(c.origin, end1, nend1);
        Triangle t2 = Triangle_(c.origin, end2, nend2);
        end1 = nend1;
        end2 = nend2;
        if (Triangle_Triangle(t1, t) || Triangle_Triangle(t2, t))
            return True;
    }
    return False;
}