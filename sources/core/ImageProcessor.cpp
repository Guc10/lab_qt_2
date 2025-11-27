#include "headers/core/ImageProcessor.h"
#include "headers/io/ImageFileHandler.h"
#include <QMessageBox>
#include <QFileInfo>

ImageProcessor::ImageProcessor()
    : m_isModified(false)
    , m_fileHandler(std::make_unique<ImageFileHandler>()) {
}

// Loading image

bool ImageProcessor::loadImage(const QString& filePath) {
    QImage image = m_fileHandler->load(filePath);
    if (image.isNull()) {
        return false;
    }

    QFileInfo info(filePath);
    m_currentImage = image;
    m_originalImage = image;
    m_currentPath = filePath;
    m_suffix = info.suffix();
    isPNM();
    m_isModified = false;

    return true;
}

// Export and save

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

        QFileInfo info(newFilePath);
        m_suffix = info.suffix();


        return true;
    }

    return false;
}

// Applying filters and converting

void ImageProcessor::applyFilter(std::unique_ptr<IImageFilter> filter) {
    if (m_currentImage.isNull()) return;

    m_currentImage = filter->apply(m_currentImage);
    m_isModified = true;
}

// Clearing effects and Canvas

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

// Parameters

bool ImageProcessor::hasUnsavedChanges() const {
    return m_isModified;
}

QString ImageProcessor::getCurrentPath() const {
    return m_currentPath;
}

QString ImageProcessor::getCurrentSufix() const {
    return m_suffix;
}

PNM ImageProcessor::getCurrentMagicType() const {
    return m_PNMType;
}

QImage ImageProcessor::getCurrentImage() const {
    return m_currentImage;
}

bool ImageProcessor::isPNM() {
    if (m_suffix != "pbm" && m_suffix != "pgm" && m_suffix != "ppm") {
        m_PNMType = undefined;
        return false;
    }

    if (m_currentPath.isEmpty()) {
        m_PNMType = undefined;
        return false;
    }

    QFile file(m_currentPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_PNMType = undefined;
        return false;
    }

    QTextStream in(&file);
    QString firstLine = in.readLine().trimmed();
    file.close();

    if (firstLine == "P1") {
        m_PNMType = p1;
        return true;
    } else if (firstLine == "P2") {
        m_PNMType = p2;
        return true;
    } else if (firstLine == "P3") {
        m_PNMType = p3;
        return true;
    } else if (firstLine == "P4") {
        m_PNMType = p4;
        return true;
    } else if (firstLine == "P5") {
        m_PNMType = p5;
        return true;
    } else if (firstLine == "P6") {
        m_PNMType = p6;
        return true;
    } else {
        m_PNMType = undefined;
        return false;
    }

}

bool ImageProcessor::hasImage() const {
    return !m_currentImage.isNull();
}
