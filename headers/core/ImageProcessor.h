#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>
#include <QString>
#include <memory>
#include "../filters/ImageFilterInterface.h"
#include "../converters/ImageConverterInterface.h"
#include "../io/ImageFileHandlerInterface.h"

class ImageProcessor {
private:
    QImage m_currentImage;
    QImage m_originalImage;
    QString m_currentPath;
    bool m_isModified;
    std::unique_ptr<IImageFileHandler> m_fileHandler;

public:
    ImageProcessor();
    bool loadImage(const QString& filePath);
    bool saveImage();
    bool exportImage(const QString& newFilePath);
    void applyFilter(std::unique_ptr<IImageFilter> filter);
    void convertImage(std::unique_ptr<IImageConverter> converter, const QString& format);
    void resetToOriginal();
    bool hasUnsavedChanges() const;
    QString getCurrentPath() const;
    QImage getCurrentImage() const;
    bool hasImage() const;
};

#endif
