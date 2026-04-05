#ifndef SLIDINGWINDOWFILTER_H
#define SLIDINGWINDOWFILTER_H

#include "filter.h"

/**
 * @brief Filtro de processamento baseado em deslocamento de valores de intensidade,
 * *aplicando um deslocamento cíclico (offset) aos valores
 * de cor baseado no parâmetro windowValue_.
 */
class SlidingWindowFilter : public ImageFilter {
public:
    explicit SlidingWindowFilter(int windowValue = 0);

    std::string name() const override;
    void applyLUT(std::vector<uint8_t> &lut) const override;

    int windowValue() const {
        return windowValue_;
    }

    void setWindowValue(int windowValue) {
        windowValue_ = windowValue;
    }

private:
    int windowValue_;
};

#endif