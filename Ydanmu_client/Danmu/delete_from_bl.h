#ifndef DELETE_FROM_BL_H
#define DELETE_FROM_BL_H

#include <QDialog>
#include <QTreeWidget>

namespace Ui {
class Delete_from_bl;
}

class Delete_from_bl : public QDialog
{
    Q_OBJECT

public:
    explicit Delete_from_bl(QWidget *parent = 0);
    void Show_blacklist();
    //QTreeWidget *Dfb_treeWidget;
    ~Delete_from_bl();

private slots:


    void on_Dfb_button_clicked();

private:
    Ui::Delete_from_bl *ui;
};

#endif // DELETE_FROM_BL_H
