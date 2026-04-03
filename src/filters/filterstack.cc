#include "filterstack.h"

void FilterStack::addFilter(std::unique_ptr<ImageFilter> filter) {
    filters_.push_back(std::move(filter));
}

void FilterStack::removeFilter(int index) {
    if (index >= 0 && index < static_cast<int>(filters_.size())) {
        filters_.erase(filters_.begin() + index);
    }
}

void FilterStack::clear() {
    filters_.clear();
}

std::vector<uint8_t> FilterStack::generateCompositeLUT() const {
    std::vector<uint8_t> lut(256);
    for (int i = 0; i < 256; ++i) {
        lut[i] = static_cast<uint8_t>(i);
    }

    for (const auto &filter : filters_) {
        filter->applyLUT(lut);
    }
    return lut;
}
