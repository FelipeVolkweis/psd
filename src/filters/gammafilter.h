#ifndef GAMMAFILTER_H
#define GAMMAFILTER_H

#include "filter.h"

class GammaFilter : public ImageFilter {
public:
    explicit GammaFilter(float gamma = 1.0f);

    std::string name() const override;
    void applyLUT(std::vector<uint8_t> &lut) const override;

    float gamma() const {
        return gamma_;
    }

    void setGamma(float gamma) {
        gamma_ = gamma;
    }

private:
    float gamma_;
};

#endif
