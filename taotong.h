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
class TaoTong : public QObject
{
    Q_OBJECT
public:
    explicit TaoTong(QObject *parent = 0);
    
signals:
    void taotong_num(int);
    void sendbattery(QString);
public slots:
    void T_start();
    void taotong_timer();
private:
    QThread m_thread;
    QTimer m_timer;
    int count_num1,count_num2,count_num3,count_num4;
    int k1,k2,k3,k4;
    QString k_power;
    QString k_before;
};

#endif // TAOTONG_H
