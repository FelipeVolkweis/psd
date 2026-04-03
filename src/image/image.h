#ifndef IMAGE_H
#define IMAGE_H

#include "transformations/transformation.h"
#include <cstdint>
#include <vector>

class Image {
public:
    Image();
    Image(int w, int h, int c);

    int width() const;
    int height() const;
    int channels() const;

    const std::vector<uint8_t> &data() const;
    std::vector<uint8_t> &data();

    void resize(int w, int h, int c);

    void applyLUT(const std::vector<uint8_t> &lut);

    Image applyTransformationTight(const Transformation &transform, int &outX, int &outY) const;
    Image applyTransformationToCanvas(const Transformation &transform, int canvasW,
                                      int canvasH) const;

private:
    int width_;
    int height_;
    int channels_;
    std::vector<uint8_t> data_;

    Image applyTransformationInternal(const Transformation &transform, int outWidth, int outHeight,
                                      int offsetX, int offsetY) const;
};

#endif