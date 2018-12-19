#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <Danmu.h>
#include <QDesktopWidget>
#include <QTcpSocket>
#include <QChar>
#include <iostream>
#include <add_to_bl.h>
#include <QSqlDatabase>
#include <logindialog.h>
//#include <QPluginLoader>
//void loadMySqlDriver();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loginDialog logdlg;
    logdlg.setFixedSize(400,300);
    MainWindow w;
    w.Db_connect();
    w.Socket_connect();
    if(logdlg.exec() == QDialog::Accepted){
        w.setWindowFlags(w.windowFlags()&~Qt::WindowMinMaxButtonsHint|Qt::WindowMinimizeButtonHint);
        w.setFixedSize(433,330);
        w.show();
        return a.exec();
    }
    else{
        return 0;
    }
}

//void loadMySqlDriver() {
//    QPluginLoader loader;
//    // MySQL 驱动插件的路径
//    loader.setFileName("F:/QT/5.5/mingw492_32/plugins/sqldrivers/qsqlmysqld.dll");
//    qDebug() << loader.load();
//    qDebug() << loader.errorString();
//}
