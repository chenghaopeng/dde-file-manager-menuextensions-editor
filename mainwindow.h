#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QString showDialog(QString filter);
    ~MainWindow();

private slots:
    void on_btnExec_clicked();

    void on_btnIcon_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
