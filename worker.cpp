#include "worker.h"
#include "GlobalVarible.h"
#include <QSettings>
Worker::Worker(QObject *parent) :
    QObject(parent)
{
    this->moveToThread(&m_thread);
    m_thread.start();
}

void Worker::fromNew1thread()
{
    tcpServer->close();
    m_pTcpSocket->close();
    delete tcpServer;
    delete m_pTcpSocket;
}

void Worker::fun1()
{
    qDebug()  << "work Thread start...";
    QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);

    tcpServer = new QTcpServer(this);
    if(!tcpServer->listen(QHostAddress::Any,config->value("baseinfo/PortB").toInt()))
    {  //**本地主机的4710端口，如果出错就输出错误信息，并关闭
        qDebug() << tcpServer->errorString();
    }

    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(DataReceived()));

    //if(!tcpServer->listen(QHostAddress("192.168.1.1"),4710))

    delete config;
}

void Worker::DataReceived()
{
    qDebug() << "4710 port connected";
    m_pTcpSocket = tcpServer->nextPendingConnection();
    connect(m_pTcpSocket, SIGNAL(readyRead()),this, SLOT(recMessage()));
    emit  havedconnect(true);
    //qDebug() << "emit havedconnct(true1)";
    //  qDebug() << m_pTcpSocket->state();


}

//xml解析
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
    //  qDebug() << "receiveResult" << temp;
    //获得根节点
    QDomElement root = doc.documentElement();
    //获得第一个子节点:版本
    node = root.firstChild();
    //qDebug() << node.nodeName()<< "nodename";
    if(node.nodeName() == "PRC_SST")
    {
        //曲线
        CURVE_STR = temp;
        if(SYSS!="ING")
        {
            return;
        }

        //  qDebug() << " PRC_SSTPRC_SSTPRC_SSTPRC_SST" ;
        int i,j,k,l,m,n,p,q,r,blc=-1;
        QString DATE_blc,TIME_blc,STATE_blc="NOK",MI_blc,WI_blc,Cycle_CSR,Program,Channel="1";


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
                                            if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().nodeName()=="PRG" && node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().text().toInt()==99)
                                            {
                                                //PRG=99 99号程序 不处理 只应答
                                                //  qDebug()<<"program 99";
                                                break;
                                            }
                                            else
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
                                                if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().nodeName()=="STA")
                                                {
                                                    STATE_blc = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().text();
                                                    if(STATE_blc == "IO")
                                                        STATE_blc = "OK";
                                                    else if(STATE_blc == "NIO")
                                                        STATE_blc = "NOK";
                                                }
                                                if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(m).toElement().nodeName()=="BLC")
                                                {
                                                    blc=m;
                                                }
                                            }
                                        }
                                        if(blc!=-1)
                                        {
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
                                                                    }
                                                                }
                                                                if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(q).toElement().nodeName()=="NAM"&&node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(q).toElement().text()=="WI")
                                                                {
                                                                    for(r=0;r<MAR_child;r++)
                                                                    {
                                                                        //角度
                                                                        if(node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(r).toElement().nodeName()=="VAL")
                                                                           WI_blc = node.toElement().childNodes().at(i).toElement().childNodes().at(j).toElement().childNodes().at(k).toElement().childNodes().at(l).toElement().childNodes().at(blc).toElement().childNodes().at(n).toElement().childNodes().at(p).toElement().childNodes().at(r).toElement().text();
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
                            }
                        }
                    }
                }
            }
        }
        if(blc!=-1)
        {
            qDebug() << "work Thread parse data:" << VIN_PIN_SQL+"||"+Cycle_CSR+"||"+MI_blc+"||"+WI_blc+"||"+Program+"||"+Channel;
            if(Cycle_CSR != data_model[7])
            {
                if(MI_blc == "")
                    MI_blc = "0";
                if(WI_blc  == "")
                    WI_blc = "0";
                if(SYSS == "ING")
                {
                    if(STATE_blc == "OK")
                    {
                        if(bound_enabled)
                        {

                            QSettings *config = new QSettings("/config_bound.ini", QSettings::IniFormat);
                            double torque_max = config->value("/ProNumber"+proNum+"/Torque_max").toDouble();
                            double torque_min = config->value("/ProNumber"+proNum+"/Torque_min").toDouble();
                            double Angle_max = config->value("/ProNumber"+proNum+"/Angle_max").toDouble();
                            double Angle_min = config->value("/ProNumber"+proNum+"/Angle_min").toDouble();
                            delete config;
                            if(torque_max == 0)
                            {
                                torque_max = torqueValue.toDouble();
                                torque_min = torqueValue.toDouble();
                            }
                            if(torque_min == 0)
                                torque_min = torqueValue.toDouble();
                            if(Angle_max == 0)
                            {
                                Angle_max = angleValue.toDouble();
                                Angle_min = angleValue.toDouble();
                            }
                            if(Angle_min == 0)
                                Angle_min = angleValue.toDouble();
                            if((torqueValue.toDouble()<torque_min) || (torqueValue.toDouble()>torque_max) ||
                                    (angleValue.toDouble()<Angle_min) || (angleValue.toDouble() >Angle_max))
                            {
                                STATE_blc = "NOK";
                            }
                        }
                    }
                    data_model[0] = DATE_blc;
                    data_model[1] = TIME_blc;
                    data_model[2] = STATE_blc;
                    data_model[3] = MI_blc;
                    data_model[4] = WI_blc;
                    data_model[5] = SCREWID_SQL;
                    data_model[6] = VIN_PIN_SQL;
                    data_model[7] = Cycle_CSR;
                    data_model[8] = CURVE_STR;
                    data_model[9] = Program;
                    data_model[10] = Channel;
                    emit sendfromworkthread(data_model);
                    emit send_mainwindow(MI_blc,WI_blc,STATE_blc);
                }
            }
            else
            {
                qDebug()<<"Duplicate data";
            }
        }
    }
    else
    {//心跳 解析 EVT
        //  qDebug() << "接受4710 " << temp;
        //qDebug() << "evt herehere";
        QString EVTname = node.toElement().childNodes().at(1).nodeName();
        // qDebug() << EVTname << "ffffffff";
        if(EVTname == "EVT")
        {
            //qDebug() << "evt come in";
            QString rdyname =  node.toElement().childNodes().at(1).toElement().childNodes().at(2).toElement().childNodes().at(0).toElement().childNodes().at(0).nodeName();
            if(rdyname == "RDY")
            {
                int RDY = node.toElement().childNodes().at(1).toElement().childNodes().at(2).toElement().childNodes().at(0).toElement().childNodes().at(0).toElement().text().toInt();
                //qDebug() << "受到复位RDY 1" << RDY;
                if(RDY == 1)
                {//复位成功
                    RDYY = true;
                    emit  havedconnect(true);
                    // qDebug() << "RDY(true)";
                    //emit  havedconnect(1);
                }else
                {// 复位失败 报错 给主界面发消息。
                   // qDebug()<< "rdyy = 0";
                    RDYY = false;
                    emit  havedconnect(false);
                }
            }
        }

    }
}


void Worker::recMessage()
{
    //接受到信息 先判断侦头55AA
    receiveResult  = m_pTcpSocket->readAll();
    //qDebug()<<"one zhen JJJJ" << receiveResult.size() << receiveResult;
    if(receiveResult.mid(0,4) == "55AA")
    {
        if(receiveResult.size() -32  == (receiveResult.mid(8,8).toInt()))
        {
            // 短侦
//            QDateTime cur = QDateTime::currentDateTime();
//            QString current_time = cur.toString("hh:mm:ss:zzz");
//            qDebug() << current_time << "4710@@@@@@@@@@@";
            //qDebug() << "   duan zhen receive 4710 " << receiveResult;
            QByteArray heart =receiveResult.mid(0,8).append("000000000002000000000000");
           // qDebug() << tr("back to cs351") << heart;
            m_pTcpSocket->write(heart);
            m_pTcpSocket->waitForBytesWritten();
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
            //qDebug() << "chang zhen##########################" << receiveResultDo;
            QByteArray heart =receiveResultDo.mid(0,8).append("000000000002000000000000");
           //  qDebug() << "4710 back" << heart;
            m_pTcpSocket->write(heart);
            m_pTcpSocket->waitForBytesWritten();
//            CURVE_STR = receiveResult;
            xml_parse(receiveResultDo);

        }
        else
        {//未接完

        }
    }
}
