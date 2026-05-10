#ifndef GAUSSIANFILTER_H
#define GAUSSIANFILTER_H

#include "convolutionfilter.h"
#include <cmath>

class GaussianFilter : public ConvolutionFilter {
public:
    GaussianFilter(int size = 3, float sigma = 1.0f) 
        : size_(size), sigma_(sigma) {}
    
    std::string name() const override { return "Gaussiano"; }
    
    Eigen::MatrixXf kernel() const override {
        int s = std::max(1, size_);
        if (s % 2 == 0) s++; // Ensure odd size
        
        Eigen::MatrixXf k(s, s);
        int half = s / 2;
        float sum = 0.0f;
        
        for (int y = -half; y <= half; ++y) {
            for (int x = -half; x <= half; ++x) {
                float val = std::exp(-(x * x + y * y) / (2 * sigma_ * sigma_));
                k(y + half, x + half) = val;
                sum += val;
            }
        }
        
        // Normalize
        if (sum > 0.0f) {
            k /= sum;
        }
        
        return k;
    }

    int size() const { return size_; }
    void setSize(int size) { size_ = size; }

    float sigma() const { return sigma_; }
    void setSigma(float sigma) { sigma_ = sigma; }

private:
    int size_;
    float sigma_;
};

#endif // GAUSSIANFILTER_H
