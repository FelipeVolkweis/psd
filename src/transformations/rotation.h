#ifndef ROTATION_H
#define ROTATION_H

#include "transformation.h"
#include <cmath>

class Rotation : public Transformation {
public:
    /**
     * @brief Cria uma transformacão de rotação.
     * @param angle Ângulo de rotação em radiamos.
     */
    explicit Rotation(float angle) {
        float c = std::cos(angle);
        float s = std::sin(angle);
        matrix_ << c,   -s,    0.0f,
                   s,    c,    0.0f,
                   0.0f, 0.0f, 1.0f;
    }
};

#endif