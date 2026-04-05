#ifndef SCALE_H
#define SCALE_H

#include "transformation.h"

class Scale : public Transformation {
public:
    /**
     * @brief Aplica uma transformação de escala nos eixos.
     * @param sx Fator de escala no eixo x.
     * @param sy Fator de escala no eixo y.
     */
    Scale(float sx, float sy) {
        matrix_ << sx, 0.0f, 0.0f,
                   0.0f, sy, 0.0f,
                   0.0f, 0.0f, 1.0f;
    }
    
    /**
     * @brief Cria escala uniforme.
     * @param s Fator global de escla.
     */
    explicit Scale(float s) : Scale(s, s) {}
};

#endif