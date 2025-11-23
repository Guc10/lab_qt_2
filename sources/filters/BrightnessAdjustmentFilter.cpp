#include "headers/filters/BrightnessAdjustmentFilter.h"

BrightnessAdjustmentFilter::BrightnessAdjustmentFilter(int adjustment) : m_adjustment(adjustment) {}

QRgb BrightnessAdjustmentFilter::processPixel(QRgb pixel) {
    return qRgb(qBound(0, qRed(pixel) + m_adjustment, 255),
                qBound(0, qGreen(pixel) + m_adjustment, 255),
                qBound(0, qBlue(pixel) + m_adjustment, 255));
}
