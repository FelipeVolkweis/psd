#include "image.h"
#include "types/vec3.h"
#include <algorithm>
#include <cmath>

Image::Image() : width_(0), height_(0), channels_(0) {}

Image::Image(int w, int h, int c) : width_(w), height_(h), channels_(c), data_(w * h * c, 0) {}

int Image::width() const {
    return width_;
}

int Image::height() const {
    return height_;
}

int Image::channels() const {
    return channels_;
}

const std::vector<uint8_t> &Image::data() const {
    return data_;
}

std::vector<uint8_t> &Image::data() {
    return data_;
}

void Image::resize(int w, int h, int c) {
    width_ = w;
    height_ = h;
    channels_ = c;
    data_.assign(w * h * c, 0);
}

void Image::applyLUT(const std::vector<uint8_t> &lut) {
    if (data_.empty() || lut.size() < 256)
        return;

    for (int i = 0; i < width_ * height_; ++i) {
        for (int c = 0; c < std::min(channels_, 3); ++c) {
            data_[i * channels_ + c] = lut[data_[i * channels_ + c]];
        }
    }
}

Image Image::applyTransformationTight(const Transformation &transform, int &outX, int &outY) const {
    if (data_.empty()) {
        outX = 0;
        outY = 0;
        return *this;
    }

    Vec3 tl = transform.getTransformationMatrix() * Vec3(0, 0, 1.0f);
    Vec3 tr = transform.getTransformationMatrix() * Vec3(width_, 0, 1.0f);
    Vec3 bl = transform.getTransformationMatrix() * Vec3(0, height_, 1.0f);
    Vec3 br = transform.getTransformationMatrix() * Vec3(width_, height_, 1.0f);

    int minX = std::floor(std::min({tl.x(), tr.x(), bl.x(), br.x()}));
    int minY = std::floor(std::min({tl.y(), tr.y(), bl.y(), br.y()}));
    int maxX = std::ceil(std::max({tl.x(), tr.x(), bl.x(), br.x()}));
    int maxY = std::ceil(std::max({tl.y(), tr.y(), bl.y(), br.y()}));

    int outWidth = std::max(1, maxX - minX);
    int outHeight = std::max(1, maxY - minY);

    outX = minX;
    outY = minY;

    return applyTransformationInternal(transform, outWidth, outHeight, minX, minY);
}

Image Image::applyTransformationToCanvas(const Transformation &transform, int canvasW,
                                         int canvasH) const {
    if (data_.empty())
        return *this;

    int outWidth = canvasW > 0 ? canvasW : width_;
    int outHeight = canvasH > 0 ? canvasH : height_;

    return applyTransformationInternal(transform, outWidth, outHeight, 0, 0);
}

Image Image::applyTransformationInternal(const Transformation &transform, int outWidth,
                                         int outHeight, int offsetX, int offsetY) const {
    Mat3x3 invMatrix = transform.getTransformationMatrix().inverse();
    Image dstImage(outWidth, outHeight, channels_);

    // Unpack matrix to avoid Eigen overhead in inner loops
    float m00 = invMatrix(0, 0);
    float m01 = invMatrix(0, 1);
    float m02 = invMatrix(0, 2);
    float m10 = invMatrix(1, 0);
    float m11 = invMatrix(1, 1);
    float m12 = invMatrix(1, 2);

    for (int y = 0; y < outHeight; ++y) {
        for (int x = 0; x < outWidth; ++x) {
            float destX = x + offsetX;
            float destY = y + offsetY;

            int srcX = std::round(m00 * destX + m01 * destY + m02);
            int srcY = std::round(m10 * destX + m11 * destY + m12);

            if (srcX >= 0 && srcX < width_ && srcY >= 0 && srcY < height_) {
                for (int c = 0; c < channels_; ++c) {
                    dstImage.data_[(y * outWidth + x) * channels_ + c] =
                        data_[(srcY * width_ + srcX) * channels_ + c];
                }
            } else {
                for (int c = 0; c < channels_; ++c) {
                    dstImage.data_[(y * outWidth + x) * channels_ + c] = 0;
                }
            }
        }
    }

    return dstImage;
}