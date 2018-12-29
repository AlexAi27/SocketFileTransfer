#include "addrinfowindow.h"
#include "ui_addrinfowindow.h"
#include <QMessageBox>

AddrinfoWindow::AddrinfoWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddrinfoWindow)
{
    ui->setupUi(this);
}

AddrinfoWindow::~AddrinfoWindow()
{
    delete ui;
}

void AddrinfoWindow::on_pushButton_clicked()
{
    QString ipaddr, port;
    ipaddr = ui->Addr0LineEdit->text() + "."
            + ui->Addr1LineEdit->text() + "."
            + ui->Addr2LineEdit->text() + "."
            + ui->Addr3LineEdit->text();
    port = ui->PortLineEdit->text();
    int err = 0;
    try {
        connectSocket.initSocket(ipaddr, port, err);
        emit initializingFinished();
        this->hide();
    } catch (const char *msg) {
        QMessageBox::warning(this, "Error", msg, QMessageBox::Ok);
    }
}
