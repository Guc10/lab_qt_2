#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet("QWidget { background-color: #2e2e2e; color: white } #imageLabel { background-color: white; }");
    MainWindow w;
    w.show();
    return a.exec();
}
