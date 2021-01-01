#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SFML/Graphics.h>
#include "ValueField.h"
#include "MarchingSquare.h"

INIT_PERLIN(0);

float perlin_map(float x, float y, PerlinGenerationInfos *userdata)
{
    float fX = (float)x;
    float fY = (float)y;

    return Perlin_Get2d(fX, fY, userdata->frequency, userdata->depth);
}

void VertexArray_MarchingCubeTriangles(sfVertexArray *out, size_t index, Vec2 worldpositionA, Vec2 worldSteps, float values[4])
{
    sfVertex v;
    float HSV[3] = {0, 1.f, 1.f};

    v.color = sfWhite;
    v.texCoords = Vec2_(0, 0);

    size_t count = MarchingSquareTriangleCasesTable[index].count * 3;

    int av[2] = {1, 2};
    for (size_t i = 0; i < count; ++i) {
        v.position = Vec2_(
            worldpositionA.x + MarchingSquareTriangleCasesTable[index].vertices[i].x * worldSteps.x,
            worldpositionA.y + MarchingSquareTriangleCasesTable[index].vertices[i].y * worldSteps.y
        );
        int a = (int)(round(MarchingSquareTriangleCasesTable[index].vertices[i].x) * av[0] + round(MarchingSquareTriangleCasesTable[index].vertices[i].y) * av[1]);
        int b = (int)(floor(MarchingSquareTriangleCasesTable[index].vertices[i].x) * av[0] + floor(MarchingSquareTriangleCasesTable[index].vertices[i].y) * av[1]);
        float l = lerp(values[a], values[b], 0.5f);
        v.color = HSVtoRGB(lerp(0, 180, l), 100.0f, 100.0f);

        sfVertexArray_append(out, v);
    }
}

void ValueField_to_VertexArray(ValueField const *field, sfVertexArray *out, float threshold)
{
    const size_t maxY = field->height - 1;
    const size_t maxX = field->width - 1;
    for (size_t y = 0; y < maxY; ++y) {
        for (size_t x = 0; x < maxX; ++x) {
            float fX = field->worldSpacePosition.x + ((float)x * field->worldSpaceStep.x);
            float fY = field->worldSpacePosition.y + ((float)y * field->worldSpaceStep.y);
            float values[4] = {
                field->values[y * field->width + x], // A
                field->values[y * field->width + (x + 1)], // D
                field->values[(y + 1) * field->width + x], // B
                field->values[(y + 1) * field->width + (x + 1)], // C
            };

            size_t index = MarchingSquare_getIndex(
                field->values[y * field->width + x],
                field->values[(y + 1) * field->width + x],
                field->values[(y + 1) * field->width + (x + 1)],
                field->values[y * field->width + (x + 1)],
                threshold
            );

            VertexArray_MarchingCubeTriangles(out, index, Vec2_(fX, fY), field->worldSpaceStep, values);
        }
    }
}

void sincos_map(float x, float y, void *userdata)
{
    float res = sinf(x);
    return (res < 0.f) ? -res : res;
}

static void SFML_run(const char *name)
{
    sfRenderWindow *win = sfRenderWindow_create((sfVideoMode){1920, 1080, 32}, name, sfClose | sfFullscreen, 0);
    sfVertexArray *triangles = sfVertexArray_create();
    sfVertexArray_setPrimitiveType(triangles, sfTriangles);

    // Build app data
    ValueField *f = new_ValueField(10, 10, Vec2_(0, 0), Vec2_(75, 75));
    PerlinGenerationInfos pInfos = {0.01f, 10.f};
    ValueField_map(f, sincos_map, &pInfos);
    ValueField_display(f);

    ValueField_to_VertexArray(f, triangles, 0.5f);

    while (sfRenderWindow_isOpen(win)) {
        sfEvent e;
        while (sfRenderWindow_pollEvent(win, &e)) {
            if (e.type == sfEvtClosed || (e.type == sfEvtKeyPressed && e.key.code == sfKeyEscape))
                sfRenderWindow_close(win);
            else if (e.type == sfEvtKeyReleased && e.key.code == sfKeySpace) {
                SET_PERLIN_SEED(rand());
                ValueField_map(f, perlin_map, &pInfos);
                sfVertexArray_clear(triangles);
                ValueField_to_VertexArray(f, triangles, 0.4);
            }
        }
        sfRenderWindow_clear(win, sfTransparent);
        sfRenderWindow_drawVertexArray(win, triangles, 0);
        sfRenderWindow_display(win);
    }
    sfRenderWindow_destroy(win);
}

int main()
{
    srand(getpid());
    SET_PERLIN_SEED(rand());

    SFML_run("MarchingSquare");
    return 0;
}