#ifndef MEANFILTER_H
#define MEANFILTER_H

#include "convolutionfilter.h"

class MeanFilter : public ConvolutionFilter {
public:
    MeanFilter(int size = 3) : size_(size) {}
    
    std::string name() const override { return "Média"; }
    
    Eigen::MatrixXf kernel() const override {
        int s = std::max(1, size_);
        Eigen::MatrixXf k = Eigen::MatrixXf::Constant(s, s, 1.0f / (s * s));
        return k;
    }

    int size() const { return size_; }
    void setSize(int size) { size_ = size; }

private:
    int size_;
};

#endif // MEANFILTER_H
