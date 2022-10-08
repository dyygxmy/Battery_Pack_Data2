#include "updatetime.h"
#include <QTimer>
#include "GlobalVarible.h"
#include <QDebug>
#include <QProcess>

updateTime::updateTime(QObject *parent) :
    QThread(parent)
{
    this->start();
    updateTimeFunc();
}

void updateTime::run()
{
    qDebug()<<"updateTime run";
//    updateTimeFunc();//thread里面可以打开函数，但信号槽无法绑定
}


void updateTime::updateTimeFunc()
{
    if(GUpdateTimeFlag){
        GUpdateTimeFlag = false;
        qDebug()<<"killDateStart";
        //kill掉原date -s进程
        QProcess psPro;
        psPro.start("ps");
        QString text  = "";
        QString delPID = "";
        if(!psPro.waitForFinished()){
            qDebug()<<"QProcess:"<<psPro.readAllStandardError();
        }else{
            text = psPro.readAllStandardOutput();//获取所有进程信息
            QStringList lineTextList = text.split("\n");//分割所有进程信息为行
            for(int i = 0;i<lineTextList.size();i++){
                QString lineText =  lineTextList.at(i);
                lineText = lineText.trimmed();
                if(lineText.contains("date -s")){
                    lineText.replace(QRegExp("[\\s]+"), " ");//多余空格变一个空格
                    QStringList oneList = lineText.split(" ");//用空格分割
                    delPID = oneList.at(0);//提取第一个即为PID
                    QString killBash = "kill -9 " + delPID;
                    qDebug()<<"kill date -s == "+ killBash +":";
                    if(delPID.size() > 0){
                        system(killBash.toLatin1().data());
                    }
                    break;
                }
            }
        }
        qDebug()<<"setDateStart";
        system((QString("date -s \"") +GUpdateTimeVal+QString("\" &")).toLatin1().data());
        //将系统时间写入RTC
        qDebug()<<"setHwclockStart";
        system("hwclock -w &");
        qDebug()<<"GetCurrentTime success "<<GUpdateTimeVal;
    }
    QTimer::singleShot(5000,this,SLOT(updateTimeFunc()));
}
