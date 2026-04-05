#include "slidingwindowfilter.h"

SlidingWindowFilter::SlidingWindowFilter(int windowValue) : windowValue_(windowValue) {}

std::string SlidingWindowFilter::name() const {
    return "Sliding Window";
}

void SlidingWindowFilter::applyLUT(std::vector<uint8_t> &lut) const {
    for (int i = 0; i < 256; ++i) {
        lut[i] = static_cast<uint8_t>((lut[i] + windowValue_) % 255);
    }
}