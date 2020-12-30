#pragma once

#include <stdint.h>
#include <SFML/Graphics/Color.h>
#include <SFML/Graphics/Rect.h>
#include <SFML/System/Vector2.h>
#include "ray.h"

typedef struct QuadTreeNode QuadTreeNode;

#define NE  0
#define NW  1
#define SE  2
#define SW  3

#define MAX_DATA_PER_DIVISION 4

struct QuadTreeNode {
    QuadTreeNode *children[4];
    Quad bound;
    uint32_t depth;
    sfVector2f *point_ref;
};

typedef struct QuadTree {
    QuadTreeNode root;
    uint32_t maxdepth;
} QuadTree;

typedef struct QuadTreeNodeList {
    QuadTreeNode **list;
    size_t n;
} QuadTreeNodeList;

QuadTree *new_QuadTree(sfVector2f center, float size);
QuadTreeNodeList new_QuadTreeNodeList(size_t n);
sfBool QuadTree_intersection(QuadTree *t, const Ray r, float tmin, float tmax, QuadTreeNodeList *qlist);
sfBool QuadTree_insert(QuadTree *t, sfVector2f point);
void QuadTree_reset(QuadTree *t, sfVector2f center, float size);