#include "danmu.h"
#include <QTime>
#include <QPen>
#include <QPainter>
Danmu::Danmu()
{

}

void Danmu::Show_Danmu(QString text, QString color, QRect rect)
{
    Text = text;
    //把对应的颜色字符串转为rgb值
    if(color == "White"){
        qcolor = QColor(255,255,246,255);
    }else if(color =="Red"){
        qcolor = QColor(255,0,0,255);
    }else if(color =="Yellow"){
        qcolor = QColor(254,241,2,255);
    }else if(color == "Green"){
        qcolor = QColor(0,152,67,255);
    }else if(color == "Blue"){
        qcolor = QColor(0,160,234,255);
    }else if(color == "Pink"){
        qcolor = QColor(226,2,127,255);
    }else if(color == "Grass"){
        qcolor = QColor(144,195,32,255);
    }else if(color == "DBlue"){
        qcolor = QColor(0,46,114,255);
    }else if(color == "DYellow"){
        qcolor = QColor(240,171,42,255);
    }else if(color =="DPurple"){
        qcolor = QColor(255,0,255,255);
    }else if(color == "LBlue"){
        qcolor = QColor(129,193,205,255);
    }else if(color =="Brown"){
        qcolor = QColor(149,119,57,255);
    }

    //获取弹幕的起始位置和终止位置
    QFontMetrics metrics(font);
    this->setFixedHeight(metrics.height() + 5);
    this->setFixedWidth(metrics.width(text) + 5);
    int y = qrand()%(rect.height() - metrics.height() - 5);
    int x = rect.width();
    this->move(x,y);

    //设置窗体属性
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFocusPolicy(Qt::NoFocus);
    this->hide();

    //设置窗体运动
    animation = new QPropertyAnimation(this,"pos");  //设置为pos属性
    animation->setDuration(15000);
    animation->setStartValue(QPoint(x,y));
    animation->setEndValue(QPoint(-(this->width()),y));
    animation->setEasingCurve(QEasingCurve::Linear);

    this->show();
    this->repaint();
    animation->start();
}

void Danmu::paintEvent(QPaintEvent *)    //画弹幕
{
    QPainter painter(this);  //以弹幕窗口为画布
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing,true); //设置painter属性为反走样
    QPainterPath path;  //绘图操作的一个容器
    QPen pen(QColor(0,0,0,188)); //画笔样式 r g b 透明度
    pen.setWidth(4);

    //定弹幕基线最左端的坐标，从而确定path的路径
    QFontMetrics metrics(font);
    int font_width = metrics.width(Text);
    int font_height = metrics.height();
    int window_width = this->width();
    int window_height = this->height();
    int x = (window_width - font_width)/2;
    int y = (window_height - font_height)/2 + metrics.ascent();

    //画轮廓，刷字体
    path.addText(x,y,font,Text);
    painter.strokePath(path,pen);
    painter.drawPath(path);
    painter.fillPath(path,QBrush(qcolor));
    painter.restore();
}

