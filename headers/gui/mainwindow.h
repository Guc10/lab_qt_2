#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QSlider>
#include <QCheckBox>
#include "../core/ImageProcessor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    ImageProcessor m_imageProcessor;

    void setupConnections();
    void updateImageInfo();
    void updateImageDisplay();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // File operations
    void onActionOpenTriggered();
    void onActionSaveTriggered();
    void onActionExportTriggered();
    void onActionCloseTriggered();
    void onActionClearTriggered();

    // Filter operations
    void onGrayscaleSliderValueChanged(int value);
    void onNegativeCheckboxStateChanged(int state);
    void onContrastSliderValueChanged(int value);
    void onBrightnessSliderValueChanged(int value);
    void onSaturationSliderValueChanged(int value);

    // Tab changes
    void onOperationsTabChanged(int index);
};

#endif
