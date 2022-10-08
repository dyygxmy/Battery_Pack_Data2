#include "worker.h"

Worker::Worker(QObject *parent) :
    QObject(parent)
{
    CurveStr = "";
    this->moveToThread(&m_thread);
    m_thread.start();
    CyclePrevious="";
    channelPrevious = "";
    ctrl_Type = "CS351";
    tightenNum = 0;
}
void Worker::fun1()
{
    qDebug()  << "work Thread start...";
    tcpServer = new QTcpServer(this);
    if(!tcpServer->listen(QHostAddress::Any,4710))
    {  //**本地主机的4710端口，如果出错就输出错误信息，并关闭
        qDebug() << tcpServer->errorString();
    }
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(DataReceived()));
    heartTimer = new QTimer(this);
    connect(heartTimer,SIGNAL(timeout()),this,SLOT(heartTimerFunc()));
    timerCount=0;
    socketFlag4710 = false;
    channelData = 0;
    dutouFlag = false;
    moduleFlag = false;
}
/***************************************************/
//link success
/***************************************************/
void Worker::DataReceived()
{
    qDebug() << "4710 port  ************ connect success";
    m_pTcpSocket = tcpServer->nextPendingConnection();
    connect(m_pTcpSocket, SIGNAL(readyRead()),this, SLOT(recMessage()));
    heartTimer->start(5000);
    emit havedconnect(true);
    socketFlag4710 = true;
}
/***************************************************/
//5000ms timer function
/***************************************************/
void Worker::heartTimerFunc()
{
    timerCount++;
    if(timerCount>3)
    {
        qDebug() << "4710 heart time out";
        timerCount=0;
        RDYY = false;
        emit disconnectTellPortA();
        heartTimer->stop();
        closePortBTcpServer();
    }
}
/***************************************************/
//protA disconnect portB
/***************************************************/
void Worker::portAsendPortB()
{
    RDYY = false;
    heartTimer->stop();
    closePortBTcpServer();
}
/***************************************************/
//close link
/***************************************************/
void Worker::closePortBTcpServer()
{
    qDebug()<<"************closePortBTcpServer****************";
    //    if(tcpServer->hasPendingConnections())
    if(socketFlag4710)
    {
        socketFlag4710 = false;
        qDebug()<<"m_pTcpSocket connected or connecting";
        m_pTcpSocket->abort();
        m_pTcpSocket->deleteLater();
    }
}

/***************************************************/
//xml解析
/***************************************************/
void Worker::xml_parse(QByteArray temp)
{
    QFile file("/a.xml");
    //新建文件并保存
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << temp.mid(32);
    file.close();
    //打开xml文件
    QDomDocument doc;
    file.open(QIODevice::ReadOnly);
    //qDebug() <<"read all" << file.readAll();
    doc.setContent(&file);
    file.close();
//      qDebug() << "receiveResult" << temp;
    //获得根节点
    QDomElement root = doc.documentElement();
    //获得第一个子节点:版本
    node = root.firstChild();
    qDebug() << node.nodeName()<< "nodename";
    if(node.nodeName() == "PRC_SST")
    {
        //曲线
        CurveStr = temp;
        qDebug()<<"thisissyss"<<SYSS;
//        if(Station == "A050new")
        if(GTightResetOnline == "Yes")
        {

        }
        else
        {
            if(SYSS!="ING")
            {
                return;
            }
        }
        qDebug()<<"this is xml xml_parse";
        int i,j,k,l,m,n,p,q,r,blc=-1,num=0;
        QString DATE_blc,TIME_blc,STATE_blc="NOK",MI_blc,WI_blc,Cycle_CSR,Program="-1",Channel="0";
        QString MI_blc_max;//最大扭矩
        QString MI_blc_min;//最小扭矩
        QString WI_blc_max;//最大角度
        QString WI_blc_min;//最小角度


        int PRC_SST_child = node.toElement().childNodes().length();

        for(i=0;i<PRC_SST_child;i++)
        {
            if(node.toElement().childNodes().at(i).toElement().nodeName()=="PAR")
            {
                int PAR_child = node.toElement().childNodes().at(i).toElement().childNodes().length();
                for(j=0;j<PAR_child;j++)
                {
                    if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().nodeName()=="FAS")
                    {
                        int FAS_child = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().length();
                        for(k=0;k<FAS_child;k++)
                        {
                            if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().nodeName()=="GRP")
                            {
                                int GPR_child = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().length();
                                for(l=0;l<GPR_child;l++)
                                {
                                    if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().nodeName()=="TIP")
                                    {
                                        int TIP_child = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().length();
                                        for(m=0;m<TIP_child;m++)
                                        {
                                            //通道号
                                            if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().nodeName()=="KNR")
                                                Channel = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().text();
                                            //程序号
                                            if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().nodeName()=="PRG")
                                                Program = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().text();
                                            //循环号
                                            if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().nodeName()=="CSR")
                                                Cycle_CSR = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().text();
                                            //日期
                                            if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().nodeName()=="DAT")
                                                DATE_blc = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().text();
                                            //时间
                                            if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().nodeName()=="TIM")
                                                TIME_blc = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().text();
                                            //I0状态
                                            if(Program == "99")
                                            {
                                                STATE_blc = "NOK";
                                                GNokFlag = true;
                                            }
                                            else
                                            {
                                                if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().nodeName()=="STA")
                                                {
                                                    STATE_blc = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().text();
                                                    GPareXMLFlag = true;//收到“STA”格式的拧紧数据
                                                    if(STATE_blc == "IO")
                                                    {
                                                        STATE_blc = "OK";
                                                        GNokFlag = false;
                                                    }
                                                    else if(STATE_blc == "NIO")
                                                    {
                                                        STATE_blc = "NOK";
                                                        GNokFlag = true;
                                                        qDebug()<<"this is GNokFlag"<<Channel;
                                                        GNokNumber.append(Channel);
                                                    }
                                                    else
                                                    {
                                                        STATE_blc = "NOK";
                                                        GNokFlag = true;
                                                        GNokNumber.append(Channel);
                                                    }
                                                    qDebug()<<"this is state_ble"<<STATE_blc;
                                                }
                                            }
                                            if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().nodeName()=="BLC")
                                            {
                                                blc=m;
                                            }
                                        }
                                        if(blc!=-1)
                                        {
                                            qDebug()<<"this is blc not equ -1";
                                            int BLC_child = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().length();
                                            for(n=0;n<BLC_child;n++)
                                            {
                                                if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().nodeName()=="PRO")
                                                {
                                                    int PRO_child = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().length();
                                                    for(p=0;p<PRO_child;p++)
                                                    {
                                                        if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().nodeName()=="MAR")
                                                        {
                                                            int MAR_child = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().length();
                                                            for(q=0;q<MAR_child;q++)
                                                            {
                                                                if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(q).toElement().nodeName()=="NAM"&&node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(q).toElement().text()=="MI")
                                                                {
                                                                    for(r=0;r<MAR_child;r++)
                                                                    {
                                                                        //扭矩
                                                                        if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(r).toElement().nodeName()=="VAL")
                                                                            MI_blc = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(r).toElement().text();
                                                                        qDebug()<<"this is MI_blc"<<MI_blc;
                                                                    }
                                                                }
                                                                if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(q).toElement().nodeName()=="NAM"&&node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(q).toElement().text()=="WI")
                                                                {
                                                                    for(r=0;r<MAR_child;r++)
                                                                    {
                                                                        //角度
                                                                        if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(r).toElement().nodeName()=="VAL")
                                                                            WI_blc = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(r).toElement().text();
                                                                        qDebug()<<"this is WI_blc"<<WI_blc;
                                                                    }
                                                                }
                                                                if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(q).toElement().nodeName()=="NAM"&&node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(q).toElement().text()=="W+")
                                                                {
                                                                    for(r=0;r<MAR_child;r++)
                                                                    {
                                                                        //角度最大值
                                                                        if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(r).toElement().nodeName()=="VAL")
                                                                            WI_blc_max = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(r).toElement().text();
                                                                        qDebug()<<"this is WI_blc_max"<<WI_blc_max;
                                                                    }
                                                                }
                                                                if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(q).toElement().nodeName()=="NAM"&&node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(q).toElement().text()=="W-")
                                                                {
                                                                    for(r=0;r<MAR_child;r++)
                                                                    {
                                                                        //角度最小值
                                                                        if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(r).toElement().nodeName()=="VAL")
                                                                            WI_blc_min = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(r).toElement().text();
                                                                        qDebug()<<"this is WI_blc_min"<<WI_blc_min;
                                                                    }
                                                                }
                                                                if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(q).toElement().nodeName()=="NAM"&&node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(q).toElement().text()=="M+")
                                                                {
                                                                    for(r=0;r<MAR_child;r++)
                                                                    {
                                                                        //扭矩最大值
                                                                        if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(r).toElement().nodeName()=="VAL")
                                                                            MI_blc_max = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(r).toElement().text();
                                                                        qDebug()<<"this is MI_blc_max"<<MI_blc_max;
                                                                    }
                                                                }
                                                                if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(q).toElement().nodeName()=="NAM"&&node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(q).toElement().text()=="M-")
                                                                {
                                                                    for(r=0;r<MAR_child;r++)
                                                                    {
                                                                        //扭矩最小值
                                                                        if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(r).toElement().nodeName()=="VAL")
                                                                            MI_blc_min = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(r).toElement().text();
                                                                        qDebug()<<"this is MI_blc_min"<<MI_blc_min;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        if(Program != "-1")
                                        {
                                            qDebug() << "work Thread parse data:" << VIN_PIN_SQL+"||"+Cycle_CSR+"||"+MI_blc+"||"+WI_blc+"||"+Program+"||"+Channel;
                                            if( (Cycle_CSR != CyclePrevious)||(Channel != channelPrevious) )
                                            {
                                                if(MI_blc == "")
                                                    MI_blc = "0";
                                                if(WI_blc  == "")
                                                    WI_blc = "0";
//                                                if(SYSS == "ING" || Station == "A050new")
                                                if(SYSS == "ING" || GTightResetOnline == "Yes")
                                                {
                                                    double torque_max = 0.0;
                                                    double torque_min = 0.0;
                                                    double Angle_max = 0.0;
                                                    double Angle_min = 0.0;
                                                    if(STATE_blc == "OK")
                                                    {
                                                        if(bound_enabled)
                                                        {
                                                            QSettings *config = new QSettings("/config_bound.ini", QSettings::IniFormat);
                                                            torque_max = config->value("/ProNumber"+Program+"/Torque_max").toDouble();
                                                            torque_min = config->value("/ProNumber"+Program+"/Torque_min").toDouble();
                                                            Angle_max = config->value("/ProNumber"+Program+"/Angle_max").toDouble();
                                                            Angle_min = config->value("/ProNumber"+Program+"/Angle_min").toDouble();
                                                            delete config;
                                                            if(torque_max == 0)
                                                            {
                                                                torque_max = MI_blc.toDouble();
                                                                torque_min = MI_blc.toDouble();
                                                            }
                                                            if(torque_min == 0)
                                                                torque_min = MI_blc.toDouble();
                                                            if(Angle_max == 0)
                                                            {
                                                                Angle_max = WI_blc.toDouble();
                                                                Angle_min = WI_blc.toDouble();
                                                            }
                                                            if(Angle_min == 0)
                                                                Angle_min = WI_blc.toDouble();
                                                            if((MI_blc.toDouble()<torque_min) || (MI_blc.toDouble()>torque_max) ||
                                                                    (WI_blc.toDouble()<Angle_min) || (WI_blc.toDouble() >Angle_max))
                                                            {
                                                                STATE_blc = "NOK";
                                                                GNokFlag = true;
                                                                qDebug()<<"this is GNokNumber add"<<Channel;
                                                                GNokNumber.append(Channel);
                                                            }
                                                        }
                                                    }

                                                    CyclePrevious = Cycle_CSR;
                                                    channelPrevious = Channel;

                                                    DATA_STRUCT demo;
                                                    demo.data_model[0] = DATE_blc;
                                                    demo.data_model[1] = TIME_blc;
                                                    demo.data_model[2] = STATE_blc;
                                                    demo.data_model[3] = MI_blc;
                                                    demo.data_model[4] = WI_blc;
                                                    if(ctrl_Type == "SB356")
                                                    {
//                                                        if(Station == "A050new" && GErrorTight)
                                                        if(GErrorTight)
                                                        {
                                                            demo.data_model[5] = carInfor.boltSN[(Channel.toInt() - 1)];
                                                            demo.data_model[6] = pack_SN;
                                                        }
                                                        else
                                                        {
                                                            demo.data_model[5] = carInfor.boltSN[num];
                                                            demo.data_model[6] = pack_SN;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        demo.data_model[5] = carInfor.boltSN[groupNumCh];
                                                        demo.data_model[6] = VIN_PIN_SQL;
                                                    }
                                                    demo.data_model[7] = Cycle_CSR;
                                                    demo.data_model[8] = CurveStr.mid(32);
                                                    demo.data_model[9] = Program;
                                                    demo.data_model[10] = Channel;


                                                    demo.data_model[11] = Type;
                                                    if(STATE_blc == "OK")
                                                    {
                                                        demo.data_model[12] = QString::number(BoltOrder[0]++);
                                                    }
                                                    else
                                                    {
                                                        demo.data_model[12] = QString::number(BoltOrder[0]);
                                                    }
                                                    demo.data_model[13] = QString::number(torque_max);
                                                    demo.data_model[14] = QString::number(torque_min);
                                                    demo.data_model[15] = "0";

                                                    demo.data_model[16] = WI_blc_max;//最大角度
                                                    demo.data_model[17] = WI_blc_min;//最小角度
                                                    demo.data_model[18] = MI_blc_max;//最大扭矩
                                                    demo.data_model[19] = MI_blc_min;//最小扭矩


                                                    QVariant DataVar;
                                                    DataVar.setValue(demo);
                                                    if(ctrl_Type != "SB356")
                                                    {
                                                        if(STATE_blc == "NOK")
                                                        {
                                                            if(Program == "99")
                                                            {
                                                                qDebug()<<"this is recycle";
                                                            }
                                                            else
                                                            {
                                                                GBlotErrorCount++;
                                                            }
                                                         }
                                                        else if(STATE_blc == "OK")
                                                        {
                                                            GBlotErrorCount = 0;
                                                        }
                                                        emit send_mainwindow(MI_blc,WI_blc,STATE_blc,0,groupNumCh);
                                                    }
                                                    else//SB356拧紧机的拧紧结果
                                                    {
                                                        if(Station.mid(0,4) == "A350")
                                                        {
                                                            if(STATE_blc == "OK")
                                                            {
                                                                GtightGlueresult = 1;
                                                            }
                                                            else
                                                            {
                                                                qDebug()<<"GtightGlueresult is change";
                                                                GtightGlueresult = 2;
                                                            }
                                                        }
                                                        GBoltAllNum++;
                                                        GrecEnableCount++;
                                                        if(STATE_blc != "OK")//不合格数据把通道号记下来
                                                        {
                                                            GErrorChannel.append(QString::number(Channel.toInt()));
                                                            qDebug()<<"this is nok"<<GErrorChannel;
                                                        }
                                                        emit send_mainwindowSB356(MI_blc,WI_blc,STATE_blc,Channel.toInt(),num);
                                                    }
                                                    qDebug()<<"demo***"<<demo.data_model[2];
                                                    emit sendfromworkthread(DataVar);//发到SqlThreadSVW2处理拧紧数据
                                                    if(GRemaningCountSpearhead > 0){//枪头剩余使用次数减1并保存到文件
                                                        GRemaningCountSpearhead--;
                                                    }
                                                    QSettings("/config.ini", QSettings::IniFormat).setValue("baseinfo/remaningCountSpearhead",GRemaningCountSpearhead);
                                                    if(GRemaningCountSpearhead < GWarningCountSpearhead){//低于阀值报警
                                                        emit sendWarningSpearhead();
                                                    }
                                                    num++;
                                                }
                                            }
                                            else
                                            {
                                                qDebug()<<"Duplicate data";
                                            }
//                                            qDebug() << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxx" << CurveStr;
                                        }
                                        else
                                        {
                                            qDebug() << "error curve" << CurveStr;
                                        }
                                        blc=-1;
                                        STATE_blc="NOK";
                                        Program="-1";
                                        Channel="0";
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if(ctrl_Type == "SB356")
        {
            if(channelData == 1)
            {
                qDebug()<<"QualifiedNum111"<<QualifiedNum<<"BoltTotalNum"<<BoltTotalNum;
//                if(Station == "A050new")
                if(GTightResetOnline == "Yes")
                {
                    qDebug()<<"this is A050new station"<<GErrorChannel;
                    sleep(1);
                }
                else
                {
                    sleep(1);
                    emit sendTighteningResult1(3,1);
                }
            }
            else if(channelData == 2)
            {
                sleep(1);
                qDebug()<<"QualifiedNum222"<<QualifiedNum<<"BoltTotalNum"<<BoltTotalNum;
                emit sendTighteningResult2(3,1);
            }
        }
    }
    else
    {//心跳 解析 EVT
        //        qDebug() << "4710 receive data:" << temp;
        //        qDebug() << "evt herehere";
        int MSL_MSG_child = node.toElement().childNodes().length();
        int i,j,k,l=0;
        for(i=0;i<MSL_MSG_child;i++)
        {
            if(node.toElement().childNodes().at(i).toElement().nodeName()=="EVT")
            {
                int EVT_child = node.toElement().childNodes().at(i).toElement().childNodes().length();
                for(j=0;j<EVT_child;j++)
                {
                    if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().nodeName()=="STS")
                    {
                        int STS_child = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().length();
                        for(k=0;k<STS_child;k++)
                        {
                            if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().nodeName()=="ONC")
                            {
                                int ONC_child = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().length();
                                for(l=0;l<ONC_child;l++)
                                {
                                    if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().nodeName()=="RDY")
                                    {
                                        int RDY = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().text().toInt();
                                        //                                        qDebug() << "RDY 1" << RDY;
                                        if(RDY == 1)
                                        {//复位成功
                                            if(!RDYY)
                                            {
                                                qDebug() << "******cs351 RDYY(true)";
                                                RDYY = true;
                                                emit  havedconnect(true);
                                            }
                                        }
                                        else
                                        {// 复位失败 报错 给主界面发消息。
//                                            qDebug()<< "******cs351 RDYY(false)";
//                                            RDYY = false;
//                                            emit  havedconnect(false);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
//    qDebug()<<"GNokNumber"<<GNokNumber;
    if(!GNokNumber.isEmpty())
    {
        qDebug()<<"this is GNokNumber false";
        GNokFlagTemp = true;
    }
    else
    {
        qDebug()<<"this is GNokNumber";
        GNokFlagTemp = false;
    }
//    if(Station == "A050new")
    if(GTightResetOnline == "Yes")
    {
        //收到“STA”格式的拧紧数据 && 上次有几个不合格计数 == 本次收到第几次拧紧结果
        if(GPareXMLFlag && (GsendEnableCount == GrecEnableCount))//初始状态或者上次全合格就是5，到第5个才会进来
        {
            GPareXMLFlag = false;
            GrecEnableCount = 0;
            if(GErrorChannel.isEmpty())//本次都合格
            {
                qDebug()<<"this tinght is success";
                GgetRobotTightenReadyFlag = false;
                GsendEnableCount = 5;
                emit sendTighteningResult1(3,1);
            }
            else//有不合格螺栓
            {
                GsendEnableCount = GErrorChannel.size();//记录本次不合格螺栓数量
                qDebug()<<"there are NOK in this tighten:"<<GsendEnableCount<<GErrorChannel;
                emit sendTighteningResult1(6,1);
            }
            GrecErrorResult = true;
        }
    }
}

/***************************************************/
//receive data and Ack
/***************************************************/
void Worker::recMessage()
{
    //接受到信息 先判断侦头55AA
    timerCount = 0;
    receiveResult  = m_pTcpSocket->readAll();
    qDebug()<<"rec xml message" << receiveResult.size() << receiveResult;
    if(receiveResult.mid(0,4) == "55AA")
    {
        qDebug()<<"this is 55AA";
        if(receiveResult.size() -32  == (receiveResult.mid(8,8).toInt()))
        {
            QByteArray heart =receiveResult.mid(0,8).append("000000000002000000000000");
            m_pTcpSocket->write(heart);
            m_pTcpSocket->waitForBytesWritten();
            //            qDebug() <<"4710 receive head" << receiveResult;//.mid(0,32);
            xml_parse(receiveResult);
        }
        else
        {//长侦
            receiveResultDo = receiveResult;
        }
    }else
    {
        //曲线 第X侦
        receiveResultDo.append(receiveResult);
        // qDebug() << receiveResultDo.size()-32;
        //   qDebug() << "##########################receiveResultDo.size()" << receiveResultDo.size();
        //   qDebug() << "##########################receiveResultDo.mid(8,8).toInt()" << receiveResultDo.mid(8,8).toInt();
        if((receiveResultDo.size()-32) == (receiveResultDo.mid(8,8).toInt()))
        {
            //长帧接收完毕
//            qDebug() << "chang zhen##########################" << receiveResultDo;
            QByteArray heart =receiveResultDo.mid(0,8).append("000000000002000000000000");
            //  qDebug() << "4710 back" << heart;
            m_pTcpSocket->write(heart);
            m_pTcpSocket->waitForBytesWritten();
            xml_parse(receiveResultDo);

        }
        else
        {//未接完

        }
    }
}

/**************************************************/
//receive any channel enable
/**************************************************/
void Worker::revChannleEnFunc(int channel,int type,QString vin)
{
    qDebug()<<"@@@@@@@@@@@@@@@@@@@ channel @@@@@@@@@"<<channel << type << vin;
    channelData = channel;
    if(type == 1)
    {
        pack_SN = vin;
    }
    else if(type == 2)
    {
        dutouFlag = true;
    }
    else if(type == 3)
    {
        moduleFlag = true;
    }
}
/**************************************************/
//receive mainwindow button
/**************************************************/
void Worker::revbuttonFunc()
{
    if(channelData == 2)
    {
        emit sendTighteningResult2(4,4);
    }
    else
    {
        emit sendTighteningResult1(99,99);
    }
}

