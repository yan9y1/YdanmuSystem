#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <registerdialog.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <mainwindow.h>
#include <QCloseEvent>

QString loginDialog::teacher_name = QString();
loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);

}

loginDialog::~loginDialog()
{
    delete ui;
}

void loginDialog::on_logpushButton_clicked()
{
    //从登录对话框中获取用户名和密码
    QString username = ui->userlineEdit->text();
    QString password = ui->pwdlineEdit->text();
    teacher_name = username;
    //账号密码验证
    QSqlQuery check_user;
    check_user.prepare("SELECT * FROM teacher_info where username = ?");
    check_user.addBindValue(username);
    check_user.exec();
    if(check_user.next()){
        QString pswd = check_user.value("password").toString();
        if(pswd == password){
            //一旦登陆成功，则关闭登陆对话框，打开mainwindow对话框
            accept();
            //一旦登陆成功，将用户名发送给服务器
            MainWindow::textTcpSocket->write(username.toStdString().c_str(),strlen(username.toStdString().c_str()));
            //一旦登陆成功，清空老师的弹幕表
            QString DanmuListName = teacher_name + "_danmulist";
            QString query = QString("TRUNCATE table %1").arg(DanmuListName);
            QSqlQuery clear;
            clear.exec(query);
        }else{
            QMessageBox::warning(this,tr("警告"),tr("密码错误！"));
        }
    }else{
        QMessageBox::warning(this,tr("警告"),tr("该用户名不存在！"));
    }

}

void loginDialog::on_regpushButton_clicked()
{
    registerdialog regdlg;
    regdlg.exec();
}

void loginDialog::closeEvent(QCloseEvent *event)
{
    MainWindow::textTcpSocket->abort();
    MainWindow::textTcpSocket->close();
    MainWindow::db.close();
}

