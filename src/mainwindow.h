#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:

    void on_btn_finish_clicked();
    void on_processtable_itemClicked(QTableWidgetItem* item);
    void removeSubTab(int index);
    void about();
    void shelldialog();
    void DisplayCpuInfo();
    void DisplayMemInfo();
    void DisplayLoadInfo();
    //void DisplayProcess();
    void DisplayProTable();
    void DisplayNetInfo();
    void initializeNetTab();
    void initializeDevTab();
    void DisplayDevInfo();
    void DisplayDiskInfo();
    void reboot();
};

#endif // MAINWINDOW_H
