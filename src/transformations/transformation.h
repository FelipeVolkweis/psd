#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "types/mat3x3.h"

class Transformation {
protected:
    Mat3x3 matrix_;

public:
    Transformation() : matrix_(Mat3x3::Identity()) {}

    Transformation(const Mat3x3 &m) : matrix_(m) {}

    virtual ~Transformation() = default;

    const Mat3x3 &getTransformationMatrix() const {
        return matrix_;
    }

    Transformation operator*(const Transformation &rhs) const {
        return Transformation(matrix_ * rhs.getTransformationMatrix());
    }

    Transformation &operator*=(const Transformation &rhs) {
        matrix_ = matrix_ * rhs.getTransformationMatrix();
        return *this;
    }
};

#endif
