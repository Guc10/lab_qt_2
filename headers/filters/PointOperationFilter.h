#ifndef POINTOPERATIONFILTER_H
#define POINTOPERATIONFILTER_H

#include "ImageFilterInterface.h"

class PointOperationFilter : public IImageFilter {
public:
    QImage apply(const QImage& image) override;

protected:
    virtual QRgb processPixel(QRgb pixel) = 0;
};

#endif
