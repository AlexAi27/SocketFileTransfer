#include "mainwindow.h"
#include <QApplication>
#include "addrinfowindow.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AddrinfoWindow addrW;
    addrW.show();
    MainWindow w;
    QObject::connect(&addrW, &AddrinfoWindow::initializingFinished, &w, &MainWindow::init);

    return a.exec();
}
