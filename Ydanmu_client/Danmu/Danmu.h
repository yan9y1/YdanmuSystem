#ifndef DANMU_H
#define DANMU_H
#include <QLabel>
#include <QPropertyAnimation>

class Danmu : public QLabel
{
    Q_OBJECT

public:
    Danmu();
    void Show_Danmu(QString text,QString color,QRect rect);

protected:
    void paintEvent(QPaintEvent*);

private:
    QFont font = QFont("SimHei",20,100);
    QColor qcolor;
    QPropertyAnimation *animation;
    QString Text;
};



#endif // DANMU_H
