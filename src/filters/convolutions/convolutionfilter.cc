#include "convolutionfilter.h"
#include <algorithm>
#include <cmath>

Image ConvolutionFilter::apply(const Image& source) const {
    int w = source.width();
    int h = source.height();
    int c = source.channels();
    
    Image result(w, h, c);
    
    Eigen::MatrixXf k = kernel();
    int kRows = k.rows();
    int kCols = k.cols();
    
    int padY = kRows / 2;
    int padX = kCols / 2;
    
    const auto& srcData = source.data();
    auto& dstData = result.data();
    
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            for (int ch = 0; ch < c; ++ch) {
                float sum = 0.0f;
                
                for (int ky = 0; ky < kRows; ++ky) {
                    for (int kx = 0; kx < kCols; ++kx) {
                        int srcY = y + ky - padY;
                        int srcX = x + kx - padX;

                        srcY = std::max(0, std::min(srcY, h - 1));
                        srcX = std::max(0, std::min(srcX, w - 1));
                        
                        int idx = (srcY * w + srcX) * c + ch;
                        sum += srcData[idx] * k(ky, kx);
                    }
                }
                
                int dstIdx = (y * w + x) * c + ch;
                dstData[dstIdx] = static_cast<uint8_t>(std::max(0.0f, std::min(255.0f, std::round(sum))));
            }
        }
    }
    
    return result;
}
