#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <qimage.h>
#include <qpixmap.h>

#include <QMessageBox>
#include <QByteArray>
#include <QTextStream>
#include <QColor>
#include <QFile>

enum Type{
    p1 = 1,
    p2 = 2,
    p3 = 3,
    p4 = 4,
    p5 = 5,
    p6 = 6
};

class imageUtils
{
    QMessageBox msg;
public:
    QPixmap pixmap;
    imageUtils();
    ~imageUtils();

    Type type;

    bool Open(QString filename);
    bool Convert(QString filename);
private:
};

#endif // IMAGEUTILS_H
