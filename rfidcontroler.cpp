#include "rfidcontroler.h"
#include <QDomDocument>
//QByteArray sendToRfid;
RfidControler::RfidControler(QObject *parent) :
    QObject(parent)
{
    RFIDIsConnect = false;

    this->moveToThread(&m_thread);
    m_thread.start();
    readRFIDFlag = false;
    count1 = 0;
    dataTemp = "";
}

void RfidControler::rfidInit()
{
    qDebug()<< "rfidcontroller thread start 32111111111"<<rfidip<<rfidport;
    m_pTcpSocket = new QTcpSocket;
    connect(m_pTcpSocket,SIGNAL(readyRead()),this,SLOT(receivemss()));
    connect(m_pTcpSocket,SIGNAL(connected()),this,SLOT(connectedDo()));
    connect(m_pTcpSocket,SIGNAL(disconnected()),this,SLOT(disconnectedDo()));
    connect(&pingTimer,SIGNAL(timeout()),this,SLOT(pingTimers()));
    connect(m_pTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    pingTimer.start(2000);

}

void RfidControler::newConnect()
{
    m_pTcpSocket->abort();                        //取消已有的连接
    m_pTcpSocket->connectToHost(rfidip,rfidport); //连接到主机
}

void RfidControler::displayError(QAbstractSocket::SocketError)
{
    // qDebug() << "df" << m_pTcpSocket->errorString(); //输出错误信息
    RFIDIsConnect = false;
}

void RfidControler::connectedDo()
{
    qDebug() << "rfid connect success";
    pingTimer.stop();
    emit rfidConnected(true);
    RFIDIsConnect = true;
    sendReset();
}

void RfidControler::disconnectedDo()
{
    qDebug() << "rfid connect dis";
    emit rfidConnected(false);
    RFIDIsConnect = false;
    pingTimer.start(2000);
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
    if(!RFIDIsConnect)
    {
        newConnect();
    }
    if(readRFIDFlag)
    {
        count1 ++;
    }
}
/*************************************************/
//receive RFID Data
/*************************************************/
void RfidControler::receivemss()
{
    QByteArray data = m_pTcpSocket->readAll();
    qDebug() <<"RfidControler::receivemss()"<< data;
    sendToRfid=data;
    dataTemp = data;
    emit sendRequest();
    //    emit sendPinToMasterTcp(true,data);
    //qDebug() << "emit sendPinToMasterTcp";
    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(data, false, &errorStr, &errorLine,
                        &errorColumn)) {
        qDebug()<<tr("Parse error at line %1, column %2: %3").arg(errorLine).arg(errorColumn).arg(errorStr);
        return;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "reply") {
        qCritical("Not a valid reply");
        return;
    }

    parseReplyElement(root);
    return;
}

void RfidControler::parseReplyElement(const QDomElement &element)
{
    QDomNode child = element.firstChild();
    while (!child.isNull()) {
        if (child.toElement().tagName() == "resultCode") {
            QString resultCode = child.toElement().text();
            qDebug()<<"resultCode:"<<resultCode;
            if(resultCode != "0000")//readwrite fail
            {
                if(resultCode == "0018" || resultCode == "0019" || resultCode == "001F")
                {
//                    sleep(5);
                    sendReset();
                    qDebug()<<"this is reset rfid";
                }
                else
                {
                    emit RFIDDataToPLC("0003","");//RFID read error data
                    return;
                }
            }
        }
        else if (child.toElement().tagName() == "reset") {
            qDebug()<<"reset success!";
            sleep(2);
            read_data("0000","0015");
        }
        else if (child.toElement().tagName() == "writeTagData") {
            emit RFIDSuccess("write","");
            qDebug()<<"write success!";
        }
        else if (child.toElement().tagName() == "readTagData") {
            emit sendPinToMasterTcp(true,dataTemp);
            parseReadElement(child.toElement());
        }
        child = child.nextSibling();
    }
}

void RfidControler::parseReadElement(const QDomElement &element)
{
    QDomNode child = element.firstChild();
    while (!child.isNull()) {
        if (child.toElement().tagName() == "returnValue") {
            QDomNode returnValueChild = child.toElement().firstChild();
            while (!returnValueChild.isNull()) {
                if (returnValueChild.toElement().tagName() == "data") {
                    QString data = returnValueChild.toElement().text();
                    QByteArray data1 = data.left(data.indexOf("0D")).toAscii();
                    emit RFIDSuccess("read",data1);
                    emit RFIDDataToPLC("0000",QByteArray::fromHex(data1));//RFID read success
                    qDebug()<<"rfid read data:"<<data;
                }
                returnValueChild = returnValueChild.nextSibling();
            }
        }
        child = child.nextSibling();
    }
}


void RfidControler::write_data(QByteArray add,QByteArray data)
{
    if(RFIDIsConnect)
    {
        QByteArray header =  "<command><writeTagData><startAddress>";
        QByteArray tail = "</data></writeTagData></command>";
        QByteArray writeCMD = header+add+"</startAddress><data>"+data+tail;
        qDebug() <<"write RFID:"<< writeCMD;
        m_pTcpSocket->write(writeCMD,writeCMD.length());
    }
}


//请求rfid发送数据
void RfidControler::read_data(QByteArray add,QByteArray lenth)
{
    if(!readRFIDFlag)
    {
        readRFIDFlag = true;
        count1 = 0;
    }
    if(RFIDIsConnect)
    {
        readRFIDFlag = false;
        count1 = 0;
        QByteArray header =  "<command><readTagData><startAddress>";
        QByteArray tail = "</dataLength></readTagData></command>";
        QByteArray readCMD = header+add+"</startAddress><dataLength>"+lenth+tail;
        qDebug() <<"read RFID"<<readCMD;
        m_pTcpSocket->write(readCMD,readCMD.length());
    }
    else
    {
         if(count1 >= 2)
         {
            count1 = 0 ;
            readRFIDFlag = false;
            emit RFIDDataToPLC("0001","");//RFID no connect
         }
    }
}

void RfidControler::sendReset()
{   
    QByteArray resetdata = "<command><reset><param>0005000000010001</param></reset></command>";
    qDebug()<<"this is rfid reset"<<resetdata;
    m_pTcpSocket->write(resetdata,resetdata.length());
}
