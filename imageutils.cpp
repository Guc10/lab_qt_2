#include "imageutils.h"

imageUtils::imageUtils(){

}

imageUtils::~imageUtils()
{

}


bool imageUtils::Convert(QString filename)
{
    try {
        QImage image = pixmap.toImage().convertToFormat(QImage::Format_Mono);

        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            msg.setText("Cannot open file for write...");
            msg.exec();
            return 0;
        }

        QByteArray header = "P4\n";
        header += QByteArray::number(image.width()) + " " + QByteArray::number(image.height()) + "\n";
        file.write(header);

        int width = image.width();
        int height = image.height();
        int bytesPerLine = (width + 7) / 8;

        for (int y = 0; y < height; ++y) {
            QByteArray line(bytesPerLine, '\0');
            for (int x = 0; x < width; ++x) {
                int bit = image.pixelIndex(x, y) ? 0 : 1;
                line[x / 8] |= bit << (7 - (x % 8));
            }
            file.write(line);
        }

        file.close();

        return 1;
    } catch (...) {
        msg.setText("Fatal error...");
        msg.exec();
        return 0;
    }
}

bool imageUtils::Open(QString filename){
    pixmap.load(filename);

    if(filename.isEmpty() || pixmap.isNull())
    {
        msg.setText("Provided file does not exist...");
        msg.exec();
        return 0;
    }
    else{
        return 1;
   }
}
