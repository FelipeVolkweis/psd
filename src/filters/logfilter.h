#ifndef LOGFILTER_H
#define LOGFILTER_H

#include "filter.h"

/**
 * @brief Filtro de transformação logarítmica.
 * Realiza o mapeamento das intensidades dos pixels para uma escala logarítmica.
 */
class LogFilter : public ImageFilter {
public:
    explicit LogFilter(float c = 1.0f);

    std::string name() const override;
    void applyLUT(std::vector<uint8_t> &lut) const override;

    float c() const {
        return c_;
    }

    void setC(float c) {
        c_ = c;
    }

private:
    float c_;
};

#endif
