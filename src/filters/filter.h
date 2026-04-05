#ifndef FILTER_H
#define FILTER_H

#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Interface base para filtros de processamento de imagem.
 * Define a interface para filtros que operam transformando a intensidade dos pixels
 * através de uma lookup table.
 */
class ImageFilter {
public:
    virtual ~ImageFilter() = default;
    virtual std::string name() const = 0;
    virtual void applyLUT(std::vector<uint8_t> &lut) const = 0;
};

#endif
