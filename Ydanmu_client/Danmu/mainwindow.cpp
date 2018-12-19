#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <Danmu.h>
#include <QMessageBox>
#include <QDesktopWidget>
#include <add_to_bl.h>
#include <delete_from_bl.h>
#include <QSqlDatabase>
#include <QCloseEvent>
#include <QDebug>
#include <logindialog.h>
#include <QTime>
//两个静态类型变量的初始化
QTcpSocket* MainWindow::textTcpSocket = new QTcpSocket();
QSqlDatabase MainWindow::db = QSqlDatabase();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //弹幕数据获取
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QDesktopWidget* desktopWidget;                                    //获取桌面设备
    desktopWidget = QApplication::desktop();                   //获取桌面设备
    screenRect = desktopWidget->screenGeometry();

    connect(textTcpSocket,SIGNAL(readyRead()),this,SLOT(receiveNews()));

}

void MainWindow::receiveNews(){
    QByteArray s = textTcpSocket->readAll();
    QString out = QVariant(s).toString();
    Danmu *danmu = new Danmu();
    qDebug()<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")<<"收到弹幕："<<out;
    danmu->Show_Danmu(out,Danmu_color,screenRect);
}

void MainWindow::on_Color_Box_currentIndexChanged(int index)
{
    switch(index){
    case 0:
        Danmu_color = "Red";
        break;
    case 1:
        Danmu_color = "White";
        break;
    case 2:
        Danmu_color = "Blue";
        break;
    case 3:
        Danmu_color = "Green";
        break;
    case 4:
        Danmu_color = "Yellow";
        break;
    case 5:
        Danmu_color = "Pink";
        break;
    case 6:
        Danmu_color = "DPurple";
        break;
    case 7:
        Danmu_color = "Brown";
        break;
    case 8:
        Danmu_color = "Grass";
        break;
    case 9:
        Danmu_color = "DBlue";
        break;
    case 10:
        Danmu_color = "DYellow";
        break;
    case 11:
        Danmu_color = "LBlue";
        break;
    }
}

void MainWindow::on_Add_clicked()
{
    Add_to_bl w2;
    w2.setFixedSize(659,679);
    w2.Show_danmu();
    w2.exec();
}

void MainWindow::on_Delete_clicked()
{
    Delete_from_bl w3;
    w3.setFixedSize(832,709);
    w3.Show_blacklist();
    w3.exec();
}

//当关闭mainwindow窗口时，触发
void MainWindow::closeEvent(QCloseEvent *event){
    //Debug("点了关闭了");
    textTcpSocket->abort();
    textTcpSocket->close();
    textTcpSocket->disconnectFromHost();
    db.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Db_connect()
{
    //连接数据库
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    //db.setHostName("118.89.240.74");
    db.setDatabaseName("ydanmu_system");
    db.setUserName("root");
    db.setPassword("541536638yy.");
    //db.setPassword("541536638Yy.");
    bool ok = db.open();
    if(ok){
        qDebug()<<"connect to database successfully";
    }else{
        QMessageBox::warning(NULL,tr("warning"),tr("can not connect to database"));
    }
}

void MainWindow::Socket_connect()
{
    //连接服务器
    //textTcpSocket = new QTcpSocket();
    textTcpSocket -> abort();
    textTcpSocket->connectToHost("127.0.0.1", 23333);
    //textTcpSocket->connectToHost("118.89.240.74", 45876);
}

