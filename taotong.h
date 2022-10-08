#ifndef TAOTONG_H
#define TAOTONG_H

#include <QObject>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <stdio.h>
#include <QTimer>
#include "GlobalVarible.h"
#include <QThread>

#include "inoutput.h"

class TaoTong : public QObject
{
    Q_OBJECT
public:
    explicit TaoTong(QObject *parent = 0);
    InOutput *In_Out_Put;
    QString vari1_type;

signals:
    void taotong_num(int);
    void sendbattery(QString);
//    void sendStopSignal(bool);
public slots:
    void T_start();
    void IO_Collect_timer();
    void rev_TT_IO_Func(int,int,int);
    void rev_IO_Reset();
private:
    QThread m_thread;
    QTimer m_timer;

    int count_num1,count_num2;
    int tt1,tt2,tt3,tt4,IO1,IO2,IO3,IO4,IO5;
    int tt_Value[4];
    int IO_Value[5];
    bool ttSendEnFlag;
    bool IOSendEnFlag;
    bool firstStatusFlag;//
    QString k_power;
    QString k_before;
};

#endif // TAOTONG_H
