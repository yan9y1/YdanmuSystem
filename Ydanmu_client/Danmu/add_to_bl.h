#ifndef ADD_TO_BL_H
#define ADD_TO_BL_H

#include <QDialog>
#include <mainwindow.h>
//#include <QCloseEvent>

namespace Ui {
class Add_to_bl;
}

class Add_to_bl : public QDialog
{
    Q_OBJECT

public:
    explicit Add_to_bl(QWidget *parent = 0);
    ~Add_to_bl(); 
    void Show_danmu();

private slots:
    void on_Atb_pushButton_clicked();

private:
    Ui::Add_to_bl *ui;

protected:
    //void closeEvent(QCloseEvent *event);
};

#endif // ADD_TO_BL_H
