#include "mastertcp.h"
#include <QDomDocument>
#include "GlobalVarible.h"

MasterTCP::MasterTCP(QObject *parent) :
    QObject(parent)
{
    this->moveToThread(&tcp_thread);
    tcp_thread.start();
    qDebug()  << "MasterTCP Thread setup...";
    connect(&requestTime,SIGNAL(timeout()),this,SLOT(reRequest()));
}

void MasterTCP::masterServerStart()
{
    systemStatueFlag = false;
    controlStatueFlag = false;
    socketConnectFlag = false;
    connectFlag = false;
    sendPinFlag = false;
    sendToFlag = false;
    readFlagTemp = false;
    isReady = false;
    sendBuf = "";
    timerCount = 0;
    revBuf = "";
    pin_Code = "";
    G9_Code = "";
    qDebug()  << "MasterTCP Thread start...";
    tcpServer1 = new QTcpServer(this);
    if(!tcpServer1->listen(QHostAddress::Any,4545))
    {  //**本地主机的4710端口，如果出错就输出错误信息，并关闭
        qDebug() << tcpServer1->errorString();
    }
    connect(tcpServer1,SIGNAL(newConnection()),this,SLOT(DataReceived()));
}
/***************************************************/
//link success
/***************************************************/
void MasterTCP::DataReceived()
{
    qDebug() << "****** Listening ****** connect success";
    tcpSocket1 = tcpServer1->nextPendingConnection();
    connect(tcpSocket1, SIGNAL(readyRead()),this, SLOT(recMessage()));
    QTimer::singleShot(2000,this,SLOT(heartTimerFunc()));
    socketConnectFlag = true;
    connectFlag = true;
}
/***************************************************/
//5000ms timer function
/***************************************************/
void MasterTCP::heartTimerFunc()
{
    if (sendToFlag && readFlagTemp)
    {
        sendToFlag=false;
        readFlagTemp = false;
    }
    QTimer::singleShot(10,this,SLOT(heartTimerFunc()));
}
/***************************************************/
//close link
/***************************************************/
void MasterTCP::closeTcpServer()
{
    qDebug()<<"****closeTcpServer******";
//    if(tcpServer->hasPendingConnections())
    if(socketConnectFlag)
    {
        qDebug()<<"**** tcpSocket1 delete******";
        socketConnectFlag = false;
        tcpSocket1->abort();
        delete tcpSocket1;
    }
    connectFlag = false;
}

/***************************************************/
//receive data and Ack
//ACK : 0x02   0x01   0x04       0x00 0x00                 0x 0x03
//          receive ACK cmd      error code
//ACK : 0x02   0x81   0x04       0x31 0x31                 0x 0x03
//                   system status and controller status
/***************************************************/
void MasterTCP::recMessage()
{
    timerCount = 0;
    revBuf  = tcpSocket1->readAll();
    //tcpSocket1->write(sendToRfid,sendToRfid.length());
    if(requestTime.isActive()){
        requestTime.stop();
    }
    requestTime.start(500);
}
void MasterTCP::reRfidData()
{
    isReady = true;
}
void MasterTCP::reRequest()
{
    if(isReady){
        tcpSocket1->write(sendToRfid,sendToRfid.length());
        isReady = false;
        requestTime.stop();
    }
}
/***************************************************/
//receive ping and G9 code
/***************************************************/
void MasterTCP::revRFID_PIN(bool validFlag, QByteArray G9Code)
{
    /*bool codeValidFlag;
    if( (!systemStatueFlag)&&(controlStatueFlag) )//not ING and controller ready
    {
        sendPinFlag = true;
        pin_Code = pinCode;
        G9_Code = G9Code;
    }
    codeValidFlag = validFlag;*/
    sendToFlag=validFlag;
    sendBuf = G9Code;
    qDebug()<<"*****validFlag*****"<<validFlag;
    //tcpSocket1->write(getFromRfid_toMastertcp,getFromRfid_toMastertcp.length());
}












