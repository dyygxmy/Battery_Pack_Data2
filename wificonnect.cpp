#include "wificonnect.h"
#include <QDebug>
#include "GlobalVarible.h"
Wificonnect::Wificonnect(QObject *parent) :
    QObject(parent)
{
    DataConnected = false;
    ApConnected = false;
    RfidConnected = false;
    wificonnect_thread.start();
    this->moveToThread(&wificonnect_thread);
}

void Wificonnect::Wifi_ConnectStart()
{
    qDebug()<<"wificonnect_thread start";
    QObject::connect(&timerwifi,SIGNAL(timeout()),this,SLOT(timerWifi()));
    timerwifi.start(5000);
}

void Wificonnect::timerWifi()
{

//    QSettings *configIniRead = new QSettings("/var/test_wifi.ini", QSettings::IniFormat);
    QFile wifi_test("/var/test_wifi");
    wifi_test.open(QIODevice::ReadOnly);
    QByteArray test_wifi = wifi_test.readAll();

//    gateway = configIniRead->value("/test_wifi/gateway").toInt();
//    dataserver = configIniRead->value("/test_wifi/DataServerIp").toInt();
//    rfidserver = configIniRead->value("/test_wifi/RfidIp").toInt();
    gateway = test_wifi.mid(0,1).toInt();
    dataserver = test_wifi.mid(2,1).toInt();
    rfidserver = test_wifi.mid(4,1).toInt();
    //qDebug() << "fsdf" << gateway << dataserver << rfidserver;
    if(dataserver == 1)
    {
        if(!DataConnected)
        {
            DataConnected = true;
            WIFI_STATE = true;
            emit data_connect(true);
            qDebug() << "sql server connected success!";
        }
    }
    else
    {
        if(DataConnected)
        {
            DataConnected = false;
            WIFI_STATE = false;
            emit data_connect(false);
            qDebug() << "sql server connected fail!";
        }
    }
    if(gateway == 1)
    {
        if(!ApConnected)
        {
            ApConnected = true;
            emit wifi_connect(true);
            qDebug() << "wifi connected success !";
        }
    }
    else
    {
        if(ApConnected)
        {
            ApConnected = false;
            emit wifi_connect(false);
            qDebug() << "wifi connected fail";
        }
    }
    if(rfidserver== 1)
    {
        if(!RfidConnected)
        {
            RfidConnected = true;
            emit rfidconnect(true);
            qDebug() << "RfidConnect  success";
        }
    }
    else
    {
        if(RfidConnected)
        {
            RfidConnected = false;
            emit rfidconnect(false);
            qDebug() << "Rfid Connect fail";
        }
    }
  wifi_test.close();
//delete configIniRead;
}
