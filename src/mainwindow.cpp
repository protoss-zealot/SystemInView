#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "shelldialog.h"
#include "network.h"
#include "inputDev.h"
#include "fs.h"

#include <QLabel>
#include <QTimer>
#include <QProgressBar>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <string.h>
#include <sys/statfs.h>
#include <stdlib.h>

#include <QDir>
#include <QMessageBox>
#include "dataclass.h"

using namespace std;

/*Global Object Variables Declaration*/
CpuInfo cpu_info;
MemInfo mem_infoKB,mem_infoMB;
LoadInfo load_info;
BasicInfo basic_info;
DiskInfo disk_info;

QString pid;
int row;


/*Function Front Declaration*/
extern void TestBasicInfo();
extern void TestCpuInfo();
extern void TestMemInfo();
extern void TestLoadInfo();
extern void TestDiskInfo();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QTableWidget *pGPUtable = new QTableWidget(4,7,0);
    //QTableWidget *pDisktable = new QTableWidget(4,6,0);

    QTimer *testtimer = new QTimer(this);

    TestBasicInfo();
    ui->ker_ver_contex->setText(basic_info.kernel_version.c_str());
    ui->hostname_contex->setText(basic_info.hostname.c_str());
    ui->process_contex->setText(basic_info.cpu_name.c_str());

   // ui->tabWidget->addTab(new QLabel,"CpuInfo");
    DisplayCpuInfo();
   // DisplayProcess();
    DisplayProTable();

    initializeNetTab();

    initializeDevTab();
    DisplayDevInfo();
    //DisplayNetInfo();
   /*
    ui->tabWidget->addTab(new QLabel,"HardDisk");
    ui->tabWidget->addTab(pGPUtable,"GpuInfo");
    ui->tabWidget->addTab(new QLabel,"Power");
    ui->tabWidget->addTab(new QLabel,"I/Otest ");
    */
    //TestDiskInfo();



    //ui->disk_total_num->setText(disk_info.capacity);
    //ui->disk_free_num->setText(disk_info.free);

    //ui->progressBar->setRange(0,100);
    //ui->progressBar->setValue((int)disk_info.used_pecent);

    DisplayDiskInfo();

    ui->tabWidget->setTabsClosable(false);

    connect(testtimer,SIGNAL(timeout()),this,SLOT(DisplayLoadInfo()));
    connect(testtimer,SIGNAL(timeout()),this,SLOT(DisplayMemInfo()));
    connect(testtimer,SIGNAL(timeout()),this,SLOT(DisplayNetInfo()));
    //connect(testtimer,SIGNAL(timeout()),this,SLOT(DisplayProTable()));

    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(removeSubTab(int)));
    connect(ui->actionReboot,SIGNAL(activated()),this,SLOT(reboot()));
    connect(ui->actionExit,SIGNAL(activated()),this,SLOT(close()));
    connect(ui->actionGroup,SIGNAL(activated()),this,SLOT(about()));
    connect(ui->pingButton,SIGNAL(clicked()),this,SLOT(shelldialog()));
    //connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(DisplayCpuInfo()));
    // connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(DisplayMemInfo()));


    testtimer->start(1000);     //start timer of loadinfo every 1 second
    // connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(DisplayLoadInfo()));
    // connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(close()));
}

void MainWindow::DisplayDiskInfo()
{
    int totalNum;
    struct fsInfo fsi[MAXDISKNUM];
    getFSinfo(fsi,&totalNum);

    ui->diskwidget->setRowCount(totalNum);

    int index;
    for(index=0;index<totalNum;index++)
    {

        ui->diskwidget->setItem(index,1,new QTableWidgetItem(fsi[index].fsName));
        ui->diskwidget->setItem(index,2,new QTableWidgetItem(fsi[index].mountPt));
        ui->diskwidget->setItem(index,3,new QTableWidgetItem(fsi[index].filesystem));
        //ui->diskwidget->setItem(index,4,new QTableWidgetItem(fsi[index].size));
    }

    for(index=0;index<totalNum;index++)
    {
        QString psize = QString(QLatin1String(fsi[index].size));
        double disksize = psize.toDouble();
        disksize = disksize/(1024*1024*1024);
        psize = QString::number(disksize,10.0);
        ui->diskwidget->setItem(index,4,new QTableWidgetItem(psize.toStdString().c_str()));
    }


    QVector<QProgressBar*>  pbt;
    for(index=0;index<totalNum;index++)
    {
        QProgressBar *tmp = new QProgressBar();
        tmp->setRange(0,100);
        QString qsize = QString(QLatin1String(fsi[index].size));
        QString qavail= QString(QLatin1String(fsi[index].avail));

        double size = qsize.toDouble();
        double avail = qavail.toDouble();
        double ratio;
        if(size!=0)
            ratio = 100*(size-avail)/size;
        else
            ratio = 0.0;

        tmp->setValue((int)ratio);;
        pbt.append(tmp);
    }

    for(index=0;index<totalNum;index++)
    {
        ui->diskwidget->setCellWidget(index,0,pbt.at(index));
    }
}

void MainWindow::DisplayCpuInfo()
{
     TestCpuInfo();
     QTableWidget *pCPUtable = new QTableWidget(cpu_info.CpuNum,4,0);
     ui->tabWidget->addTab(pCPUtable,"CpuInfo");

     /*设置表格的行列抬头*/
     QStringList labels_h,labels_v;

     labels_h.append("Num");
     labels_h.append("VendorID");
     labels_h.append("Freq(MHz)");
     labels_h.append("Cache");

     for(int j=0;j<cpu_info.CpuNum;j++)
         labels_v.append("Processor");

     pCPUtable->setHorizontalHeaderLabels(labels_h);
     pCPUtable->setVerticalHeaderLabels(labels_v);

     /*设置列的宽度*/
     pCPUtable->setColumnWidth(0,100);
     pCPUtable->setColumnWidth(1,150);
     pCPUtable->setColumnWidth(2,150);
     pCPUtable->setColumnWidth(3,240);

     for(int i=0;i<cpu_info.CpuNum;i++)
     {
         ostringstream o_temp;
         o_temp<<i;

         pCPUtable->setItem(i,0,new QTableWidgetItem(o_temp.str().c_str()));
         pCPUtable->setItem(i,1,new QTableWidgetItem(cpu_info.name.c_str()));
         pCPUtable->setItem(i,2,new QTableWidgetItem(cpu_info.freq.c_str()));
         pCPUtable->setItem(i,3,new QTableWidgetItem(cpu_info.CapOfCache.c_str()));

     }


}

void MainWindow::initializeNetTab()
{
    ui->nettable->setColumnCount(7);
    QStringList labels_h;

    //labels_h.append("Name");
    labels_h.append("IP");
    labels_h.append("MAC ADDR");
    labels_h.append("MTU");
    labels_h.append("BytesRcv");
    labels_h.append("BytesTrans");
    labels_h.append("PackRcv");
    labels_h.append("PackTrans");

    ui->nettable->setHorizontalHeaderLabels(labels_h);
    ui->nettable->horizontalHeader()->show();


    ui->nettable->setColumnWidth(0,150);
    ui->nettable->setColumnWidth(1,200);
    ui->nettable->setColumnWidth(2,100);
    ui->nettable->setColumnWidth(3,100);
    ui->nettable->setColumnWidth(4,100);
    ui->nettable->setColumnWidth(5,100);
    ui->nettable->setColumnWidth(6,100);
}


/*  real time detect    */
void MainWindow::DisplayNetInfo()
{
    int NumOfCards;
    int j;


    struct netCardInfo netcards[MAXINTERFACES];

    getNetCardInfo(netcards,&NumOfCards);

    //printf("flag is %d\n",flag);

    /*
    QTableWidget *pNettable = new QTableWidget(NumOfCards,7,0);
    ui->tabWidget->addTab(pNettable,"NetWorks");

    QStringList labels_h,labels_v;

    //labels_h.append("Name");
    labels_h.append("IP");
    labels_h.append("MAC ADDR");
    labels_h.append("MTU");
    labels_h.append("BytesRcv");
    labels_h.append("BytesTrans");
    labels_h.append("PackRcv");
    labels_h.append("PackTrans");

    for(j=0;j<NumOfCards;j++)
        labels_v.append(netcards[j].Name);

    pNettable->setHorizontalHeaderLabels(labels_h);
    pNettable->setVerticalHeaderLabels(labels_v);


    pNettable->setColumnWidth(0,150);
    pNettable->setColumnWidth(1,200);
    pNettable->setColumnWidth(2,100);
    pNettable->setColumnWidth(3,100);
    pNettable->setColumnWidth(4,100);
    pNettable->setColumnWidth(5,100);
    pNettable->setColumnWidth(6,100);

    */
    ui->nettable->setRowCount(NumOfCards);

    QStringList labels_v;

    for(j=0;j<NumOfCards;j++)
        labels_v.append(netcards[j].Name);

    ui->nettable->setVerticalHeaderLabels(labels_v);

    ui->processtable->verticalHeader()->show();

    for(j=0;j<NumOfCards;j++)
    {
        // pNettable->setItem(i,0,new QTableWidgetItem(netcard[j].Name));
        ui->nettable->setItem(j,0,new QTableWidgetItem(netcards[j].IP));
        ui->nettable->setItem(j,1,new QTableWidgetItem(netcards[j].MacAdd));

        //char *temp = QString::number(netcards[j].MTU).toStdString().c_str();
        ui->nettable->setItem(j,2,new QTableWidgetItem(QString::number(netcards[j].MTU).toStdString().c_str()));
        //temp = QString::number(netcards[j].BytesRcv).toStdString().c_str();
        ui->nettable->setItem(j,3,new QTableWidgetItem(QString::number(netcards[j].BytesRcv).toStdString().c_str()));
        //temp = QString::number(netcards[j].BytesTrans).toStdString().c_str();
        ui->nettable->setItem(j,4,new QTableWidgetItem(QString::number(netcards[j].BytesTrans).toStdString().c_str()));
        //temp = QString::number(netcards[j].PackRcv).toStdString().c_str();
        ui->nettable->setItem(j,5,new QTableWidgetItem(QString::number(netcards[j].PackRcv).toStdString().c_str()));
        //temp = QString::number(netcards[j].PackTrans).toStdString().c_str();
        ui->nettable->setItem(j,6,new QTableWidgetItem(QString::number(netcards[j].PackTrans).toStdString().c_str()));
    }

}


void MainWindow::DisplayMemInfo()
{
    TestMemInfo();
    ui->memwidget->setItem(0,0,new QTableWidgetItem(mem_infoKB.MemTotal.c_str()));
    ui->memwidget->setItem(0,1,new QTableWidgetItem(mem_infoKB.MemFree.c_str()));
    ui->memwidget->setItem(0,2,new QTableWidgetItem(mem_infoKB.Buffers.c_str()));
    ui->memwidget->setItem(0,3,new QTableWidgetItem(mem_infoKB.Cached.c_str()));
    ui->memwidget->setItem(0,4,new QTableWidgetItem(mem_infoKB.SwapTotal.c_str()));
    ui->memwidget->setItem(0,5,new QTableWidgetItem(mem_infoKB.SwapFree.c_str()));

    ui->memwidget->setItem(1,0,new QTableWidgetItem(mem_infoMB.MemTotal.c_str()));
    ui->memwidget->setItem(1,1,new QTableWidgetItem(mem_infoMB.MemFree.c_str()));
    ui->memwidget->setItem(1,2,new QTableWidgetItem(mem_infoMB.Buffers.c_str()));
    ui->memwidget->setItem(1,3,new QTableWidgetItem(mem_infoMB.Cached.c_str()));
    ui->memwidget->setItem(1,4,new QTableWidgetItem(mem_infoMB.SwapTotal.c_str()));
    ui->memwidget->setItem(1,5,new QTableWidgetItem(mem_infoMB.SwapFree.c_str()));
}


void MainWindow::DisplayLoadInfo()
{
    TestLoadInfo();
    ui->lcdNumber_2->display(load_info.m_OneMin);
    ui->lcdNumber_3->display(load_info.m_FivMin);
    ui->lcdNumber_1->display(load_info.m_FiftMin);
}


void MainWindow::initializeDevTab()
{
    ui->devtable->setColumnCount(4);
    QStringList labels_h;

    //labels_h.append("Dev Name");
    labels_h.append("Name");
    labels_h.append("Phys");
    labels_h.append("Type");
    labels_h.append("Status");

    ui->devtable->setHorizontalHeaderLabels(labels_h);
    ui->devtable->horizontalHeader()->show();


    ui->devtable->setColumnWidth(0,200);
    ui->devtable->setColumnWidth(1,200);
    ui->devtable->setColumnWidth(2,200);
    ui->devtable->setColumnWidth(3,200);
}

void MainWindow::DisplayDevInfo()
{

    struct inputDevInfo idevinfo[MAXIDEVNUM];
     int devNum;
    getInputDev(idevinfo,&devNum);

    ui->devtable->setRowCount(devNum);

    int j;
    for(j=0;j<devNum;j++)
    {

        ui->devtable->setItem(j,0,new QTableWidgetItem(idevinfo[j].name));
        ui->devtable->setItem(j,1,new QTableWidgetItem(idevinfo[j].phys));

        //char *temp = QString::number(netcards[j].MTU).toStdString().c_str();
        switch(idevinfo[j].type)
        {
            case 0:
                ui->devtable->setItem(j,2,new QTableWidgetItem("KEYBOARD DEVICE"));
                break;
            case 1:
                ui->devtable->setItem(j,2,new QTableWidgetItem("JOYSTICK DEVICE"));
                break;
            case 2:
                ui->devtable->setItem(j,2,new QTableWidgetItem("MOUSE DEVICE"));
                break;
            case 3:
                ui->devtable->setItem(j,2,new QTableWidgetItem("SPEAKER DEVICE"));
                break;
            case 4:
                ui->devtable->setItem(j,2,new QTableWidgetItem("OTHER DEVICE"));
                break;
            default:
                   break;
        }
        //temp = QString::number(netcards[j].BytesRcv).toStdString().c_str();
        ui->devtable->setItem(j,3,new QTableWidgetItem("TRUE"));

    }

}


void MainWindow::DisplayProTable()
{
    /*设置宽度的代码似乎没有用*/
    ui->processtable->setColumnWidth(0,100);//pid
    ui->processtable->setColumnWidth(1,150);//name
    ui->processtable->setColumnWidth(2,100);//stats
    ui->processtable->setColumnWidth(3,100);//nice
    ui->processtable->setColumnWidth(4,150);//mem
    QStringList qsName;
    qsName.push_back("PID");
    qsName.push_back("Name");
    qsName.push_back("Status");
    qsName.push_back("PRI");
    qsName.push_back("Mem");

    QString tempStr;
    QFile tempFile;
    ui->processtable->clear();
    QDir qd("/proc");
    QStringList qsList = qd.entryList();
    QString qs = qsList.join("\n");
    QString id_of_pro;
    bool ok;
    int find_start = 3;
    int a, b;
    int i=0;
    int nProPid; //进程PID
    int number_of_sleep = 0, number_of_run = 0, number_of_zombie = 0;
    int totalProNum = 0; //进程总数
    QString proName; //进程名
    QString proState; //进程状态
    QString proPri; //进程优先级
    QString proMem; //进程占用内存

    while(true)
    {
        a = qs.indexOf("\n",find_start);
        b = qs.indexOf("\n", a+1);
        find_start = b;
        id_of_pro = qs.mid(a+1, b-a-1);
        totalProNum++;
        nProPid = id_of_pro.toInt(&ok, 10);
        if(!ok)
        {
            break;
        }
    }

    ui->processtable->setHorizontalHeaderLabels(qsName);

    ui->processtable->setRowCount(--totalProNum);
    ui->processtable->horizontalHeader()->show();
    //ui->processtable->verticalHeader()->show();

    find_start = 3;

    while (1)
    {
        //获取进程PID
        a = qs.indexOf("\n", find_start);
        b = qs.indexOf("\n", a+1);
        find_start = b;
        id_of_pro = qs.mid(a+1, b-a-1);

        nProPid = id_of_pro.toInt(&ok, 10);
        if(!ok)
        {
            break;
        }

        //打开PID所对应的进程状态文件
        tempFile.setFileName("/proc/" + id_of_pro + "/stat");
        if ( !tempFile.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(this, tr("warning"), tr("The pid stat file can not open!"), QMessageBox::Yes);
            return;
        }
        tempStr = tempFile.readLine();
        if (tempStr.length() == 0)
        {
            break;
        }
        a = tempStr.indexOf("(");
        b = tempStr.indexOf(")");
        proName = tempStr.mid(a+1, b-a-1);
        proName.trimmed(); //删除两端的空格
        proState = tempStr.section(" ", 2, 2);
        proPri = tempStr.section(" ", 17, 17);
        proMem = tempStr.section(" ", 22, 22);


        switch ( proState.at(0).toLatin1() )
        {
            case 'S':   number_of_sleep++; break; //Sleep
            case 'R':   number_of_run++; break; //Running
            case 'Z':   number_of_zombie++; break; //Zombie
            default :   break;
        }


        ui->processtable->setItem(i,0,new QTableWidgetItem(id_of_pro.toStdString().c_str()));
        ui->processtable->setItem(i,1,new QTableWidgetItem(proName.toStdString().c_str()));
        ui->processtable->setItem(i,2,new QTableWidgetItem(proState.toStdString().c_str()));
        ui->processtable->setItem(i,3,new QTableWidgetItem(proPri.toStdString().c_str()));
        ui->processtable->setItem(i,4,new QTableWidgetItem(proMem.toStdString().c_str()));

        i++;
        tempFile.close(); //关闭该PID进程的状态文件

    }
    /*
    QString temp;
    temp = QString::number(totalProNum, 10);
    ui->label_pNum->setText(temp);
    temp = QString::number(number_of_run, 10);
    ui->label_pRun->setText(temp);
    temp = QString::number(number_of_sleep, 10);
    ui->label_pSleep->setText(temp);
    temp = QString::number(number_of_zombie, 10);
    ui->label_pZombie->setText(temp);
    */
    QString temp_num;
    temp_num = QString::number(totalProNum,10);
    ui->total_num->setText(temp_num);
    temp_num = QString::number(number_of_run,10);
    ui->run_num->setText(temp_num);
    temp_num = QString::number(number_of_sleep,10);
    ui->sleep_num->setText(temp_num);
    temp_num = QString::number(number_of_zombie,10);
    ui->zom_num->setText(temp_num);


}

void TestBasicInfo()
{
    ifstream infile("/proc/version");
    string word;    //Maybe string should be QString
    infile>>word;
    basic_info.kernel_version = word;

    infile>>word;
    infile>>word;
    basic_info.kernel_version = basic_info.kernel_version+" "+word;


    ifstream infile2("/proc/sys/kernel/hostname");
    infile2>>word;
    basic_info.hostname = word;

    /*这里麒麟显示不出来，因为信息头字符不一样*/
    ifstream infile3("/proc/cpuinfo");

    while(infile3>>word)
    {
        if(word=="name")
        {
            infile3>>word;
            infile3>>word;
            basic_info.cpu_name = word; //intel

            while(infile3>>word&&word!="stepping")
            {

                    basic_info.cpu_name+=" ";
                    basic_info.cpu_name+= word; //pentium

            }
            break;

        }

    }

}


void TestCpuInfo()
{
    //getcpuinfo
    ifstream infile("/proc/cpuinfo");
    string word;
    cpu_info.CpuNum = 0;


    while(infile>>word)
    {
        if(word=="processor")
        {
            cpu_info.CpuNum++;
            continue;
        }

        if(word=="vendor_id")
        {
            infile>>word;
            infile>>word;
            cpu_info.name = word;
            continue;
        }
        if(word=="MHz")
        {
            infile>>word;
            infile>>word;
            cpu_info.freq = word;
            continue;
        }
        /*kylin changed to other databases*/
        if(word=="cache")
        {
            infile>>word;
            infile>>word;
            infile>>word;
            cpu_info.CapOfCache = word+" KB";
            continue;
        }
        /*continue*/
    }
}


void TestMemInfo()
{
    ifstream infile("/proc/meminfo");
    string word;
    double db_temp=0;

    /*search the function to switch between string and int*/

    while(infile>>word)
    {

        istringstream in_temp;
        ostringstream o_temp;

        if(word=="MemTotal:")
        {
            infile>>word;
            mem_infoKB.MemTotal = word;
            in_temp.clear();
            in_temp.str(word);
            in_temp>>db_temp;
            o_temp<<(db_temp/1024);
            mem_infoMB.MemTotal = o_temp.str();
            continue;
        }
        if(word=="MemFree:")
        {
            infile>>word;
            mem_infoKB.MemFree = word;
            in_temp.clear();
            in_temp.str(word);
            in_temp>>db_temp;
            o_temp<<(db_temp/1024);
            mem_infoMB.MemFree = o_temp.str();
            continue;
        }
        if(word=="Buffers:")
        {
            infile>>word;
            mem_infoKB.Buffers = word;
            in_temp.clear();
            in_temp.str(word);
            in_temp>>db_temp;
            o_temp<<(db_temp/1024);
            mem_infoMB.Buffers = o_temp.str();
            continue;
        }
        if(word=="Cached:")
        {
            infile>>word;
            mem_infoKB.Cached = word;
            in_temp.clear();
            in_temp.str(word);
            in_temp>>db_temp;
            o_temp<<(db_temp/1024);
            mem_infoMB.Cached =o_temp.str() ;
            continue;
        }
        if(word=="SwapTotal:")
        {
            infile>>word;
            mem_infoKB.SwapTotal = word;
            in_temp.clear();
            in_temp.str(word);
            in_temp>>db_temp;
            o_temp<<(db_temp/1024);
            mem_infoMB.SwapTotal = o_temp.str();
            continue;
        }
        if(word=="SwapFree:")
        {
            infile>>word;
            mem_infoKB.SwapFree = word;
            in_temp.clear();
            in_temp.str(word);
            in_temp>>db_temp;
            o_temp<<(db_temp/1024);
            mem_infoMB.SwapFree = o_temp.str();
            continue;
        }
    }
}

void TestLoadInfo()
{
    ifstream infile("/proc/loadavg");
    string word;
    double db_temp;
    istringstream in_temp;

    infile>>word;
    in_temp.clear();
    in_temp.str(word);
    in_temp>>db_temp;

    load_info.m_OneMin = db_temp ;

    infile>>word;
    in_temp.clear();
    in_temp.str(word);
    in_temp>>db_temp;

    load_info.m_FivMin = db_temp;

    infile>>word;
    in_temp.clear();
    in_temp.str(word);
    in_temp>>db_temp;

    load_info.m_FiftMin = db_temp;
}


void TestDiskInfo()
{
    struct statfs diskInfo;
    statfs("/",&diskInfo);
    unsigned long long blocksize = diskInfo.f_bsize;
    unsigned long long totalsize = blocksize*diskInfo.f_blocks;

    unsigned long long freesize = diskInfo.f_bfree*blocksize;

    disk_info.capacity = QString::number(totalsize/(1024.0*1024.0*1024.0))+" GB";
    disk_info.free = QString::number(freesize/(1024.0*1024.0*1024.0))+" GB";
    disk_info.used_pecent = (double)(100*(diskInfo.f_blocks-diskInfo.f_bfree))/diskInfo.f_blocks;

}


void MainWindow::removeSubTab(int index)
{
    ui->tabWidget->removeTab(index);
}


void MainWindow::about()
{
    AboutDialog *dbm = new AboutDialog();
    dbm->show();
}

void MainWindow::shelldialog()
{
    ShellDialog *ptr = new ShellDialog();
    //ptr->pCmd = pingCmd;
    ptr->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_finish_clicked()
{
    string killCmd = "kill -9 ";
    killCmd.append(pid.toStdString().c_str());
    cout<<killCmd<<endl;
    const char *cmd = killCmd.c_str();
    system(cmd);

    /*  Delete process row    */
    ui->processtable->removeRow(row);
}

void MainWindow::on_processtable_itemClicked(QTableWidgetItem* item)
{
    QTableWidgetItem *temp = ui->processtable->item(item->row(),0);
    pid = temp->text();
    //cout<<pid.toStdString()<<endl;
    row = temp->row();
}


void MainWindow::reboot()
{
    system("reboot");
}

/*  Get mod information */
/*
void getModInfo(char store[], int i, char modName[], char memUse[], char times[])
{
    int j = 0;
    int cflags = 0; //记录读取的回车键个数以便判断行数
    int k = 0;
    char name2[25];
    char mem2[20];
    char times2[5];
    while (cflags < i - 1)
    {
        if (store[j] == '\n')
            cflags++; //回车数加1
        j++;
    }
    while (store[j] != ' ')
    { //读取进程名
        name2[k++] = store[j];
        j++;
    }
    name2[k] = '\0';
    j++; //跳转到下一个不是空格的地方
    k = 0;
    while (store[j] != ' ')
    {
        mem2[k++] = store[j];
        j++;
    }
    mem2[k] = '\0'; //封口
    j++;
    times2[0] = store[j]; //读取模块的使用次数
    times2[1] = '\0'; //封口
    strcpy(modName, name2);
    strcpy(memUse, mem2);
    strcpy(times, times2);
}

*/

/* can kao  */
/*
void set_modul_info()
{
    // 原来是这个函数中出现某种问题，
    // 删除这个函数后可以出现所有控件
    // 于是我干脆把那个函数直接整合到这个函数中来
    // 结果可以运行来！
    // 我认为可能是将控件作为函数参数传递出的问题
    //读取当前进程信息，并显示到列表框中
    //add_modules_view(clist_modul);
    // 原来用的是5000，出现问题，于是变小，可以
    char infoBuf[2000]; //暂存读取的modules文件内容
    int fd = open("/proc/modules", O_RDONLY);
    read(fd, infoBuf, sizeof(infoBuf));
    close(fd);
    unsigned int lines = 0;
    unsigned int i = 0;
    gtk_clist_clear(clist_modul);
    // 先取得行数
    while (i != sizeof(infoBuf) / sizeof(char))
    {
        if (infoBuf[i] == '\n')
            lines++;
        i++;
    }
    i = 0;
    for (i = 1; i <= lines; i++)
    {
        char convert_mem[25];
        char modName[25]; //模块名
        char memUse[20]; //内存量
        char times[5]; //使用次数
        int mem_num;
        float real_mem;
        getModInfo(infoBuf, i, modName, memUse, times);
        mem_num = atoi(memUse);
        real_mem = (float) mem_num / (1024);
        gcvt(real_mem, 3, convert_mem);
        gchar *list[1][3] =
        {
        { modName, convert_mem, times } };
        gtk_clist_append((GtkCList*) clist_modul, list[0]);
        gtk_clist_thaw((GtkCList *) clist_modul); //更新list列表显示
    }
}
*/

/*
void MainWindow::on_pingButton_clicked()
{
    if(ui->ipLine->text().isEmpty())
    {
        QMessageBox::warning(this, tr("warning"), tr("Please input ipaddress!"), QMessageBox::Yes);
    }
    QString ipaddress = ui->ipLine->text();
    pingCmd = "ping " + ipaddress;
}
*/
