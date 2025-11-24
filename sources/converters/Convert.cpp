#include "headers/converters/Convert.h"

#include "headers/converters/PNMImageConverter.h"
#include "headers/converters/ImageFormatConverter.h"

std::unique_ptr<IImageConverter> Convert::convertNormalImage() {
    return std::make_unique<PNMImageConverter>();
}

std::unique_ptr<IImageConverter> Convert::convertPNMImage() {
    return std::make_unique<ImageFormatConverter>();
}
