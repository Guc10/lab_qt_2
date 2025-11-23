#ifndef SATURATIONADJUSTMENTFILTER_H
#define SATURATIONADJUSTMENTFILTER_H

#include "PointOperationFilter.h"

class SaturationAdjustmentFilter : public PointOperationFilter {
private:
    double m_factor;

public:
    explicit SaturationAdjustmentFilter(double factor);

protected:
    QRgb processPixel(QRgb pixel) override;
};

#endif
