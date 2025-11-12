#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "imageutils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MIT image manipulation programme 6.7");
}

MainWindow::~MainWindow()
{
    delete image;
    delete ui;
}

void MainWindow::on_actionopen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Image", "", "PBM (*.pbm);; PGM (*.pgm);; PPM (*.ppm)");

    if(image->Open(filename)){
        this->setWindowTitle(filename);
        ui->imageLabel->setPixmap(image->pixmap);
        ui->imageLabel->adjustSize();

        sendStatus("File loaded...", 3000);
        updateStatus(image->type);
    }else{
        sendStatus("Task failed...", 0);
    }
}


void MainWindow::on_actionSave_triggered()
{
    if(!image->pixmap.isNull()){
        QString filename = QFileDialog::getSaveFileName(this, "Save image", "", "PBM (*.pbm);; PGM (*.pgm);; PPM (*.ppm)");

        bool success = image->Convert(filename);

        if(success){
            sendStatus("Saved successfully", 3000);
        }
    }else{
        sendStatus("Canvas is empty - there is nothing to save", 5000);
    }
}


void MainWindow::on_actionClose_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionclear_triggered()
{
    if(!image->pixmap.isNull()){
        ui->imageLabel->clear();
        image->pixmap = QPixmap();
        updateStatus(0);

        sendStatus("Canvas cleared successfully", 3000);
    }else{
        sendStatus("Window is already cleared...", 2000);
    }
}

void MainWindow::sendStatus(QString params, int period){
    statusBar()->clearMessage();
        statusBar()->showMessage(params, period);
}

void MainWindow::updateStatus(int type){
    switch(type){
    case 1:
        ui->imageType->setText("p1");
        break;
    case 2:
        ui->imageType->setText("p2");
        break;
    case 3:
        ui->imageType->setText("p3");
        break;
    case 4:
        ui->imageType->setText("p4");
        break;
    case 5:
        ui->imageType->setText("p5");
        break;
    case 6:
        ui->imageType->setText("p6");
        break;
    default:
        ui->imageType->setText("");
        break;
    }
}
