#include "headers/io/ImageFileHandler.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QImage>
#include <QBuffer>
#include <QCoreApplication>

QImage ImageFileHandler::load(const QString& filePath) {
    QImage image;
    if (!image.load(filePath)) {
        QMessageBox::warning(nullptr, "Load Error", "Cannot load image: " + filePath);
        return QImage();
    }
    return image;
}

bool ImageFileHandler::save(const QImage& image, const QString& filePath) {
    if (!image.save(filePath)) {
        QMessageBox::warning(nullptr, "Save Error", "Cannot save image: " + filePath);
        return false;
    }
    return true;
}

static bool writeAsciiPBM(const QImage &img, QFile &out) {
    QTextStream stream(&out);
    stream << "P1\n" << img.width() << " " << img.height() << "\n";
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            stream << ((qGray(img.pixel(x, y)) < 128) ? 1 : 0) << " ";
        }
        stream << "\n";
        if ((y & 0x1F) == 0) QCoreApplication::processEvents();
    }
    return true;
}

static bool writeAsciiPGM(const QImage &img, QFile &out) {
    QImage g = img.convertToFormat(QImage::Format_Grayscale8);
    QTextStream stream(&out);
    stream << "P2\n" << g.width() << " " << g.height() << "\n255\n";
    for (int y = 0; y < g.height(); ++y) {
        const uchar *line = g.constScanLine(y);
        for (int x = 0; x < g.width(); ++x) stream << QString::number(line[x]) << " ";
        stream << "\n";
        if ((y & 0x1F) == 0) QCoreApplication::processEvents();
    }
    return true;
}

static bool writeAsciiPPM(const QImage &img, QFile &out) {
    QImage r = img.convertToFormat(QImage::Format_RGB888);
    QTextStream stream(&out);
    stream << "P3\n" << r.width() << " " << r.height() << "\n255\n";
    for (int y = 0; y < r.height(); ++y) {
        const uchar *line = r.constScanLine(y);
        for (int x = 0; x < r.width(); ++x) {
            int i = x * 3;
            stream << QString::number(line[i]) << " "
                   << QString::number(line[i + 1]) << " "
                   << QString::number(line[i + 2]) << " ";
        }
        stream << "\n";
        if ((y & 0x1F) == 0) QCoreApplication::processEvents();
    }
    return true;
}

bool ImageFileHandler::savePNM(const QImage& image, const QString& filePath, const QString& magic, bool ascii) {
    QString m = magic.toLower();

    // Binary P4/P5/P6: rely on QImage::save (Qt6 writes PNM in binary by default)
    if (!ascii && (m == "p4" || m == "p5" || m == "p6")) {
        if (!image.save(filePath)) {
            QMessageBox::warning(nullptr, "Export Error", "Cannot save image: " + filePath);
            return false;
        }
        return true;
    }

    // ASCII conversions: only p1/p2/p3 are ASCII; convert image appropriately and write
    QFile out(filePath);
    if (!out.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Export Error", "Cannot save image: " + filePath);
        return false;
    }

    bool ok = false;
    if (m == "p1") {
        QImage mono = image.convertToFormat(QImage::Format_Mono);
        ok = writeAsciiPBM(mono, out);
    } else if (m == "p2") {
        QImage g = image.convertToFormat(QImage::Format_Grayscale8);
        ok = writeAsciiPGM(g, out);
    } else if (m == "p3") {
        QImage rgb = image.convertToFormat(QImage::Format_RGB888);
        ok = writeAsciiPPM(rgb, out);
    } else if (m == "p4" || m == "p5" || m == "p6") {
        // fallback to binary save if ascii==false was not requested but user still selected.
        ok = image.save(filePath);
        if (!ok) QMessageBox::warning(nullptr, "Export Error", "Cannot save image: " + filePath);
    } else {
        // Unknown magic, fallback to normal save
        ok = image.save(filePath);
        if (!ok) QMessageBox::warning(nullptr, "Export Error", "Cannot save image: " + filePath);
    }

    out.close();
    return ok;
}
