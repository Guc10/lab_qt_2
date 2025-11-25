#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>
#include <QString>
#include <memory>
#include <QMessageBox>
#include "../filters/ImageFilterInterface.h"
#include "../converters/ImageConverterInterface.h"
#include "../io/ImageFileHandlerInterface.h"

enum PNM {
    undefined = 0,
    p1 = 1,
    p2 = 2,
    p3 = 3,
    p4 = 4,
    p5 = 5,
    p6 = 6
};

class ImageProcessor {
private:
    QMessageBox msg;
    QImage m_currentImage;
    QImage m_originalImage;
    QString m_currentPath;
    QString m_suffix;
    PNM m_PNMType;
    bool m_isModified;
    std::unique_ptr<IImageFileHandler> m_fileHandler;

public:
    ImageProcessor();
    bool loadImage(const QString& filePath);
    bool saveImage();
    bool exportImage(const QString& newFilePath);
    void applyFilter(std::unique_ptr<IImageFilter> filter);
    void convertImage(std::unique_ptr<IImageConverter> converter, const QString& format, const QString& filePath);
    void resetToOriginal();
    void clearImage();
    bool hasUnsavedChanges() const;
    QString getCurrentPath() const;
    QString getCurrentSufix() const;
    PNM getCurrentMagicType() const;
    QImage getCurrentImage() const;
    bool isPNM();
    bool hasImage() const;
};

#endif
