#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>

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

QString MainWindow::showDialog(QString filter) {
    QString curPath = QDir::currentPath();
    QString fileName = QFileDialog::getOpenFileName(this, "选择文件", curPath, filter);
    return fileName;
}

void MainWindow::on_btnExec_clicked()
{
    QString fileName = this->showDialog("文件 (*.*)");
    if (!fileName.isEmpty()) this->ui->editExec->setText(fileName);
}

void MainWindow::on_btnIcon_clicked()
{
    QString fileName = this->showDialog("图标/图片 (*.jpg *.bmp *.png *.svg *.ico)");
    if (!fileName.isEmpty()) this->ui->editIcon->setText(fileName);
}
