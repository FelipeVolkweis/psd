#include "contrastfilter.h"
#include <algorithm>
#include <cmath>

ContrastFilter::ContrastFilter() {
    points_.push_back({0, 0});
    points_.push_back({255, 255});
}

std::string ContrastFilter::name() const {
    return "Contrast";
}

void ContrastFilter::applyLUT(std::vector<uint8_t> &lut) const {
    if (points_.empty())
        return;

    for (int i = 0; i < 256; ++i) {
        uint8_t r = lut[i];

        auto it = std::lower_bound(points_.begin(), points_.end(), ContrastPoint{r, 0});

        if (it == points_.begin()) {
            lut[i] = it->s;
        } else if (it == points_.end()) {
            lut[i] = std::prev(it)->s;
        } else {
            const auto &p2 = *it;
            const auto &p1 = *std::prev(it);

            if (p2.r == p1.r) {
                lut[i] = p1.s;
            } else {
                float t = static_cast<float>(r - p1.r) / (p2.r - p1.r);
                float val = static_cast<float>(p1.s) + t * (static_cast<float>(p2.s) - p1.s);
                lut[i] = static_cast<uint8_t>(std::clamp(std::round(val), 0.0f, 255.0f));
            }
        }
    }
}

void ContrastFilter::addPoint(uint8_t r, uint8_t s) {
    uint8_t targetR = r;
    bool occupied = true;
    int offset = 0;

    while (occupied && targetR + offset < 255 && targetR + offset > 0) {
        occupied = false;
        for (const auto &p : points_) {
            if (p.r == targetR + offset) {
                occupied = true;
                break;
            }
        }
        if (occupied) {
            if (offset >= 0)
                offset = -(offset + 1);
            else
                offset = -offset;
        }
    }

    if (!occupied) {
        points_.push_back({static_cast<uint8_t>(targetR + offset), s});
        std::sort(points_.begin(), points_.end());
    }
}

void ContrastFilter::removePoint(int index) {
    if (index > 0 && index < static_cast<int>(points_.size()) - 1) {
        points_.erase(points_.begin() + index);
    }
}

void ContrastFilter::updatePoint(int index, uint8_t r, uint8_t s) {
    if (index >= 0 && index < static_cast<int>(points_.size())) {
        if (index == 0) {
            points_[index].s = s;
        } else if (index == static_cast<int>(points_.size()) - 1) {
            points_[index].s = s;
        } else {
            uint8_t minR = points_[index - 1].r + 1;
            uint8_t maxR = points_[index + 1].r - 1;
            points_[index].r = std::clamp(r, minR, maxR);
            points_[index].s = s;
        }
    }
}
