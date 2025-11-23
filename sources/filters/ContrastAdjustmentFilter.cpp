#include "headers/filters/ContrastAdjustmentFilter.h"
#include <QtMath>

ContrastAdjustmentFilter::ContrastAdjustmentFilter(double factor) : m_factor(factor) {}

QRgb ContrastAdjustmentFilter::processPixel(QRgb pixel) {
    int r = qRed(pixel);
    int g = qGreen(pixel);
    int b = qBlue(pixel);

    r = qBound(0, static_cast<int>((r - 127) * m_factor + 127), 255);
    g = qBound(0, static_cast<int>((g - 127) * m_factor + 127), 255);
    b = qBound(0, static_cast<int>((b - 127) * m_factor + 127), 255);

    return qRgb(r, g, b);
}
