#include "mainwindow.h"
#include <QApplication>
#include "listensockethelper.h"
#include <QFileDialog>
#include "listenthread.h"

extern QString path;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    ListenSocketHelper listenSocketHelper;
    int err;
    listenSocketHelper.initSocket(err);
    ListenSocketHelper::path = "./";
    ListenThread *listenThread = new ListenThread();
    listenThread->start();

    return a.exec();
}
