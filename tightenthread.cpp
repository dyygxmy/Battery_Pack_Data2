#include "tightenthread.h"

TightenThread::TightenThread(QObject *parent) :
    QObject(parent)
{
    flag = 0;
    cycleId =0;
    isreset = 1;
    isSendHeart = 0;
    config351count = 0;
    log351In  = false;
    log351out = false;
    isconnected = false;
    isdisconnect = false;
    xmlhead ="<?xml version=\"1.0\" encoding=\"UTF-8\"?><root:ROOT xmlns:root=\"http://pdps.in.audi.vwg/legacy_schema/20.7.2/root\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://pdps.in.audi.vwg/legacy_schema/20.7.2/root root.xsd\">";
    enableTemp = "<MSL_MSG><PNR>21</PNR><SDR><SER>1</SER></SDR><PID><PRT>T</PRT><PI1>VIN12345678901234</PI1></PID><PRS><PRG>1</PRG> <SIO>2</SIO> <MNO>2</MNO><NUT>0</NUT></PRS><TOL><VAL>1</VAL></TOL></MSL_MSG></root:ROOT>";
    aliveTemp = "<MSL_MSG><PNR>21</PNR><SDR><SER>1</SER></SDR></MSL_MSG></root:ROOT>";
    resetTemp="<MSL_MSG><PNR>21</PNR><RST/><SYN><DAT>2014-12-23</DAT><TIM>15:20:00</TIM></SYN><SDR><SER>1</SER></SDR><PDC><CIO>5</CIO><CNO>5</CNO></PDC><PID><PRT>T</PRT><PI1/></PID><TOL><VAL>0</VAL></TOL></MSL_MSG></root:ROOT>";
    Data_Xml_Tx =(xmlhead+aliveTemp).toLatin1();
    this->moveToThread(&m_thread);
    m_thread.start();
}
//线程开始
void TightenThread::tightenStart()
{
    int iscs351 = 0;
    if(!iscs351)
    {
        tcpSocket =new QTcpSocket;
        connect(tcpSocket, SIGNAL(readyRead()),this, SLOT(recsocket()));
        connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(receivedisconnect()));
        sendheartt = new QTimer(this);
        timer351 = new QTimer(this);
        connect(sendheartt,SIGNAL(timeout()),this,SLOT(sendheart()));
        connect(timer351,SIGNAL(timeout()),this,SLOT( newConnect()));
        timer351->start(3000);
    }
}
//************通道4700连接351
void TightenThread::newConnect()
{
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    if(!isconnected)
    {
        tcpSocket->abort(); //取消已有的连接
        if(!log351In)
        {
            qDebug() << "connecting 351";
            log351In = true;
        }
        tcpSocket->connectToHost(configIniRead->value("baseinfo/cs351Ip").toString(),configIniRead->value(QString("baseinfo/PortA")).toInt());
        if(tcpSocket->waitForConnected(2000))
        {
            //qDebug() << "connect!!!";
            isconnected = true;
        }
    }
    else
    {
        if(!log351out)
        {
            qDebug() << "connect 351 success";
            log351out = true;
        }
    }
    delete configIniRead;
}
void TightenThread::recsocket()
{
    config351count = 0;
    QByteArray a = tcpSocket->readAll();
    //qDebug() << "4700 head" << a;
    if( a.mid(20,4).toInt() != 0)
    {
        qDebug() << "error head" << a;
        if(a.mid(20,4).toInt() == 1)
        {
            cycleId = 1;
        }
    }
}
//***************发送使能
void TightenThread::sendReadOperate()
{
    if(ISRESET)
        flag = 0;
    sendMessage();
}
//************通道4710连接上351他通知
void TightenThread::receivehavedconnect(bool a)
{
    if(ISRESET)  //复位不通知主界面
    {
        ISRESET = false;
    }
    else
    {
        isconnected = true;
        if(!RDYY)
        {
            if(!a)
            {
                qDebug() << "gun not ready";
                CsIsConnect = false;
                emit GunNotReady();
            }
            else
            {
                if(a && tcpSocket->state() == QAbstractSocket::ConnectedState)
                {
                    qDebug() << "init()";
                    sendheartt->start(5000);
                    sendMessage();
                }
            }
        }
        else
        {
            if(SYSS == "ING" || SYSS == "OK" || SYSS == "NOK")
                ;
            else
                SYSS = "Ready";
            if(isdisconnect) //断线重连
            {
                sendheart();
                if(sendheartt->isActive())
                    sendheartt->stop();
                if(SYSS == "ING")
                    emit ReconnectSendOperate();
                sendheartt->start(5000);
            }
            if(!CsIsConnect)
            {
                emit IsTigntenReady(true);
                CsIsConnect = true;
                isdisconnect = false;
            }
        }
    }

}
//***********************发送心跳  复位 使能
void TightenThread::sendMessage()
{
    // qDebug() << "flag is " << flag;
    if(tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        if(!flag)
        {
            isreset = 1;
            cycleId = 0; //复位后循环号置1
            getfile(1);
            ISRESET = false;

        }else
        {
            qDebug() << "send operate" << proNum << lsNum;
            if(enablenumberLeft)
            {
                lsNum = QString::number(enablenumberLeft);
            }
            getfile(1);
        }
        flag++;
    }

}
//**********************获取xml文件
void TightenThread::getfile(unsigned short type)
{
    QMutexLocker lock(&mutex);
    QByteArray heads = "55AA0001000000000002000000000000";
    char *st = heads.data();
    if(cycleId == 10000)
        cycleId = 1;
    if(isSendHeart)
    { //发心跳
        cs351_header_func(cycleId++,Data_Xml_Tx.size(),type,st);
        QByteArray byte(st);
        isSendHeart = 0;
        //qDebug() << "heart" << byte+Data_Xml_Tx;
        tcpSocket->write(byte+Data_Xml_Tx) ;
        tcpSocket->waitForBytesWritten();
    }else
    {
        if(isreset)
        {
            //发复位
            QString Data_Xml_T = xmlhead+resetTemp;
            cs351_header_func(cycleId++,Data_Xml_T.size(),type,st);
            //299  320
            QDateTime cur = QDateTime::currentDateTime();
            QString current_date = cur.toString("yyyy-MM-dd");
            QString current_time = cur.toString("hh:mm:ss");
            Data_Xml_T.replace(284,current_date.size(),current_date).replace(305,current_time.size(),current_time);
            isreset = 0;
            QByteArray byte(st);
            //qDebug() << "send to 4710 isreset" << byte << Data_Xml_T;
            tcpSocket->write(byte+Data_Xml_T.toLatin1());

        }
        else
        {
            QString s = xmlhead+enableTemp;
            if(isRFID == 1)
                s.replace(313,17,VIN_PIN_SQL+"   ");
            else
                s.replace(313,17,VIN_PIN_SQL);
            int len1 = 0;
            if(proNum.length() == 1)
                len1 = 7;
            else
                len1 = 8;
            s.replace(352,len1,proNum+"</PRG>");
            s.replace(365,8,lsNum+"</SIO>");
            //qDebug() << "s is" << s;
            cs351_header_func(cycleId++,s.size(),type,st);
            QByteArray byte(st);
            s.prepend(byte);
            if(CsIsConnect)
            {
                qDebug() << "send operate !!" << s.mid(0,32);
                tcpSocket->write(s.toLatin1());
                tcpSocket->waitForBytesWritten();
            }

        }
    }

}
//**************************发送心跳函数
void TightenThread::sendheart()
{
    //qDebug() << "heart send";
    config351count++;
    if(config351count != 3)
    {
        isSendHeart = 1;
        getfile(3);
        isNormal = true;
    }
    else
    {
        qDebug() << "heart time out";
        receivedisconnect();
    }


}
//**************************输出错误信息
void TightenThread::displayError(QAbstractSocket::SocketError)
{
    qDebug() << "df" << tcpSocket->errorString(); //输出错误信息
}
//***************************351主动断开 处理
void TightenThread::receivedisconnect()
{
    //disconnect
    if(!isdisconnect)
    {
        qDebug() << "here disconnect";
        system("echo 1 > /sys/class/leds/control_led4/brightness");
        CsIsConnect = false;
        isconnected = false;
        log351In  = false;
        log351out = false;
        emit IsTigntenReady(false);  //发信号断开;
        sendheartt->stop();
        //flag = 0;
        isdisconnect = true;
        cycleId = 1;
        newConnect();//重新连接
    }
}


/**********************32位数据头的获取*****************************/
void TightenThread::cs351_header_func(unsigned short mid,unsigned short size,unsigned  short type,char *buf)
{
    unsigned short i;
    *buf++ =0x35;
    *buf++ =0x35;
    *buf++ =0x41;
    *buf++ =0x41;
    number_change_ascii(mid,4,1000,buf);
    buf += 4;
    number_change_ascii(size,8,10000000,buf);
    buf += 8;
    number_change_ascii(type,4,1000,buf);
    buf += 4;
    for(i=0;i<12;i++)
    {
        *buf++ = 0x30;
    }
}

/**********************转换成ASCII*****************************/

void TightenThread::number_change_ascii(unsigned int  num,unsigned char count,unsigned int div, char *ascii_buf)
{
    unsigned short i;
    for(i=0;i<count;i++)
    {
        *ascii_buf++ = num/div+0x30;
        num %= div;
        div /= 10;
    }
}
