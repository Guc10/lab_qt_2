#ifndef IMAGEFILEHANDLER_H
#define IMAGEFILEHANDLER_H

#include "ImageFileHandlerInterface.h"
#include <QImage>
#include <QString>

class ImageFileHandler : public IImageFileHandler {
public:
    QImage load(const QString& filePath) override;
    bool save(const QImage& image, const QString& filePath) override;
    bool savePNM(const QImage& image, const QString& filePath, const QString& magic, bool ascii) override;
};

#endif