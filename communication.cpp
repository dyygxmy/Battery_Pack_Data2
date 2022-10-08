#include "communication.h"
#include "GlobalVarible.h"
#include <unistd.h>

#include <QDataStream>
Communication::Communication(QObject *parent) :
    QObject(parent)
{
//    tran_data_len=0;//待发数据长度
//    rev_data_len=0;//待收数据长度
    update_len=0;//更新文件长度
    update_filename="";//更新文件名称
//    Pronum = 0; //程序号
//    LsNum = 0;  //额定螺栓数
//    qualifiedNum = 0;   //完成螺栓数
    timeLeft = 0;       //操作剩余时间
    OK = 2;
//    plus_flag=0;
//    bag_len=0;
//    dowm_len=0;
//    file_dowm=0;
//    update_dowm=0;
//    index=0;

    communication_thread.start();
    this->moveToThread(&communication_thread);
}
void Communication::mycommunicationtimer()
{
    qDebug()<< "communication thread start";
    udpSocket = new QUdpSocket(this);
    if(!udpSocket->bind(9000))
    {
        qDebug()<<udpSocket->errorString();
    }
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
}

void Communication::processPendingDatagram()
{
//    qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";
    QByteArray datagram;
    QHostAddress addr;
    quint16 port;
    do {
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size(),&addr,&port);
    } while (udpSocket->hasPendingDatagrams());

    int i=0,j=0;
    int sum=0;
    bool ok;
    int data_len=0;//数据长度
    bool Work_ShutDown = false;//0:无信号1:关机信号
    bool Work_Restart = false;//0:无信号 1:重启信号
    bool Work_Restore = false;//

    QString rev_buff = datagram.toHex()     ;// 转成16进制方便后面各种判断
//    qDebug()<<"rev_buff"<<rev_buff;
    int len = rev_buff.size()/2;//每两个长度为一个字节
    QString R_buff[len];//接收数据数组
    QString T_buff[18];//待发送数组
    QString NUM;
    QByteArray time_info;
//    time_info.resize(15);

    if(len == 0)
    {
    }
    else
    {
        j=0;
        for(i=0;i<len;i++)//数据每两位为一个字符串放入数组
        {
            R_buff[i]=rev_buff.mid(j,2);
            j+=2;
        }
        if((R_buff[0]=="02"&&R_buff[len-1]=="03")&&(R_buff[1]=="01"||R_buff[1]=="02"||R_buff[1]=="03"||R_buff[1]=="04"||R_buff[1]=="05"))//判断帧头帧尾及命令
        {
            sum=0;
            for(i=0;i<len-2;i++)//除去最后两项求和
            {
                sum += R_buff[i].toInt(&ok,16);
            }
            NUM=QString::number(sum,16);//求和
//            qDebug()<<NUM.right(2).toInt(&ok,16)<< R_buff[len-2].toInt(&ok,16);
            if(NUM.right(2).toInt(&ok,16) == R_buff[len-2].toInt(&ok,16))//求和校验 求和部分超过一个字节取最后一个字节
            {
//                qDebug()<<"************************************************************************";
                if(R_buff[1]=="01")//命令1 获取实时信息
                {
                    QString Version_1 = Version.mid(1);
//                    QString point = ".";
                    int Version_2 = Version_1.toDouble()*1000;
//                    qDebug() << "**********version********** " << Version_2 <<Version_1;
//                    QString Version_2 = Version_1.mid(0,Version_1.indexOf(point));
//                    QString Version_3 = Version_1.mid(Version_1.indexOf(point)+1);
//                    qDebug()<<Version_2<<Version_3;
//                    time_info[0] = ToSixteen(Version_2.toInt()).toInt();
//                    time_info[1] = ToSixteen(Version_3.toInt()).toInt();
                    time_info[0] = Version_2;
                    time_info[1] = Version_2>>8;
                    if(ISmaintenance)  //工作模式
                        time_info[2] =  1;
                    else
                        time_info[2] =  2;

                    if(isBarCode && !isRFID )
                        time_info[3] = 0;
                    else if(!isBarCode && isRFID )
                        time_info[3] = 1;
                    else if(!isBarCode && !isRFID )
                        time_info[3] = 2;
                    else if(isBarCode && isRFID)
                        time_info[3] = 3;
                    else
                        time_info[3] = 0;     //可扩展



                    if(ControlType_1 == "OFF")
                        time_info[4] = 0;     //
                    else if(ControlType_1 == "CS351")
                        time_info[4] = 1;
                    else if(ControlType_1 == "Nexo")
                        time_info[4] = 2;
                    else if(ControlType_1 == "SB356")
                        time_info[4] = 3;
                    else if(ControlType_1 == "SB356_PLC")
                        time_info[4] = 4;
                    else if(ControlType_1 == "Cleco")
                        time_info[4] = 5;
                    else if(ControlType_1 == "PFD")
                        time_info[4] = 6;
                    else if(ControlType_1 == "PFC")
                        time_info[4] = 7;
                    else if(ControlType_1 == "PMM")
                        time_info[4] = 8;
                    else if(ControlType_1 == "PM")
                        time_info[4] = 9;
                    else if(ControlType_2 == "PM600")
                        time_info[4] = 10;
                    else if(ControlType_2 == "IR")
                        time_info[4] = 11;
                    else if(ControlType_2 == "GWK")
                        time_info[4] = 12;
                    else if(ControlType_2 == "PF_XML")
                        time_info[4] = 13;

                    if(ControlType_2 == "OFF")
                        time_info[5] = 0;     //
                    else if(ControlType_2 == "CS351")
                        time_info[5] = 1;     //
                    else if(ControlType_2 == "Nexo")
                        time_info[5] = 2;     //
                    else if(ControlType_2 == "SB356")
                        time_info[5] = 3;     //
                    else if(ControlType_2 == "SB356_PLC")
                        time_info[5] = 4;     //
                    else if(ControlType_2 == "Cleco")
                        time_info[5] = 5;     //
                    else if(ControlType_2 == "PFD")
                        time_info[5] = 6;     //
                    else if(ControlType_2 == "PFC")
                        time_info[5] = 7;     //
                    else if(ControlType_2 == "PMM")
                        time_info[5] = 8;
                    else if(ControlType_2 == "PM")
                        time_info[5] = 9;
                    else if(ControlType_2 == "PM600")
                        time_info[5] = 10;
                    else if(ControlType_2 == "IR")
                        time_info[5] = 11;
                    else if(ControlType_2 == "GWK")
                        time_info[5] = 12;
                    else if(ControlType_2 == "PF_XML")
                        time_info[5] = 13;


                    if(isController1)   //拧紧设备1是否准备
                        time_info[6] = 1;
                    else
                        time_info[6] = 0;

                    if(isController2)   //拧紧设备2是否准备
                        time_info[7] = 1;
                    else
                        time_info[7] = 0;

                    if(RFIDIsConnect) //RFID是否连接
                        time_info[8] = 1;
                    else
                        time_info[8] = 0;
                    WIFIlock.lockForRead();
                    if(WIFI_STATE)//中控服务器
                    {
                        time_info[9] = 1;
                    }
                    else
                    {
                        time_info[9] = 0;
                    }
                    WIFIlock.unlock();

                    time_info[10] =  OK;
//                    time_info[9] =  Pronum; //程序号
//                    time_info[10] =  LsNum;  //额定螺栓数
//                    time_info[11] =  qualifiedNum;   //完成螺栓数
                    time_info[11] =  timeLeft;       //操作剩余时间

                    T_buff[0] = "02";
                    T_buff[1] = "01";
                    if(len == 7)
                    {
                        T_buff[2] = "00";
                        T_buff[3] = "0F";
                        for(j=0;j<12;j++)
                        {
                            T_buff[4+j]=QString::number(time_info[j]);
//                            qDebug() <<"********************8" << T_buff[4+j];
                        }
                        data_len = 18;
                    }
                    else//长度错误
                    {
                        T_buff[2] = "00";
                        T_buff[3] = "01";
                        T_buff[4] = "cc";
                        data_len = 7;
                    }

                }
                else if(R_buff[1]=="02")//命令2 关机
                {
                    T_buff[0] = "02";
                    T_buff[1] = "02";
                    T_buff[2] = "00";
                    T_buff[3] = "01";
                    if(len == 7)
                    {
                        if(ISmaintenance)
                        {
                            Work_ShutDown=true;
                            T_buff[4]="00";
                        }
                        else
                        {
                            Work_ShutDown=true;
                            T_buff[4]="00";
                        }
                    }
                    else//长度错误
                    {
                        T_buff[4]="cc";
                    }
                    data_len=7;
                }
                else if(R_buff[1]=="03")//命令3 重启
                {
                    T_buff[0] = "02";
                    T_buff[1] = "03";
                    T_buff[2] = "00";
                    T_buff[3] = "01";
                    if(len == 7)
                    {
                        if(ISmaintenance)
                        {
                            Work_Restart=true;
                            T_buff[4]="00";
                        }
                        else
                        {
                            Work_Restart=true;
                            T_buff[4]="00";
                        }
                    }
                    else//长度错误
                    {
                        T_buff[4]="cc";
                    }
                    data_len=7;
                }
                else if(R_buff[1]=="04")//命令4 还原
                {
                    T_buff[0] = "02";
                    T_buff[1] = "04";
                    T_buff[2] = "00";
                    T_buff[3] = "01";
                    if(len == 7)
                    {
                        Work_Restore=true;
                        T_buff[4]="00";
                    }
                    else//长度错误
                    {
                        T_buff[2]="cc";
                    }
                    data_len=7;
                }
                else if(R_buff[1]=="05")//命令5
                {
                    update_len = R_buff[3].right(1).toInt(&ok,16)+R_buff[3].left(1).toInt(&ok,16)*qPow(16,1)+
                            R_buff[2].right(1).toInt(&ok,16)*qPow(16,2)+R_buff[2].left(1).toInt(&ok,16)*qPow(16,3);

                    QByteArray name = datagram.mid(4,update_len);

                    update_filename=QString(name);
//                    qDebug()<<"update_filename"<<update_filename<<update_len;

                    T_buff[0] = "02";
                    T_buff[1] = "05";
                    T_buff[2] = "00";
                    T_buff[3] = "01";
                    if(len == 6 + update_len)
                    {
                        T_buff[4]="00";
                        checkUpdate();
                        update_program();
                        system((QString("chmod 744 /etc/data2/")+update_filename+" &").toLocal8Bit().data());
                        qDebug()<<"program update"<<update_filename;
                    }
                    else//长度错误
                    {
                        T_buff[4]="cc";
                    }
                    data_len=7;
                }
                else//校验位错误
                {
                    T_buff[0] = R_buff[0];
                    T_buff[1] = R_buff[1];
                    T_buff[2] = "00";
                    T_buff[3] = "01";
                    T_buff[4] = "bb";
                    data_len = 7;
                }
                sum=0;
                for(i=0;i<data_len-2;i++)//校验位求和
                {
                    sum += T_buff[i].toInt();
                }
                T_buff[data_len-2]=QString::number(sum%256);//取校验位最后两位
                T_buff[data_len-1]="03";
                data.resize(data_len);
                for(i=0;i<data_len;i++)
                {
                    data[i]= T_buff[i].toInt();//数组放入字节数组中
//                    qDebug() << "**************((((((((((((((("<<T_buff[i];
                }
//                qDebug()<<"*****************************************"<<data.size();
//                QByteArray datagram = "hello world!";
//                udpSocket.writeDatagram(datagram.data(), datagram.size(),
//                                      QHostAddress("192.168.3.177"), 9001);
//                qDebug()<<udpSocket.peerAddress().toString();
                udpSocket->writeDatagram(data.data(),data.size(), addr, port);
//                qDebug()<<"reply data:"<<data.toHex();
                if(Work_ShutDown)
                {
                    qDebug()<<"poweroff!!!!";
                    system("echo 0 > /sys/class/leds/control_lvds/brightness");   //关背光
                    system("poweroff");
                }
                if(Work_Restart)
                {
                    qDebug()<<"reboot!!!!";
                    system("reboot");
                }
                if(Work_Restore)
                {
                    qDebug()<<"restore!!!";
                    system("chmod 744 /etc/data2/*");
                    system("chmod 744 /etc/test_wifi.sh");
                    system("chmod 744 /etc/out.sh");
                    system("chmod 644 /config.ini");
                    system("chmod 644 /config_bount.ini");
                    system("chmod 644 /etc/profile");
                    system("chmod 744 /etc/systemConfig.sh");
                    system("chmod 644 /etc/inittab");
                    system("chmod 644 /usr/local/arm/freetds/etc/freetds.conf");
                    system("chmod 644 /usr/local/arm/unixODBC/etc/odbc.ini");
                    system("chmod 644 /PDM/*");
                    system("chmod 644 /PDM_1/*");
                    system("chmod 744 /etc/poweroff");
                    system("chmod 744 /etc/data2Child/*");
                }
            }
            else
            {
                ;
            }
        }
        else
        {
            ;
        }
    }
}

void Communication::receiveInfo(bool a, bool b, int c, int d)
{
//    qDebug()<<"################# receiveInfo $$$$$$$$$$$$$$$$$$$$$$$$$$";
    isController1 = a; //拧紧机1
    isController2 = b;  //拧紧机2
    timeLeft = c;       //操作剩余时间
    OK = d;
}

void Communication::closeMyself()
{
    udpSocket->abort();
    delete udpSocket;
}


void Communication::checkUpdate()
{
    QString item,item_vision,minitem,maxitem,minitem_vision,maxitem_vision;
    int i=0;
    int filenum;
    QDir *dir=new QDir("/etc/data2/");
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList());
    filenum=fileInfo->count();
    minitem = fileInfo->at(2).fileName();
    maxitem = fileInfo->at(2).fileName();
    minitem_vision = minitem.mid(1);
    maxitem_vision = minitem.mid(1);
    for(i=2;i<filenum;i++)
    {
        item=fileInfo->at(i).fileName();
        item_vision = item.mid(1);

        if(maxitem_vision.toDouble()>item_vision.toDouble())
        {
            maxitem = maxitem;
        }
        else
        {
            maxitem=item;
        }
        if(minitem_vision.toDouble()<item_vision.toDouble())
        {
            minitem=minitem;
        }
        else
        {
            minitem=item;
        }
    }
    //qDebug()<<minitem<<"****************";
    if(filenum>4)
    {
        unlink((QString("/etc/data2/")+minitem).toLocal8Bit().data());
    }
    update_filename=maxitem;

}

void Communication::update_program()
{
//    QString fileName = "/etc/profile";
    QString fileName = "/etc/systemConfig.sh";
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly| QIODevice::Text)){
        qDebug()   << "Cannot open systemConfig.sh file for Reading";
        return;
    }
    else
    {
        QString str (file.readAll());
        if(str.contains("/etc/data2/", Qt::CaseInsensitive)){
            qDebug()<<"########  update systemConfig.sh ############";
            str.replace(QRegExp("/etc/data2/\\S*"),QString("/etc/data2/")+update_filename);
        }
        file.close();
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
            qDebug()   << "Cannot open systemConfig.sh file for Writing";
            return;
        }
        else
        {
            file.write(str.toUtf8());
            file.close();
        }
    }
    //    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    //    configIniRead->setValue("baseinfo/Version","V"+update_filename.mid(4));
    //    delete configIniRead;
//    Version = "V"+update_filename.mid(1);
}


QString Communication::ToSixteen(int decimal)
{
    QString hexadecimal;
    QString a_hex;
    QString b_hex;
    int a;
    int b;
    if(decimal>15)
    {
        a = decimal/16;
        b = decimal%16;
//        qDebug()<<a<<b;
        switch(a)
        {
        case 10:
            a_hex="a";
            break;
        case 11:
            a_hex="b";
            break;
        case 12:
            a_hex="c";
            break;
        case 13:
            a_hex="d";
            break;
        case 14:
            a_hex="e";
            break;
        case 15:
            a_hex="f";
            break;
        default:
            a_hex=QString::number(a,10);
            break;
        }
        switch(b)
        {
        case 10:
            b_hex="a";
            break;
        case 11:
            b_hex="b";
            break;
        case 12:
            b_hex="c";
            break;
        case 13:
            b_hex="d";
            break;
        case 14:
            b_hex="e";
            break;
        case 15:
            b_hex="f";
            break;
        default:
            b_hex=QString::number(b,10);
            break;
        }
        hexadecimal=a_hex+b_hex;
    }
    else
    {
        a = decimal;
        b = 0;
        switch(a)
        {
        case 10:
            a_hex="a";
            break;
        case 11:
            a_hex="b";
            break;
        case 12:
            a_hex="c";
            break;
        case 13:
            a_hex="d";
            break;
        case 14:
            a_hex="e";
            break;
        case 15:
            a_hex="f";
            break;
        default:
            a_hex=QString::number(a,10);
            break;
        }
        hexadecimal=a_hex;
    }

    return hexadecimal;
}


