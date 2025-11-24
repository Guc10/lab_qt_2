#ifndef IMAGECONVERTERINTERFACE_H
#define IMAGECONVERTERINTERFACE_H

#include <QImage>
#include <QString>

class IImageConverter {
public:
    virtual ~IImageConverter() = default;
    virtual QImage convertImage(const QImage& image, const QString& format) = 0;
};

#endif
