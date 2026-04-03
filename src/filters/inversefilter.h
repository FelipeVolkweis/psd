#ifndef INVERSEFILTER_H
#define INVERSEFILTER_H

#include "filter.h"

class InverseFilter : public ImageFilter {
public:
    std::string name() const override;
    void applyLUT(std::vector<uint8_t> &lut) const override;
};

#endif
