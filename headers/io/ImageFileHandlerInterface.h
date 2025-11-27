#ifndef IMAGEFILEHANDLERINTERFACE_H
#define IMAGEFILEHANDLERINTERFACE_H

#include <QImage>
#include <QString>

class IImageFileHandler {
public:
    virtual ~IImageFileHandler() = default;
    virtual QImage load(const QString& filePath) = 0;
    virtual bool save(const QImage& image, const QString& filepath) = 0;

    // magic: "p1","p2","p3","p4","p5","p6"
    // ascii: if true write ASCII variant (only relevant for p1/p2/p3)
    virtual bool savePNM(const QImage& image, const QString& filepath, const QString& magic, bool ascii) = 0;
};

#endif