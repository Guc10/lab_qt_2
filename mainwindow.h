#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "imageutils.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionopen_triggered();

    void on_actionSave_triggered();

    void on_actionClose_triggered();

    void on_actionclear_triggered();

    void sendStatus(QString params, int period);

    void updateStatus(int type);

private:
    Ui::MainWindow *ui;
    QMessageBox msg;
    imageUtils *image = new imageUtils();
};
#endif // MAINWINDOW_H
