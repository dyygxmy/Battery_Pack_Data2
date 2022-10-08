#include "RollSubtitleLable.h"
#include <QDebug>

RollSubtitleLable::RollSubtitleLable(QWidget *parent) :
    QLabel(parent)
{
    curIndex =0;
    charWidth=fontMetrics().width("a");//每个字符的宽度
    QTimer *timer =new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateIndex()));
    //timer->start(300);
}
void RollSubtitleLable::paintEvent(QPaintEvent*event)
{
    QPen pen;
    QPainter painter(this);
    pen.setColor(QColor(255,0,255));
    pen.setStyle(Qt::DashDotDotLine);
    QFont font("楷体",16,QFont::Bold);
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(0,50,showText.mid(curIndex));
    painter.drawText(width()-charWidth*curIndex,50,showText.left(curIndex));
}

void RollSubtitleLable::updateIndex()
{
    update();
    curIndex++;
    if (curIndex*charWidth > width())
        curIndex = 0;
}
void RollSubtitleLable::getText(QString text)
{
    showText = text;
    qDebug()<<"this is getText showText"<<showText;
}
