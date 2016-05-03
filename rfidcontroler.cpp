#include "rfidcontroler.h"

RfidControler::RfidControler(QObject *parent) :
    QObject(parent)
{
    RFIDISConnect = false;
    this->moveToThread(&m_thread);
    m_thread.start();
}

void RfidControler::rfidInit()
{
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    rfidip = configIniRead->value("baseinfo/RfidIp").toString();
    rfidport = configIniRead->value("baseinfo/RfidPort").toInt();
    m_pTcpSocket = new QTcpSocket;
    connect(m_pTcpSocket,SIGNAL(readyRead()),this,SLOT(receivemss()));
    connect(m_pTcpSocket,SIGNAL(connected()),this,SLOT(connectedDo()));
    connect(m_pTcpSocket,SIGNAL(disconnected()),this,SLOT(disconnectedDo()));
    connect(&pingTimer,SIGNAL(timeout()),this,SLOT(pingTimers()));
    connect(m_pTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    pingTimer.start(5000);
    delete configIniRead;
}

void RfidControler::newConnect()
{
    m_pTcpSocket->abort();                        //取消已有的连接
    m_pTcpSocket->connectToHost(rfidip,rfidport); //连接到主机
}

void RfidControler::displayError(QAbstractSocket::SocketError )
{
    // qDebug() << "df" << m_pTcpSocket->errorString(); //输出错误信息
    RFIDISConnect = false;
}

void RfidControler::connectedDo()
{
    qDebug() << "rfid connect success";
    pingTimer.stop();
    emit rfidConnected(true);
    RFIDISConnect = true;

}

void RfidControler::disconnectedDo()
{
    qDebug() << "rfidconnect dis";
    emit rfidConnected(false);
    RFIDISConnect = false;
    pingTimer.start(5000);
}
void RfidControler::rfidDisconnectDo(bool istrue)
{
    if(!istrue)
    {
        disconnectedDo();
    }
}
void RfidControler::pingTimers()
{
    if(!RFIDISConnect)
    {
        newConnect();
    }
}

//判断条码10条是否重复
void RfidControler::pinIsEqual()
{
    isequeal = false;
    if(seriallist.isEmpty())
    {
        seriallist.push_back(tempPin);
        whichindex++;
    }
    else
    {
        QList<QString>::iterator i;
        for (i = seriallist.begin(); i != seriallist.end(); ++i)
        {
            if(!(QString::compare(tempPin, *i, Qt::CaseSensitive)))
            {
                isequeal = true;
                break;
            }
        }
        if(!isequeal)
        {
            if(seriallist.size() == 10)
            {
                seriallist[whichindex] = tempPin;
            }
            else
            {
                seriallist.push_back(tempPin);
            }
            whichindex++;
            if(whichindex == 10)
                whichindex = 0;
        }
    }
}

void RfidControler::receivemss()
{
    if(!CsIsConnect)
    {
        m_pTcpSocket->readAll();
    }
    else
    {
        QString hex = QString(m_pTcpSocket->readAll());
       // qDebug() << "hex" << hex;
        QByteArray ret;
        QString &org = hex;
        hex=hex.trimmed();
        int n =2;
        int size= org.size();
        int space= qRound(size*1.0/n+0.5)-1;
        if(space<=0)
            return ;
        for(int i=0,pos=n;i<space;++i,pos+=(n+1))
        {
            org.insert(pos,' ');
        }

        QStringList sl=hex.split(" ");
        foreach(QString s,sl)
        {
            if(!s.isEmpty())
                ret.append((char)s.toInt(0,16)&0xFF);
        }
        QString pin = QString(ret);
        tempPin = pin.mid(2,14);
        tempG9  = pin.mid(16,2);
        pinIsEqual();
        if(isequeal)
        {
            //qDebug() << tempPin << "fd1" << tempG9;
        }
        else
        {
            if(SYSS!="ING")
            {
               // qDebug() << tempPin << "fd2" << tempG9;
                emit sendPinToUi(tempPin,isequeal,tempG9);
            }
            else
            {
                //是ING状态，切断当前使能，等待工人确认
                RFIDlock.lockForWrite();
                if(rfidNextCom)
                {
                    RFIDlock.unlock();
                    VIN_PIN_SQL_RFID = tempPin+tempG9;
                }
                else
                {
                    rfidNextCom = true;
                    RFIDlock.unlock();
                    VIN_PIN_SQL_RFID = tempPin+tempG9;
                    emit sendGetCar();
                }
            }
        }
    }
}

