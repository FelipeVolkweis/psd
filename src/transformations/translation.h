#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "transformation.h"

/**
 * @brief Classe responsável por aplicar translação.
 */
class Translation : public Transformation {
public:
    /**
     * @brief Cria uma transformacão de translação.
     * @param dx Deslocamento horizontal.
     * @param dy Deslocamento vertical.
     */
    Translation(float dx, float dy) {
        matrix_ << 1.0f, 0.0f, dx,
                   0.0f, 1.0f, dy,
                   0.0f, 0.0f, 1.0f;
    }
};

#endif