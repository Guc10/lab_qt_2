#ifndef IMAGEFILEHANDLER_H
#define IMAGEFILEHANDLER_H

#include "ImageFileHandlerInterface.h"

class ImageFileHandler : public IImageFileHandler {
public:
    QImage load(const QString& filePath) override;
    bool save(const QImage& image, const QString& filePath) override;
};

#endif
