#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init() {
    this->show();
}



void MainWindow::on_selectFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select File"),"./");
    qDebug() << fileName;
    ui->filePathLineEdit->setText(fileName);
}

void MainWindow::on_uploadButton_clicked()
{
    try {
//        connectSocket.setBuflen(29);
        connectSocket.transferFile(ui->filePathLineEdit->text());
    } catch (const char *msg) {
        QMessageBox::warning(this, "Error", QString::fromLocal8Bit(msg), QMessageBox::Ok);
    }
}
