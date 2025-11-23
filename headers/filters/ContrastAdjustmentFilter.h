#ifndef CONTRASTADJUSTMENTFILTER_H
#define CONTRASTADJUSTMENTFILTER_H

#include "PointOperationFilter.h"

class ContrastAdjustmentFilter : public PointOperationFilter {
private:
    double m_factor;

public:
    explicit ContrastAdjustmentFilter(double factor);

protected:
    QRgb processPixel(QRgb pixel) override;
};

#endif
