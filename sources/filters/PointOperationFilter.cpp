#include "headers/filters/PointOperationFilter.h"

QImage PointOperationFilter::apply(const QImage& image) {
    if (image.isNull()) return image;

    QImage result(image.size(), image.format());

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QRgb pixel = image.pixel(x, y);
            QRgb newPixel = processPixel(pixel);
            result.setPixel(x, y, newPixel);
        }
    }

    return result;
}
