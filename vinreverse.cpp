#include "vinreverse.h"
#include <QDebug>
#include <QSettings>

VinReverse::VinReverse(QObject *parent) :
    QObject(parent)
{
    isNexoConnect = false;
    CS351_4545count = 0;
    m_thread.start();
    this->moveToThread(&m_thread);
}


void VinReverse::newconnects()
{
    m_pTcpSocket =  new QTcpSocket;
    connect(m_pTcpSocket, SIGNAL(readyRead()),this, SLOT(revNexoData()));
    connect(m_pTcpSocket, SIGNAL(connected()),this,SLOT(connectDo()));
//    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
//            this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(&nexoheart,SIGNAL(timeout()),this,SLOT(nexohearts()));
    connect(&connect_Timer,SIGNAL(timeout()),this,SLOT(connectTimer()));
    connect_Timer.start(5000);
    newConnect();
}

void VinReverse::newConnect()
{
    isNexoConnect = false;
    m_pTcpSocket->abort(); //取消已有的连接
    qDebug() << "waiting for connecting 4545 port";
    m_pTcpSocket->connectToHost(ControllerIp_1,4545);
}

void VinReverse::connectTimer()
{
    qDebug() << "4545 connectState:";
    if(m_pTcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "4545 connect success";
        connect_Timer.stop();
    }
    else if(m_pTcpSocket->state() == QAbstractSocket::ConnectingState)
    {
        qDebug() << "4545 connecting";
        newConnect();
    }
    else
    {
        qDebug() << "4545 m_pTcpSocket->state():" << m_pTcpSocket->state();
        newConnect();
    }
}

void VinReverse::connectDo()
{
    qDebug() << "4545 connectDo";
    sendBuf = "00200001001         ";
    m_pTcpSocket->write(sendBuf,21);
    nexoheart.start(7000);
}

void VinReverse::disconnectdo()
{
    nexoheart.stop();
    connect_Timer.start(5000);
    newConnect();
}


void VinReverse::revNexoData()
{
    CS351_4545count = 0;
    QByteArray tmp =  m_pTcpSocket->readAll();
    int midnum = tmp.mid(4,4).toInt();
    if(midnum == 2)
    {
        isNexoConnect = true;
        if(nexo_Vin.size() >= 14)
        {
            receiveVin(nexo_Vin);
        }
    }
}

void VinReverse::nexohearts()
{
    //qDebug() << "heart send";
    CS351_4545count++;
    if(CS351_4545count != 2)
    {
        sendBuf = "00209999001         ";
        m_pTcpSocket->write(sendBuf,21);
    }
    else
    {
        qDebug() << "4545 heart time out";
        disconnectdo();
    }
}

void VinReverse::receiveVin(QString Vin)
{
    qDebug() << "4545 VIN is" << Vin;
    QByteArray tmmp = "00370050            ";
    if(isNexoConnect)
    {
        if(Vin.size() == 17)
        {
            sendBuf = tmmp.append(Vin);
        }
        else if(Vin.size() == 14)
        {
            sendBuf = tmmp.append(Vin).append("***");
        }
        else
        {
            sendBuf = tmmp.append("L5027724850277248");
        }
        m_pTcpSocket->write(sendBuf,38);
        m_pTcpSocket->waitForBytesWritten();
    }
    else
    {
        nexo_Vin = Vin;
    }
}
