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
    void resetSliders();
    void updateStatusBat(QString param, int period);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // File operations
    void onActionOpenTriggered();
    void onActionSaveTriggered();
    void onActionExportTriggered();
    void onActionCloseTriggered();
    void onActionClearStylingTriggered();
    void onActionClearCanvasTriggered();

    // Filter operations
    void onGrayscaleApply();
    void onNegativeCheckboxStateChange(int state);
    void onContrastApply();
    void onBrightnessApply();
    void onSaturationApply();

    // Tab changes
    void onMenuBarSelection(int index);
};

#endif
