#ifndef FILTERFACTORY_H
#define FILTERFACTORY_H

#include <memory>
#include "../filters/ImageFilterInterface.h"

class FilterFactory {
public:
    static std::unique_ptr<IImageFilter> createGrayscaleFilter();
    static std::unique_ptr<IImageFilter> createSaturationFilter(double factor);
    static std::unique_ptr<IImageFilter> createNegativeFilter();
    static std::unique_ptr<IImageFilter> createBrightnessFilter(int adjustment);
    static std::unique_ptr<IImageFilter> createContrastFilter(double factor);
};

#endif
