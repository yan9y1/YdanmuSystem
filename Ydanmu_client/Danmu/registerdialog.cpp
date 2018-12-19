#include "registerdialog.h"
#include "ui_registerdialog.h"
#include <QSqlQuery>
#include <QMessageBox>

registerdialog::registerdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerdialog)
{
    ui->setupUi(this);
}

registerdialog::~registerdialog()
{
    delete ui;
}

void registerdialog::on_pushButton_clicked()
{
    //获取账号和密码
    QString reg_username = ui->userlineEdit->text();
    QString reg_pwd1 = ui->pwdlineEdit->text();
    QString reg_pwd2 = ui->pwdagainlineEdit->text();
    if(reg_username == ""){
        QMessageBox::warning(this,tr("警告"),tr("用户名不能为空！"));
    }else if(reg_pwd1 == "" || reg_pwd2 == ""){
        QMessageBox::warning(this,tr("警告"),tr("密码不能为空！"));
    }else if(reg_pwd1 != reg_pwd2){
        QMessageBox::warning(this,tr("警告"),tr("两次密码不一致！"));
    }else{
        QSqlQuery reg_query;
        reg_query.prepare("SELECT * FROM teacher_info where userName = ?");
        reg_query.addBindValue(reg_username);
        reg_query.exec();
        if(reg_query.next()){
            QMessageBox::warning(this,tr("警告"),tr("该用户名已存在！"));
        }else{
            QSqlQuery new_user;
            new_user.prepare("INSERT INTO teacher_info (userName,password) VALUES (?,?)");
            new_user.addBindValue(reg_username);
            new_user.addBindValue(reg_pwd1);
            if(new_user.exec()){
                QMessageBox::information(this,tr("提示"),tr("注册成功！"));
                //一旦注册成功，创建这个老师的弹幕信息表和黑名单表
                QString Danmu_list_name = reg_username;
                QString Danmu_list_end = "_danmulist";
                Danmu_list_name += Danmu_list_end;
                QString str1 = QString("CREATE TABLE %1 (SID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,userID VARCHAR(40) NOT NULL,content VARCHAR(300) NOT NULL)").arg(Danmu_list_name);
                QSqlQuery create_msg_list;
                create_msg_list.exec(str1);

                QString bl_list_name = reg_username;
                QString bl_list_end = "_blacklist";
                bl_list_name += bl_list_end;
                QString str2 = QString("CREATE TABLE %1 (b_ID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,b_userID VARCHAR(40) NOT NULL,b_content VARCHAR(300) NOT NULL,add_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP)").arg(bl_list_name);
                QSqlQuery create_bl_list;
                create_bl_list.exec(str2);

            }else{
                QMessageBox::warning(this,tr("警告"),tr("注册失败！"));
            }
        }
    }
}
