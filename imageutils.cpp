#include "imageutils.h"

imageUtils::imageUtils(){

}

imageUtils::~imageUtils()
{

}


bool imageUtils::Convert(QString filename)
{
    try {
        QImage img = pixmap.toImage();

        switch (type)
        {
        case Type::p1:
        {
            img = img.convertToFormat(QImage::Format_Mono);
            QFile out(filename);
            if(!out.open(QIODevice::WriteOnly))
                return false;

            out.write("P4\n");
            out.write(QByteArray::number(img.width()) + " " + QByteArray::number(img.height()) + "\n");

            for (int y = 0; y < img.height(); ++y) {
                QByteArray line;
                uchar byte = 0;
                int bits = 0;
                for (int x = 0; x < img.width(); ++x) {
                    bool bit = (qGray(img.pixel(x, y)) < 128);
                    byte = (byte << 1) | (bit ? 1 : 0);
                    bits++;
                    if (bits == 8) {
                        line.append(byte);
                        byte = 0;
                        bits = 0;
                    }
                }
                if (bits > 0) {
                    byte <<= (8 - bits);
                    line.append(byte);
                }
                out.write(line);
            }
            out.close();
            break;
        }
        case Type::p4:
        {
            img = img.convertToFormat(QImage::Format_Mono);
            QFile out(filename);
            if(!out.open(QIODevice::WriteOnly | QIODevice::Text))
                return false;

            QTextStream stream(&out);
            stream << "P1\n" << img.width() << " " << img.height() << "\n";
            for (int y = 0; y < img.height(); ++y) {
                for (int x = 0; x < img.width(); ++x)
                    stream << ((qGray(img.pixel(x, y)) < 128) ? 1 : 0) << " ";
                stream << "\n";
            }
            out.close();
            break;
        }
        case Type::p2:
        {
            img = img.convertToFormat(QImage::Format_Grayscale8);
            QFile out(filename);
            if(!out.open(QIODevice::WriteOnly))
                return false;

            out.write("P5\n");
            out.write(QByteArray::number(img.width()) + " " + QByteArray::number(img.height()) + "\n255\n");

            for (int y = 0; y < img.height(); ++y) {
                out.write(reinterpret_cast<const char*>(img.constScanLine(y)), img.width());
            }
            out.close();
            break;
        }
        case Type::p5:
        {
            img = img.convertToFormat(QImage::Format_Grayscale8);
            QFile out(filename);
            if(!out.open(QIODevice::WriteOnly | QIODevice::Text))
                return false;

            QTextStream stream(&out);
            stream << "P2\n" << img.width() << " " << img.height() << "\n255\n";
            for (int y = 0; y < img.height(); ++y) {
                const uchar *line = img.constScanLine(y);
                for (int x = 0; x < img.width(); ++x)
                    stream << QString::number(line[x]) << " ";
                stream << "\n";
            }
            out.close();
            break;
        }
        case Type::p3:
        {
            img = img.convertToFormat(QImage::Format_RGB888);
            QFile out(filename);
            if(!out.open(QIODevice::WriteOnly))
                return false;

            out.write("P6\n");
            out.write(QByteArray::number(img.width()) + " " + QByteArray::number(img.height()) + "\n255\n");
            for (int y = 0; y < img.height(); ++y)
                out.write(reinterpret_cast<const char*>(img.constScanLine(y)), img.width() * 3);
            out.close();
            break;
        }
        case Type::p6:
        {
            img = img.convertToFormat(QImage::Format_RGB888);
            QFile out(filename);
            if(!out.open(QIODevice::WriteOnly | QIODevice::Text))
                return false;

            QTextStream stream(&out);
            stream << "P3\n" << img.width() << " " << img.height() << "\n255\n";
            for (int y = 0; y < img.height(); ++y) {
                const uchar *line = img.constScanLine(y);
                for (int x = 0; x < img.width(); ++x) {
                    int i = x * 3;
                    stream << QString::number(line[i]) << " "
                           << QString::number(line[i + 1]) << " "
                           << QString::number(line[i + 2]) << " ";
                }
                stream << "\n";
            }
            out.close();
            break;
        }

        default:
            return 0;
        }

        return 1;
    } catch (...) {
        msg.setText("Fatal error...");
        msg.exec();
        return 0;
    }
}

bool imageUtils::Open(QString filename){
    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly)){
        msg.setText("Provided file does not exist...");
        msg.exec();
        return 0;
    }
    QByteArray header = f.readLine().trimmed();
    f.close();

    if(header[0] != 'P'){
        msg.setText("The file is either corrupted, has comments or has different extension");
        msg.exec();
        return 0;
    }

    bool ok = true;
    switch (header[1]) {
    case '1': type = Type::p1; break;
    case '2': type = Type::p2; break;
    case '3': type = Type::p3; break;
    case '4': type = Type::p4; break;
    case '5': type = Type::p5; break;
    case '6': type = Type::p6; break;
    default: ok = false; break;
    }

    if (!ok)
        return 0;

    if (!pixmap.load(filename)) {
        msg.setText("Provided file does not exist or cannot be opened...");
        msg.exec();
        return 0;
    }

    return 1;
}
