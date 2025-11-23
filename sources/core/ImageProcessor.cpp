#include "headers/core/ImageProcessor.h"
#include "headers/io/ImageFileHandler.h"
#include <QMessageBox>

ImageProcessor::ImageProcessor()
    : m_isModified(false)
    , m_fileHandler(std::make_unique<ImageFileHandler>()) {
}

bool ImageProcessor::loadImage(const QString& filePath) {
    QImage image = m_fileHandler->load(filePath);
    if (image.isNull()) {
        return false;
    }

    m_currentImage = image;
    m_originalImage = image;
    m_currentPath = filePath;
    m_isModified = false;

    return true;
}

bool ImageProcessor::saveImage() {
    if (m_currentPath.isEmpty() || m_currentImage.isNull()) {
        return false;
    }

    if (m_fileHandler->save(m_currentImage, m_currentPath)) {
        m_isModified = false;
        return true;
    }

    return false;
}

bool ImageProcessor::exportImage(const QString& newFilePath) {
    if (m_currentImage.isNull()) {
        return false;
    }

    if (m_fileHandler->save(m_currentImage, newFilePath)) {
        m_currentPath = newFilePath;
        m_isModified = false;
        return true;
    }

    return false;
}

void ImageProcessor::applyFilter(std::unique_ptr<IImageFilter> filter) {
    if (m_currentImage.isNull()) return;

    m_currentImage = filter->apply(m_currentImage);
    m_isModified = true;
}

void ImageProcessor::convertImage(std::unique_ptr<IImageConverter> converter, const QString& format) {
    if (m_currentImage.isNull()) return;

    m_currentImage = converter->convert(m_currentImage, format);
    m_isModified = true;
}

void ImageProcessor::resetToOriginal() {
    if (!m_originalImage.isNull()) {
        m_currentImage = m_originalImage;
        m_isModified = false;
    }
}

void ImageProcessor::clearImage() {
    m_currentImage = QImage();
    m_originalImage = QImage();
}

bool ImageProcessor::hasUnsavedChanges() const {
    return m_isModified;
}

QString ImageProcessor::getCurrentPath() const {
    return m_currentPath;
}

QImage ImageProcessor::getCurrentImage() const {
    return m_currentImage;
}

bool ImageProcessor::hasImage() const {
    return !m_currentImage.isNull();
}
