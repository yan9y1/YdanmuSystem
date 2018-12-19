#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QCloseEvent>
#include <mainwindow.h>

namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit loginDialog(QWidget *parent = 0);
    static QString teacher_name;
    ~loginDialog();


private slots:
    void on_logpushButton_clicked();
    void on_regpushButton_clicked();

private:
    Ui::loginDialog *ui;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // LOGINDIALOG_H
