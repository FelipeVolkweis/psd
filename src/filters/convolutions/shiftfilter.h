#ifndef SHIFTFILTER_H
#define SHIFTFILTER_H

#include "convolutionfilter.h"
#include <cmath>

class ShiftFilter : public ConvolutionFilter {
public:
    ShiftFilter(int dx = 1, int dy = 0) : dx_(dx), dy_(dy) {}
    
    std::string name() const override { return "Shift"; }
    
    Eigen::MatrixXf kernel() const override {
        if (dx_ == 0 && dy_ == 0) {
            Eigen::MatrixXf k(1, 1);
            k << 1.0f;
            return k;
        }

        int sizeY = std::abs(dy_) * 2 + 1;
        int sizeX = std::abs(dx_) * 2 + 1;
        Eigen::MatrixXf k = Eigen::MatrixXf::Zero(sizeY, sizeX);
        
        int cy = std::abs(dy_);
        int cx = std::abs(dx_);
        
        k(cy - dy_, cx - dx_) = 1.0f;
        
        return k;
    }

    int dx() const { return dx_; }
    void setDx(int dx) { dx_ = dx; }

    int dy() const { return dy_; }
    void setDy(int dy) { dy_ = dy; }

private:
    int dx_;
    int dy_;
};

#endif // SHIFTFILTER_H
