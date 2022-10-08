/***********************************************
 *BJB program Apprcation
 ***********************************************/
#include "inputmodule.h"

inputModule::inputModule(QObject *parent) :
    QObject(parent)
{
    IO_thread.start();
    this->moveToThread(&IO_thread);
    IOModuleConnectFlag = false;
    timerCount = 0;
}
/***************************************/
//IO_Module Init
/***************************************/
void inputModule::IO_Module_Init()
{
    qDebug() << "read IO module status thread init &&&&&&&&&&&&&&&"<<IO_Module_IP;
    IO_Module_Socket = new QTcpSocket;
    connect(IO_Module_Socket, SIGNAL(readyRead()),this, SLOT(revIOModuleDataFunc()));
    connect(IO_Module_Socket,SIGNAL(disconnected()),this,SLOT(IOModuleDisConnect()));
    IO_Module_Connect();
    QTimer::singleShot(2000,this,SLOT(IOModuleTimerFunc()));
}
/***************************************/
//connect IO module
/***************************************/
void inputModule::IO_Module_Connect()
{
    IO_Module_Socket->abort(); //取消已有的连接
    IO_Module_Socket->connectToHost(IO_Module_IP,502); //连接到主机，这里从界面获取主机地址和端口号
    IO_Module_Socket->waitForConnected(2000); //waitting 2000ms
    qDebug() << "IO_Module_Socket connect waiting>>>>>>>>"<<IO_Module_IP;
    if(IO_Module_Socket->state() == QAbstractSocket::ConnectedState)
    {
        IOModuleConnectFlag = true;
        qDebug() << "IO_Module_Socket connect success!!!!!!!!!!!!!";
    }
    else
    {
        qDebug() << "IO_Module_Socket connect failXXXXXXXXXXXXXXX";
    }
}
/***************************************/
//dicconnet
/***************************************/
void inputModule::IOModuleDisConnect()
{
    IOModuleConnectFlag = false;
}
/***************************************/
//receive IO module data
//00 00 00 00 00 05 01 02 02 00 00
/***************************************/
void inputModule::revIOModuleDataFunc()
{
    QByteArray recBuf;
    int revLen;
    timerCount = 0;
    recBuf = IO_Module_Socket->readAll();
    revLen = recBuf.size();
    qDebug()<<"IO module XXXXXXXXXXXXXX:"<<recBuf.toHex()<<revLen;
    if(revLen >= 11)
    {
        if( ((unsigned char)recBuf[6] == 0x01)&&((unsigned char)recBuf[7] == 0x02) )
        {
            IO_Value = recBuf[9];
            IO_Value = (IO_Value<<8) + recBuf[10];
            emit sendIOValueToStep(IO_Value);
        }
    }
}
/***************************************/
//1000ms timer
/***************************************/
void inputModule::IOModuleTimerFunc()
{
    if(IOModuleConnectFlag)
    {
        readIOModuleStatus();
    }
    if(timerCount > 3)
    {
        timerCount = 0;
        IOModuleConnectFlag = false;
        IO_Module_Connect();
    }
    timerCount++;
    QTimer::singleShot(500,this,SLOT(IOModuleTimerFunc()));
}
/***************************************/
//read IO module CMD
/***************************************/
void inputModule::readIOModuleStatus()
{
    QByteArray sendBuf = "";
    sendBuf[0] = 0x00;
    sendBuf[1] = 0x00;
    sendBuf[2] = 0x00;
    sendBuf[3] = 0x00;
    sendBuf[4] = 0x00;
    sendBuf[5] = 0x06;
    sendBuf[6] = 0x01;//module address
    sendBuf[7] = 0x02;//CMD
    sendBuf[8] = 0x00;
    sendBuf[9] = 0x00;//read start address
    sendBuf[10] = 0x00;
    sendBuf[11] = 0x10;//IO number
    IO_Module_Socket->write(sendBuf,sendBuf.size());
//    qDebug()<<"IO send XXXXXXXXXXXXXXX"<<sendBuf.toHex()<<sendBuf.size();
}

