#ifndef GRAYSCALEFILTER_H
#define GRAYSCALEFILTER_H

#include "PointOperationFilter.h"

class GrayscaleFilter : public PointOperationFilter {
protected:
    QRgb processPixel(QRgb pixel) override;
};

#endif
