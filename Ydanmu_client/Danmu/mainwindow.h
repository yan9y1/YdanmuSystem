#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <Danmu.h>
#include <QEventLoop>
#include <QCloseEvent>
#include <QSqlDatabase>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QString Danmu_color = "Red";
    static QSqlDatabase db;
    static QTcpSocket *textTcpSocket;
    void Db_connect();
    void Socket_connect();
    //Danmu * danmu;
//    void send_username(QString name);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QEventLoop loop;
    QRect screenRect;

public slots:
    void receiveNews();

private slots:
    void on_Color_Box_currentIndexChanged(int index);
    void on_Add_clicked();
    void on_Delete_clicked();

protected:
    void closeEvent(QCloseEvent *event);
    //void closeEvent();
};

#endif // MAINWINDOW_H
