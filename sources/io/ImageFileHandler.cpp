#include "headers/io/ImageFileHandler.h"
#include <QMessageBox>
#include <QFileInfo>

QImage ImageFileHandler::load(const QString& filePath) {
    QImage image;

    if (!image.load(filePath)) {
        QMessageBox::warning(nullptr, "Load Error",
                             "Cannot load image: " + filePath);
        return QImage();
    }

    return image;
}

bool ImageFileHandler::save(const QImage& image, const QString& filePath) {
    if (!image.save(filePath)) {
        QMessageBox::warning(nullptr, "Save Error",
                             "Cannot save image: " + filePath);
        return false;
    }

    return true;
}
