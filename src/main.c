
#include <SFML/Graphics/CircleShape.h>
#include <SFML/Graphics/Color.h>
#include <SFML/Window/Keyboard.h>
#include <SFML/Window/Mouse.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Graphics/VertexArray.h>
#include <SFML/System/Clock.h>
#include <SFML/System/Time.h>
#include <SFML/Window/Window.h>
#include <SFML/Window/Event.h>
#include "Cone.h"
#include "Intersection.h"
#include "IO.h"
#include "Ray.h"
#include "Triangle.h"
#include "Type.h"
#include "array/array.h"

void draw_ray(const Ray r, sfRenderWindow *win, sfColor color)
{
    sfVertex points[2];

    points[0].position = r.o;
    points[0].color = color;
    points[0].texCoords = (sfVector2f){0, 0};

    points[1].position = At_parameter(r, 500);
    points[1].color = color;
    points[1].texCoords = (sfVector2f){0, 0};

    sfRenderWindow_drawPrimitives(win, points, 2, sfLines, 0);
}

void draw_Line(const Line l, sfRenderWindow *win, Color c)
{
    sfVertex v[2];

    v[0].position = l.a;
    v[0].color = c;

    v[1].position = l.b;
    v[1].color = c;

    sfRenderWindow_drawPrimitives(win, v, 2, sfLines, 0);
}

void draw_Triangle(const Triangle t, sfRenderWindow *win, Color c)
{
    sfVertex v[3];

    v[0].position = t.a;
    v[0].color = c;

    v[1].position = t.b;
    v[1].color = c;

    v[2].position = t.c;
    v[2].color = c;

    sfRenderWindow_drawPrimitives(win, v, 3, sfTriangles, 0);
}

void draw_TriangleWire(const Triangle t, sfRenderWindow *win, Color c)
{
    const Line l1 = Line_(t.a, t.b);
    const Line l2 = Line_(t.b, t.c);
    const Line l3 = Line_(t.c, t.a);

    draw_Line(l1, win, c);
    draw_Line(l2, win, c);
    draw_Line(l3, win, c);
}

void draw_Cone(const Cone c, sfRenderWindow *win, Color co, int res)
{
    float r = (c.angle / 2.f) / (float)res;
    Vec2 end1 = c.end;
    Vec2 end2 = c.end;

    for (int i = 0; i < res; ++i) {
        Vec2 nend1 = Vec2_rotate_around(end1, c.origin, radian(r));
        Vec2 nend2 = Vec2_rotate_around(end2, c.origin, -radian(r));
        Triangle t1 = Triangle_(c.origin, end1, nend1);
        Triangle t2 = Triangle_(c.origin, end2, nend2);
        end1 = nend1;
        end2 = nend2;
        draw_TriangleWire(t1, win, co);
        draw_TriangleWire(t2, win, co);
    }
}

void draw_Quad(const Quad q, sfRenderWindow *win, sfColor c)
{
    sfVertex points[4] = {0};

    points[0].color = c;
    points[1].color = c;
    points[2].color = c;
    points[3].color = c;

    points[0].position = (sfVector2f){q.center.x - q.radius, q.center.y - q.radius};
    points[1].position = (sfVector2f){q.center.x - q.radius, q.center.y + q.radius};
    points[2].position = (sfVector2f){q.center.x + q.radius, q.center.y + q.radius};
    points[3].position = (sfVector2f){q.center.x + q.radius, q.center.y - q.radius};

    sfRenderWindow_drawPrimitives(win, points, 4, sfQuads, 0);
}

void append_Quad(sfVertexArray *a, const Quad q, Color c)
{
    sfVertex points[4] = {0};

    points[0].color = c;
    points[1].color = c;
    points[2].color = c;
    points[3].color = c;

    points[0].position = (sfVector2f){q.center.x - q.radius, q.center.y - q.radius};
    points[1].position = (sfVector2f){q.center.x - q.radius, q.center.y + q.radius};
    points[2].position = (sfVector2f){q.center.x + q.radius, q.center.y + q.radius};
    points[3].position = (sfVector2f){q.center.x + q.radius, q.center.y - q.radius};

    sfVertexArray_append(a, points[0]);
    sfVertexArray_append(a, points[1]);
    sfVertexArray_append(a, points[2]);
    sfVertexArray_append(a, points[3]);
}

typedef struct CQuad {
    Quad q;
    Color c;
} CQuad;

sfBool compute_hit_list(const Ray r, const CQuad *quads, size_t n, float tmin, float tmax, Record *h)
{
    Record th;
    sfBool hit = sfFalse;
    float closest_so_far = tmax;

    for (size_t i = 0; i < n; ++i) {
        if (Intersect_Quad(r, quads[i].q, tmin, closest_so_far, &th)) {
            closest_so_far = th.t;
            th.c = quads[i].c;
            hit = sfTrue;
            *h = th;
        }
    }
    return hit;
}

void fade_colors(Array(CQuad) *quads)
{
    float fade = 1;

    Array_foreach(*quads, ite, {
        ite->c.a -= fade;
    });
    if ((*quads)[0].c.a <= 16)
        Array_erase_shift(*quads, 0);
}

// int main()
// {
//     srand(getpid());
//     sfRenderWindow *win = sfRenderWindow_create((sfVideoMode){1280, 769, 32}, "2DPathtracer", sfClose | sfFullscreen, 0);
//     sfClock *clock = sfClock_create();
//     sfCircleShape *circle = sfCircleShape_create();
//     sfCircleShape_setFillColor(circle, sfTransparent);
//     sfCircleShape_setRadius(circle, 500);
//     sfCircleShape_setOutlineColor(circle, Color_(150, 150, 150));
//     sfCircleShape_setOutlineThickness(circle, 20.f);
//     sfCircleShape_setPosition(circle, Vec2_(960, 540));
//     sfCircleShape_setOrigin(circle, Vec2_(500, 500));
//     sfCircleShape_setPointCount(circle, 60);

//     size_t n = 20;
//     CQuad *qlist = calloc(n, sizeof(CQuad));
//     for (size_t i = 0; i < n; ++i) {
//         qlist[i].q = Quad_(
//             Vec2_(randf(10.f, 1910.f), randf(10.f, 1070.f)),
//             randf(5.f, 200.f)
//         );
//         qlist[i].c = Color_(
//             (int)(randf(0.001f, 0.999f) * 255),
//             (int)(randf(0.001f, 0.999f) * 255),
//             (int)(randf(0.001f, 0.999f) * 255));
//     }

//     float angle = 0.1f;
//     Ray base = Ray_(Vec2_(960, 540), Vec2_(0, -1));
//     Ray r = base;

//     Array(CQuad) drawable = 0;

//     Record record;
//     record.c = sfBlue;

//     while (sfRenderWindow_isOpen(win)) {
//         sfEvent e;
//         while (sfRenderWindow_pollEvent(win, &e)) {
//             if (e.type == sfEvtClosed || (e.type == sfEvtKeyPressed && e.key.code == sfKeyEscape))
//                 sfRenderWindow_close(win);
//             else if (e.type == sfEvtKeyReleased && e.key.code == sfKeySpace) {
//                 for (size_t i = 0; i < n; ++i) {
//                     qlist[i].q = Quad_(
//                         Vec2_(randf(10.f, 1910.f), randf(10.f, 1070.f)),
//                         randf(5.f, 200.f)
//                     );
//                     qlist[i].c = Color_(
//                         (int)(randf(0.001f, 0.999f) * 255),
//                         (int)(randf(0.001f, 0.999f) * 255),
//                         (int)(randf(0.001f, 0.999f) * 255));
//                 }
//                 Array_clear(drawable);
//                 r = base;
//             }
//         }
//         sfRenderWindow_clear(win, sfBlack);

//         if (sfTime_asMilliseconds(sfClock_getElapsedTime(clock)) > 1) {
//             r.d = Vec2_rotate(r.d, radian(angle));

//             if (compute_hit_list(r, qlist, n, 0.001, 500, &record)) {
//                 Quad h;
//                 h.center = At_parameter(r, record.t);
//                 h.radius = lerp(3.0f, 1.0f, record.t / 1920.f);
//                 if (approximate(record.t, 0.f, 0.01f))
//                     h.radius = 5.0f;
//                 CQuad d;
//                 d.q = h;
//                 d.c = record.c;
//                 Array_push(drawable, d);
//             }
//             fade_colors(&drawable);
//             sfClock_restart(clock);
//         }
//         draw_ray(r, win, Color_(128, 128, 128));
//         Array_foreach(drawable, ite, {
//             draw_Quad(ite->q, win, ite->c);
//         });

//         sfRenderWindow_drawCircleShape(win, circle, 0);

//         sfRenderWindow_display(win);
//     }
//     sfRenderWindow_destroy(win);
//     Array_free(drawable);
//     free(qlist);
//     return 0;
// }

Array(Vec2) generate_points(size_t n, Quad bound)
{
    Vec2 min = Quad_min(bound);
    Vec2 max = Quad_max(bound);

    Array(Vec2) v = 0;

    for (size_t i = 0; i < n; ++i) {
        Vec2 p = Vec2_(randf(min.x, max.x), randf(min.y, max.y));
        Array_push(v, p);
    }
    return v;
}

void cull(Array(Triangle) *culled, Array(Triangle) mesh, Cone c, int cone_res)
{
    Array_foreach(mesh, ite, {
        if (Triangle_in_Cone(c, *ite, cone_res)) {
            Array_push(*culled, *ite);
        }
    })
}

int main()
{
    srand(getpid());
    int sphere_res = 1;
    sfRenderWindow *win = sfRenderWindow_create((sfVideoMode){1280, 769, 32}, "2DPathtracer", sfClose | sfFullscreen, 0);
    Cone c = Cone_(Vec2_(960, 540), Vec2_(960, 0), 45);
    Array(Triangle) culled = 0;
    Array(Triangle) mesh = generate_mesh(sphere_res, Vec2_(1440, 540), 120);
    Array(Triangle) mesh1 = generate_grid(10, 10, Vec2_(480.f - 120.f, 540.f - 120.f), 240);
    float r = 0.1f;
    int cone_res = 2;
    float distance = 540;

    while (sfRenderWindow_isOpen(win)) {
        sfEvent e;
        while (sfRenderWindow_pollEvent(win, &e)) {
            if (e.type == sfEvtClosed || (e.type == sfEvtKeyPressed && e.key.code == sfKeyEscape))
                sfRenderWindow_close(win);
            else if (e.type == sfEvtMouseMoved)
                c.end = Vec2_(e.mouseMove.x, e.mouseMove.y);
            else if (e.type == sfEvtKeyPressed) {
                if (e.key.code == sfKeyUp) {
                    Array_free(mesh);
                    sphere_res += 1;
                    mesh = generate_mesh(sphere_res, Vec2_(1440, 540), 120);
                } else if (e.key.code == sfKeyDown) {
                    Array_free(mesh);
                    sphere_res -= 1;
                    mesh = generate_mesh(sphere_res, Vec2_(1440, 540), 120);
                } else if (e.key.code == sfKeyRight) cone_res += 1;
                else if (e.key.code == sfKeyLeft) cone_res -= 1;
            }
        }
        sfRenderWindow_clear(win, sfBlack);
        draw_Cone(c, win, sfRed, cone_res);
        Array_foreach(mesh, ite, {
            draw_TriangleWire(*ite, win, sfBlue);
        });
        Array_foreach(mesh1, ite, {
            draw_TriangleWire(*ite, win, sfBlue);
        });
        Array_clear(culled);
        cull(&culled, mesh, c, cone_res);
        cull(&culled, mesh1, c, cone_res);
        Array_foreach(culled, ite, {
            draw_TriangleWire(*ite, win, sfGreen);
        })
        sfRenderWindow_display(win);
    }
    sfRenderWindow_destroy(win);
    Array_free(culled);
    Array_free(mesh);
    return 0;
}