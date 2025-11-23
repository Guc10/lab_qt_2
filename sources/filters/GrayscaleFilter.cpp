#include "headers/filters/GrayscaleFilter.h"

QRgb GrayscaleFilter::processPixel(QRgb pixel) {
    int gray = qGray(pixel);
    return qRgb(gray, gray, gray);
}
