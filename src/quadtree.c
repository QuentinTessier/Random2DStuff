#include <SFML/Graphics/Rect.h>
#include <SFML/System/Vector2.h>
#include <stdlib.h>
#include <stdio.h>
#include "quadtree.h"
#include "ray.h"

QuadTree *new_QuadTree(sfVector2f center, float size)
{
    QuadTree *n = calloc(1, sizeof(QuadTree));

    if (n != 0) {
        n->root.bound = Quad_(center, size);
        n->root.depth = 1;
        n->maxdepth = 1;
    }
    return n;
}

QuadTreeNode *new_QuadTreeNode(sfVector2f center, float size, uint32_t depth)
{
    QuadTreeNode *n = calloc(1, sizeof(QuadTreeNode));

    if (n != 0) {
        n->bound = Quad_(center, size);
        n->depth = depth;
        n->point_ref = 0;
    }
    return n;
}

QuadTreeNodeList new_QuadTreeNodeList(size_t n)
{
    QuadTreeNodeList l;

    l.list = calloc(n, sizeof(QuadTreeNode *));
    l.n = n;
    return l;
}

QuadTreeNodeList *QuadTreeNodeList_append(QuadTreeNodeList *self, QuadTreeNode *node)
{
    for (size_t i = 0; i < self->n; ++i) {
        if (self->list[i] == 0) {
            self->list[i] = node;
            return self;
        }
    }
    return self;
}

QuadTreeNodeList *QuadTreeNodeList_clear(QuadTreeNodeList *self)
{
    for (size_t i = 0; i < self->n; ++i)
        self->list[i] = 0;
    return self;
}

void QuadTreeNode_subdivide(QuadTreeNode *t)
{
    sfVector2f centers[4];
    float nSize = t->bound.radius / 2.0f;

    // NORTH WEST
    centers[NW].x = t->bound.center.x + nSize;
    centers[NW].y = t->bound.center.y - nSize;

    // NORTH EAST
    centers[NE].x = t->bound.center.x - nSize;
    centers[NE].y = t->bound.center.y - nSize;

    // SOUTH WEST
    centers[SW].x = t->bound.center.x + nSize;
    centers[SW].y = t->bound.center.y + nSize;

    // SOUTH EAST
    centers[SE].x = t->bound.center.x - nSize;
    centers[SE].y = t->bound.center.y + nSize;

    // Construct
    t->children[NW] = new_QuadTreeNode(centers[NW], nSize, t->depth + 1);
    t->children[NE] = new_QuadTreeNode(centers[NE], nSize, t->depth + 1);
    t->children[SW] = new_QuadTreeNode(centers[SW], nSize, t->depth + 1);
    t->children[SE] = new_QuadTreeNode(centers[SE], nSize, t->depth + 1);
}

sfBool QuadTreeNode_insert(QuadTreeNode *t, sfVector2f point)
{
    if (t == 0)
        return sfFalse;
    if (!Quad_contains(t->bound, point)) {
        return sfFalse;
    }
    if (!t->point_ref) {
        t->point_ref = malloc(sizeof(sfVector2f));
        t->point_ref[0] = point;
        return sfTrue;
    }
    if (t->children[NE] == 0)
        QuadTreeNode_subdivide(t);
    if (QuadTreeNode_insert(t->children[NW], point)) return sfTrue;
    if (QuadTreeNode_insert(t->children[NE], point)) return sfTrue;
    if (QuadTreeNode_insert(t->children[SW], point)) return sfTrue;
    if (QuadTreeNode_insert(t->children[SE], point)) return sfTrue;
    return sfFalse;
}

sfBool QuadTree_insert(QuadTree *t, sfVector2f point)
{
    return QuadTreeNode_insert(&t->root, point);
}

void QuadTreeNode_reset(QuadTreeNode *t, sfVector2f center, float size)
{
    if (t == 0)
        return;
    t->bound.center = center;
    t->bound.radius = size;
    if (t->children[NW] != 0) {
        sfVector2f centers[4];
        float nSize = t->bound.radius / 2.f;

        // NORTH WEST
        centers[NW].x = t->bound.center.x + nSize;
        centers[NW].y = t->bound.center.y - nSize;

        // NORTH EAST
        centers[NE].x = t->bound.center.x - nSize;
        centers[NE].y = t->bound.center.y - nSize;

        // SOUTH WEST
        centers[SW].x = t->bound.center.x + nSize;
        centers[SW].y = t->bound.center.y + nSize;

        // SOUTH EAST
        centers[SE].x = t->bound.center.x - nSize;
        centers[SE].y = t->bound.center.y + nSize;

        QuadTreeNode_reset(t->children[NW], centers[NW], nSize);
        QuadTreeNode_reset(t->children[NE], centers[NW], nSize);
        QuadTreeNode_reset(t->children[SW], centers[NW], nSize);
        QuadTreeNode_reset(t->children[SE], centers[NW], nSize);
    }
}

void QuadTree_reset(QuadTree *t, sfVector2f center, float size)
{
    QuadTreeNode_reset(&t->root, center, size);
}

sfBool QuadTreeNode_intersection(QuadTreeNode *t, const Ray r, float tmin, float tmax, QuadTreeNodeList *qlist)
{
    Hit h;
    if (t != 0 && Quad_intersect2(r, t->bound, tmin, tmax, &h)) {
        QuadTreeNodeList_append(qlist, t);
        QuadTreeNode_intersection(t->children[NE], r, tmin, tmax, qlist);
        QuadTreeNode_intersection(t->children[NW], r, tmin, tmax, qlist);
        QuadTreeNode_intersection(t->children[SE], r, tmin, tmax, qlist);
        QuadTreeNode_intersection(t->children[SW], r, tmin, tmax, qlist);
        return sfTrue;
    }
    return sfFalse;
}

sfBool QuadTree_intersection(QuadTree *t, const Ray r, float tmin, float tmax, QuadTreeNodeList *qlist)
{
    return QuadTreeNode_intersection(&t->root, r, tmin, tmax, qlist);
}