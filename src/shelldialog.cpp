#include "mainwindow.h"
#include "shelldialog.h"
#include "ui_shelldialog.h"
#include <stdlib.h>
#include <iostream>

using namespace std;


ShellDialog::ShellDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShellDialog)
{
    ui->setupUi(this);
    ui->outinfo->clear();

    ping_running = false;

    cmd = new QProcess;



    //ui->outinfo->setPlainText(output);
    connect(ui->Ping,SIGNAL(clicked()),this,SLOT(on_Ping_clicked()));
    connect(cmd,SIGNAL(readyRead()),this,SLOT(readOutput()));
    connect(ui->End,SIGNAL(clicked()),this,SLOT(on_End_clicked()));
}


void ShellDialog::readOutput()
{
    output += cmd->readAllStandardOutput();
    //cout<<output.toStdString()<<endl;
    ui->outinfo->setText(output);
}


ShellDialog::~ShellDialog()
{
    delete ui;
}

void ShellDialog::on_Ping_clicked()
{
    QString input = ui->lineEdit->text();
    input = "ping " + input;

    cmd->start(input);

    ping_running = true;    /*set ping running flag*/
    output = tr("");
    ui->outinfo->setText(output);   /*show all the messages*/
}

void ShellDialog::on_End_clicked()
{
    Q_PID id = cmd->pid();

    QString temp = QString::number(id,10);
    QString endcmd = "kill -2 "+ temp;

    const char *pend = endcmd.toStdString().c_str();

    /* Send SIGINT signal to the process,end it. */
    if(ping_running)
    {
        system(pend);
        ping_running = false;   /* When ping finished, we could not send SIGINT.*/
    }
}
