#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>

const QString MENUEXTENSIONSPATH = QDir::homePath() + "/.config/deepin/dde-file-manager/menuextensions/", MENUEXTENSIONS = "menuextensions.json";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentRow(-1)
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
        this->showInList();
    }
    catch (const char *e) {
        QMessageBox::warning(this, "提示", e, QMessageBox::Ok);
        this->entries = new QJsonArray();
    }
}

void MainWindow::writeToFile() {
    QDir dir;
    if (!dir.exists(MENUEXTENSIONSPATH)) {
        dir.mkdir(MENUEXTENSIONSPATH);
    }
    QFile file(MENUEXTENSIONSPATH + MENUEXTENSIONS);
    try {
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) throw "无法保存菜单文件，请检查权限";
        QJsonDocument jsonDocument(*this->entries);
        QByteArray byteArray = jsonDocument.toJson();
        file.write(byteArray);
        file.close();
    }
    catch (const char *e) {
        QMessageBox::warning(this, "提示", e, QMessageBox::Ok);
    }
}

void MainWindow::showInList() {
    this->ui->lstEntry->clear();
    for (auto it = this->entries->constBegin(); it != this->entries->constEnd(); ++it) {
        QJsonObject jsonObject = (*it).toObject();
        this->ui->lstEntry->addItem(jsonObject.find("Text[zh_CN]")->toString());
    }
    if (this->ui->lstEntry->count() == 0) {
        this->currentRow = -1;
    }
    else if (this->currentRow >= 0 && this->currentRow < this->ui->lstEntry->count()) {
        this->ui->lstEntry->setCurrentRow(this->currentRow);
    }
    else {
        this->ui->lstEntry->setCurrentRow(this->currentRow = 0);
    }
    this->showEntry();
}

void MainWindow::showEntry() {
    this->currentRow = this->ui->lstEntry->currentRow();
    if (this->currentRow < 0) return;
    QJsonObject jsonObject = this->entries->at(this->currentRow).toObject();
    this->ui->editText->setText(jsonObject.find("Text[zh_CN]")->toString());
    this->ui->editExec->setText(jsonObject.find("Exec")->toString());
    this->ui->editIcon->setText(jsonObject.find("Icon")->toString());
    QJsonArray notShowIn = jsonObject.find("NotShowIn")->toArray();
    this->ui->chkShowInDesktop->setCheckState(Qt::CheckState(notShowIn.contains("Desktop"))? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    QJsonArray args = jsonObject.find("Args")->toArray();
    QString argsStr = "";
    if (!args.empty()) argsStr = args.at(0).toString();
    for (int i = 1; i < args.count(); ++i) {
        argsStr.append(" ");
        argsStr.append(args.at(i).toString());
    }
    this->ui->editArgs->setText(argsStr);
}

void MainWindow::saveEntry() {
    if (this->currentRow == -1) return;
    QJsonArray notShowIn, args;
    notShowIn.append(this->ui->chkShowInDesktop->checkState() == Qt::CheckState::Checked ? "Desktop" : "");
    QStringList argsl = this->ui->editArgs->text().split(" ");
    for (auto arg : argsl) {
        args.append(arg);
    }
    QJsonObject jsonObject;
    jsonObject.insert("MenuType", "EmptyArea");
    jsonObject.insert("Text[zh_CN]", this->ui->editText->text());
    jsonObject.insert("Exec", this->ui->editExec->text());
    jsonObject.insert("Args", args);
    jsonObject.insert("NotShowIn", notShowIn);
    jsonObject.insert("Icon", this->ui->editIcon->text());
    this->entries->replace(this->currentRow, jsonObject);
    this->showInList();
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
    QJsonArray notShowIn, args;
    notShowIn.append("Desktop");
    args.append(".");
    QJsonObject jsonObject;
    jsonObject.insert("MenuType", "EmptyArea");
    jsonObject.insert("Text[zh_CN]", "新菜单项");
    jsonObject.insert("Exec", "");
    jsonObject.insert("Args", args);
    jsonObject.insert("NotShowIn", notShowIn);
    jsonObject.insert("Icon", "");
    this->entries->append(jsonObject);
    this->showInList();
    this->writeToFile();
}

void MainWindow::on_lstEntry_itemClicked(QListWidgetItem *item)
{
    this->showEntry();
}

void MainWindow::on_btnDelete_clicked()
{
    if (this->currentRow == -1) return;
    this->entries->removeAt(this->currentRow);
    this->showInList();
    this->writeToFile();
}

void MainWindow::on_btnSave_clicked()
{
    this->saveEntry();
    this->writeToFile();
}

void MainWindow::on_btnAddRefresh_clicked()
{
    QJsonArray notShowIn, args;
    notShowIn.append("");
    args.append("");
    QJsonObject jsonObject;
    jsonObject.insert("MenuType", "EmptyArea");
    jsonObject.insert("Text[zh_CN]", "刷新");
    jsonObject.insert("Exec", "");
    jsonObject.insert("Args", args);
    jsonObject.insert("NotShowIn", notShowIn);
    jsonObject.insert("Icon", "");
    this->entries->append(jsonObject);
    this->showInList();
    this->writeToFile();
}

void MainWindow::swapEntry(int i, int  j) {
    if (i > j) std::swap(i, j);
    QJsonValue temp = this->entries->at(i);
    this->entries->removeAt(i);
    this->entries->insert(j, temp);
    this->showInList();
    this->writeToFile();
}

void MainWindow::on_btnUp_clicked()
{
    if (this->currentRow <= 0) return;
    this->currentRow--;
    swapEntry(this->currentRow, this->currentRow + 1);
}

void MainWindow::on_btnDown_clicked()
{
    if (this->currentRow >= this->ui->lstEntry->count() - 1) return;
    this->currentRow++;
    swapEntry(this->currentRow, this->currentRow - 1);
}
