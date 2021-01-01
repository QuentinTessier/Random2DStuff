#pragma once

#include <stddef.h>
#include "../Type.h"

/*

        A---D    1---2
        |   | => |   |
        B---C    4---8

*/

enum SquareCorner {
    CornerA = 1,
    CornerB = 4,
    CornerC = 8,
    CornerD = 2
};

struct MarchingSquareTriangleCases {
    size_t count;
    Vec2 vertices[12];
} MarchingSquareTriangleCasesTable[] = {
    {0, {}},                                        // No Corner

    // Base case
    {1, {{0, 0}, {0, 0.5}, {0.5, 0}}},              // Corner A => 1
    {1, {{0.5, 0}, {1, 0.5}, {1, 0}}},              // Corner D => 2
    {2, {                                           // Corners D && A => 3
        {1, 0.5}, {1, 0}, {0, 0},
        {1, 0.5}, {0, 0}, {0, 0.5},
    }},
    {1, {{0, 0.5}, {0, 1}, {0.5, 1}}},              // Corner B => 4
    {2, {                                           // Corners A && B => 5
        {0, 0}, {0, 1}, {0.5, 0},
        {0.5, 0}, {0, 1}, {0.5, 1},
    }},
    {2, {                                           // Corners B && D => 6
        {0, 0.5}, {0, 1}, {0.5, 0},
        {0.5, 0}, {0, 1}, {1, 0.5},
    }},
    {3, {                                           // Corners A && B && D => 7
        {0, 1}, {1, 0}, {0, 0},
        {0, 1}, {1, 0.5}, {1, 0},
        {0, 1}, {0.5, 1}, {1, 0.5}
    }},
    {1, {{0.5, 1}, {1, 1}, {1, 0.5}}},              // Corner C => 8
    {2, {                                           // Corners A && C => 9
        {0, 0}, {0, 0.5}, {0.5, 0},
        {0.5, 1}, {1, 1}, {1, 0.5},
    }},
    {2, {                                           // Corners C && D => 10
        {0.5, 1}, {1, 1}, {1, 0},
        {0.5, 1}, {1, 0}, {0.5, 0},
    }},
    {3, {                                           // Corners A && C && D => 11
        {0, 0}, {1, 1}, {1, 0},
        {0, 0}, {0.5, 1}, {1, 1},
        {0, 0}, {0, 0.5}, {0.5, 1}
    }},
    {2, {                                           // Corners B && C => 12
        {0, 0.5}, {0, 1}, {1, 1},
        {0, 0.5}, {1, 1}, {1, 0.5},
    }},
    {3, {                                           // Corners A && B && C => 13
        {0, 0}, {0, 1}, {1, 1},
        {0, 0}, {1, 1}, {1, 0.5},
        {0, 0}, {1, 0.5}, {0.5, 0}
    }},
    {3, {                                           // Corners B && C && D => 14
        {0, 1}, {1, 1}, {1, 0},
        {0, 1}, {1, 0}, {0.5, 0},
        {0, 1}, {0.5, 0}, {0, 0.5}
    }},
    {2, {                                           // Corners A && B && C && D => 15
        {0, 0}, {0, 1}, {1, 0},
        {1, 0}, {0, 1}, {1, 1}
    }}
};

static inline size_t MarchingSquare_getIndex(float A, float B, float C, float D, float t)
{
    size_t res = (A >= t) ? 1 : 0;
    res += (D >= t) ? 2 : 0;
    res += (B >= t) ? 4 : 0;
    res += (C >= t) ? 8 : 0;

    return res;
}