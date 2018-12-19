#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui {
class registerdialog;
}

class registerdialog : public QDialog
{
    Q_OBJECT

public:
    explicit registerdialog(QWidget *parent = 0);
    ~registerdialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::registerdialog *ui;
};

#endif // REGISTERDIALOG_H
