#include "inversefilter.h"

std::string InverseFilter::name() const {
    return "Inverse";
}

void InverseFilter::applyLUT(std::vector<uint8_t> &lut) const {
    for (int i = 0; i < 256; ++i) {
        lut[i] = 255 - lut[i];
    }
}
