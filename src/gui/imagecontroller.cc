#include "imagecontroller.h"
#include "filterlistmodel.h"
#include "filters/contrastfilter.h"
#include "filters/gammafilter.h"
#include "filters/inversefilter.h"
#include "filters/logfilter.h"
#include "filters/slidingwindowfilter.h"
#include "transformations/rotation.h"
#include "transformations/scale.h"
#include "transformations/translation.h"
#include "types/vec3.h"
#include <QDebug>
#include <QUrl>
#include <algorithm>
#include <cmath>

ImageController::ImageController(QObject *parent)
    : QObject(parent), provider_(new ImageProvider()),
      filterModel_(new FilterListModel(filterStack_, this)) {}

ImageController::~ImageController() {}

QObject *ImageController::filters() const {
    return filterModel_;
}

bool ImageController::loadImage(const QString &fileUrl) {
    QUrl url(fileUrl);
    QString path = url.isLocalFile() ? url.toLocalFile() : fileUrl;

    QImage qimg;
    if (!qimg.load(path)) {
        qWarning() << "Failed to load image:" << path;
        emit errorOccurred("Failed to load image: " + path);
        return false;
    }

    qimg = qimg.convertToFormat(QImage::Format_RGBA8888);
    originalImage_.resize(qimg.width(), qimg.height(), 4);

    // Respect QImage scanline padding by copying line by line
    for (int y = 0; y < qimg.height(); ++y) {
        std::copy(qimg.constScanLine(y), qimg.constScanLine(y) + qimg.width() * 4,
                  originalImage_.data().begin() + y * qimg.width() * 4);
    }

    lastModifiedImage_ = originalImage_;
    currentTransform_ = Transformation(); // Reset to identity matrix
    hasImage_ = true;

    // Initial canvas size is the image size
    canvasWidth_ = qimg.width();
    canvasHeight_ = qimg.height();
    emit canvasSizeChanged();

    emit hasImageChanged();
    applyFilters();

    return true;
}

bool ImageController::saveImage(const QString &fileUrl) {
    if (!hasImage_)
        return false;

    QUrl url(fileUrl);
    QString path = url.isLocalFile() ? url.toLocalFile() : fileUrl;

    if (path.isEmpty()) {
        qWarning() << "Save path is empty";
        emit errorOccurred("Save path is empty");
        return false;
    }

    Image currentImage = lastModifiedImage_.applyTransformationToCanvas(
        currentTransform_, canvasWidth_, canvasHeight_);

    // Create a deep copy QImage to ensure data ownership and avoid alignment quirks
    QImage qimg(currentImage.width(), currentImage.height(), QImage::Format_RGBA8888);
    for (int y = 0; y < currentImage.height(); ++y) {
        std::copy(currentImage.data().begin() + y * currentImage.width() * 4,
                  currentImage.data().begin() + (y + 1) * currentImage.width() * 4,
                  qimg.scanLine(y));
    }

    bool success = false;
    // Explicitly handle JPEG conversion if needed
    if (path.endsWith(".jpg", Qt::CaseInsensitive) || path.endsWith(".jpeg", Qt::CaseInsensitive)) {
        success = qimg.convertToFormat(QImage::Format_RGB888).save(path);
    } else if (!path.endsWith(".png", Qt::CaseInsensitive)) {
        path += ".png";
        success = qimg.save(path);
    } else {
        success = qimg.save(path);
    }

    if (!success) {
        qWarning() << "Failed to save image:" << path;
        emit errorOccurred("Failed to save image: " + path);
    }

    return success;
}

void ImageController::setCanvasSize(int w, int h) {
    if (w != canvasWidth_ || h != canvasHeight_) {
        canvasWidth_ = w > 0 ? w : 1;
        canvasHeight_ = h > 0 ? h : 1;
        emit canvasSizeChanged();
        if (hasImage_) {
            updateImage();
        }
    }
}

void ImageController::fitCanvasToImage() {
    if (!hasImage_)
        return;

    int w = originalImage_.width();
    int h = originalImage_.height();

    Mat3x3 inv = currentTransform_.getTransformationMatrix().inverse();
    float a = std::abs(inv(0, 0));
    float b = std::abs(inv(0, 1));
    float d = std::abs(inv(1, 0));
    float e = std::abs(inv(1, 1));

    float c1 = w / 2.0f;
    float c2 = h / 2.0f;

    float bestArea = 0;
    float bestDx = 0;
    float bestDy = 0;

    auto checkAndSet = [&](float dx, float dy) {
        if (dx <= 0 || dy <= 0)
            return;
        if (a * dx + b * dy <= c1 + 1e-3f && d * dx + e * dy <= c2 + 1e-3f) {
            float area = dx * dy;
            if (area > bestArea) {
                bestArea = area;
                bestDx = dx;
                bestDy = dy;
            }
        }
    };

    if (a > 1e-6f && b > 1e-6f)
        checkAndSet(c1 / (2.0f * a), c1 / (2.0f * b));
    if (d > 1e-6f && e > 1e-6f)
        checkAndSet(c2 / (2.0f * d), c2 / (2.0f * e));

    float det = a * e - b * d;
    if (std::abs(det) > 1e-6f) {
        float dx = (c1 * e - c2 * b) / det;
        float dy = (a * c2 - d * c1) / det;
        checkAndSet(dx, dy);
    }

    if (bestArea == 0)
        return;

    Vec3 centerSrc(w / 2.0f, h / 2.0f, 1.0f);
    Vec3 centerDest = currentTransform_.getTransformationMatrix() * centerSrc;

    float shiftX = -(centerDest.x() - bestDx);
    float shiftY = -(centerDest.y() - bestDy);

    currentTransform_ = Translation(shiftX, shiftY) * currentTransform_;
    canvasWidth_ = std::max(1, static_cast<int>(std::round(2.0f * bestDx)));
    canvasHeight_ = std::max(1, static_cast<int>(std::round(2.0f * bestDy)));

    emit canvasSizeChanged();
    updateImage();
}

QPointF ImageController::getCorner(int index) const {
    if (!hasImage_)
        return QPointF(0, 0);
    float w = originalImage_.width();
    float h = originalImage_.height();
    Vec3 pt;
    if (index == 0)
        pt = Vec3(0, 0, 1.0f);
    else if (index == 1)
        pt = Vec3(w, 0, 1.0f);
    else if (index == 2)
        pt = Vec3(w, h, 1.0f);
    else
        pt = Vec3(0, h, 1.0f);

    pt = currentTransform_.getTransformationMatrix() * pt;
    return QPointF(pt.x(), pt.y());
}

void ImageController::applyTranslation(float dx, float dy) {
    if (hasImage_) {
        // Multiply new translation with existing transformation to accumulate
        currentTransform_ = Translation(dx, dy) * currentTransform_;
        updateImage();
    }
}

void ImageController::applyScale(float sx, float sy, float cx, float cy) {
    if (hasImage_) {
        currentTransform_ =
            Translation(cx, cy) * Scale(sx, sy) * Translation(-cx, -cy) * currentTransform_;
        updateImage();
    }
}

void ImageController::applyRotation(float angleDegrees, float cx, float cy) {
    if (hasImage_) {
        float rad = angleDegrees * M_PI / 180.0f;
        currentTransform_ =
            Translation(cx, cy) * Rotation(rad) * Translation(-cx, -cy) * currentTransform_;
        updateImage();
    }
}

void ImageController::reset() {
    if (hasImage_) {
        currentTransform_ = Transformation();
        canvasWidth_ = originalImage_.width();
        canvasHeight_ = originalImage_.height();
        emit canvasSizeChanged();
        updateImage();
    }
}

void ImageController::addInverseFilter() {
    filterStack_.addFilter(std::make_unique<InverseFilter>());
    filterModel_->notifyFilterAdded();
    applyFilters();
}

void ImageController::addGammaFilter(float gamma) {
    filterStack_.addFilter(std::make_unique<GammaFilter>(gamma));
    filterModel_->notifyFilterAdded();
    applyFilters();
}

void ImageController::addLogFilter(float c) {
    filterStack_.addFilter(std::make_unique<LogFilter>(c));
    filterModel_->notifyFilterAdded();
    applyFilters();
}

void ImageController::addContrastFilter(float contrast) {
    filterStack_.addFilter(std::make_unique<ContrastFilter>());
    filterModel_->notifyFilterAdded();
    applyFilters();
}

void ImageController::addSlidingWindowFilter(int windowValue) {
    filterStack_.addFilter(std::make_unique<SlidingWindowFilter>(windowValue));
    filterModel_->notifyFilterAdded();
    applyFilters();
}

void ImageController::addContrastPoint(int filterIndex, int r, int s) {
    const auto &filters = filterStack_.filters();
    if (filterIndex >= 0 && filterIndex < static_cast<int>(filters.size())) {
        if (auto f = dynamic_cast<ContrastFilter *>(filters[filterIndex].get())) {
            f->addPoint(static_cast<uint8_t>(std::clamp(r, 0, 255)),
                        static_cast<uint8_t>(std::clamp(s, 0, 255)));
            filterModel_->notifyFilterChanged(filterIndex);
            applyFilters();
        }
    }
}

void ImageController::removeContrastPoint(int filterIndex, int pointIndex) {
    const auto &filters = filterStack_.filters();
    if (filterIndex >= 0 && filterIndex < static_cast<int>(filters.size())) {
        if (auto f = dynamic_cast<ContrastFilter *>(filters[filterIndex].get())) {
            f->removePoint(pointIndex);
            filterModel_->notifyFilterChanged(filterIndex);
            applyFilters();
        }
    }
}

void ImageController::updateContrastPoint(int filterIndex, int pointIndex, int r, int s) {
    const auto &filters = filterStack_.filters();
    if (filterIndex >= 0 && filterIndex < static_cast<int>(filters.size())) {
        if (auto f = dynamic_cast<ContrastFilter *>(filters[filterIndex].get())) {
            f->updatePoint(pointIndex, static_cast<uint8_t>(std::clamp(r, 0, 255)),
                           static_cast<uint8_t>(std::clamp(s, 0, 255)));
            filterModel_->notifyFilterChanged(filterIndex);
            applyFilters();
        }
    }
}

void ImageController::removeFilter(int index) {
    filterStack_.removeFilter(index);
    filterModel_->notifyFilterRemoved(index);
    applyFilters();
}

void ImageController::clearFilters() {
    filterStack_.clear();
    filterModel_->notifyFiltersCleared();
    applyFilters();
}

void ImageController::updateFilterValue(int index, float value) {
    const auto &filters = filterStack_.filters();
    if (index >= 0 && index < static_cast<int>(filters.size())) {
        auto &filter = filters[index];
        bool changed = false;
        if (auto f = dynamic_cast<GammaFilter *>(filter.get())) {
            f->setGamma(value);
            changed = true;
        } else if (auto f = dynamic_cast<LogFilter *>(filter.get())) {
            f->setC(value);
            changed = true;
        } else if (auto f = dynamic_cast<SlidingWindowFilter *>(filter.get())) {
            f->setWindowValue(static_cast<int>(std::round(value)));
            changed = true;
        }
        if (changed) {
            filterModel_->notifyFilterChanged(index);
            applyFilters();
        }
    }
}

void ImageController::applyFilters() {
    if (!hasImage_)
        return;

    lastModifiedImage_ = originalImage_;
    auto lut = filterStack_.generateCompositeLUT();
    lastModifiedImage_.applyLUT(lut);
    updateImage();
}

void ImageController::updateImage() {
    int outX = 0, outY = 0;
    Image currentImage = lastModifiedImage_.applyTransformationTight(currentTransform_, outX, outY);

    imageX_ = outX;
    imageY_ = outY;
    imageWidth_ = currentImage.width();
    imageHeight_ = currentImage.height();
    emit imageBoundsChanged();

    QImage qimg(currentImage.data().data(), currentImage.width(), currentImage.height(),
                currentImage.width() * currentImage.channels(), QImage::Format_RGBA8888);
    provider_->setImage(qimg.copy());
    emit imageUpdated();
}
