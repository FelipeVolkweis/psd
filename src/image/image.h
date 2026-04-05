#ifndef IMAGE_H
#define IMAGE_H

#include "transformations/transformation.h"
#include <cstdint>
#include <vector>

/**
 * @brief Representação principal de uma imagem em memória.
 * Armazena pixels em um buffer contíguo de 8 bits.
 */
class Image {
public:
    Image();

    /**
     * @brief Construtor com dimensões específicas.
     * @param w Largura.
     * @param h Altura.
     * @param c Canais de cor.
     */
    Image(int w, int h, int c);

    int width() const;
    int height() const;
    int channels() const;

    const std::vector<uint8_t> &data() const;
    std::vector<uint8_t> &data();

    /**
     * @brief Redimensiona o buffer da imagem e limpa o conteúdo.
     * @param w Nova largura.
     * @param h Nova altura.
     * @param c Novos canais.
     */
    void resize(int w, int h, int c);

    /**
     * @brief Aplica uma lookup table aos canais de cor.
     * @param lut Vetor de 256 valores para mapeamento de intensidade.
     */
    void applyLUT(const std::vector<uint8_t> &lut);

    /**
     * @brief Aplica uma transformação e retorna uma imagem ajustada ao novo conteúdo.
     * @param transform Objeto de transformação geométrica.
     * @param outX Saída: coordenada X da origem no espaço global.
     * @param outY Saída: coordenada Y da origem no espaço global.
     * @return Nova imagem recortada para conter o resultado transformado.
     */
    Image applyTransformationTight(const Transformation &transform, int &outX, int &outY) const;

    /**
     * @brief Aplica uma transformação projetando-a em um canvas de tamanho fixo.
     * @param transform Objeto de transformação geométrica.
     * @param canvasW Largura do canvas de destino.
     * @param canvasH Altura do canvas de destino.
     * @return Imagem no tamanho do canvas.
     */
    Image applyTransformationToCanvas(const Transformation &transform, int canvasW,
                                      int canvasH) const;

private:
    int width_;
    int height_;
    int channels_;
    std::vector<uint8_t> data_;

    /**
     * @brief Função interna para processamento de transformações geométricas.
     * Realiza o inverse mappin com arredondamento.
     */
    Image applyTransformationInternal(const Transformation &transform, int outWidth, int outHeight,
                                      int offsetX, int offsetY) const;
};

#endif