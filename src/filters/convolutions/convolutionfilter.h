#ifndef CONVOLUTIONFILTER_H
#define CONVOLUTIONFILTER_H

#include "image/image.h"
#include <eigen3/Eigen/Dense>
#include <string>

class ConvolutionFilter {
public:
    virtual ~ConvolutionFilter() = default;

    virtual std::string name() const = 0;
    virtual Eigen::MatrixXf kernel() const = 0;

    virtual Image apply(const Image& source) const;
};

#endif // CONVOLUTIONFILTER_H
