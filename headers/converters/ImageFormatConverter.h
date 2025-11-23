#ifndef IMAGEFORMATCONVERTER_H
#define IMAGEFORMATCONVERTER_H

#include "ImageConverterInterface.h"

class ImageFormatConverter : public IImageConverter {
public:
    QImage convert(const QImage& image, const QString& format) override;
};

#endif
