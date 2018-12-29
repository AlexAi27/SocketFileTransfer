#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "listensockethelper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_selectFileButton_clicked();

private:
    Ui::MainWindow *ui;
    ListenSocketHelper listenSocketHelper;
    QString path;
};

#endif // MAINWINDOW_H
