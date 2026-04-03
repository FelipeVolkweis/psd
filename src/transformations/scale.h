#ifndef SCALE_H
#define SCALE_H

#include "transformation.h"

class Scale : public Transformation {
public:
    Scale(float sx, float sy) {
        matrix_ << sx, 0.0f, 0.0f,
                   0.0f, sy, 0.0f,
                   0.0f, 0.0f, 1.0f;
    }
    
    explicit Scale(float s) : Scale(s, s) {}
};

#endif