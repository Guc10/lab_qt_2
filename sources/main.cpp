#include <QApplication>
#include "headers/gui/mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setApplicationName("Image Processor");
    app.setApplicationVersion("1.0");

    app.setStyleSheet("QWidget { background-color: #2e2e2e; color: white } #imageLabel { background-color: white; }");

    MainWindow window;
    window.show();

    return app.exec();
}
