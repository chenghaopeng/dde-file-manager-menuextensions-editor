#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

const QString MENUEXTENSIONSPATH = QDir::homePath() + "/.config/deepin/dde-file-manager/menuextensions/", MENUEXTENSIONS = "menuextensions.json";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setIconSize(QSize(32, 32));
    this->readFromFile();
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

void MainWindow::readFromFile() {
    QDir dir;
    if (!dir.exists(MENUEXTENSIONSPATH)) {
        dir.mkdir(MENUEXTENSIONSPATH);
    }
    try {
        QFile file(MENUEXTENSIONSPATH + MENUEXTENSIONS);
        if (!file.exists()) throw "菜单文件不存在，将为您新建";
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) throw "无法打开菜单文件";
        QByteArray byteArray = file.readAll();
        file.close();
        QJsonParseError jsonParseError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(byteArray, &jsonParseError);
        if (jsonParseError.error != QJsonParseError::NoError) throw "菜单文件格式错误";
        if (!jsonDocument.isArray()) throw "菜单文件格式错误";
        this->entries = new QJsonArray(jsonDocument.array());
        //QMessageBox::information(this, "成功", "有" + QString::number(this->entries->size()) + "条");
        for (auto it = this->entries->constBegin(); it != this->entries->constEnd(); ++it) {
            QJsonObject jsonObject = (*it).toObject();
            this->ui->lstEntry->addItem(jsonObject.find("Text[zh_CN]")->toString());
        }
    }
    catch (QString e) {
        QMessageBox::warning(this, "提示", e, QMessageBox::Yes);
        this->entries = new QJsonArray();
    }
}

void MainWindow::showEntry() {
    int row = this->ui->lstEntry->currentRow();
    QJsonObject jsonObject = this->entries->at(row).toObject();
    this->ui->editText->setText(jsonObject.find("Text[zh_CN]")->toString());
    this->ui->editExec->setText(jsonObject.find("Exec")->toString());
    this->ui->editIcon->setText(jsonObject.find("Icon")->toString());
    // TODO
}

void MainWindow::on_btnExec_clicked()
{
    QString fileName = this->showDialog("文件 (*.*)");
    if (!fileName.isEmpty()) this->ui->editExec->setText(fileName);
}

void MainWindow::on_btnIcon_clicked()
{
    QString fileName = this->showDialog("图标 (*.jpg *.bmp *.png *.svg *.ico)");
    if (!fileName.isEmpty()) this->ui->editIcon->setText(fileName);
}

void MainWindow::on_editIcon_textChanged(const QString &arg1)
{
    QFileInfo fileInfo(arg1);
    if (fileInfo.exists()) {
        this->ui->btnIcon->setIcon(QIcon(arg1));
        this->ui->btnIcon->setText("");
    }
    else {
        this->ui->btnIcon->setIcon(QIcon(""));
        this->ui->btnIcon->setText("..");
    }
}

void MainWindow::on_btnNew_clicked()
{
    
}

void MainWindow::on_lstEntry_itemClicked(QListWidgetItem *item)
{
    this->showEntry();
}

void MainWindow::on_lstEntry_currentRowChanged(int currentRow)
{
    this->showEntry();
}
