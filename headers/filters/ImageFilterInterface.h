#ifndef IMAGEFILTERINTERFACE_H
#define IMAGEFILTERINTERFACE_H

#include <QImage>
#include <memory>

class IImageFilter {
public:
    virtual ~IImageFilter() = default;
    virtual QImage apply(const QImage& image) = 0;
};

#endif
