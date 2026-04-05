#ifndef FILTERSTACK_H
#define FILTERSTACK_H

#include "filter.h"
#include <cstdint>
#include <memory>
#include <vector>

/**
 * @brief Gerenciador de composição de filtros de imagem.
 * A classe é responsável por unir todas as transformações individuais em
 * uma única lookuptable, permitindo que diversos efeitos
 * sejam aplicados à imagem em uma única passada de processamento.
 */
class FilterStack {
public:
    void addFilter(std::unique_ptr<ImageFilter> filter);
    void removeFilter(int index);
    void clear();

    const std::vector<std::unique_ptr<ImageFilter>> &filters() const {
        return filters_;
    }

    std::vector<uint8_t> generateCompositeLUT() const;

private:
    std::vector<std::unique_ptr<ImageFilter>> filters_;
};

#endif
