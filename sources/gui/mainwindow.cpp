#include "headers/gui/mainwindow.h"
#include "../../ui_mainwindow.h"
#include "headers/core/FilterFactory.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QApplication>

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
    for (QAction *action : ui->menucolor->actions()) {
        int idx = index;
        connect(action, &QAction::triggered, this, [this, idx]() {
            ui->Operations->setCurrentIndex(idx);
        });
        ++index;
    }
}

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
    if (!image.isNull()) {
        ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    } else {
        ui->imageLabel->clear();
    }

    ui->actionSave->setEnabled(m_imageProcessor.hasImage() && m_imageProcessor.hasUnsavedChanges());
    ui->actionExport->setEnabled(m_imageProcessor.hasImage());

    updateImageInfo();
}

void MainWindow::onActionOpenTriggered() {
    QString filePath = QFileDialog::getOpenFileName(this,
        "Open Image", "", "*.png *.jpg *.jpeg *.bmp *.pnm *.pbm *.pgm *.ppm");

    if (filePath.isEmpty()) return;

    ui->statusbar->showMessage("Loading image...", 2000);
    QApplication::processEvents();

    if (m_imageProcessor.loadImage(filePath)) {
        resetSliders();
        updateImageDisplay();
        ui->statusbar->showMessage("Image loaded", 3000);
    } else {
        QMessageBox::warning(this, "Load Error", "Failed to load image: " + filePath);
        ui->statusbar->showMessage("Failed to load image", 3000);
    }
}

void MainWindow::onActionSaveTriggered() {
    if (!m_imageProcessor.hasImage()) {
        ui->statusbar->showMessage("No image to save", 3000);
        return;
    }

    ui->statusbar->showMessage("Saving image...", 2000);
    QApplication::processEvents();

    if (m_imageProcessor.saveImage()) {
        QMessageBox::information(this, "Success", "Image saved successfully");
        updateImageInfo();
        ui->statusbar->showMessage("Image saved", 3000);
    } else {
        QMessageBox::warning(this, "Error", "Failed to save image");
        ui->statusbar->showMessage("Save failed", 3000);
    }
}

void MainWindow::onActionExportTriggered() {
    if (!m_imageProcessor.hasImage()) {
        ui->statusbar->showMessage("No image to export", 3000);
        return;
    }

    // If current file is PNM, offer ASCII/BINARY p1..p6 variants
    if (m_imageProcessor.isPNM()) {
        QString selectedFilter;
        QString filePath = QFileDialog::getSaveFileName(this, "Export Image", "",
            "PBM Binary Image (*.pbm);;PBM ASCII Image (*.pbm);;PGM Binary Image (*.pgm);;PGM ASCII Image (*.pgm);;PPM Binary Image (*.ppm);;PPM ASCII Image (*.ppm)",
            &selectedFilter);
        if (filePath.isEmpty()) return;

        QString targetMagic;
        bool ascii = false;
        if (selectedFilter.contains("PBM Binary")) targetMagic = "p4";
        else if (selectedFilter.contains("PBM ASCII")) { targetMagic = "p1"; ascii = true; }
        else if (selectedFilter.contains("PGM Binary")) targetMagic = "p5";
        else if (selectedFilter.contains("PGM ASCII")) { targetMagic = "p2"; ascii = true; }
        else if (selectedFilter.contains("PPM Binary")) targetMagic = "p6";
        else if (selectedFilter.contains("PPM ASCII")) { targetMagic = "p3"; ascii = true; }

        ui->statusbar->showMessage("Exporting image...", 0);
        QApplication::processEvents();

        if (m_imageProcessor.exportPNM(filePath, targetMagic, ascii)) {
            QMessageBox::information(this, "Success", "Image exported successfully");
            updateImageInfo();
            ui->statusbar->showMessage("Export finished", 3000);
        } else {
            QMessageBox::warning(this, "Error", "Failed to export image");
            ui->statusbar->showMessage("Export failed", 3000);
        }
        return;
    }

    // Non-PNM exports: let ImageProcessor handle regular export
    QString selected;
    QString filePath = QFileDialog::getSaveFileName(this,
        "Export Image",
        "",
        "PNG Image (*.png);;JPEG Image (*.jpg *.jpeg);;BMP Image (*.bmp)",
        &selected);

    if (filePath.isEmpty()) return;

    ui->statusbar->showMessage("Exporting image...", 0);
    QApplication::processEvents();

    if (m_imageProcessor.exportImage(filePath)) {
        QMessageBox::information(this, "Success", "Image exported successfully");
        updateImageInfo();
        ui->statusbar->showMessage("Export finished", 3000);
    } else {
        QMessageBox::warning(this, "Error", "Failed to export image");
        ui->statusbar->showMessage("Export failed", 3000);
    }
}

void MainWindow::onActionCloseTriggered() {
    if (m_imageProcessor.hasUnsavedChanges()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this,
          "Unsaved Changes", "You have unsaved changes. Are you sure you want to close?",
          QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No) return;
    }
    close();
}

void MainWindow::onActionClearStylingTriggered() {
    if (!m_imageProcessor.hasUnsavedChanges()) {
        updateStatusBat("Nothing to clear", 5000);
    } else {
        m_imageProcessor.resetToOriginal();
        updateStatusBat("Styling cleared", 3000);
    }
    resetSliders();
    updateImageDisplay();
}

void MainWindow::onActionClearCanvasTriggered() {
    if (!m_imageProcessor.hasImage()) {
        updateStatusBat("Canvas is already cleared", 5000);
    } else {
        m_imageProcessor.clearImage();
        updateStatusBat("Canvas cleared", 3000);
    }
    resetSliders();
    updateImageDisplay();
}

void MainWindow::onGrayscaleApply() {
    if (!m_imageProcessor.hasImage()) return;

    int value = ui->desaturation_slider->value();
    if (value <= 0) {
        updateStatusBat("No desaturation applied", 2000);
        return;
    }

    ui->statusbar->showMessage("Applying desaturation...", 0);
    QApplication::processEvents();

    double intensity = value / 100.0;
    auto filter = FilterFactory::createSaturationFilter(1.0 - intensity);
    m_imageProcessor.applyFilter(std::move(filter));

    updateImageDisplay();
    ui->statusbar->showMessage("Desaturation applied", 3000);
}

void MainWindow::onNegativeCheckboxStateChange(int state) {
    if (!m_imageProcessor.hasImage()) return;

    ui->statusbar->showMessage(state == Qt::Checked ? "Applying negative..." : "Removing negative...", 0);
    QApplication::processEvents();

    if (state == Qt::Checked) {
        auto filter = FilterFactory::createNegativeFilter();
        m_imageProcessor.applyFilter(std::move(filter));
    } else {
        m_imageProcessor.resetToOriginal();
    }

    updateImageDisplay();
    ui->statusbar->showMessage("Operation complete", 3000);
}

void MainWindow::onContrastApply() {
    if (!m_imageProcessor.hasImage()) return;

    ui->statusbar->showMessage("Applying contrast...", 0);
    QApplication::processEvents();

    int value = ui->horizontalSlider->value();
    double contrast = value / 100.0;
    auto filter = FilterFactory::createContrastFilter(contrast);
    m_imageProcessor.applyFilter(std::move(filter));

    updateImageDisplay();
    ui->statusbar->showMessage("Contrast applied", 3000);
}

void MainWindow::onBrightnessApply() {
    if (!m_imageProcessor.hasImage()) return;

    ui->statusbar->showMessage("Applying brightness...", 0);
    QApplication::processEvents();

    int value = ui->horizontalSlider_2->value();
    auto filter = FilterFactory::createBrightnessFilter(value);
    m_imageProcessor.applyFilter(std::move(filter));

    updateImageDisplay();
    ui->statusbar->showMessage("Brightness applied", 3000);
}

void MainWindow::onSaturationApply() {
    if (!m_imageProcessor.hasImage()) return;

    ui->statusbar->showMessage("Applying saturation...", 0);
    QApplication::processEvents();

    int value = ui->horizontalSlider_3->value();
    double saturation = value / 100.0;
    auto filter = FilterFactory::createSaturationFilter(saturation);
    m_imageProcessor.applyFilter(std::move(filter));

    updateImageDisplay();
    ui->statusbar->showMessage("Saturation applied", 3000);
}

void MainWindow::onMenuBarSelection(int index) {
    ui->Operations->setCurrentIndex(index);
}

void MainWindow::updateStatusBat(QString param, int period) {
    ui->statusbar->clearMessage();
    ui->statusbar->showMessage(param, period);
}

void MainWindow::resetSliders() {
    onMenuBarSelection(0);
    ui->desaturation_slider->setValue(0);
    ui->checkBox->setChecked(false);
    ui->horizontalSlider->setValue(100);
    ui->horizontalSlider_2->setValue(0);
    ui->horizontalSlider_3->setValue(100);
}