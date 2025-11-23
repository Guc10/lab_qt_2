#ifndef BRIGHTNESSADJUSTMENTFILTER_H
#define BRIGHTNESSADJUSTMENTFILTER_H

#include "PointOperationFilter.h"

class BrightnessAdjustmentFilter : public PointOperationFilter {
private:
    int m_adjustment;

public:
    explicit BrightnessAdjustmentFilter(int adjustment);

protected:
    QRgb processPixel(QRgb pixel) override;
};

#endif
