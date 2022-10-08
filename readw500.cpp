#include "readw500.h"

READW500::READW500(QObject *parent) :
    QObject(parent)
{
    this->moveToThread(&W500_Thread);
    W500_Thread.start();
    W500IP = "";
    channelNum = 1;
    W500ConnectFlag = false;
    reqW500ResultFlag = false;
    timerCount = 0;
    IP_Right = 0;
    channelNum = 0;
    writeSNFlag = false;
    snBuf = "";
}
/***************************************/
//w500 threat start()
/***************************************/
void READW500::W500Start()
{
    qDebug() << "read W500 thread start &&&&&&&&&&&&&&&"<<W500IP;
    QStringList list = W500IP.split(".",QString::SkipEmptyParts);
    if(list.size() > 1)
    {
        IP_Right = list[list.size()-1].toInt();
    }
    else
    {
        IP_Right = 0 ;
    }
    W500_Socket = new QTcpSocket;
    connect(W500_Socket, SIGNAL(readyRead()),this, SLOT(W500ReceiveDataFunc()));
    connect(W500_Socket,SIGNAL(disconnected()),this,SLOT(W500DisConnect()));
    W500Connect();
    QTimer::singleShot(2000,this,SLOT(W500TimerFunc()));
}
/***************************************/
//connect w500
/***************************************/
void READW500::W500Connect()
{
    W500_Socket->abort(); //取消已有的连接
    W500_Socket->connectToHost(W500IP,5000); //连接到主机，这里从界面获取主机地址和端口号
    W500_Socket->waitForConnected(2000); //waitting 2000ms
    qDebug() << "W500 connect waiting>>>>>>>>"<< W500_Socket->state();
    if(W500_Socket->state() == QAbstractSocket::ConnectedState)
    {
        W500ConnectFlag = true;
        qDebug() << "W500 connect success!!!!!!!!!!!!!";
    }
    else
    {
        qDebug() << "W500 connect fail?????????????????" ;
    }
}
/***************************************/
//disconnect w500
/***************************************/
void READW500::W500DisConnect()
{
    W500ConnectFlag = false;
}
/***************************************/
//receive w500 data
//3ab30302cb0001009c5a3b
/***************************************/
void READW500::W500ReceiveDataFunc()
{
    int revLen;
    QByteArray recBuf;
    timerCount = 0;
    recBuf = W500_Socket->readAll();
    revLen = recBuf.size();
    if( ((unsigned char)recBuf[0] == 0x3A)&&((unsigned char)recBuf[1] == IP_Right) )
    {
        if(reqW500ResultFlag)
        {
            if(((unsigned char)recBuf[8]&0x03) != 0)
            {
                reqW500ResultFlag = false;
                if(channelNum == 1)
                {
                    emit sendTighteningResult1(4,4);
                }
                else if(channelNum == 2)
                {
                    emit sendTighteningResult2(4,4);
                }
            }
        }
    }
    qDebug() << "&&&&&&&&&&&&&&&&&&&&&&&&&&&& read data:::" <<revLen << recBuf.toHex();
}
/***************************************/
//1000ms timer
/***************************************/
void READW500::W500TimerFunc()
{
    if(timerCount>3)
    {
        timerCount = 0;
        W500ConnectFlag = false;
        W500Connect();
    }
    if(W500ConnectFlag)
    {
//        if(writeSNFlag)
//        {
//            writeSNFlag = false;
//            writeW500Cmd(snBuf.size(),snBuf.toLatin1());
//        }
//        else
//        {
            readW500Cmd();
//        }
    }
    timerCount ++;
    QTimer::singleShot(2000,this,SLOT(W500TimerFunc()));
}
/***************************************/
//1000ms timer
/***************************************/
void READW500::revW500StartCmd(int chl,QString cmd)
{
    channelNum = chl;
    if(cmd == "read")
    {
        reqW500ResultFlag = true;
    }
//    else
//    {
//        snBuf = cmd;
//        writeSNFlag = true;
//    }
    qDebug()<<"*********** code ************"<<channelNum<<snBuf;
}
/***************************************/
//read w500 data cmd
/***************************************/
void READW500::readW500Cmd()
{
    QByteArray sendBuf;
    unsigned char sum=0;
    sendBuf[0] = 0x3A;
    sendBuf[1] = IP_Right;
    sendBuf[2] = 0x03;
    sendBuf[3] = 0x02;
    sendBuf[4] = 0xcb;
    sendBuf[5] = 0x00;
    sendBuf[6] = 0x01;
    for(int i=0;i<7;i++)
    {
        sum += sendBuf[i];
    }
    sendBuf[7] = sum;
    sendBuf[8] = 0x3b;
    W500_Socket->write(sendBuf,sendBuf.size());
    qDebug()<<"######### read ################"<<sendBuf.toHex();
}
/***************************************/
//write w500 data cmd
//3A 01 10 0306 0002 1111 2222 00 3B
/***************************************/
void READW500::writeW500Cmd(int len ,QByteArray buf)
{
    QByteArray sendBuf;
    unsigned char sum=0;
    int bufSize = buf.size();
    sendBuf[0] = 0x3A;
    sendBuf[1] = IP_Right;
    sendBuf[2] = 0x10;
    sendBuf[3] = 0x03;
    sendBuf[4] = 0x06;
    sendBuf[5] = len>>8;
    sendBuf[6] = len;
    for(int n=0;n<bufSize;n++)
    {
        sendBuf[7+n] = buf[n];
    }
    sendBuf[7+bufSize] = 0x0d;
    for(int i=0;i<(8+bufSize);i++)
    {
        sum += sendBuf[i];
    }
    sendBuf[8+bufSize] = sum;
    sendBuf[9+bufSize] = 0x3b;
    W500_Socket->write(sendBuf,sendBuf.size());
    qDebug()<<"######### write ################"<<sendBuf.toHex();

}



