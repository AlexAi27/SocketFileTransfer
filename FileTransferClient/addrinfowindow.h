#ifndef ADDRINFOWINDOW_H
#define ADDRINFOWINDOW_H

#include <QMainWindow>
#include "connectsockethelper.h"

namespace Ui {
class AddrinfoWindow;
}

class AddrinfoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddrinfoWindow(QWidget *parent = 0);
    ~AddrinfoWindow();

private slots:
    void on_pushButton_clicked();

signals:
    void initializingFinished();

private:
    ConnectSocketHelper connectSocket;
    Ui::AddrinfoWindow *ui;
};

#endif // ADDRINFOWINDOW_H
