#include "counttime.h"
#include <QTimer>
#include <QDebug>

extern int countDown;
//处理涂胶计时
countTime::countTime(QObject *parent) :
    QObject(parent)
{
    this->moveToThread(&thread);
    thread.start();
    stopFlag = true;
    countManage();
}


void countTime::countStart()
{
    stopFlag = false;
    qDebug()<<"countStart!";
}

void countTime::countStop()
{
    stopFlag = true;
    qDebug()<<"countStop!";
}


void countTime::countManage()
{
    if(!stopFlag){
        qDebug()<<"countManage:"<<countDown;
        countDown++;
    }
    QTimer::singleShot(1000,this,SLOT(countManage()));
}
