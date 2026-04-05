#ifndef IMAGECONTROLLER_H
#define IMAGECONTROLLER_H

#include "filters/filterstack.h"
#include "image/image.h"
#include "transformations/transformation.h"
#include <QImage>
#include <QObject>
#include <QPointF>
#include <QQuickImageProvider>
#include <mutex>

class FilterListModel;

class ImageProvider : public QQuickImageProvider {
public:
    ImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {
        image_ = QImage(1, 1, QImage::Format_RGBA8888);
        image_.fill(Qt::transparent);
    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override {
        std::lock_guard<std::mutex> lock(mutex_);
        if (size)
            *size = image_.size();
        return image_;
    }

    void setImage(const QImage &img) {
        std::lock_guard<std::mutex> lock(mutex_);
        image_ = img;
    }

private:
    QImage image_;
    std::mutex mutex_;
};

class ImageController : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool hasImage READ hasImage NOTIFY hasImageChanged)
    Q_PROPERTY(int canvasWidth READ canvasWidth WRITE setCanvasWidth NOTIFY canvasSizeChanged)
    Q_PROPERTY(int canvasHeight READ canvasHeight WRITE setCanvasHeight NOTIFY canvasSizeChanged)
    Q_PROPERTY(int imageX READ imageX NOTIFY imageBoundsChanged)
    Q_PROPERTY(int imageY READ imageY NOTIFY imageBoundsChanged)
    Q_PROPERTY(int imageWidth READ imageWidth NOTIFY imageBoundsChanged)
    Q_PROPERTY(int imageHeight READ imageHeight NOTIFY imageBoundsChanged)
    Q_PROPERTY(QObject *filters READ filters NOTIFY filtersChanged)

public:
    explicit ImageController(QObject *parent = nullptr);
    ~ImageController();

    Q_INVOKABLE bool loadImage(const QString &fileUrl);
    Q_INVOKABLE bool saveImage(const QString &fileUrl);
    Q_INVOKABLE void applyTranslation(float dx, float dy);
    Q_INVOKABLE void applyScale(float sx, float sy, float cx = 0, float cy = 0);
    Q_INVOKABLE void applyRotation(float angleDegrees, float cx = 0, float cy = 0);
    Q_INVOKABLE void reset();
    Q_INVOKABLE void setCanvasSize(int w, int h);
    Q_INVOKABLE void fitCanvasToImage();
    Q_INVOKABLE QPointF getCorner(int index) const;

    // Filter management
    Q_INVOKABLE void addInverseFilter();
    Q_INVOKABLE void addGammaFilter(float gamma);
    Q_INVOKABLE void addLogFilter(float c);
    Q_INVOKABLE void addContrastFilter(float contrast);
    Q_INVOKABLE void addSlidingWindowFilter(int windowValue);
    Q_INVOKABLE void addContrastPoint(int filterIndex, int r, int s);
    Q_INVOKABLE void removeContrastPoint(int filterIndex, int pointIndex);
    Q_INVOKABLE void updateContrastPoint(int filterIndex, int pointIndex, int r, int s);
    Q_INVOKABLE void removeFilter(int index);
    Q_INVOKABLE void clearFilters();
    Q_INVOKABLE void updateFilterValue(int index, float value);

    bool hasImage() const {
        return hasImage_;
    }

    int canvasWidth() const {
        return canvasWidth_;
    }

    int canvasHeight() const {
        return canvasHeight_;
    }

    int imageX() const {
        return imageX_;
    }

    int imageY() const {
        return imageY_;
    }

    int imageWidth() const {
        return imageWidth_;
    }

    int imageHeight() const {
        return imageHeight_;
    }

    void setCanvasWidth(int w) {
        setCanvasSize(w, canvasHeight_);
    }

    void setCanvasHeight(int h) {
        setCanvasSize(canvasWidth_, h);
    }

    ImageProvider *provider() {
        return provider_;
    }

    QObject *filters() const;

signals:
    void imageUpdated();
    void hasImageChanged();
    void canvasSizeChanged();
    void imageBoundsChanged();
    void errorOccurred(const QString &message);
    void filtersChanged();

private:
    void updateImage();
    void applyFilters();

    ImageProvider *provider_;
    Image originalImage_;
    Image lastModifiedImage_;
    Transformation currentTransform_;
    FilterStack filterStack_;
    FilterListModel *filterModel_;
    bool hasImage_ = false;
    int canvasWidth_ = 0;
    int canvasHeight_ = 0;
    int imageX_ = 0;
    int imageY_ = 0;
    int imageWidth_ = 0;
    int imageHeight_ = 0;
};

#endif