#ifndef LAPLACEFILTER_H
#define LAPLACEFILTER_H

#include "convolutionfilter.h"

class LaplaceFilter : public ConvolutionFilter {
public:
    enum class Mode { Basic, Extended };

    LaplaceFilter(Mode mode = Mode::Basic) 
        : mode_(mode) {}
    
    std::string name() const override { return "Laplace"; }
    
    Eigen::MatrixXf kernel() const override {
        Eigen::MatrixXf k(3, 3);
        if (mode_ == Mode::Extended) {
            k <<  1,  1,  1,
                  1, -8,  1,
                  1,  1,  1;
        } else {
            k <<  0,  1,  0,
                  1, -4,  1,
                  0,  1,  0;
        }
        return k;
    }

    Mode mode() const { return mode_; }
    void setMode(Mode mode) { mode_ = mode; }

private:
    Mode mode_;
};

#endif // LAPLACEFILTER_H
