#ifndef SOBELFILTER_H
#define SOBELFILTER_H

#include "convolutionfilter.h"

class SobelFilter : public ConvolutionFilter {
public:
    enum class Direction { X, Y };

    SobelFilter(Direction dir = Direction::X) : dir_(dir) {}
    
    std::string name() const override { 
        return dir_ == Direction::X ? "Sobel X" : "Sobel Y"; 
    }
    
    Eigen::MatrixXf kernel() const override {
        Eigen::MatrixXf k(3, 3);
        if (dir_ == Direction::X) {
            k << -1,  0,  1,
                 -2,  0,  2,
                 -1,  0,  1;
        } else {
            k <<  1,  2,  1,
                  0,  0,  0,
                 -1, -2, -1;
        }
        return k;
    }

    Direction direction() const { return dir_; }
    void setDirection(Direction dir) { dir_ = dir; }

private:
    Direction dir_;
};

#endif // SOBELFILTER_H
