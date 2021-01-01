#include <stdlib.h>
#include <stdio.h>
#include "ValueField.h"

ValueField *new_ValueField(size_t width, size_t height, Vec2 position, Vec2 steps)
{
    ValueField *n = malloc(sizeof(ValueField) + (width * height * sizeof(float)));

    if (n == 0)
        return 0;
    n->width = width;
    n->height = height;
    n->worldSpacePosition = position;
    n->worldSpaceStep = steps;
    n->values = (float *)(n + 1);
    return n;
}

void destroy_ValueField(ValueField *self)
{
    free(self);
}

ValueField *ValueField_fill(ValueField *self, float value)
{
    if (self == 0)
        return 0;
    size_t size = self->height * self->width;
    for (size_t i = 0; i < size; ++i) {
        self->values[i] = value;
    }
    return self;
}

ValueField *ValueField_map(ValueField *self, float (*f)(float x, float y, void *userdata), void *userdata)
{
    if (self == 0 || f == 0)
        return 0;
    for (size_t y = 0; y < self->height; ++y) {
        for (size_t x = 0; x < self->width; ++x) {
            float fX = self->worldSpacePosition.x + ((float)x * self->worldSpaceStep.x);
            float fY = self->worldSpacePosition.y + ((float)y * self->worldSpaceStep.y);
            self->values[y * self->width + x] = f(fX, fY, userdata);
        }
    }
    return self;
}

void ValueField_display(ValueField const *self)
{
    if (self == 0)
        return;
    for (size_t y = 0; y < self->height; ++y) {
        for (size_t x = 0; x < self->width; ++x) {
            float fX = self->worldSpacePosition.x + ((float)x * self->worldSpaceStep.x);
            float fY = self->worldSpacePosition.y + ((float)y * self->worldSpaceStep.y);
            printf("(%.2f, %.2f) => %.2f\n", fX, fY, self->values[y * self->width + x]);
        }
    }
}