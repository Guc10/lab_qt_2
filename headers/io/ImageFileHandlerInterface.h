#ifndef IMAGEFILEHANDLERINTERFACE_H
#define IMAGEFILEHANDLERINTERFACE_H

#include <QImage>
#include <QString>

class IImageFileHandler {
public:
    virtual ~IImageFileHandler() = default;
    virtual QImage load(const QString& filePath) = 0;
    virtual bool save(const QImage& image, const QString& filePath) = 0;
};

#endif
