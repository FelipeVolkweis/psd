#include "logfilter.h"
#include <algorithm>
#include <cmath>

LogFilter::LogFilter(float c) : c_(c) {}

std::string LogFilter::name() const {
    return "Log";
}

void LogFilter::applyLUT(std::vector<uint8_t> &lut) const {
    float logMax = std::log(256.0f);
    for (int i = 0; i < 256; ++i) {
        float r = static_cast<float>(i);
        float s = c_ * std::log(1.0f + r) / logMax;
        lut[i] = static_cast<uint8_t>(std::clamp(s * 255.0f, 0.0f, 255.0f));
    }
}
