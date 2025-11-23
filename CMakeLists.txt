cmake_minimum_required(VERSION 3.3)

project(lab_qt_5 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Ustawienie ścieżek include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    set(SOURCES
        sources/main.cpp
        sources/filters/PointOperationFilter.cpp
        sources/filters/GrayscaleFilter.cpp
        sources/filters/SaturationAdjustmentFilter.cpp
        sources/filters/NegativeFilter.cpp
        sources/filters/BrightnessAdjustmentFilter.cpp
        sources/filters/ContrastAdjustmentFilter.cpp
        sources/converters/PNMImageConverter.cpp
        sources/converters/ImageFormatConverter.cpp
        sources/io/ImageFileHandler.cpp
        sources/core/ImageProcessor.cpp
        sources/core/FilterFactory.cpp
        sources/gui/mainwindow.cpp
    )

    set(HEADERS
        headers/filters/ImageFilterInterface.h
        headers/filters/PointOperationFilter.h
        headers/filters/GrayscaleFilter.h
        headers/filters/SaturationAdjustmentFilter.h
        headers/filters/NegativeFilter.h
        headers/filters/BrightnessAdjustmentFilter.h
        headers/filters/ContrastAdjustmentFilter.h
        headers/converters/ImageConverterInterface.h
        headers/converters/PNMImageConverter.h
        headers/converters/ImageFormatConverter.h
        headers/io/ImageFileHandlerInterface.h
        headers/io/ImageFileHandler.h
        headers/core/ImageProcessor.h
        headers/core/FilterFactory.h
        headers/gui/mainwindow.h
    )

    qt_add_executable(lab_qt_5
        MANUAL_FINALIZATION
        ${SOURCES}
        ${HEADERS}
        ${FORMS}
        resource.qrc
        mainwindow.ui
        readme.md
    )

    # KLUCZOWE: Ustawienie ścieżek include dla targetu
    target_include_directories(lab_qt_5 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/headers
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
    )

else()
    if(ANDROID)
        add_library(lab_qt_5 SHARED
            ${SOURCES}
            ${HEADERS}
            ${FORMS}
        )
    else()
        add_executable(lab_qt_5
            ${SOURCES}
            ${HEADERS}
            ${FORMS}
        )
    endif()

    # Dla Qt5 również ustawiamy ścieżki include
    target_include_directories(lab_qt_5 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/headers
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
    )
endif()

target_link_libraries(lab_qt_5 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.lab_qt_5)
endif()

set_target_properties(lab_qt_5 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS lab_qt_5
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(lab_qt_5)
endif()
