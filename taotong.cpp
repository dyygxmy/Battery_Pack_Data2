#include "taotong.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
TaoTong::TaoTong(QObject *parent) :
    QObject(parent)
{
    count_num1 = 0;
    count_num2 = 0;
    count_num3 = 0;
    count_num4 = 0;
    k_power = "";
    k_before = "";
    this->moveToThread(&m_thread);
    m_thread.start();
}

void TaoTong::T_start()
{
    k1 = 0,k2 = 0,k3 = 0,k4 = 0;
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(taotong_timer()));
    m_timer.start(100);
}

void TaoTong::taotong_timer()
{
    if(battery)
    {
        QFile power("/var/power");
        power.open(QIODevice::ReadOnly);
        QTextStream in_power(&power);
        in_power >> k_power;
        if(k_power != k_before)
        {
            k_before = k_power;
            emit sendbattery(k_power);
        }
        power.close();
    }

    if(SYSS == "ING" && TaoTongState)
    {
        QFile taotong1("/sys/class/gpio/gpio160/value");
        QFile taotong2("/sys/class/gpio/gpio165/value");
        QFile taotong3("/sys/class/gpio/gpio164/value");
        QFile taotong4("/sys/class/gpio/gpio159/value");
        taotong1.open(QIODevice::ReadOnly);
        taotong2.open(QIODevice::ReadOnly);
        taotong3.open(QIODevice::ReadOnly);
        taotong4.open(QIODevice::ReadOnly);
        QTextStream in1(&taotong1);
        QTextStream in2(&taotong2);
        QTextStream in3(&taotong3);
        QTextStream in4(&taotong4);
        in1 >> k1;
        in2 >> k2;
        in3 >> k3;
        in4 >> k4;

        taotong1.close();
        taotong2.close();
        taotong3.close();
        taotong4.close();
        if(k1 ==1 && k2==1 && k3 == 1 && k4==0)
        {
            //套筒4
            count_num1++;
            if(count_num1 == 3)
            {
                //  qDebug() << "send tao tong signal 4 !!!!!!";
                emit taotong_num(4);
                //TaoTongState = false;
                count_num1 = 0;

            }
        }
        else if(k1 ==1 && k2==1 && k3 == 0 && k4==1)
        {
            //套筒3
            count_num2++;
            if(count_num2 == 3)
            {
                // qDebug() << "send tao tong signal 3 !!!!!!";
                emit taotong_num(3);
                //TaoTongState = false;
                count_num2 = 0;
            }
        }
        else if(k1 ==1 && k2==0 && k3 == 1 && k4==1)
        {
            //套筒2
            count_num3++;
            if(count_num3 == 3)
            {
                // qDebug() << "send tao tong signal 2 !!!!!!";
                emit taotong_num(2);
                //TaoTongState = false;
                count_num3 = 0;
            }
        }
        else if(k1 ==0 && k2==1 && k3 == 1 && k4==1)
        {
            //套筒1
            count_num4++;
            if(count_num4++)
            {
                //qDebug() << "send tao tong signal 1 !!!!!!";
                //qDebug() << TAOTONG;
                emit taotong_num(1);
                //TaoTongState = false;
                count_num4=0;
            }
        }
        else
        {
            emit taotong_num(10);
        }
    }

}

