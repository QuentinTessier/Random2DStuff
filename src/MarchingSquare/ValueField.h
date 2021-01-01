#pragma once

#include <stddef.h>
#include "../Type.h"

typedef struct ValueField ValueField;

struct ValueField {
    Vec2 worldSpacePosition;
    Vec2 worldSpaceStep;
    size_t width;
    size_t height;
    float *values;
};

typedef float (*ValueFieldFMAP)(float, float, void *);

ValueField *new_ValueField(size_t width, size_t height, Vec2 position, Vec2 steps);
void destroy_ValueField(ValueField *self);
ValueField *ValueField_fill(ValueField *self, float value);
ValueField *ValueField_map(ValueField *self, ValueFieldFMAP f, void *userdata);
ValueField *ValueField_updateNorth(ValueField *self, float (*f)(size_t x, size_t y, void *userdata), void *userdata);
void ValueField_display(ValueField const *self);
extern int PerlinSeed;

#define INIT_PERLIN(value) int PerlinSeed = value;

#define SET_PERLIN_SEED(value) PerlinSeed = value

float Perlin_Get2d(float x, float y, float freq, int depth);

typedef struct PerlinGenerationInfos {
    float frequency;
    float depth;
} PerlinGenerationInfos;