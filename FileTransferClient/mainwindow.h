#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connectsockethelper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();

private slots:
    void on_selectFileButton_clicked();

    void on_uploadButton_clicked();

private:
    Ui::MainWindow *ui;
    ConnectSocketHelper connectSocket;
};

#endif // MAINWINDOW_H
