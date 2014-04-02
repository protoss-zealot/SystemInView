#ifndef SHELLDIALOG_H
#define SHELLDIALOG_H

#include <QDialog>
#include <QString>
#include <QProcess>

namespace Ui {
    class ShellDialog;
}

class ShellDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShellDialog(QWidget *parent = 0);
    ~ShellDialog();
     QString pCmd;
private:
    Ui::ShellDialog *ui;

    QProcess *cmd;
    QString output;       //用于保存输出结果
    bool ping_running;

private slots:
    void on_End_clicked();
    void on_Ping_clicked();
    void readOutput();
};

#endif // SHELLDIALOG_H
