#include "tightenthread.h"

TightenThread::TightenThread(QObject *parent) :
    QObject(parent)
{
    cycleId =1;
    sendCysleID = 0;
    config351count = 0;
    port4700LinkFlag = false;
    port4700DisconnectFlag = false;
    cs351InitFlag = false;
    timerCount = 0;
    timerCount1 = 0;
    sendEnableFlag = false;
    enableFlag = false;
    ctrlType = "CS351";
    ctrlIP = "192.168.3.1";
    testPro = "1"; //test program
    //    resetTemp="<MSL_MSG><PNR>21</PNR><RST/><SYN><DAT>2014-12-23</DAT><TIM>15:20:00</TIM></SYN><SDR><SER>1</SER></SDR><PDC><CIO>5</CIO><CNO>5</CNO></PDC><PID><PRT>T</PRT><PI1/></PID><TOL><VAL>0</VAL></TOL></MSL_MSG></root:ROOT>";
    //    resetTemp_timewrong="<MSL_MSG><PNR>21</PNR><RST/><SDR><SER>1</SER></SDR><PDC><CIO>5</CIO><CNO>5</CNO></PDC><PID><PRT>T</PRT><PI1/></PID><TOL><VAL>0</VAL></TOL></MSL_MSG></root:ROOT>";
    //    enableTemp         = "<MSL_MSG><PNR>21</PNR><SDR><SER>1</SER></SDR><PDC><CIO>5</CIO><CNO>5</CNO></PDC><PID><PRT>T</PRT><PI1>VIN12345678901234</PI1></PID><PRS><PRG>1</PRG> <SIO>2</SIO> <MNO>2</MNO><NUT>0</NUT><KNR>1</KNR> </PRS><TOL><VAL>1</VAL><KNR>1</KNR> </TOL><STR><VAL>1</VAL><KNR>1</KNR> </STR></MSL_MSG></root:ROOT>";

    xmlhead            = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><root:ROOT xmlns:root=\"http://pdps.in.audi.vwg/legacy_schema/20.7.2/root\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://pdps.in.audi.vwg/legacy_schema/20.7.2/root root.xsd\">";
    aliveTemp          = "<MSL_MSG><PNR>21</PNR><SDR><SER>1</SER></SDR></MSL_MSG></root:ROOT>";
    enableTemp         = "<MSL_MSG><PNR>21</PNR><SDR><SER>1</SER></SDR><PDC><CIO>5</CIO><CNO>5</CNO></PDC><PID><PRT>T</PRT><PI1>VIN12345678901234</PI1></PID><PRS><PRG>1</PRG> <SIO>2</SIO> <MNO>50</MNO><NUT>0</NUT><KNR>1</KNR> </PRS><TOL><VAL>1</VAL><KNR>1</KNR> </TOL></MSL_MSG></root:ROOT>";
    resetTemp          = "<MSL_MSG><PNR>21</PNR><SYN><DAT>2014-12-23</DAT><TIM>15:20:00</TIM></SYN></MSL_MSG></root:ROOT>";
    resetTemp_timewrong= "<MSL_MSG><PNR>21</PNR><SDR><SER>1</SER></SDR></MSL_MSG></root:ROOT>";

    if(GchannelNum != "" || GchannelNum != "1")
    {
        if(GchannelNum.size() == 1)
        {
            enableTemp .replace(192,1,GchannelNum).replace(288,1,GchannelNum);
            resetTemp.replace(182,1,GchannelNum);
            resetTemp_timewrong.replace(131,1,GchannelNum);
        }
        else if(GchannelNum.size() == 2)
        {
            qDebug()<<"this is matou test";
            enableTemp.replace(192,8,GchannelNum+"</KNR>").replace(228,8,GchannelNum+"</KNR>");
            resetTemp.replace(182,8,GchannelNum+"</KNR>");
            resetTemp_timewrong.replace(131,8,GchannelNum+"</KNR>");
        }
    }
//    enableTemp = "<MSL_MSG><PNR>21</PNR><PRS><PRG>1</PRG><SIO>1</SIO><KNR>1</KNR></PRS><TOL><VAL>1</VAL><KNR>1</KNR></TOL><STR><VAL>1</VAL><KNR>1</KNR></STR></MSL_MSG></root:ROOT>";//CH1 enable
//    enableTemp = "<MSL_MSG><PNR>21</PNR><PRS><PRG>1</PRG><SIO>1</SIO><KNR>4</KNR></PRS><TOL><VAL>1</VAL><KNR>4</KNR></TOL><STR><VAL>1</VAL><KNR>4</KNR></STR></MSL_MSG></root:ROOT>";//CH2 enable
    xml_Grs = "<MSL_MSG><PNR>21</PNR><GRS><GRP><GNR>1</GNR><GNS>1</GNS><SIO>1</SIO> </GRP></GRS>";
    xml_TOL = "<TOL><VAL>1</VAL><GRS><GRP><GNR>1</GNR></GRP></GRS></TOL>";
    xml_STR = "<STR><VAL>1</VAL><GRS><GNR>1</GNR></GRS></STR></MSL_MSG></root:ROOT>";
    KNR_Temp = "<KAN><KNR>1</KNR></KAN>";
    xml_Grs_KNR = "";

    this->moveToThread(&m_thread);
    m_thread.start();
}
//线程开始
void TightenThread::tightenStart()
{
    qDebug() <<"Tighten thread start!!"<<ctrlType<<ctrlIP<<testPro;
    if(ctrlType == "CS351")
    {
        resetTemp          = "<MSL_MSG><PNR>21</PNR><SYN><DAT>2014-12-23</DAT><TIM>15:20:00</TIM></SYN><SDR><SER>1</SER></SDR><PDC><CIO>5</CIO><CNO>5</CNO></PDC><PID><PRT>T</PRT><PI1/></PID><TOL><VAL>0</VAL><KNR>1</KNR> </TOL></MSL_MSG></root:ROOT>";
        resetTemp_timewrong= "<MSL_MSG><PNR>21</PNR><SDR><SER>1</SER></SDR><PDC><CIO>5</CIO><CNO>5</CNO></PDC><PID><PRT>T</PRT><PI1/></PID><TOL><VAL>0</VAL><KNR>1</KNR> </TOL></MSL_MSG></root:ROOT>";
    }
    strChannel = "12345";
    tcpSocket =new QTcpSocket;
    connect(tcpSocket, SIGNAL(readyRead()),this, SLOT(recsocket()));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(receivedisconnect()));
    timer351 = new QTimer(this);
    connect(timer351,SIGNAL(timeout()),this,SLOT( newConnect()));
    timer351->start(250);
}

/******************************************************/
//************通道4700连接351   1000ms timer
/******************************************************/
void TightenThread::newConnect()
{
    qDebug()<<"GrecvStatusMsee this 0 point"<<GrecvStatusMsee<<enableFlag;
    timerCount++;
    timerCount1++;
    if(sendEnableFlag && ((GgetBoltIsOk == "Yes" && handleRecvStatusMess(GrecvStatusMsee,GPackType,GProgramIndex))
                          ||(GgetBoltIsOk != "Yes"))){
        if(!enableFlag)
        {
            sendCmdMessage(1);//send reset
        }else{
            sendCmdMessage(2);//send enable
        }
        sendEnableFlag = false;
        timerCount = 0;
    }
    else
    {
        if(timerCount>=20)
        {
            timerCount = 0;
            //                qDebug() << "heat 5000ms 4700 port" <<  cs351InitFlag  << port4700LinkFlag << ctrlIP;
            if(!port4700LinkFlag)
            {
                tcpSocket->abort(); //取消已有的连接
                tcpSocket->connectToHost(ctrlIP,4700);
                if(tcpSocket->waitForConnected(2000))
                {
                    port4700LinkFlag = true;
                    qDebug() << "4700 port connect success";
                }
                else
                {
                    emit IsTigntenReady(false);  //发信号断开 mainwindo
                    qDebug() << "4700 port connect fail";
                }
            }
            else  //send alive
            {
                if(cs351InitFlag)
                {
                    config351count++;
                    if(config351count < 3)
                    {
                        sendCmdToCs351(3);//send alive
                    }
                    else
                    {
                        qDebug() << "4700 heart time out";
                        emit disconnectTellPortB();
                        CsIsConnect = false;
                        config351count = 0;
                        port4700LinkFlag = false;
                        port4700DisconnectFlag = true;
                        emit IsTigntenReady(false);  //发信号断开 mainwindow
                        cs351InitFlag  = false;
                    }
                }
            }
        }
    }
}
/******************************************************/
//port B disconnect tell port A
/******************************************************/
void TightenThread::portBSendPortA()
{
    tcpSocket->abort();
    port4700LinkFlag = false;
    CsIsConnect = false;        //cs351 no ready
    emit IsTigntenReady(false); //mainwindow display cs351 no ready
    config351count = 0;
    cs351InitFlag  = false;
}
//************通道4710连接上351他通知
void TightenThread::receivehavedconnect(bool flag)
{
    if(!RDYY) //cs351 no ready
    {
        if(!flag)
        {
            qDebug() << "gun not ready";
            CsIsConnect = false;        //cs351 no ready
            emit IsTigntenReady(false); //mainwindow display cs351 no ready
        }
        else
        {
            if(tcpSocket->state() == QAbstractSocket::ConnectedState)
            {
                qDebug() << "cs351 4700  %%%%%%%%%%%%%%% port init()" << cs351InitFlag;
                //                if(!cs351InitFlag)
                //                {
                cs351InitFlag = true;//data power on
                cycleId = 1;
                sendCmdMessage(4);//send reset cmd
                //                }
            }
        }
    }
    else
    {
        if(SYSS == "ING" || SYSS == "OK" || SYSS == "NOK")
        {}
        else
        {
            SYSS = "Ready";
        }
        if(port4700DisconnectFlag) //断线重连
        {
            if(SYSS == "ING")
            {
                emit ReconnectSendOperate();
            }
            qDebug() << "******cs351 reconnect*******";
        }
        if(!CsIsConnect)
        {
            CsIsConnect = true;          //cs351 ready
            emit IsTigntenReady(true);   //mainwindow display cs351 no ready
            port4700DisconnectFlag = false;
            qDebug() << "******cs351 connect success**********";
        }
    }
}
//**************************输出错误信息
void TightenThread::displayError(QAbstractSocket::SocketError)
{
    qDebug() << "df" << tcpSocket->errorString(); //输出错误信息
}
/**********************************************/
//************351主动断开 处理 or alive no ack over 3 times
/**********************************************/
void TightenThread::receivedisconnect()
{
    qDebug() << "here 4700 port disconnect";
    emit disconnectTellPortB();
    config351count = 0;
    CsIsConnect = false;
    port4700LinkFlag = false;
    port4700DisconnectFlag = true;
    emit IsTigntenReady(false);  //发信号断开 mainwindow
    //    timer351->stop();
    //    timer351->start(5000);
}

/******************************************************/
//***************mianwondow emit 发送使能
/******************************************************/
void TightenThread::sendReadOperate(bool enable,int n)
{
//    Q_UNUSED(n);
    sendEnableFlag = true;
    enableFlag = enable;
    strChannel = QString::number(n);
    xml_Grs_KNR = "";
    xml_Grs = "<MSL_MSG><PNR>21</PNR><GRS><GRP><GNR>1</GNR><GNS>1</GNS><SIO>1</SIO> </GRP></GRS>";
    qDebug()<<"^^^^^^^^^^^^^^^^^^^^^^^^^ strChannel::"<<strChannel;
}

/******************************************************/
//type = 1   发送 切断使能
//type = 2   发送 使能
//type = 3   发送 alive
//type = 4   发送 复位
/******************************************************/
void TightenThread::sendCmdMessage(int type)
{
    if(tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        if(type == 1) //send reset
        {
            sendCmdToCs351(1);//send disable
        }
        else if(type == 2)//send enable
        {
            sendCmdToCs351(2);
            qDebug() << "send operate" << carInfor.proNo[groupNumCh] << carInfor.boltNum[groupNumCh];
        }
        else if(type == 3) //send alive
        {
            sendCmdToCs351(3);
        }
        else//send disable
        {
            sendCmdToCs351(4);//send reset
        }
    }
}
/*****************************************************/
//4700 port send cmd to cs351
//cmd = 1 send disable;
//cmd = 2 send enable;
//cmd = 3 send alive;
//cmd = 4 send reset;
/*****************************************************/
void TightenThread::sendCmdToCs351(int type)
{
    QString xmlCmdHead = "55AA0001000000000002000000000000";
    QString Data_Xml_T = "";
    QString strProNo = carInfor.proNo[0];//testPro;//;
    QString cs351ProNo = carInfor.proNo[GProgramIndex];
    qDebug()<<"this is Cs351 program number"<<cs351ProNo;
    QString strBoltNum = QString::number(carInfor.boltNum[groupNumCh]);
    QString strTemp = "*****************";
    if(type == 1) // send disable
    {
        Data_Xml_T = xmlhead+resetTemp_timewrong;
        xmlCmdHead = getHeaderFunc(cycleId,Data_Xml_T.size(),type);
        Data_Xml_T = xmlCmdHead + Data_Xml_T;
        tcpSocket->write(Data_Xml_T.toLatin1());
        qDebug() << "4700 send disable:" <<Data_Xml_T;//.mid(0,32);
    }
    else if(type == 4) // send reset
    {
        QDateTime cur = QDateTime::currentDateTime();
        QString current_date = cur.toString("yyyy-MM-dd");
        QString current_time = cur.toString("hh:mm:ss");
        if(cur.date().year()>2015)
        {
            qDebug()<<"send reset cmd";
            Data_Xml_T = xmlhead+resetTemp;
            xmlCmdHead = getHeaderFunc(cycleId,Data_Xml_T.size(),type);
            Data_Xml_T.replace(278,current_date.size(),current_date).replace(299,current_time.size(),current_time);
        }
        else
        {
            qDebug()<<"send reset cmd_timewrong";
            Data_Xml_T = xmlhead+resetTemp_timewrong;
            xmlCmdHead = getHeaderFunc(cycleId,Data_Xml_T.size(),type);
        }
        Data_Xml_T = xmlCmdHead + Data_Xml_T;
        tcpSocket->write(Data_Xml_T.toLatin1());
        qDebug() << "4700 send reset:" <<Data_Xml_T.mid(0,32);
    }
    else if(type == 2)//send enable
    {
        if(ctrlType == "SB356")
        {
            xml_Grs = "<MSL_MSG><PNR>21</PNR><GRS><GRP><GNR>1</GNR><GNS>1</GNS><SIO>1</SIO> </GRP></GRS>";
            xml_TOL = "<TOL><VAL>1</VAL><GRS><GRP><GNR>1</GNR></GRP></GRS></TOL>";
            xml_STR = "<STR><VAL>1</VAL><GRS><GNR>1</GNR></GRS></STR></MSL_MSG></root:ROOT>";
            KNR_Temp = "<KAN><KNR>1</KNR></KAN>";
            for(int i=0;i<strChannel.size();i++)
            {
                xml_Grs_KNR.append(KNR_Temp.replace(10,1,strChannel.mid(i,1)));
            }
//            qDebug()<<"############"<<xml_Grs_KNR;
            xml_Grs.replace(68,1,xml_Grs_KNR);

            enableTemp = sendGroupEnable(strProNo);
            Data_Xml_T = xmlhead+enableTemp;
        }
        else
        {
            Data_Xml_T = xmlhead+enableTemp;
            if(VIN_PIN_SQL.size()>=17)
            {
                Data_Xml_T.replace(313+35,17,VIN_PIN_SQL);
            }
            else
            {
                strTemp.replace(0,VIN_PIN_SQL.size(),VIN_PIN_SQL);
                Data_Xml_T.replace(313+35,17,strTemp);
//                Data_Xml_T.replace(313+35,17,"***"+VIN_PIN_SQL);
            }
            int len1 = 0;
            if(cs351ProNo.length() == 1)
            {
                len1 = 7;
            }
            else
            {
                len1 = 8;
            }
            Data_Xml_T.replace(352+35,len1,cs351ProNo+"</PRG>");
            Data_Xml_T.replace(365+35,8,strBoltNum+"</SIO>");//7 or 8 replace 8
        }

        xmlCmdHead = getHeaderFunc(cycleId,Data_Xml_T.size(),type);
        qDebug()<<"send enable >> CsIsConnect:" <<CsIsConnect;
//        if(CsIsConnect)
//        {
            sendCysleID = cycleId;
            Data_Xml_T = xmlCmdHead + Data_Xml_T;
            //QString hhh = "55AA2447000005090001000000000000<?xml version=\"1.0\" encoding=\"UTF-8\"?><root:ROOT xmlns:root=\"http://pdps.in.audi.vwg/legacy_schema/20.7.2/root\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://pdps.in.audi.vwg/legacy_schema/20.7.2/root root.xsd\"><MSL_MSG><PNR>21</PNR><SDR><SER>1</SER></SDR><PDC><CIO>5</CIO><CNO>5</CNO></PDC><PID><PRT>T</PRT><PI1></PI1></PID><PRS><PRG>0</PRG> <SIO>1</SIO><MNO>50</MNO><NUT>0</NUT><KNR>1</KNR> </PRS><TOL><VAL>1</VAL><KNR>1</KNR> </TOL><STR><VAL>1</VAL><KNR>1</KNR></STR></MSL_MSG></root:ROOT>";
            //QString mmm = "55AA0204000005660001000000000000<?xml version=\"1.0\" encoding=\"UTF-8\"?><root:ROOT xmlns:root=\"http://pdps.in.audi.vwg/legacy_schema/20.7.2/root\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://pdps.in.audi.vwg/legacy_schema/20.7.2/root root.xsd\"><MSL_MSG><PNR>21</PNR><GRS><GRP><GNR>1</GNR><GNS>1</GNS><SIO>1</SIO><KAN><KNR>1</KNR></KAN><KAN><KNR>2</KNR></KAN><KAN><KNR>3</KNR></KAN><KAN><KNR>4</KNR></KAN><KAN><KNR>5</KNR></KAN></GRP></GRS><TOL><VAL>1</VAL><GRS><GRP><GNR>1</GNR></GRP></GRS></TOL><STR><VAL>1</VAL><GRS><GNR>1</GNR></GRS></STR></MSL_MSG></root:ROOT>";
            //tcpSocket->write(mmm.toLatin1());
//            QString mmm = "55AA9882000005660001000000000000<?xml version=\"1.0\" encoding=\"UTF-8\"?><root:ROOT xmlns:root=\"http://pdps.in.audi.vwg/legacy_schema/20.7.2/root\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://pdps.in.audi.vwg/legacy_schema/20.7.2/root root.xsd\"><MSL_MSG><PNR>21</PNR><PRS><PRG>1</PRG><SIO>1</SIO><KNR>3</KNR></PRS><TOL><VAL>1</VAL><KNR>3</KNR></TOL><STR><VAL>1</VAL><KNR>3</KNR></STR></MSL_MSG></root:ROOT>";
//            tcpSocket->write(mmm.toLatin1());
            tcpSocket->write(Data_Xml_T.toLatin1());
            tcpSocket->waitForBytesWritten();
            qDebug() << "4700 send enable:" <<Data_Xml_T;//.mid(0,32);
//        }
    }
    else //send alive
    {
        Data_Xml_T = xmlhead+aliveTemp;
        xmlCmdHead = getHeaderFunc(cycleId,Data_Xml_T.size(),type);
        Data_Xml_T = xmlCmdHead + Data_Xml_T;
        tcpSocket->write(Data_Xml_T.toLatin1());
        tcpSocket->waitForBytesWritten();
        qDebug() << "4700 send alive:" <<Data_Xml_T.mid(0,32);
    }
    cycleId ++;
    if(cycleId == 10000)
    {
        cycleId = 1;
    }
    //    qDebug() <<"/************/";
}

/***************************************************/
//get 32 data head
//alive  Typ = 0003 ; other Typ = 0001
/***************************************************/
QString TightenThread::getHeaderFunc(int mid,int size,int type)
{
    //    QByteArray xmlCmdHead = "55AA 0001 00000000 0002 0000 00000000";
    QString xmlCmdHead = "55AA";
    QString strTemp1 = "0000";
    QString strTemp2 ="";
    strTemp2 = QString::number(mid);
    strTemp1.replace(4-strTemp2.size(),strTemp2.size(),strTemp2);
    xmlCmdHead = xmlCmdHead + strTemp1;

    strTemp1 = "00000000";
    strTemp2 = QString::number(size);
    strTemp1.replace(8-strTemp2.size(),strTemp2.size(),strTemp2);
    xmlCmdHead = xmlCmdHead + strTemp1;

    if(type == 3)//alive
    {
        xmlCmdHead = xmlCmdHead + "0003";
    }
    else
    {
        xmlCmdHead = xmlCmdHead + "0001";
    }

    xmlCmdHead = xmlCmdHead + "000000000000";
    return xmlCmdHead;
}
/******************************************************/
//***********4710 port receive cs351 data
/******************************************************/
void TightenThread::recsocket()
{
    config351count = 0;
    QByteArray a = tcpSocket->readAll();
    qDebug() << "4700 port receive head" << a.mid(0,32);
    if( a.mid(20,4).toInt() != 0)
    {
        //        qDebug() << "4700 port error head" << a;
        if(a.mid(20,4).toInt() == 1)
        {
            cycleId = 1;
        }
        else
        {
        }
    }
}

/******************************************************/
//***Group send enable
/******************************************************/
QString TightenThread::sendGroupEnable(QString gNum)
{
    QString sendBuf = "";
    xml_Grs.replace(37,1,gNum);
    xml_TOL.replace(32,1,gNum);
    xml_STR.replace(27,1,gNum);
    sendBuf = xml_Grs + xml_TOL + xml_STR;
    return sendBuf;
}

QString TightenThread::funcSwitchChannel(QString channel)
{
    int x = channel.toInt();
    int mmm = x + (x +1)*100 + 10;
    QString GNR = QString::number(mmm);
    return GNR;
}

//螺栓确认，传感器使能
bool TightenThread::handleRecvStatusMess(QByteArray mess , QString boltType ,int seatNum)
{
    /*qint32 message = mess.toInt(0,16);
    qDebug()<<"this is handleRecvStatusMess"<<mess<<mid<<message;
    if(mid == 0)
    {
        if(((message & 0xffff)>>15) && (((message<<1) & 0xffff)>>15) && (((message<<2) & 0xffff)>>15))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if(mid == 1)
    {
        if((((message<<3) & 0xffff)>>15) && (((message<<4) & 0xffff)>>15) && (((message<<5) & 0xffff)>>15))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if(mid == 2)
    {
        if((((message<<6) & 0xffff)>>15) && (((message<<7) & 0xffff)>>15) && (((message<<8) & 0xffff)>>15))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if(mid == 3)
    {
        if((((message<<9) & 0xffff)>>15) && (((message<<10) & 0xffff)>>15) && (((message<<11) & 0xffff)>>15))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }*/
    bool isOK = false;
    quint16 data = ((mess.mid(60,2).toInt(0,16))*0x0100 + (mess.mid(58,2)).toInt(0,16));
    quint16 numOk[5] = {};
    int countOk = 0;
    for(int i = 0;i<12;i++){
        if(data>>(12-i-1)&1){
            qDebug()<<"sensor:"<<12-i;
            numOk[5-1-countOk] = 12 - i;
            countOk++;
        }
    }
    //显示连接的传感器编号
//    if(numOk[0]||numOk[1]||numOk[2]||numOk[3]||numOk[4]){
//        emit sendStates("传感器："+QString::number(numOk[0])+" "+
//                QString::number(numOk[1])+" "+
//                QString::number(numOk[2])+" "+
//                QString::number(numOk[3])+" "+
//                QString::number(numOk[4]));
//    }
    if(boltType == "PHEV")
    {
        switch (seatNum+1) {
        case 1://位置1
            if(numOk[0] == 1 && numOk[1] == 4 && numOk[2] == 5
                    &&numOk[3] == 7 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 2:
            if(numOk[0] == 1 && numOk[1] == 3 && numOk[2] == 6
                    &&numOk[3] == 7 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 3:
            if(numOk[0] == 1 && numOk[1] == 4 && numOk[2] == 6
                    &&numOk[3] == 7 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 4:
            if(numOk[0] == 1 && numOk[1] == 3 && numOk[2] == 5
                    &&numOk[3] == 7 && numOk[4] == 12){
                seatNum++;
                sendEnableFlag = false;
            }else{
                return false;
            }
            break;
        default:
            break;
        }
        seatNum = seatNum >= 3 ?0:GProgramIndex;
        isOK = true;
    }
    if(boltType == "BEV"){
        switch (seatNum+1) {
        case 1://位置1
            if(numOk[0] == 2 && numOk[1] == 4 && numOk[2] == 5
                    &&numOk[3] == 11 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 2:
            if(numOk[0] == 2 && numOk[1] == 3 && numOk[2] == 6
                    &&numOk[3] == 11 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 3:
            if(numOk[0] == 2 && numOk[1] == 4 && numOk[2] == 6
                    &&numOk[3] == 11 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 4:
            if(numOk[0] == 2 && numOk[1] == 3 && numOk[2] == 5
                    &&numOk[3] == 11 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 5:
            if(numOk[0] == 1 && numOk[1] == 4 && numOk[2] == 5
                    &&numOk[3] == 10 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 6:
            if(numOk[0] == 1 && numOk[1] == 3 && numOk[2] == 6
                    &&numOk[3] == 10 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 7:
            if(numOk[0] == 1 && numOk[1] == 4 && numOk[2] == 6
                    &&numOk[3] == 10 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 8:
            if(numOk[0] == 1 && numOk[1] == 3 && numOk[2] == 5
                    &&numOk[3] == 10 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 9:
            if(numOk[0] == 2 && numOk[1] == 4 && numOk[2] == 5
                    &&numOk[3] == 8 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 10:
            if(numOk[0] == 2 && numOk[1] == 3 && numOk[2] == 6
                    &&numOk[3] == 8 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 11:
            if(numOk[0] == 2 && numOk[1] == 4 && numOk[2] == 6
                    &&numOk[3] == 8 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 12:
            if(numOk[0] == 2 && numOk[1] == 3 && numOk[2] == 5
                    &&numOk[3] == 8 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 13:
            if(numOk[0] == 1 && numOk[1] == 4 && numOk[2] == 5
                    &&numOk[3] == 7 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 14:
            if(numOk[0] == 1 && numOk[1] == 3 && numOk[2] == 6
                    &&numOk[3] == 7 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 15:
            if(numOk[0] == 1 && numOk[1] == 4 && numOk[2] == 6
                    &&numOk[3] == 7 && numOk[4] == 12){
                seatNum++;
            }else{
                return false;
            }
            break;
        case 16:
            if(numOk[0] == 1 && numOk[1] == 3 && numOk[2] == 5
                    &&numOk[3] == 7 && numOk[4] == 12){
                seatNum++;
                sendEnableFlag = false;
            }else{
                return false;
            }
            break;
        default:
            break;
        }
        seatNum = seatNum >= 15 ?0:GProgramIndex;
        isOK = true;
    }
    return isOK;
}

