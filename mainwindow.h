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
    void writeToFile();
    void showEntry();
    void saveEntry();
    void showInList();
    void swapEntry(int i, int  j);
    ~MainWindow();

private slots:
    void on_btnExec_clicked();

    void on_btnIcon_clicked();

    void on_editIcon_textChanged(const QString &arg1);
    
    void on_btnNew_clicked();
    
    void on_lstEntry_itemClicked(QListWidgetItem *item);
    
    void on_btnDelete_clicked();
    
    void on_btnSave_clicked();
    
    void on_btnAddRefresh_clicked();
    
    void on_btnUp_clicked();
    
    void on_btnDown_clicked();
    
private:
    Ui::MainWindow *ui;
    QJsonArray *entries;
    int currentRow;
};

#endif // MAINWINDOW_H
