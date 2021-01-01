#pragma once

#include <stdlib.h>
#include <math.h>
#include <SFML/System/Vector2.h>
#include <SFML/System/Vector3.h>
#include <SFML/Graphics/Color.h>
#include <SFML/Graphics/Transform.h>

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
    return (1 - t) * min + t * max;
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

static inline void RGBtoHSV(Color c, float HSV[3])
{
    float r = c.r / 255.0f;
    float g = c.g / 255.0f;
    float b = c.b / 255.0f;

    float h, s, v; // h:0-360.0, s:0.0-1.0, v:0.0-1.0

    float fmax = max(r, max(g, b));
    float fmin = min(r, min(g, b));

    v = fmax;

    if (fmax == 0.0f) {
        s = 0;
        h = 0;
    }
    else if (fmax - fmin == 0.0f) {
        s = 0;
        h = 0;
    }
    else {
        s = (fmax - fmin) / fmax;

        if (fmax == r) {
            h = 60 * ((g - b) / (fmax - fmin)) + 0;
        }
        else if (fmax == g) {
            h = 60 * ((b - r) / (fmax - fmin)) + 120;
        }
        else {
            h = 60 * ((r - g) / (fmax - fmin)) + 240;
        }
    }

    if (h < 0) h += 360.0f;

    HSV[0] = h;
    HSV[1] = s;
    HSV[2] = v;
}

static inline Color HSVtoRGB(float H, float S, float V)
{
    if(H > 360 || H < 0 || S > 100 || S < 0 || V > 100 || V < 0)
        return Color_(255, 255, 255);

    float s = S / 100;
    float v = V / 100;
    float C = s * v;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float m = v - C;
    float r, g, b;

    if(H >= 0 && H < 60)
        r = C, g = X, b = 0;
    else if(H >= 60 && H < 120)
        r = X, g = C, b = 0;
    else if(H >= 120 && H < 180)
        r = 0, g = C, b = X;
    else if(H >= 180 && H < 240)
        r = 0, g = X, b = C;
    else if(H >= 240 && H < 300)
        r = X, g = 0, b = C;
    else
        r = C, g = 0, b = X;
    return Color_((r + m) * 255, (g + m) * 255,(b + m) * 255);
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

typedef sfTransform Mat3x3;