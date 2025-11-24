#ifndef CONVERT_H
#define CONVERT_H

#include "ImageConverterInterface.h"

class Convert{
public:
    static std::unique_ptr<IImageConverter> convertNormalImage();
    static std::unique_ptr<IImageConverter> convertPNMImage();
};

#endif // CONVERT_H
