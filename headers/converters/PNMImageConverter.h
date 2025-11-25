#ifndef PNMIMAGECONVERTER_H
#define PNMIMAGECONVERTER_H

#include "ImageConverterInterface.h"
#include <QMessageBox>

#include <QByteArray>
#include <QTextStream>
#include <QColor>
#include <QFile>

class PNMImageConverter : public IImageConverter {
public:
    QImage convertImage(const QImage& image, const QString& format, const QString& filePath) override;
};

#endif
