#include "delete_from_bl.h"
#include "ui_delete_from_bl.h"
#include <QListWidget>
#include <QTableWidget>
#include <QTreeWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QTableWidget>
#include <QWidgetItem>
#include <logindialog.h>

Delete_from_bl::Delete_from_bl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Delete_from_bl)
{
    ui->setupUi(this);
}

void Delete_from_bl::Show_blacklist()
{
    //添加头部
    ui->Dfb_treeWidget->setColumnCount(2);
    QStringList treeWidget_header;
    treeWidget_header<<tr("    被禁原因")<<tr("被禁时间");
    ui->Dfb_treeWidget->setHeaderLabels(treeWidget_header);

    //添加内容
    QString t_name = loginDialog::teacher_name;
    QString bl_name = t_name + "_blacklist";
    QString str = QString("SELECT * FROM %1 order by add_date").arg(bl_name);
    QSqlQuery query;
    query.exec(str);  //返回结果集
    while(query.next()){                    //query指向下一条记录，每执行一次该函数，便指向相邻的下一条记录
        QStringList Delete_content;
        QString Str1 = query.value("add_date").toString();
        QString Str2 = Str1.replace("T"," ");
        Delete_content<<(query.value("b_content").toString())<<Str2;
        QTreeWidgetItem *bl = new QTreeWidgetItem(Delete_content);
        ui->Dfb_treeWidget->addTopLevelItem(bl);
    }

}

Delete_from_bl::~Delete_from_bl()
{
    delete ui;
}



void Delete_from_bl::on_Dfb_button_clicked()
{
    QString t_name = loginDialog::teacher_name;
    QString bl_name = t_name + "_blacklist";

    //获取当前选中弹幕的行号，根据行号获取当前弹幕在blacklist中的b_ID
    int q = ui->Dfb_treeWidget->currentIndex().row();
    int ID = q + 1;

    //根据b_ID在blacklist中删除该条信息
    QString str1 = QString("DELETE FROM %1 where b_ID = ?").arg(bl_name);
    QSqlQuery dl;
    dl.prepare(str1);
    dl.addBindValue(ID);
    dl.exec();

    //刷新表中显示的内容
    ui->Dfb_treeWidget->clear();
    this->Show_blacklist();

    //修改其之后所有数据的b_ID将其减小1
    QString str2 = QString("SELECT COUNT(b_userID) as count from %1").arg(bl_name);
    QSqlQuery c_query;
    c_query.exec(str2);
    c_query.next();
    int count = c_query.value("count").toInt();         //总数
    int from_ID = ID + 1;
    int end_ID = count + 1;
    if(from_ID <= end_ID){          //假如删除的不是最后一条
        for(int i = from_ID;i <= end_ID;i++){
            QString str3 = QString("UPDATE %1 set b_ID = ? where b_ID = ?").arg(bl_name);
            QSqlQuery updata_query;
            updata_query.prepare(str3);
            updata_query.addBindValue(i-1);
            updata_query.addBindValue(i);
            updata_query.exec();
        }
     }

    //更新表的自增长ID
    QString str4 = QString("ALTER TABLE %1 AUTO_INCREMENT = 1").arg(bl_name);
    QSqlQuery updata_ID;
    updata_ID.exec(str4);
}
