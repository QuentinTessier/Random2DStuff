#pragma once

#include <stdlib.h>
#include <math.h>
#include <SFML/System/Vector2.h>
#include <SFML/System/Vector3.h>
#include <SFML/Graphics/Color.h>

float Perlin_Get2d(float x, float y, float freq, int depth);

static const float randf(float min, float max)
{
    float scale = rand() / (float)RAND_MAX;
    return min + scale * (max - min);
}

static inline int approximate(float value, float target, float range)
{
    return (value < target - range) ? 0 : (value > target + range) ? 0 : 1;
}

static inline float min(float a, float b)
{ return a < b ? a : b; }

static inline float max(float a, float b)
{ return a > b ? a : b; }

static inline float clamp(float min, float max, float x)
{
    return (x < min) ? min : (x > max) ? max : x;
}

static inline float lerp(float min, float max, float t)
{
    return min + t * (max - min);
}

static inline float degree(float radian)
{ return radian * (180.f / M_PI); }

static inline float radian(float degree)
{ return degree * (M_PI / 180.f); }

typedef sfVector2f Vec2;

static inline Vec2 Vec2_(float x, float y)
{ return (Vec2){x, y}; }

#define Vec2ToFloatArray(v) {v.x, v.y}

static inline Vec2 Vec2_rotate(const Vec2 v, float radian)
{ return Vec2_(v.x * cosf(radian) - v.y * sinf(radian), v.x * sinf(radian) + v.y * cosf(radian)); }

static inline Vec2 Vec2_rotate_around(const Vec2 v, const Vec2 center, float radian)
{
    Vec2 r = Vec2_(v.x - center.x, v.y - center.y);
    r = Vec2_rotate(r, radian);
    return Vec2_(r.x + center.x, r.y + center.y);
}

static inline float Vec2_angle(const Vec2 a, const Vec2 b)
{
    return acosf((a.x * b.x + a.y * b.y) / (sqrtf(a.x * a.x + a.y * a.y) * sqrtf(b.x * b.x + b.y * b.y)));
}

static inline float dot(const Vec2 a, const Vec2 b)
{ return a.x * b.x + a.y * b.y; }

static inline float distance(const Vec2 a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}

static inline Vec2 vmin(Vec2 a, Vec2 b)
{ return Vec2_(min(a.x, b.x), min(a.y, b.y)); }

static inline Vec2 vmax(Vec2 a, Vec2 b)
{ return Vec2_(max(a.x, b.x), max(a.y, b.y)); }

typedef sfColor Color;

typedef unsigned char u8;

static inline Color Color_(u8 r, u8 g, u8 b)
{
    return (Color){r, g, b, 255};
}

typedef sfBool Bool;

#define True sfTrue
#define False sfFalse

typedef sfVector3f Vec3;

static inline Vec3 Vec3_(float x, float y, float z)
{ return (Vec3){x, y, z}; }

static inline Vec3 VVec3_(Vec2 v)
{
    return (Vec3){v.x, v.y, 0};
}

static inline Vec3 cross(const Vec3 a, const Vec3 b)
{
    return Vec3_(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
    );
}

static inline float length(const Vec3 a)
{
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}