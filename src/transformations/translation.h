#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "transformation.h"

class Translation : public Transformation {
public:
    Translation(float dx, float dy) {
        matrix_ << 1.0f, 0.0f, dx,
                   0.0f, 1.0f, dy,
                   0.0f, 0.0f, 1.0f;
    }
};

#endif