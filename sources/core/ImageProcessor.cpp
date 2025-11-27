#include "headers/core/ImageProcessor.h"
#include "headers/io/ImageFileHandler.h"
#include <QFileInfo>
#include <QFile>
#include <QTextStream>

ImageProcessor::ImageProcessor()
    : m_isModified(false)
    , m_fileHandler(std::make_unique<ImageFileHandler>())
    , m_PNMType(undefined)
{
}

bool ImageProcessor::loadImage(const QString& filePath) {
    QImage image = m_fileHandler->load(filePath);
    if (image.isNull()) return false;

    QFileInfo info(filePath);
    m_currentImage = image;
    m_originalImage = image;
    m_currentPath = filePath;
    m_suffix = info.suffix().toLower();
    isPNM();
    m_isModified = false;
    return true;
}

bool ImageProcessor::saveImage() {
    if (m_currentPath.isEmpty() || m_currentImage.isNull()) return false;
    if (m_fileHandler->save(m_currentImage, m_currentPath)) {
        m_isModified = false;
        return true;
    }
    return false;
}

bool ImageProcessor::exportImage(const QString& newFilePath) {
    if (m_currentImage.isNull()) return false;
    if (m_fileHandler->save(m_currentImage, newFilePath)) {
        m_currentPath = newFilePath;
        m_isModified = false;
        QFileInfo info(newFilePath);
        m_suffix = info.suffix().toLower();
        return true;
    }
    return false;
}

bool ImageProcessor::exportPNM(const QString& newFilePath, const QString& magic, bool ascii) {
    if (m_currentImage.isNull()) return false;
    if (m_fileHandler->savePNM(m_currentImage, newFilePath, magic, ascii)) {
        m_currentPath = newFilePath;
        m_isModified = false;
        QFileInfo info(newFilePath);
        m_suffix = info.suffix().toLower();
        // update stored PNM magic if file created is a pnm
        QString m = magic.toLower();
        if (m == "p1") m_PNMType = p1;
        else if (m == "p2") m_PNMType = p2;
        else if (m == "p3") m_PNMType = p3;
        else if (m == "p4") m_PNMType = p4;
        else if (m == "p5") m_PNMType = p5;
        else if (m == "p6") m_PNMType = p6;
        else m_PNMType = undefined;
        return true;
    }
    return false;
}

void ImageProcessor::applyFilter(std::unique_ptr<IImageFilter> filter) {
    if (m_currentImage.isNull() || !filter) return;
    m_currentImage = filter->apply(m_currentImage);
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
    m_currentPath.clear();
    m_suffix.clear();
    m_PNMType = undefined;
    m_isModified = false;
}

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
    QString firstLine;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#')) continue;
        firstLine = line;
        break;
    }
    file.close();

    if (firstLine == "P1") m_PNMType = p1;
    else if (firstLine == "P2") m_PNMType = p2;
    else if (firstLine == "P3") m_PNMType = p3;
    else if (firstLine == "P4") m_PNMType = p4;
    else if (firstLine == "P5") m_PNMType = p5;
    else if (firstLine == "P6") m_PNMType = p6;
    else m_PNMType = undefined;

    return m_PNMType != undefined;
}

bool ImageProcessor::hasImage() const {
    return !m_currentImage.isNull();
}