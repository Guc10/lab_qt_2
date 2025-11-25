#ifndef IMAGEFORMATCONVERTER_H
#define IMAGEFORMATCONVERTER_H

#include "ImageConverterInterface.h"

class ImageFormatConverter : public IImageConverter {
public:
    QImage convertImage(const QImage& image, const QString& format, const QString& filePath) override;
};

#endif
