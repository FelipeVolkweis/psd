#ifndef CONTRASTFILTER_H
#define CONTRASTFILTER_H

#include "filter.h"
#include <vector>

struct ContrastPoint {
    uint8_t r;
    uint8_t s;

    bool operator<(const ContrastPoint &other) const {
        return r < other.r;
    }
};

class ContrastFilter : public ImageFilter {
public:
    ContrastFilter();

    std::string name() const override;
    void applyLUT(std::vector<uint8_t> &lut) const override;

    const std::vector<ContrastPoint> &points() const {
        return points_;
    }

    void addPoint(uint8_t r, uint8_t s);
    void removePoint(int index);
    void updatePoint(int index, uint8_t r, uint8_t s);

private:
    std::vector<ContrastPoint> points_;
};

#endif
