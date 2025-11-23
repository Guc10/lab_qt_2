#include "headers/core/FilterFactory.h"
#include "headers/filters/GrayscaleFilter.h"
#include "headers/filters/SaturationAdjustmentFilter.h"
#include "headers/filters/NegativeFilter.h"
#include "headers/filters/BrightnessAdjustmentFilter.h"
#include "headers/filters/ContrastAdjustmentFilter.h"

std::unique_ptr<IImageFilter> FilterFactory::createGrayscaleFilter() {
    return std::make_unique<GrayscaleFilter>();
}

std::unique_ptr<IImageFilter> FilterFactory::createSaturationFilter(double factor) {
    return std::make_unique<SaturationAdjustmentFilter>(factor);
}

std::unique_ptr<IImageFilter> FilterFactory::createNegativeFilter() {
    return std::make_unique<NegativeFilter>();
}

std::unique_ptr<IImageFilter> FilterFactory::createBrightnessFilter(int adjustment) {
    return std::make_unique<BrightnessAdjustmentFilter>(adjustment);
}

std::unique_ptr<IImageFilter> FilterFactory::createContrastFilter(double factor) {
    return std::make_unique<ContrastAdjustmentFilter>(factor);
}
