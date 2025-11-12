#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <qimage.h>
#include <qpixmap.h>
#include <QMessageBox>
#include <QFile>
#include <QByteArray>
#include <QDebug>

class imageUtils
{
    QMessageBox msg;
public:
    QPixmap pixmap;
    imageUtils();
    ~imageUtils();

    bool Open(QString filename);

    bool Convert(QString filename);
private:
};

#endif // IMAGEUTILS_H
