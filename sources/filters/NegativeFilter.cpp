#include "headers/filters/NegativeFilter.h"

QRgb NegativeFilter::processPixel(QRgb pixel) {
    return qRgb(255 - qRed(pixel),
                255 - qGreen(pixel),
                255 - qBlue(pixel));
}
