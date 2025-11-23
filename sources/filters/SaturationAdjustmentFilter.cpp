#include "headers/filters/SaturationAdjustmentFilter.h"
#include <QtMath>

SaturationAdjustmentFilter::SaturationAdjustmentFilter(double factor) : m_factor(factor) {}

QRgb SaturationAdjustmentFilter::processPixel(QRgb pixel) {
    QColor color(pixel);

    float h, s, l, a;
    color.getHslF(&h, &s, &l, &a);
    s = qBound(0.0, s * m_factor, 1.0);

    QColor newColor = QColor::fromHslF(h, s, l, a);
    return newColor.rgba();
}
