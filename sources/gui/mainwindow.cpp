#include "headers/gui/mainwindow.h"
#include "../../ui_mainwindow.h"
#include "headers/core/FilterFactory.h"

#include "headers/converters/Convert.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupConnections();

    ui->desaturation_slider->setRange(0, 100);
    ui->horizontalSlider->setRange(50, 150);
    ui->horizontalSlider_2->setRange(-100, 100);
    ui->horizontalSlider_3->setRange(0, 200);

    resetSliders();
    updateImageDisplay();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupConnections() {
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onActionOpenTriggered);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onActionSaveTriggered);
    connect(ui->actionExport, &QAction::triggered, this, &MainWindow::onActionExportTriggered);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onActionCloseTriggered);
    connect(ui->actionClear_styling, &QAction::triggered, this, &MainWindow::onActionClearStylingTriggered);
    connect(ui->actionClear_canvas, &QAction::triggered, this, &MainWindow::onActionClearCanvasTriggered);

    connect(ui->desaturation_btn, &QPushButton::clicked, this, &MainWindow::onGrayscaleApply);
    connect(ui->checkBox, &QCheckBox::checkStateChanged, this, &MainWindow::onNegativeCheckboxStateChange);
    connect(ui->contrast_btn, &QPushButton::clicked, this, &MainWindow::onContrastApply);
    connect(ui->brightness_btn, &QPushButton::clicked, this, &MainWindow::onBrightnessApply);
    connect(ui->saturation_btn, &QPushButton::clicked, this, &MainWindow::onSaturationApply);

    int index = 1;
    foreach (QAction *action, ui->menucolor->actions()) {
        connect(action, &QAction::triggered, this, [this, index]() {
            ui->Operations->setCurrentIndex(index);
        });
        index++;
    }
}

// Updating Image, Image Info

void MainWindow::updateImageInfo() {
    if (m_imageProcessor.hasImage()) {
        QFileInfo fileInfo(m_imageProcessor.getCurrentPath());
        QImage image = m_imageProcessor.getCurrentImage();
        QString magicType = QString("Magic type: p%1").arg(m_imageProcessor.getCurrentMagicType());

        QString infoText = QString(
           "File Name: %1\n"
           "Image Size: %2 x %3 pixels\n"
           "File Format: %4\n"
           "File Size: %5 bytes\n"
           "%6\n"
           "Modified: %7"
           ).arg(fileInfo.fileName())
           .arg(image.width()).arg(image.height())
           .arg(fileInfo.suffix().toUpper())
           .arg(fileInfo.size())
           .arg((m_imageProcessor.getCurrentMagicType() == 0) ? "Not a magic type image" : magicType)
           .arg(m_imageProcessor.hasUnsavedChanges() ? "Yes" : "No");

        ui->imageType->setText(infoText);
    } else {
        ui->imageType->setText("No image loaded\n\nPlease open an image file\nusing File → Open");
    }
}

void MainWindow::updateImageDisplay() {
    QImage image = m_imageProcessor.getCurrentImage();
    QPixmap pixmap = QPixmap::fromImage(image);

    //pixmap = pixmap.scaled(ui->imageLabel->width(), ui->imageLabel->height(),
    //    Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->imageLabel->setPixmap(pixmap);

    if (m_imageProcessor.hasImage()) {
        ui->actionSave->setEnabled(m_imageProcessor.hasUnsavedChanges());
        ui->actionExport->setEnabled(true);
    } else {
        ui->actionSave->setEnabled(false);
        ui->actionExport->setEnabled(false);
    }

    updateImageInfo();
}

// Actions on trigger

void MainWindow::onActionOpenTriggered() {
    QString filePath = QFileDialog::getOpenFileName(this,
        "Open Image", "", "*.png *.jpg *.jpeg *.bmp *.pnm *.pbm *.pgm *.ppm");

    if (!filePath.isEmpty()) {
        if (m_imageProcessor.loadImage(filePath)) {

            resetSliders();
            updateImageDisplay();
        }
    }
}

void MainWindow::onActionSaveTriggered() {
    if (m_imageProcessor.saveImage()) {
        QMessageBox::information(this, "Success", "Image saved successfully");
        updateImageInfo();
    } else {
        QMessageBox::warning(this, "Error", "Failed to save image");
    }
}

void MainWindow::onActionExportTriggered() {
    std::unique_ptr<IImageConverter> converter;
    QString suffix = m_imageProcessor.getCurrentSufix();
    QString selectedFilter;

    QString filePath;

    PNM magicType;
    PNM newMagicType;

    bool conversionNeeded;

    if (m_imageProcessor.isPNM()) {
        magicType = m_imageProcessor.getCurrentMagicType();
        switch (magicType) {
        case 1:
            selectedFilter = "PBM Binary Image (*.pbm);;PBM ASCII Image (*.pbm)";
            break;
        case 2:
            selectedFilter = "PGM Binary Image (*.pgm);;PGM ASCII Image (*.pgm)";
            break;
        case 3:
            selectedFilter = "PPM Binary Image (*.ppm);;PPM ASCII Image (*.ppm)";
            break;
        case 4:
            selectedFilter = "PBM Binary Image (*.pbm);;PBM ASCII Image (*.pbm)";
            break;
        case 5:
            selectedFilter = "PGM Binary Image (*.pgm);;PGM ASCII Image (*.pgm)";
            break;
        case 6:
            selectedFilter = "PPM Binary Image (*.ppm);;PPM ASCII Image (*.ppm)";
            break;
        default:
            break;
        }

        filePath = QFileDialog::getSaveFileName(this,
            "Export Image",
            "",
            selectedFilter,
            &selectedFilter);

        if (filePath.isEmpty()) {
            return;
        }

        if (selectedFilter.contains("PBM Binary")) {
            newMagicType = p4;
            selectedFilter = "p4";
        } else if (selectedFilter.contains("PGM Binary")) {
            newMagicType = p5;
            selectedFilter = "p5";
        } else if (selectedFilter.contains("PPM Binary")) {
            newMagicType = p6;
            selectedFilter = "p6";
        }else if (selectedFilter.contains("PBM ASCII")) {
            newMagicType = p1;
            selectedFilter = "p1";
        } else if (selectedFilter.contains("PGM ASCII")) {
            newMagicType = p2;
            selectedFilter = "p2";
        } else if (selectedFilter.contains("PPM ASCII")) {
            newMagicType = p3;
            selectedFilter = "p3";
        }

        conversionNeeded = (magicType != newMagicType);

        converter = Convert::convertPNMImage();
        m_imageProcessor.convertImage(std::move(converter), selectedFilter, filePath);

        QImage img = m_imageProcessor.getCurrentImage();

        if (selectedFilter == "p1") {
            img = img.convertToFormat(QImage::Format_Mono);
            QFile out(filePath);
            if(!out.open(QIODevice::WriteOnly | QIODevice::Text))
                QMessageBox::warning(nullptr, "Export error",
                                     "Cannot save image: " + filePath);
            QTextStream stream(&out);
            stream << "P1\n" << img.width() << " " << img.height() << "\n";
            for (int y = 0; y < img.height(); ++y) {
                for (int x = 0; x < img.width(); ++x)
                    stream << ((qGray(img.pixel(x, y)) < 128) ? 1 : 0) << " ";
                stream << "\n";
            }
            out.close();
        }else if (selectedFilter == "p2") {
            img = img.convertToFormat(QImage::Format_Grayscale8);
            QFile out(filePath);
            if(!out.open(QIODevice::WriteOnly | QIODevice::Text))
                QMessageBox::warning(nullptr, "Export error",
                                     "Cannot save image: " + filePath);
            QTextStream stream(&out);
            stream << "P2\n" << img.width() << " " << img.height() << "\n255\n";
            for (int y = 0; y < img.height(); ++y) {
                const uchar *line = img.constScanLine(y);
                for (int x = 0; x < img.width(); ++x)
                    stream << QString::number(line[x]) << " ";
                stream << "\n";
            }
            out.close();
        }else if (selectedFilter== "p3") {
            img = img.convertToFormat(QImage::Format_RGB888);
            QFile out(filePath);
            if(!out.open(QIODevice::WriteOnly | QIODevice::Text))
                QMessageBox::warning(nullptr, "Export error",
                                     "Cannot save image: " + filePath);
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
        }else{
            if (m_imageProcessor.exportImage(filePath)) {
                QMessageBox::information(this, "Success", "Image exported successfully");
                updateImageInfo();
            } else {
                QMessageBox::warning(this, "Error", "Failed to export image");
            }
            return;
        }
        QMessageBox::information(this, "Success", "Image exported successfully");
        updateImageInfo();

    } else {
        filePath = QFileDialog::getSaveFileName(this,
            "Export Image",
            "",
            "PNG Image (*.png);;JPEG Image (*.jpg *.jpeg);;BMP Image (*.bmp)",
            &selectedFilter);

        if (filePath.isEmpty()) {
            return;
        }

        if (selectedFilter.contains("*.png")) {
            selectedFilter = "png";
        } else if (selectedFilter.contains("*.jpg") || selectedFilter.contains("*.jpeg")) {
            selectedFilter = "jpg";
        } else if (selectedFilter.contains("*.bmp")) {
            selectedFilter = "bmp";
        }

        conversionNeeded = (selectedFilter != suffix);

        if(conversionNeeded){
            converter = Convert::convertNormalImage();
            m_imageProcessor.convertImage(std::move(converter), selectedFilter, filePath);
        }

        if (m_imageProcessor.exportImage(filePath)) {
            QMessageBox::information(this, "Success", "Image exported successfully");
            updateImageInfo();
        } else {
            QMessageBox::warning(this, "Error", "Failed to export image");
        }
    }
}

void MainWindow::onActionCloseTriggered() {
    if (m_imageProcessor.hasUnsavedChanges()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this,
          "Unsaved Changes", "You have unsaved changes. Are you sure you want to close?",
          QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No) {
            return;
        }
    }
    close();
}

void MainWindow::onActionClearStylingTriggered() {
    if(!m_imageProcessor.hasUnsavedChanges()) updateStatusBat("Nothing to clear", 5000);
    else m_imageProcessor.resetToOriginal();
    resetSliders();
    updateImageDisplay();
}

void MainWindow::onActionClearCanvasTriggered() {
    if(!m_imageProcessor.hasImage()) updateStatusBat("Canvas is already cleared", 5000);
    else m_imageProcessor.clearImage();
    resetSliders();
    updateImageDisplay();
}

// Updating data depending on sliders positions

void MainWindow::onGrayscaleApply() {
    int value = ui->desaturation_slider->value();

    if (!m_imageProcessor.hasImage()) return;

    //m_imageProcessor.resetToOriginal();

    if (value > 0) {
        double intensity = value / 100.0;
        auto filter = FilterFactory::createSaturationFilter(1.0 - intensity);
        m_imageProcessor.applyFilter(std::move(filter));
    }

    updateImageDisplay();
}

void MainWindow::onNegativeCheckboxStateChange(int state) {
    if (!m_imageProcessor.hasImage()) return;

    //m_imageProcessor.resetToOriginal();

    if (state == Qt::Checked) {
        auto filter = FilterFactory::createNegativeFilter();
        m_imageProcessor.applyFilter(std::move(filter));
    }

    updateImageDisplay();
}

void MainWindow::onContrastApply() {
    int value = ui->horizontalSlider->value();

    if (!m_imageProcessor.hasImage()) return;

    //m_imageProcessor.resetToOriginal();

    double contrast = value / 100.0;
    auto filter = FilterFactory::createContrastFilter(contrast);
    m_imageProcessor.applyFilter(std::move(filter));

    updateImageDisplay();
}

void MainWindow::onBrightnessApply() {
    int value = ui->horizontalSlider_2->value();

    if (!m_imageProcessor.hasImage()) return;

    //m_imageProcessor.resetToOriginal();

    auto filter = FilterFactory::createBrightnessFilter(value);
    m_imageProcessor.applyFilter(std::move(filter));

    updateImageDisplay();
}

void MainWindow::onSaturationApply() {
    int value = ui->horizontalSlider_3->value();

    if (!m_imageProcessor.hasImage()) return;

    //m_imageProcessor.resetToOriginal();

    double saturation = value / 100.0;
    auto filter = FilterFactory::createSaturationFilter(saturation);
    m_imageProcessor.applyFilter(std::move(filter));

    updateImageDisplay();
}

// Updating active tabs

void MainWindow::onMenuBarSelection(int index) {
    ui->Operations->setCurrentIndex(index);
}

// Updatig status bar

void MainWindow::updateStatusBat(QString param, int period) {
    ui->statusbar->clearMessage();
    ui->statusbar->showMessage(param, period);
}

// Reseting sliders

void MainWindow::resetSliders() {
    onMenuBarSelection(0);
    ui->desaturation_slider->setValue(0);
    ui->checkBox->setChecked(false);
    ui->horizontalSlider->setValue(100);
    ui->horizontalSlider_2->setValue(0);
    ui->horizontalSlider_3->setValue(100);
}
