#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "types/mat3x3.h"

/**
 * @brief Classe base para transformações geométricas 2D utilizando coordenadas homogêneas.
 * Permite a composição de transformações através de operações matriciais 3x3.
 */
class Transformation {
protected:
    /** @brief Matriz de transformação interna. */
    Mat3x3 matrix_;

public:
    Transformation() : matrix_(Mat3x3::Identity()) {}

    Transformation(const Mat3x3 &m) : matrix_(m) {}

    virtual ~Transformation() = default;

    const Mat3x3 &getTransformationMatrix() const {
        return matrix_;
    }

    /**
     * @brief Compõe duas transformações.
     * @param rhs Transformação à direita na operação.
     * @return Uma nova Transformation representando o resultado da composição.
     */
    Transformation operator*(const Transformation &rhs) const {
        return Transformation(matrix_ * rhs.getTransformationMatrix());
    }

    /**
     * @brief Multiplica e atribui a composição à transformação atual.
     * @param rhs Transformação a ser composta.
     * @return Referência para a própria instância após a alteração.
     */
    Transformation &operator*=(const Transformation &rhs) {
        matrix_ = matrix_ * rhs.getTransformationMatrix();
        return *this;
    }
};

#endif