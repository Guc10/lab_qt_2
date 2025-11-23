#ifndef NEGATIVEFILTER_H
#define NEGATIVEFILTER_H

#include "PointOperationFilter.h"

class NegativeFilter : public PointOperationFilter {
protected:
    QRgb processPixel(QRgb pixel) override;
};

#endif
