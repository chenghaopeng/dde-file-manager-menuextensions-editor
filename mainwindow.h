#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QString showDialog(QString filter);
    void readFromFile();
    void showEntry();
    ~MainWindow();

private slots:
    void on_btnExec_clicked();

    void on_btnIcon_clicked();

    void on_editIcon_textChanged(const QString &arg1);
    
    void on_btnNew_clicked();
    
    void on_lstEntry_itemClicked(QListWidgetItem *item);
    
    void on_lstEntry_currentRowChanged(int currentRow);
    
private:
    Ui::MainWindow *ui;
    QJsonArray *entries;
};

#endif // MAINWINDOW_H
