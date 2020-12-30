#include <math.h>
#include <string.h>
#include "IO.h"
#include "Triangle.h"
#include "array/array.h"
#include "Type.h"

Array(Vec2) generate_map(size_t width, size_t height, float threshold)
{
    size_t n = width * height;
    float buffer[n];
    Array(Vec2) mesh = 0;

    memset(buffer, 0, n * sizeof(float));
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            buffer[y * width + x] = Perlin_Get2d(x, y, 0.1f, 4);
        }
    }
    return mesh;
}

static int getPointIndex(int c, int x) {
    if (c < 0) return 0; // In case of center point
    x = x % ((c + 1) * 6); // Make the point index circular
                           // Explanation: index = number of points in previous circles + central point + x
                           // hence: (0+1+2+...+c)*6+x+1 = ((c/2)*(c+1))*6+x+1 = 3*c*(c+1)+x+1
    return (3 * c * (c + 1) + x + 1);
}

Array(Triangle) generate_mesh(size_t n, Vec2 origin, float scale)
{
    Array(Triangle) tris = 0;
    Array(Vec2) vecs = 0;
    Array_push(vecs, origin);
    float d = 1.f / (float)n;

    for (size_t i = 0; i < n; ++i)
    {
        float angleStep = (M_PI * 2.f) / ((i + 1) * 6);
        for (size_t p = 0; p < (i + 1) * 6; ++p) {
            Vec2 point = Vec2_(
                cosf(angleStep * (float)p),
                sinf(angleStep * (float)p) * d * (float)(i + 1)
            );
            point.x *= scale;
            point.y *= scale;
            point.x += origin.x;
            point.y += origin.y;
            Array_push(vecs, point);
        }
    }

    for (size_t i = 0; i < n; ++i) {
        for (size_t p = 0, o = 0; p < (i + 1) * 6; ++p) {
            if (p % (i + 1) != 0) {
                int v10 = getPointIndex((int)i - 1, (int)o + 1);
                int v11 = getPointIndex((int)i - 1, (int)o);
                int v12 = getPointIndex((int)i, (int)p);
                int v20 = getPointIndex((int)i, (int)p);
                int v21 = getPointIndex((int)i, (int)p + 1);
                int v22 = getPointIndex((int)i - 1, (int)o + 1);

                Triangle t1 = Triangle_(vecs[v10], vecs[v11], vecs[v12]);
                Triangle t2 = Triangle_(vecs[v20], vecs[v21], vecs[v22]);
                Array_push(tris, t1);
                Array_push(tris, t2);
                o += 1;
            } else {
                size_t v10 = getPointIndex(i, p);
                size_t v11 = getPointIndex(i, p + 1);
                size_t v12 = getPointIndex(i - 1, o);
                Triangle t1 = Triangle_(vecs[v10], vecs[v11], vecs[v12]);
                Array_push(tris, t1);
            }
        }
    }
    Array_free(vecs);
    return tris;
}

Array(Triangle) generate_grid(size_t width, size_t height, Vec2 origin, float scale)
{
    float xInc = 1.f / (float)width;
    float yInc = 1.f / (float)height;
    Array(Triangle) tris = 0;
    Array(Vec2) points = 0;

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            Vec2 p = Vec2_((float)x * xInc,
                            (float)y * yInc);
            p.x *= scale;
            p.y *= scale;
            p.x += origin.x;
            p.y += origin.y;
            Array_push(points, p);
        }
    }
    for (size_t y = 0; y < height - 1; ++y) {
        for (size_t x = 0; x < width - 1; ++x) {
            int v10 = y * width + x;
            int v11 = (y + 1) * width + x;
            int v12 = y * width + (x + 1);

            Triangle t1 = Triangle_(points[v10], points[v11], points[v12]);

            int v20 = v12;
            int v21 = v11;
            int v22 = (y + 1) * width + (x + 1);

            Triangle t2 = Triangle_(points[v20], points[v21], points[v22]);

            Array_push(tris, t1);
            Array_push(tris, t2);
        }
    }
    Array_free(points);
    return tris;
}