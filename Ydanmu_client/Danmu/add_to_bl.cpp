#include "add_to_bl.h"
#include "ui_add_to_bl.h"
#include "mainwindow.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <logindialog.h>

Add_to_bl::Add_to_bl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_to_bl)
{
    ui->setupUi(this);
}

Add_to_bl::~Add_to_bl()
{
    delete ui;
}

void Add_to_bl::Show_danmu()
{
    QString msg_list_name = loginDialog::teacher_name;
    msg_list_name += "_danmulist";
    QString str = QString("SELECT * FROM %1").arg(msg_list_name);
    QSqlQuery query;
    query.exec(str);   //返回结果集
    if(query.last()){
        ui->Atb_listWidget->addItem(query.value("content").toString());
    }
    while(query.previous()){                    //query指向下一条记录，每执行一次该函数，便指向相邻的下一条记录
        ui->Atb_listWidget->addItem(query.value("content").toString());
    }
}

void Add_to_bl::on_Atb_pushButton_clicked()
{
    QString current_row_content = ui->Atb_listWidget->currentItem()->text();

    QString t_name = loginDialog::teacher_name;
    QString msg_list_name = t_name + "_danmulist";
    QString bl_name = t_name + "_blacklist";

    //查询该弹幕对应的自增长ID
    QString str1 = QString("SELECT COUNT(userID) as count from %1").arg(msg_list_name);
    QSqlQuery count_query;
    count_query.exec(str1);
    count_query.next();
    int count  = count_query.value("count").toInt();
    int ID = count - ui->Atb_listWidget->currentRow();

    //根据自增长ID找到该弹幕对应的openID
    QString str2 = QString("SELECT userID from %1 where SID = ?").arg(msg_list_name);
    QSqlQuery userID_query;
    userID_query.prepare(str2);
    userID_query.addBindValue(ID);
    userID_query.exec();
    userID_query.next();
    QString OpenID = userID_query.value("userID").toString();

    //判断这个人是否已经被拉黑了
    QString str3 = QString("SELECT * from %1 where b_userID = ?").arg(bl_name);
    QSqlQuery if_black;
    if_black.prepare(str3);
    if_black.addBindValue(OpenID);
    if_black.exec();
    if(if_black.next()){
        QMessageBox::information(NULL,"提示","该用户已被禁言过了");
    }
    else{           //否则将该openID和弹幕内容插入blacklist中
        QString str4 = QString("INSERT INTO %1 (b_userID,b_content) values (?,?)").arg(bl_name);
        QSqlQuery insert_bl;
        insert_bl.prepare(str4);
        insert_bl.addBindValue(OpenID);
        insert_bl.addBindValue(current_row_content);
        if(insert_bl.exec()){
            QMessageBox::information(NULL,"提示","禁言成功");
        }
    }
}
