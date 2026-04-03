#include "gammafilter.h"
#include <algorithm>
#include <cmath>

GammaFilter::GammaFilter(float gamma) : gamma_(gamma) {}

std::string GammaFilter::name() const {
    return "Gamma";
}

void GammaFilter::applyLUT(std::vector<uint8_t> &lut) const {
    for (int i = 0; i < 256; ++i) {
        float val = static_cast<float>(lut[i]) / 255.0f;
        val = std::pow(val, gamma_);
        lut[i] = static_cast<uint8_t>(std::clamp(val * 255.0f, 0.0f, 255.0f));
    }
}
