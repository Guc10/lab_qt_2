#include "headers/gui/mainwindow.h"
#include "../../ui_mainwindow.h"
#include "headers/core/FilterFactory.h"
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
    ui->desaturation_slider->setValue(0);

    ui->horizontalSlider->setRange(50, 150);
    ui->horizontalSlider->setValue(100);

    ui->horizontalSlider_2->setRange(-100, 100);
    ui->horizontalSlider_2->setValue(0);

    ui->horizontalSlider_3->setRange(0, 200);
    ui->horizontalSlider_3->setValue(100);

    updateImageInfo();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupConnections() {
    connect(ui->actionopen, &QAction::triggered, this, &MainWindow::onActionOpenTriggered);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onActionSaveTriggered);
    connect(ui->actionExport, &QAction::triggered, this, &MainWindow::onActionExportTriggered);
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::onActionCloseTriggered);
    connect(ui->actionclear, &QAction::triggered, this, &MainWindow::onActionClearTriggered);

    connect(ui->desaturation_slider, &QSlider::valueChanged, this, &MainWindow::onGrayscaleSliderValueChanged);
    connect(ui->checkBox, &QCheckBox::checkStateChanged, this, &MainWindow::onNegativeCheckboxStateChanged);
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::onContrastSliderValueChanged);
    connect(ui->horizontalSlider_2, &QSlider::valueChanged, this, &MainWindow::onBrightnessSliderValueChanged);
    connect(ui->horizontalSlider_3, &QSlider::valueChanged, this, &MainWindow::onSaturationSliderValueChanged);

    connect(ui->Operations, &QTabWidget::currentChanged, this, &MainWindow::onOperationsTabChanged);
}

void MainWindow::updateImageInfo() {
    if (m_imageProcessor.hasImage()) {
        QFileInfo fileInfo(m_imageProcessor.getCurrentPath());
        QImage image = m_imageProcessor.getCurrentImage();

        QString infoText = QString(
                               "File Name: %1\n"
                               "Image Size: %2 x %3 pixels\n"
                               "File Format: %4\n"
                               "File Size: %5 bytes\n"
                               "Modified: %6"
                               ).arg(fileInfo.fileName())
                               .arg(image.width()).arg(image.height())
                               .arg(fileInfo.suffix().toUpper())
                               .arg(fileInfo.size())
                               .arg(m_imageProcessor.hasUnsavedChanges() ? "Yes" : "No");

        ui->imageType->setText(infoText);
    } else {
        ui->imageType->setText("No image loaded\n\nPlease open an image file\nusing File → Open");
    }
}

void MainWindow::updateImageDisplay() {
    if (m_imageProcessor.hasImage()) {
        QImage image = m_imageProcessor.getCurrentImage();
        QPixmap pixmap = QPixmap::fromImage(image);

        pixmap = pixmap.scaled(ui->imageLabel->width(), ui->imageLabel->height(),
                               Qt::KeepAspectRatio, Qt::SmoothTransformation);

        ui->imageLabel->setPixmap(pixmap);
        updateImageInfo();

        ui->actionSave->setEnabled(m_imageProcessor.hasUnsavedChanges());
        ui->actionExport->setEnabled(true);
    } else {
        ui->imageLabel->clear();
        ui->imageLabel->setText("No image loaded");
        updateImageInfo();

        ui->actionSave->setEnabled(false);
        ui->actionExport->setEnabled(false);
    }
}

void MainWindow::onActionOpenTriggered() {
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Open Image", "", "Images (*.png *.jpg *.jpeg *.bmp *.pnm *.pbm *.pgm *.ppm)");

    if (!filePath.isEmpty()) {
        if (m_imageProcessor.loadImage(filePath)) {
            ui->desaturation_slider->setValue(0);
            ui->checkBox->setChecked(false);
            ui->horizontalSlider->setValue(100);
            ui->horizontalSlider_2->setValue(0);
            ui->horizontalSlider_3->setValue(100);

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
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    "Export Image", "", "Images (*.png *.jpg *.jpeg *.bmp *.pnm)");

    if (!filePath.isEmpty()) {
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

void MainWindow::onActionClearTriggered() {
    m_imageProcessor.resetToOriginal();
    updateImageDisplay();
}

void MainWindow::onGrayscaleSliderValueChanged(int value) {
    if (!m_imageProcessor.hasImage()) return;

    m_imageProcessor.resetToOriginal();

    if (value > 0) {
        double intensity = value / 100.0;
        auto filter = FilterFactory::createSaturationFilter(1.0 - intensity);
        m_imageProcessor.applyFilter(std::move(filter));
    }

    updateImageDisplay();
}

void MainWindow::onNegativeCheckboxStateChanged(int state) {
    if (!m_imageProcessor.hasImage()) return;

    m_imageProcessor.resetToOriginal();

    if (state == Qt::Checked) {
        auto filter = FilterFactory::createNegativeFilter();
        m_imageProcessor.applyFilter(std::move(filter));
    }

    updateImageDisplay();
}

void MainWindow::onContrastSliderValueChanged(int value) {
    if (!m_imageProcessor.hasImage()) return;

    m_imageProcessor.resetToOriginal();

    double contrast = value / 100.0;
    auto filter = FilterFactory::createContrastFilter(contrast);
    m_imageProcessor.applyFilter(std::move(filter));

    updateImageDisplay();
}

void MainWindow::onBrightnessSliderValueChanged(int value) {
    if (!m_imageProcessor.hasImage()) return;

    m_imageProcessor.resetToOriginal();

    auto filter = FilterFactory::createBrightnessFilter(value);
    m_imageProcessor.applyFilter(std::move(filter));

    updateImageDisplay();
}

void MainWindow::onSaturationSliderValueChanged(int value) {
    if (!m_imageProcessor.hasImage()) return;

    m_imageProcessor.resetToOriginal();

    double saturation = value / 100.0;
    auto filter = FilterFactory::createSaturationFilter(saturation);
    m_imageProcessor.applyFilter(std::move(filter));

    updateImageDisplay();
}

void MainWindow::onOperationsTabChanged(int index) {
    Q_UNUSED(index)
}
