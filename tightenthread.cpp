#include "tightenthread.h"
#define DTdebug()  qDebug( ) << __FILE__ << "|"  << __LINE__ << ":"

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
    YENCODER = 0;
    AENCODER = 0;
    OENCODER = 0;
    IOPOSITION = "";
    YENTol = 0;
    AENTol = 0;
    OENTol = 0;
    TOLERANEC = 0;
    YENCODERTemp = 0;
    AENCODERRemp = 0;
    IOPOSITIONTemp = "";
    last_staus = false;

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
//    DTdebug()<<"GrecvStatusMsee"<<enableFlag;
    timerCount++;
    timerCount1++;


    bool currentStaus = false;//当前定位与螺栓是否匹配
    if((GgetBoltIsOk == "Yes" && GisTightFlag)|| GgetBoltIsOk == "Test")
    {
        coordinatesFunc(GrecvStatusMsee);//提取定位数据
        currentStaus = handleEnableFlag();
        pointDebug()<<"GrecvStatusMsee:"<<currentStaus<<last_staus<<GrecvStatusMsee.mid(58,16);
        if(GrecvStatusMsee.mid(58,16) != "0000000000000000")
        {
            if(currentStaus != last_staus)//定位状态有变化
            {
                qDebug()<<"handleEnableFlagChange";
                last_staus= currentStaus;
                sendEnableFlag = true;//定位状态 准确/不准确 发送 给使能/切断使能
            }
        }
        else
        {
            qDebug()<<"plcisalived";
        }
    }

    if(sendEnableFlag)
    {
        if(GgetBoltIsOk != "Yes")//未开螺栓定位 通过MainWindow::sendOperate1发过来的enableFlag状态来切断使能或给使能
        {
            if(!enableFlag)
            {
                sendCmdMessage(1);//重置 切断使能
            }
            else
            {
                sendCmdMessage(2);//给使能
            }
            sendEnableFlag = false;
            timerCount = 0;
        }
        else//开螺栓定位 按坐标是否符合设置来给使能或切断使能
        {
            qDebug()<<"EnableFlag";
            if(currentStaus && enableFlag)
            {
                sendCmdMessage(2);//给使能
            }
            else
            {
                sendCmdMessage(1);//重置 切断使能
            }
            sendEnableFlag = false;
            timerCount = 0;
        }

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
    sendEnableFlag = true;//打开使能发送开关
    enableFlag = enable;//true给使能 false切断使能
    GiswhichChannel = n;//记录本次通道号
    strChannel = QString::number(n);
    xml_Grs_KNR = "";
    xml_Grs = "<MSL_MSG><PNR>21</PNR><GRS><GRP><GNR>1</GNR><GNS>1</GNS><SIO>1</SIO> </GRP></GRS>";
    qDebug()<<"^^^^^^^^^^^^^^^^^^^^^^^^^ strChannel::"<<strChannel<<enable;
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
//    QString cs351ProNo = carInfor.proNo[GProgramIndex];
    QString cs351ProNo = carInfor.proNo[groupNumCh];
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

void TightenThread::receiveA050ErrorChannel(bool isok, QString channel)
{
    qDebug()<<"receiveA050ErrorChannel"<<isok<<channel;
    QString xmlCmdHead = "55AA0001000000000002000000000000";
    QString Data_Xml_T = "";
    xml_Grs = "<MSL_MSG><PNR>21</PNR><GRS><GRP><GNR>1</GNR><GNS>1</GNS><SIO>1</SIO> </GRP></GRS>";
    xml_TOL = "<TOL><VAL>1</VAL><GRS><GRP><GNR>1</GNR></GRP></GRS></TOL>";
    xml_STR = "<STR><VAL>1</VAL><GRS><GNR>1</GNR></GRS></STR></MSL_MSG></root:ROOT>";
    KNR_Temp = "<KAN><KNR>1</KNR></KAN>";
    xml_Grs_KNR = "";
    xml_Grs_KNR.append(KNR_Temp.replace(10,1,channel));
    xml_Grs.replace(68,1,xml_Grs_KNR);
    QString GNRTemp = funcSwitchChannel(channel);//算出GNR的值
    enableTemp = sendGroupEnable(GNRTemp);//把GNR插入报文对应位置
    Data_Xml_T = xmlhead+enableTemp;
    xmlCmdHead = getHeaderFunc(cycleId,Data_Xml_T.size(),2);//计算报文头
    sendCysleID = cycleId;
    Data_Xml_T = xmlCmdHead + Data_Xml_T;
    //QString hhh = "55AA2447000005090001000000000000<?xml version=\"1.0\" encoding=\"UTF-8\"?><root:ROOT xmlns:root=\"http://pdps.in.audi.vwg/legacy_schema/20.7.2/root\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://pdps.in.audi.vwg/legacy_schema/20.7.2/root root.xsd\"><MSL_MSG><PNR>21</PNR><SDR><SER>1</SER></SDR><PDC><CIO>5</CIO><CNO>5</CNO></PDC><PID><PRT>T</PRT><PI1></PI1></PID><PRS><PRG>0</PRG> <SIO>1</SIO><MNO>50</MNO><NUT>0</NUT><KNR>1</KNR> </PRS><TOL><VAL>1</VAL><KNR>1</KNR> </TOL><STR><VAL>1</VAL><KNR>1</KNR></STR></MSL_MSG></root:ROOT>";
    //QString mmm = "55AA0204000005660001000000000000<?xml version=\"1.0\" encoding=\"UTF-8\"?><root:ROOT xmlns:root=\"http://pdps.in.audi.vwg/legacy_schema/20.7.2/root\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://pdps.in.audi.vwg/legacy_schema/20.7.2/root root.xsd\"><MSL_MSG><PNR>21</PNR><GRS><GRP><GNR>1</GNR><GNS>1</GNS><SIO>1</SIO><KAN><KNR>1</KNR></KAN><KAN><KNR>2</KNR></KAN><KAN><KNR>3</KNR></KAN><KAN><KNR>4</KNR></KAN><KAN><KNR>5</KNR></KAN></GRP></GRS><TOL><VAL>1</VAL><GRS><GRP><GNR>1</GNR></GRP></GRS></TOL><STR><VAL>1</VAL><GRS><GNR>1</GNR></GRS></STR></MSL_MSG></root:ROOT>";
    //tcpSocket->write(mmm.toLatin1());
//            QString mmm = "55AA9882000005660001000000000000<?xml version=\"1.0\" encoding=\"UTF-8\"?><root:ROOT xmlns:root=\"http://pdps.in.audi.vwg/legacy_schema/20.7.2/root\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://pdps.in.audi.vwg/legacy_schema/20.7.2/root root.xsd\"><MSL_MSG><PNR>21</PNR><PRS><PRG>1</PRG><SIO>1</SIO><KNR>3</KNR></PRS><TOL><VAL>1</VAL><KNR>3</KNR></TOL><STR><VAL>1</VAL><KNR>3</KNR></STR></MSL_MSG></root:ROOT>";
//            tcpSocket->write(mmm.toLatin1());
    tcpSocket->write(Data_Xml_T.toLatin1());
    tcpSocket->waitForBytesWritten();
    qDebug() << "4700 error send enable:" <<Data_Xml_T;//.mid(0,32);
    cycleId++;
    if(cycleId == 10000)
    {
        cycleId = 1;
    }

}

QString TightenThread::funcSwitchChannel(QString channel)
{
    int x = channel.toInt();
    int mmm = x + (x +1)*100 + 10;
    QString GNR = QString::number(mmm);
    return GNR;
}

void TightenThread::handlENCODER()
{
    qDebug()<<"handlENCODER:"<<GRFIDMessage;
    if(GRFIDMessage.mid(0,1) == "P")
    {
        if(GencoderFlag)   //is encoder
        {
            handINIMessage(1);
        }
        else    //is IOPosition
        {
            handINIMessage(2);
        }
    }
    else if(GRFIDMessage.mid(0,1) == "B")
    {
        if(GencoderFlag)   //is encoder
        {
            handINIMessage(3);
        }
        else    //is IOPosition
        {
            handINIMessage(4);
        }
    }
}

void TightenThread::handINIMessage(int temp)
{
    qDebug()<<"handINIMessage temp"<<temp;
    QFile stepJson("/encoder.json");
    if(stepJson.open(QIODevice::ReadOnly))
    {
        QByteArray json = stepJson.readAll();
        stepJson.close();

        QJson::Parser parser;
        bool ok;

        QVariantMap currentStep = parser.parse(json, &ok).toMap();
        if (!ok)
        {
            qDebug()<<"step.json An error occurred during parsing";
        }
        else
        {
            DTdebug()<<" OPEN true"<<temp;
            //return currentStep;
        }
        if(temp == 1)
        {
            checkList  = currentStep["PHEVENCODER"].toList();
        }
        else if(temp == 2)
        {
            checkList  = currentStep["PHEVIOPOSITION"].toList();
        }
        else if(temp == 3)
        {
            checkList  = currentStep["BEVENCODER"].toList();
        }
        else if(temp == 4)
        {
            checkList  = currentStep["BEVIOPOSITION"].toList();
        }
        else
        {
            qDebug()<<"handINIMessage is error";
        }


        if(checkList.isEmpty())//对比配置好的坐标信息
        {
            qDebug()<<"checkList is empty 0:"<<GProgramIndex;
        }
        else
        {
            foreach (QVariant tempStep, checkList)//取当前螺栓的定位配置信息
            {
                QVariantMap encoderMap = tempStep.toMap();
        //        DTdebug()<<"encoderMap"<<encoderMap["boltID"].toInt()<<GProgramIndex;
                if(encoderMap["boltID"].toInt() == (GProgramIndex))
                {
                    YENCODER = encoderMap["YENCODER"].toDouble();
                    AENCODER = encoderMap["AENCODER"].toDouble();
                    OENCODER = encoderMap["OENCODER"].toDouble();
                    AENTol = encoderMap["AENTol"].toDouble();
                    OENTol = encoderMap["OENTol"].toDouble();
                    YENTol = encoderMap["YENTol"].toDouble();
                    TOLERANEC = encoderMap["TOLERANEC"].toDouble();
                    IOPOSITION = encoderMap["IOPOSITION"].toString();
                    //qDebug()<<"this is true GProgramIndex"<<GProgramIndex;
                }
            }
        }
    }
    else
    {
        qDebug()<<"step.json stepJson read open fail";
    }

}


//处理位置传感器的数据
bool TightenThread::handleEnableFlag()
{
    if(checkList.size() > 0){
        if(Station.mid(0,2) == "OP" || Station.mid(0,1) == "M")//手动/BMCE工位用的线性传感器，允许设置误差
        {
            if((qAbs(YENCODER-AENCODERRemp) <= AENTol) && (qAbs(AENCODER-YENCODERTemp) <= YENTol))//位移跟角度，以前名字取反了，不要惊讶(YENCODER-AENCODERRemp)
            {
                qDebug()<<"thisOP is true!!!";
                return true;
            }
            else
            {
                qDebug()<<"thisOP is false";
                return false;
            }
        }
        else if(Station.mid(0,2) == "VM")//模组线用的点位传感器，直接PLC传过来的数据，没有误差一说
        {
            if(IOPOSITION.toLocal8Bit().data() == IOPOSITIONTemp)
            {
                qDebug()<<"this is io true";
                return true;
            }
            else
            {
                qDebug()<<"this is io false";
                return false;
            }
        }
        else
        {
            qDebug()<<"station error"<<Station;
            return false;
        }
    }else{
        qDebug()<<"not checkList:"<<checkList.size();
        return false;
    }


}

qint64 TightenThread::matchHandle(QByteArray message)
{
    DTdebug()<<"message bytearray"<<message<<GProgramIndex;
    QByteArray a1 = message;
    QByteArray a11 = a1.mid(0,4);
    QByteArray a12 = a1.mid(4,4);
    qint64 b1 = a11.toInt(0,16);
    qint64 b2 = a12.toInt(0,16);
    QString c1 = QString::number((b1<<16) + b2);
    qint64 d1 = c1.toDouble();
    qDebug()<<"return qint64"<<d1;
    return d1;
}

void TightenThread::receiveNewCoordinates(int boltNum)
{
    if(checkList.isEmpty())//对比配置好的坐标信息
    {
        qDebug()<<"checkList is empty:"<<boltNum;
    }
    else
    {
        foreach (QVariant tempStep, checkList)//取当前螺栓的定位配置信息
        {
            QVariantMap encoderMap = tempStep.toMap();
            //读取配置的第几行==当前打第几颗螺栓
            if(encoderMap["boltID"].toInt() == (boltNum))
            {
                YENCODER = encoderMap["YENCODER"].toDouble();
                AENCODER = encoderMap["AENCODER"].toDouble();
                OENCODER = encoderMap["OENCODER"].toDouble();
                AENTol = encoderMap["AENTol"].toDouble();
                OENTol = encoderMap["OENTol"].toDouble();
                YENTol = encoderMap["YENTol"].toDouble();
                TOLERANEC = encoderMap["TOLERANEC"].toDouble();
                IOPOSITION = encoderMap["IOPOSITION"].toString();
            }
        }
    }
}

//提取定位数据并做简单处理
void TightenThread::coordinatesFunc(QByteArray message)
{
    if(message.size() > 80 && message.mid(58,16) != "0000000000000000")
    {
        YENCODERTemp = matchHandle(message.mid(58,8));
        AENCODERRemp = matchHandle(message.mid(66,8));
        IOPOSITIONTemp = message.mid(58,4);
    }
    else
    {
        qDebug()<<"messageError!";
    }

    if(GgetBoltIsOk == "Test")//Test为设置定位采集坐标用 把XY轴定位值发到定位设置界面
    {
        emit sendEncoderToWindow(YENCODERTemp,AENCODERRemp,IOPOSITIONTemp);
    }
}
