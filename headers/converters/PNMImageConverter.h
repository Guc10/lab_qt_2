#ifndef PNMIMAGECONVERTER_H
#define PNMIMAGECONVERTER_H

#include "ImageConverterInterface.h"

class PNMImageConverter : public IImageConverter {
public:
    QImage convertImage(const QImage& image, const QString& format) override;
};

#endif
