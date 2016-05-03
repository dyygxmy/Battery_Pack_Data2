#include "communication.h"
#include "GlobalVarible.h"
#include <unistd.h>

#include <QDataStream>
Communication::Communication(QObject *parent) :
    QObject(parent)
{
    tran_data_len=0;//待发数据长度
    rev_data_len=0;//待收数据长度
    update_len=0;//更新文件长度
    update_filename="";//更新文件名称
    plus_flag=0;
    bag_len=0;
    dowm_len=0;
    file_dowm=0;
    update_dowm=0;
    index=0;
    Work_Reset=0;
    Work_Restart=0;
    Work_ShutDown=0;

    communication_thread.start();
    this->moveToThread(&communication_thread);
}
void Communication::mycommunicationtimer()
{
    this->tcpServer = new QTcpServer(this);
    this->tcpSocket = new QTcpSocket(this);

    if(!tcpServer->listen(QHostAddress::Any,9000))
    {
        qDebug()<<tcpServer->errorString();
        tcpSocket->close();
        qDebug()<<"connect fail";
    }

    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(connection()));
}

void Communication::connection()
{
    tcpSocket=tcpServer->nextPendingConnection();
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(CMD()));
    qDebug()<<"connect sucess!";
}

void Communication::CMD()
{
    int six_flag=0;
    int i=0,j=0;
    int sum=0;
    bool ok;
    int data_len=0;//数据长度
    int eve_tran_len=40000;//PC单次发送数据长度
    int eve_rev_len=40000;//PC单次接收数据长度
    int eve_rev_start=0;//接受数据的起始位
    QByteArray rev_data = tcpSocket->readAll();//获取接收数据
    QString rev_buff = rev_data.toHex();// 转成16进制方便后面各种判断
    int len = rev_buff.size()/2;//每两个长度为一个字节
    QString T_buff[41000];//待发送数组
    QString NUM;
    QByteArray head;
    QByteArray tail;
    QByteArray aa;
    QString R_buff[len];//接收数据数组
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
        //qDebug()<<rev_data.toHex()<<workmode;
        if((R_buff[0]=="02"&&R_buff[len-1]=="03")&&(R_buff[1]=="01"||R_buff[1]=="02"||R_buff[1]=="03"||R_buff[1]=="04"||R_buff[1]=="05"||R_buff[1]=="06"||R_buff[1]=="07"||R_buff[1]=="0a"))//判断帧头帧尾及命令
        {
            sum=0;
            for(i=0;i<len-2;i++)//除去最后两项求和
            {
                sum += R_buff[i].toInt(&ok,16);
            }
            NUM=QString::number(sum,16);//求和
            //qDebug()<<NUM.right(2).toInt(&ok,16)<< R_buff[len-2].toInt(&ok,16);
            if(NUM.right(2).toInt(&ok,16) == R_buff[len-2].toInt(&ok,16))//求和校验 求和部分超过一个字节取最后一个字节
            {
                if(R_buff[1]=="01")//命令1 获取实时信息
                {
                    if(workmode)  //工作模式
                        time_info[0] =  time_info[0] | 0x01;
                    else
                        time_info[0] =   time_info[0] & 0xFE;

                    if(isRFID == 1)    //is RFID
                        time_info[0] = time_info[0] | 2;
                    else
                        time_info[0] = time_info[0] & 0xFD;

                    if(RDYY)   //CS351是否准备
                        time_info[0] = time_info[0] | 4;
                    else
                        time_info[0]= time_info[0] & 0xFB;

                    if(isRFID == 1) //拧紧枪归位
                        time_info[0] = time_info[0] | 64;
                    else
                        time_info[0] =time_info[0] & 63;

                    if(RFIDISConnect) //RFID是否连接
                        time_info[0] = time_info[0] | 16;
                    else
                        time_info[0] = time_info[0] & 0xEF;

                    if(isRFID == 1)//中控服务器
                        time_info[0] = time_info[0] | 32;
                    else
                        time_info[0] = time_info[0] & 0xDF;

                    if(isRFID == 1)//曲线服务器
                        time_info[0] =  time_info[0] | 64;
                    else
                        time_info[0] = time_info[0] & 0xBF;

                    if(SYSS == "ING") //当前状态 ING or OK or NOK
                        time_info[1] = 1;
                    else if(SYSS == "OK")
                        time_info[1] = 2;
                    else if(SYSS == "NOK")
                        time_info[1] = 3;
                    else
                        time_info[1] = 3;

                    time_info[2] =  proNum.toInt(); //程序号
                    time_info[3] =  lsNum.toInt();  //额定螺栓数
                    time_info[4] =  QualifiedNum;   //完成螺栓数
                    time_info[5] =  TimeLeft;       //操作剩余时间
                    time_info[6] =  BackTimeLeft;

                    QString Version_1 = Version.mid(1);
                    QString point = ".";
                    Version_1.indexOf(point);
                    QString Version_2 = Version_1.mid(0,Version_1.indexOf(point));
                    QString Version_3 = Version_1.mid(Version_1.indexOf(point)+1);

                    time_info[7] = ToSixteen(Version_2.toInt()).toInt();
                    time_info[8] = ToSixteen(Version_3.toInt()).toInt();

                    T_buff[0] = "02";
                    T_buff[1] = "01";
                    if(len == 4)
                    {
                        for(j=0;j<7;j++)
                        {
                            T_buff[2+j]=QString::number(time_info[j]);
                        }
                        T_buff[9]=ToSixteen(Version_2.toInt());
                        T_buff[10]=ToSixteen(Version_3.toInt());

                        data_len=13;
                    }
                    else//长度错误
                    {
                        T_buff[2]="cc";
                        data_len=5;
                    }
                }
                else if(R_buff[1]=="02")//命令2 关机
                {
                    T_buff[0] = "02";
                    T_buff[1] = "02";
                    if(len == 4)
                    {
                        if(workmode==1)
                        {
                            Work_ShutDown=0;
                            T_buff[2]="01";
                        }
                        else
                        {
                            Work_ShutDown=1;
                            T_buff[2]="00";
                        }
                    }
                    else//长度错误
                    {
                        T_buff[2]="cc";
                    }
                    data_len=5;
                }
                else if(R_buff[1]=="03")//命令3 重启
                {
                    T_buff[0] = "02";
                    T_buff[1] = "03";
                    if(len == 4)
                    {
                        if(workmode==1)
                        {
                            Work_Restart=0;
                            T_buff[2]="01";
                        }
                        else
                        {
                            Work_Restart=1;
                            T_buff[2]="00";
                        }
                    }
                    else//长度错误
                    {
                        T_buff[2]="cc";
                    }
                    data_len=5;
                }
                else if(R_buff[1]=="04")//命令4 复位
                {
                    T_buff[0] = "02";
                    T_buff[1] = "04";
                    if(len == 4)
                    {
                        if(workmode==1)
                        {
                            Work_Reset=0;
                            T_buff[2]="01";
                        }
                        else
                        {
                            Work_Reset=1;
                            T_buff[2]="00";
                        }
                    }
                    else//长度错误
                    {
                        T_buff[2]="cc";
                    }
                    data_len=5;
                }
                else if(R_buff[1]=="05")//命令5 获取参数配置数据总长度
                {
                    //updateXML();//根据配置文件生成XML
                    T_buff[0] = "02";
                    T_buff[1] = "05";
                    if(workmode==1)
                    {
                        T_buff[2]="ee";
                        data_len=5;
                    }
                    else
                    {
                        if(len == 4)
                        {
                            QFile file("/filename.xml");
                            file.open(QIODevice::ReadOnly);
                            if(!file.atEnd())
                            {
                                six_data = file.readAll();
                                rev_data_len = six_data.size();
                                QString x = QString::number(rev_data_len,16);
                                int lenth = x.size();
                                T_buff[5] = x.mid(lenth-8,2);
                                T_buff[4] = x.mid(lenth-6,2);
                                T_buff[3] = x.mid(lenth-4,2);
                                T_buff[2] = x.mid(lenth-2,2);
                            }
                            else
                                qDebug()<<"cmd5 error";
                            file.close();
                            data_len=8;
                        }
                        else//长度错误
                        {
                            T_buff[2]="cc";
                            data_len=5;
                        }
                    }
                }
                else if(R_buff[1]=="06")//命令6 获取参数配置数据
                {
                    T_buff[0] = "02";
                    T_buff[1] = "06";
                    if(workmode==1)
                    {
                        T_buff[2]="ee";
                        data_len=5;
                    }
                    else
                    {
                        if(len == 8)
                        {
                            QString eve;
                            eve = R_buff[5]+R_buff[4]+R_buff[3]+R_buff[2];
                            eve_rev_start = R_buff[2].right(1).toInt(&ok,16)+R_buff[2].left(1).toInt(&ok,16)*qPow(16,1) +
                                    R_buff[3].right(1).toInt(&ok,16)*qPow(16,2)+R_buff[3].left(1).toInt(&ok,16)*qPow(16,3) +
                                    R_buff[4].right(1).toInt(&ok,16)*qPow(16,4)+R_buff[4].left(1).toInt(&ok,16)*qPow(16,5) +
                                    R_buff[5].right(1).toInt(&ok,16)*qPow(16,6)+R_buff[5].left(1).toInt(&ok,16)*qPow(16,7);
                            aa=six_data.mid(eve_rev_start,40000);
                            for(i=0;i<eve_rev_len;i++)
                            {
                                T_buff[2+i]=aa.mid(i,1);
                            }
                            six_flag=1;
                            data_len=eve_rev_len+4;
                            //qDebug()<<eve_rev_start<<rev_data_len;
                        }
                        else//长度错误
                        {
                            T_buff[2] = "cc";
                            data_len=5;
                        }
                    }
                }
                else if(R_buff[1]=="07")//命令7 上传参数配置数据长度
                {
                    T_buff[0] = "02";
                    T_buff[1] = "07";
                    if(workmode==1)
                    {
                        T_buff[2]="ee";
                        data_len=5;
                    }
                    else
                    {
                        if(len == 8)
                        {
                            tran_data_len = R_buff[2].right(1).toInt(&ok,16)+R_buff[2].left(1).toInt(&ok,16)*qPow(16,1) +
                                    R_buff[3].right(1).toInt(&ok,16)*qPow(16,2)+R_buff[3].left(1).toInt(&ok,16)*qPow(16,3) +
                                    R_buff[4].right(1).toInt(&ok,16)*qPow(16,4)+R_buff[4].left(1).toInt(&ok,16)*qPow(16,5) +
                                    R_buff[5].right(1).toInt(&ok,16)*qPow(16,6)+R_buff[5].left(1).toInt(&ok,16)*qPow(16,7);
                            data_len=4;
                        }
                        else//长度错误
                        {
                            T_buff[2]="cc";
                            data_len=5;
                        }
                    }
                }
                else if(R_buff[1]=="08")//命令8 上传参数配置数据
                {
                    //Dat2数据接收需要拼接,做特殊处理
                }
                else if(R_buff[1]=="0a")//命令10 上传更新文件长度
                {
                    T_buff[0] = "02";
                    T_buff[1] = "0a";

                    if(len >= 8)
                    {
                        update_len = R_buff[2].right(1).toInt(&ok,16)+R_buff[2].left(1).toInt(&ok,16)*qPow(16,1) +
                                R_buff[3].right(1).toInt(&ok,16)*qPow(16,2)+R_buff[3].left(1).toInt(&ok,16)*qPow(16,3) +
                                R_buff[4].right(1).toInt(&ok,16)*qPow(16,4)+R_buff[4].left(1).toInt(&ok,16)*qPow(16,5) +
                                R_buff[5].right(1).toInt(&ok,16)*qPow(16,6)+R_buff[5].left(1).toInt(&ok,16)*qPow(16,7);
                        QByteArray name = rev_data.mid(6,len-8);

                        update_filename=QString(name);
                        //qDebug()<<"update_filename"<<rev_data.mid(6,len-8)<<update_filename;
                        data_len=4;
                    }
                    else//长度错误
                    {
                        T_buff[2]="cc";
                        data_len=5;
                    }

                }
                else if(R_buff[1]=="0b")//命令11 根据索引上传数据
                {
                    //Dat2数据接收需要拼接,做特殊处理
                }
                else//命令号不正确
                {
                    T_buff[0] = R_buff[0];
                    T_buff[1] = R_buff[1];
                    T_buff[2] = "dd";
                    data_len = 5;
                }
            }
            else//校验位错误
            {
                T_buff[0] = R_buff[0];
                T_buff[1] = R_buff[1];
                T_buff[2] = "bb";
                data_len = 5;
            }
            if(six_flag==1)
            {
                sum = 0;
                head.resize(2);
                tail.resize(2);
                head[0]=2;
                head[1]=T_buff[1].toInt(&ok,16);
                aa.prepend(head);
                QByteArray xx=aa.toHex();
                int tt_size = xx.size();
                for(i=0;i<tt_size;i+=2)//校验位求和
                {
                    sum += xx.mid(i,2).toInt(&ok,16);
                }
                tail[0]=sum;
                tail[1]=3;
                aa.append(tail);
                this->tcpSocket->write(aa);
            }
            else
            {
                sum=0;
                for(i=0;i<data_len-2;i++)//校验位求和
                {
                    sum += T_buff[i].toInt(&ok,16);
                }
                T_buff[data_len-2]=QString::number(sum,16).right(2);//取校验位最后两位
                T_buff[data_len-1]="03";
                data.resize(data_len);
                for(i=0;i<data_len;i++)
                {
                    data[i]= T_buff[i].toInt(&ok,16);//数组放入字节数组中
                }
                this->tcpSocket->write(data);
                //qDebug()<<"reply data:"<<data.toHex();
            }
            if(Work_ShutDown==1)
            {
                system("halt");
            }
            if(Work_Restart==1)
            {
                system("reboot");
            }
            if(Work_Reset==1)
            {
                qDebug()<<"Reset!!!!";
            }
        }
        else//接收数据拼接
        {
            if((R_buff[0]=="02"&&R_buff[1]=="08"&&R_buff[2]=="00"&&R_buff[3]=="00"&&R_buff[4]=="00"&&R_buff[5]=="00")||
                    (R_buff[0]=="02"&&R_buff[1]=="0b"&&R_buff[2]=="00"&&R_buff[3]=="00"&&R_buff[4]=="00"&&R_buff[5]=="00"))//上位机重发和第一次传输
            {
                bag_len=0;//接收包长度初始化
                bag.clear();//接收包内容清空
                dowm_len=0;//接收数据总长度初始化
                qDebug()<<"Resend";
            }
            bag_len+=len;//拼接包长度
            bag=bag+rev_data;//数据拼接
            QString B_buff[bag_len];
            bag_buff = bag.toHex();//转成16进制
            j=0;
            for(i=0;i<bag_len;i++)
            {
                B_buff[i]=bag_buff.mid(j,2);//放入数组准备进行比较
                j+=2;//每两个长度的字符串为一个字节
            }
            sum=0;
            for(i=0;i<bag_len-2;i++)//除去最后两项求和
            {
                sum += B_buff[i].toInt(&ok,16);
            }
            NUM=QString::number(sum,16);//求和
            index = B_buff[2].right(1).toInt(&ok,16)+B_buff[2].left(1).toInt(&ok,16)*qPow(16,1) +
                    B_buff[3].right(1).toInt(&ok,16)*qPow(16,2)+B_buff[3].left(1).toInt(&ok,16)*qPow(16,3) +
                    B_buff[4].right(1).toInt(&ok,16)*qPow(16,4)+B_buff[4].left(1).toInt(&ok,16)*qPow(16,5) +
                    B_buff[5].right(1).toInt(&ok,16)*qPow(16,6)+B_buff[5].left(1).toInt(&ok,16)*qPow(16,7);
            if((B_buff[0]=="02")&&(B_buff[1]=="08")&&(B_buff[bag_len-1]=="03")&&(NUM.right(2).toInt(&ok,16) == B_buff[bag_len-2].toInt(&ok,16)))//完整的8号命令 判断是否接收完数据
            {
                if(workmode==1)
                {
                    QByteArray work;
                    int he;
                    work.resize(5);
                    work[0]=R_buff[0].toInt(&ok,16);
                    work[1]=R_buff[1].toInt(&ok,16);
                    R_buff[2]="ee";
                    he=R_buff[0].toInt(&ok,16) + R_buff[1].toInt(&ok,16) + R_buff[2].toInt(&ok,16);
                    work[2] = R_buff[2].toInt(&ok,16);
                    work[3] = QString::number(he,16).right(2).toInt(&ok,16);
                    work[4] = 3;
                    this->tcpSocket->write(work);
                }
                else
                {
                    //qDebug()<<"Correct Command 8";
                    //qDebug()<<index<<tran_data_len;
                    T_buff[0] = "0x02";
                    if(dowm_len==index)//索引
                    {
                        if(bag_len == eve_tran_len+8)//长度为10008
                        {
                            QFile file("/filename1.xml");
                            QByteArray tran_data;
                            for(i=0;i<eve_tran_len;i++)
                            {
                                tran_data += B_buff[2+i];
                            }
                            if((index+40000 > 40000)&&(index+40000 < tran_data_len))
                            {
                                file.open(QIODevice::WriteOnly|QIODevice::Append);
                                //qDebug()<<"contiue write on way 1";
                            }
                            else if((index+40000>=tran_data_len))
                            {
                                file.open(QIODevice::WriteOnly|QIODevice::Append);
                                file_dowm=1;
                                dowm_len=0;
                                //qDebug()<<"last write on way 1";
                            }
                            else if(index == 0)
                            {
                                file.open(QIODevice::WriteOnly|QIODevice::Truncate);
                                //qDebug()<<"first write on way 1";
                            }
                            file.write(bag.mid(6,40000).data(),bag.mid(6,40000).size());//去掉前六个字节 帧头(1),命令号(1),数据索引(4),xml数据(10000),校验位(1),帧尾(1)
                            dowm_len+=40000;
                            file.close();
                            T_buff[1] = "0x08";
                            T_buff[2] = "0x00";
                            data_len = 5;
                        }
                        else if(bag_len == tran_data_len-dowm_len+8)//最后一次长度可能小于10008或者文件小于10000字节
                        {
                            QFile file("/filename1.xml");
                            if((index+40000>40000)&&(index+40000 < tran_data_len))
                            {
                                file.open(QIODevice::WriteOnly|QIODevice::Append);
                                //qDebug()<<"contiue write on way 2";
                            }
                            else if((index+40000>=tran_data_len))
                            {
                                file.open(QIODevice::WriteOnly|QIODevice::Append);
                                file_dowm=1;
                                // qDebug()<<"last write on way 2";
                            }
                            else//文件小于40000字节
                            {
                                file.open(QIODevice::WriteOnly|QIODevice::Truncate);//第一次写入清空文件内容
                                file_dowm=1;
                                //qDebug()<<"first write on way 2";
                            }
                            file.write(bag.mid(6,bag_len-8).data(),bag.mid(6,bag_len-8).size());//xml数据长度:命令长度再减去4个命令字节以及4个表示索引的字节
                            dowm_len=0;
                            file.close();

                            T_buff[1] = "0x08";
                            T_buff[2] = "0x00";
                            data_len = 5;
                        }
                        else//长度错误
                        {
                            T_buff[1] = "0x09";
                            T_buff[2] = "cc";
                            data_len = 5;
                        }
                    }
                    else
                    {
                        T_buff[1] = "0x08";
                        T_buff[2] = "0x00";
                        data_len = 5;
                    }
                    sum=0;
                    for(i=0;i<data_len-2;i++)
                    {
                        sum += T_buff[i].toInt(&ok,16);
                    }
                    T_buff[data_len-2]=QString::number(sum,16).right(2);
                    T_buff[data_len-1]="03";
                    data.resize(data_len);
                    for(i=0;i<data_len;i++)
                    {
                        data[i]= T_buff[i].toInt(&ok,16);
                    }
                    this->tcpSocket->write(data,data_len);
                    len=0;
                    bag_len=0;
                    bag.clear();

                    this->tcpSocket->waitForBytesWritten(200);
                    //qDebug()<<"reply data"<<data.toHex();
                    if(file_dowm==1)
                    {
                        updateINI();
                        emit update();
                        emit xmlcreate();
                        file_dowm=0;
                    }
                }
            }
            else if((B_buff[0]=="02")&&(B_buff[1]=="0b")&&(B_buff[bag_len-1]=="03")&&(NUM.right(2).toInt(&ok,16) == B_buff[bag_len-2].toInt(&ok,16)))//完整的11号命令 判断是否接收完数据
            {
                //qDebug()<<"Correct Command 11";
                //qDebug()<<index<<update_len;

                T_buff[0] = "0x02";
                if(dowm_len == index)//索引
                {
                    if(index == 0)
                    {
                        unlink((QString("/etc/data2/")+update_filename).toLocal8Bit().data());
                    }
                    if(bag_len == eve_tran_len+8)//长度为40008
                    {
                        QFile file("/etc/data2/"+update_filename);
                        QByteArray tran_data;
                        for(i=0;i<eve_tran_len;i++)
                        {
                            tran_data += B_buff[2+i];
                        }
                        if(index+40000>40000&&index+40000 < update_len)
                        {
                            file.open(QIODevice::WriteOnly|QIODevice::Append);
                            //qDebug()<<"contiue write on way 1";
                        }
                        else if(index+40000>=update_len)
                        {
                            file.open(QIODevice::WriteOnly|QIODevice::Append);
                            update_dowm=1;
                            //qDebug()<<"last write on way 1";
                        }
                        else
                        {
                            file.open(QIODevice::WriteOnly|QIODevice::Truncate);
                            //qDebug()<<"first write on way 1";
                        }
                        file.write(bag.mid(6,40000).data(),bag.mid(6,40000).size());//去掉前六个字节 帧头(1),命令号(1),数据索引(4),xml数据(40000),校验位(1),帧尾(1)
                        dowm_len+=40000;
                        file.close();
                        T_buff[1] = "0x0b";
                        T_buff[2] = "0x00";
                        data_len = 5;
                    }
                    else if(bag_len == update_len-dowm_len+8)//最后一次长度可能小于40008或者文件小于40000字节
                    {
                        QFile file("/etc/data2/"+update_filename);
                        if((index+40000>40000)&&(index+40000 < update_len))
                        {
                            file.open(QIODevice::WriteOnly|QIODevice::Append);
                            //qDebug()<<"contiue write on way 2";
                        }
                        else if(index+40000>=update_len)
                        {
                            file.open(QIODevice::WriteOnly|QIODevice::Append);
                            update_dowm=1;
                            //qDebug()<<"last write on way 2";
                        }
                        else//小于100000字节的文件
                        {
                            file.open(QIODevice::WriteOnly|QIODevice::Truncate);//第一次写入清空文件内容
                            update_dowm=1;
                            //qDebug()<<"first write on way 2";
                        }
                        file.write(bag.mid(6,bag_len-8).data(),bag.mid(6,bag_len-8).size());//xml数据长度:命令长度再减去4个命令字节以及4个表示索引的字节
                        dowm_len=0;
                        file.close();
                        T_buff[1] = "0x0b";
                        T_buff[2] = "0x00";
                        data_len = 5;
                    }
                    else//长度错误
                    {
                        T_buff[1] = "0x0c";
                        T_buff[2] = "cc";
                        data_len = 5;
                    }
                }
                else
                {
                    T_buff[1] = "0x0b";
                    T_buff[2] = "0x00";
                    data_len = 5;
                }
                sum=0;
                for(i=0;i<data_len-2;i++)
                {
                    sum += T_buff[i].toInt(&ok,16);
                }
                T_buff[data_len-2]=QString::number(sum,16).right(2);
                T_buff[data_len-1]="03";
                data.resize(data_len);
                for(i=0;i<data_len;i++)
                {
                    data[i]= T_buff[i].toInt(&ok,16);
                }
                this->tcpSocket->write(data,data_len);
                len=0;
                bag_len=0;
                bag.clear();
                this->tcpSocket->waitForBytesWritten(200);
                //bool tcp_flag=0;
                //tcp_flag=this->tcpSocket->waitForBytesWritten();
                //qDebug()<<"reply data"<<data.toHex();
                if(update_dowm==1)
                {
                    checkUpdate();
                    update_program();
                    system((QString("chmod 744 /etc/data2/")+update_filename+" &").toLocal8Bit().data());
                    //qDebug()<<"program update";
                    update_dowm=0;
                }
            }

        }
    }
}

void Communication::updateINI()
{
    struct Parameter demo1;

    Analy_xml(&demo1);
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);

    int m;
    QString PDMpath;
    int max = configIniRead->value("baseinfo/pdmmax").toInt();  //PDM info数
    for (m=1;m<max+1;m++)
    {
        PDMpath = "/PDM/"+configIniRead->value("pdminfo"+QString::number(m)+"/pdmpath").toString();
        unlink(PDMpath.toLocal8Bit().data());
        configIniRead->remove("pdminfo"+QString::number(m));
    }

    //基本信息
    configIniRead->setValue("baseinfo/StationName",demo1.BasicPara[0]);
    configIniRead->setValue("baseinfo/StationId",demo1.BasicPara[1]);
    configIniRead->setValue("baseinfo/LocalIp",demo1.BasicPara[2]);
    configIniRead->setValue("baseinfo/ControlType",demo1.BasicPara[3]);
    configIniRead->setValue("baseinfo/cs351Ip",demo1.BasicPara[4]);
    configIniRead->setValue("baseinfo/PortA",demo1.BasicPara[5]);
    configIniRead->setValue("baseinfo/PortB",demo1.BasicPara[6]);
    configIniRead->setValue("baseinfo/isRFID",demo1.BasicPara[7]);
    configIniRead->setValue("baseinfo/RfidIp",demo1.BasicPara[8]);
    configIniRead->setValue("baseinfo/RfidPort",demo1.BasicPara[9]);
    configIniRead->setValue("baseinfo/TackTime",demo1.BasicPara[10]);
    configIniRead->setValue("baseinfo/netmask",demo1.BasicPara[11]);
    configIniRead->setValue("baseinfo/gateway",demo1.BasicPara[12]);
    configIniRead->setValue("baseinfo/SSID",demo1.BasicPara[13]);
    configIniRead->setValue("baseinfo/psk",demo1.BasicPara[14]);
    configIniRead->setValue("baseinfo/DataServerIp",demo1.BasicPara[15]);
    configIniRead->setValue("baseinfo/CurveServerIp",demo1.BasicPara[16]);
    configIniRead->setValue("baseinfo/AndonServerIp",demo1.BasicPara[17]);
    configIniRead->setValue("baseinfo/NexoCurvePath",demo1.BasicPara[18]);
    configIniRead->setValue("baseinfo/GCpassword",demo1.BasicPara[19]);
    configIniRead->setValue("baseinfo/JSpassword",demo1.BasicPara[20]);
    configIniRead->setValue("baseinfo/SN",demo1.BasicPara[21]);
    //configIniRead->setValue("baseinfo/Version",demo1.BasicPara[22]);
    configIniRead->setValue("baseinfo/BarcodeGun","9600");
    configIniRead->setValue("baseinfo/pdmmax",demo1.PDM[0]);  //PDM   info数
    configIniRead->setValue("baseinfo/pdmnumber",demo1.PDM[0]);   //PDM数


    //车型
    int i;
    int j;
    int k;
    for(i=0;i<20;i++)
    {
        configIniRead->setValue("carinfo"+QString::number(i+1)+"/carcx",demo1.CarType[i][0]);
        configIniRead->setValue("carinfo"+QString::number(i+1)+"/G9",demo1.CarType[i][1]);
        configIniRead->setValue("carinfo"+QString::number(i+1)+"/VIN",demo1.CarType[i][2]);
        configIniRead->setValue("carinfo"+QString::number(i+1)+"/pdmyinyong",demo1.CarType[i][3]);
        if(demo1.CarType[i][4]=="false")
            configIniRead->setValue("carinfo"+QString::number(i+1)+"/IsOptionalStation","0");
        else if(demo1.CarType[i][4]=="true")
            configIniRead->setValue("carinfo"+QString::number(i+1)+"/IsOptionalStation","1");


        //非选配
        for(k=1;k<21;k++)
        {
            configIniRead->setValue("carinfo"+QString::number(i+1)+"/ProNum"+QString::number(k),demo1.CarType[i][5+4*(k-1)]);
            configIniRead->setValue("carinfo"+QString::number(i+1)+"/LuoSuanNum"+QString::number(k),demo1.CarType[i][6+4*(k-1)]);
            configIniRead->setValue("carinfo"+QString::number(i+1)+"/LSNumber"+QString::number(k),demo1.CarType[i][7+4*(k-1)]);
            configIniRead->setValue("carinfo"+QString::number(i+1)+"/Taotong"+QString::number(k),demo1.CarType[i][8+4*(k-1)]);
        }
        //选配
        for(k=1;k<6;k++)
        {
            configIniRead->setValue("carinfo"+QString::number(i+1)+"/pdmyinyong"+QString::number(k),demo1.CarType[i][85+101*(k-1)]);

            for(j=1;j<21;j++)
            {
                configIniRead->setValue("carinfo"+QString::number(i+1)+"/OPProNum"+QString::number(k)+QString::number(j),demo1.CarType[i][86+101*(k-1)+3*(j-1)]);
                configIniRead->setValue("carinfo"+QString::number(i+1)+"/OPLuoSuanNum"+QString::number(k)+QString::number(j),demo1.CarType[i][87+101*(k-1)+3*(j-1)]);
                configIniRead->setValue("carinfo"+QString::number(i+1)+"/OPLSNumber"+QString::number(k)+QString::number(j),demo1.CarType[i][88+101*(k-1)+3*(j-1)]);
            }
            //必选
            for(j=1;j<11;j++)
            {
                configIniRead->setValue("carinfo"+QString::number(i+1)+"/bxuanname"+QString::number(k)+QString::number(j),demo1.CarType[i][89+101*(k-1)+57+2*(j-1)]);
                configIniRead->setValue("carinfo"+QString::number(i+1)+"/bxuancode"+QString::number(k)+QString::number(j),demo1.CarType[i][90+101*(k-1)+57+2*(j-1)]);
            }
            //可选
            for(j=1;j<11;j++)
            {
                configIniRead->setValue("carinfo"+QString::number(i+1)+"/kxuanname"+QString::number(k)+QString::number(j),demo1.CarType[i][91+101*(k-1)+57+18+2*(j-1)]);
                configIniRead->setValue("carinfo"+QString::number(i+1)+"/kxuancode"+QString::number(k)+QString::number(j),demo1.CarType[i][92+101*(k-1)+57+18+2*(j-1)]);
            }
        }
    }


    //PDM图
    int n = demo1.PDM[0].toInt();  //PDM数
    int coord[n]; //坐标数 数组
    int coords=0;
    //    QString PDMpath;
    //    QByteArray ba;
    for(i=1; i<n+1; i++)
    {
        if (i!=1)
            coords=coords+coord[i-2];
        coord[i-1]= demo1.PDM[3*i+2*coords].toInt();

        configIniRead->setValue("pdminfo"+QString::number(i)+"/pdmname",demo1.PDM[1+2*coords+3*(i-1)]);

        QByteArray rc;
        rc=QByteArray::fromBase64(demo1.PDM[2+2*coords+3*(i-1)].toAscii());
        //QByteArray rdc=qUncompress(rc);

        QImage img;
        //QImage img(1166,598,QImage::Format_Indexed8);
        //img.Format_Indexed8;
        img.loadFromData(rc);
        img.save("/PDM/"+demo1.PDM[1+2*coords+3*(i-1)]+".bmp","bmp");

        configIniRead->setValue("pdminfo"+QString::number(i)+"/pdmpath",demo1.PDM[1+2*coords+3*(i-1)]+".bmp");
        configIniRead->setValue("pdminfo"+QString::number(i)+"/num",demo1.PDM[3+2*coords+3*(i-1)]);

        //坐标
        for(k=1;k<(coord[i-1]+1);k++)
        {
            configIniRead->setValue("pdminfo"+QString::number(i)+"/tempx"+QString::number(k),demo1.PDM[4+2*coords+3*(i-1)+2*(k-1)]);
            configIniRead->setValue("pdminfo"+QString::number(i)+"/tempy"+QString::number(k),demo1.PDM[5+2*coords+3*(i-1)+2*(k-1)]);
        }
    }


    delete configIniRead;
    system("cp /config.ini /config1.ini &");
}

void Communication::update_program()
{
    QString fileName = "/etc/profile";
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly| QIODevice::Text)){
        qDebug()   << "Cannot open profile file for Reading";
        return;
    }
    QString str (file.readAll());
    if(str.contains("/etc/data2/", Qt::CaseInsensitive)){
        qDebug()<<"########  update profile ############";
        str.replace(QRegExp("/etc/data2/\\S*"),QString("/etc/data2/")+update_filename);
    }
    file.close();
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
        qDebug()   << "Cannot open profile file for Writing";
        return;
    }
    file.write(str.toUtf8());
    file.close();
    //    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    //    configIniRead->setValue("baseinfo/Version","V"+update_filename.mid(4));
    //    delete configIniRead;
    Version = "V"+update_filename.mid(4);
}

void Communication::Analy_xml(struct Parameter *p)
{
    int i,j;
    QDomDocument document;
    QFile file("/filename1.xml");//指定目标文件
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        //QMessageBox::information(NULL, QString("title"), QString("open error!"));
        qDebug()<<"Open Error!";
        return;
    }
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        //QMessageBox::information(NULL, QString("title"), QString("parse file failed at line row and column") + QString::number(row, 10) + QString(",") + QString::number(column, 10));
        qDebug()<<"parse file failed at line row and column"+QString::number(row)+","+QString::number(column);
        return;
    }
    if(document.isNull())
    {
        //QMessageBox::information(NULL, QString("title"), QString("document is null!"));
        qDebug()<<"document is null!";
        return;
    }
    // 读取基本参数
    QDomElement root = document.documentElement();//创建首节点
    QDomNodeList list = root.childNodes();//创建子节点列表
    for(i=0;i<=22;i++)//循环读取同级节点内容
    {
        QDomNode node = list.item(i);//创建列表中对象
        QDomElement Element = node.toElement();//创建子节点
        QString Element_value = Element.text();//获取该节点内容
        p->BasicPara[i] = Element_value;//放入数组
    }
    QDomNode CarTypeInfoList_node = list.item(23);//创建列表中对象
    QDomElement CarTypeInfoList = CarTypeInfoList_node.toElement();//创建子节点
    QDomNodeList CarTypeInfoList_List = CarTypeInfoList.childNodes();//创建节点列表
    QDomNode CarTypeInfo_node;//创建对象
    QDomElement CarTypeInfo;//创建节点
    QDomNodeList CarTypeInfo_List;//创建节点列表
    QDomNode ProgramList_node;//创建对象
    QDomElement ProgramList;//创建节点
    QDomNodeList ProgramList_List;//创建节点列表
    QDomElement OptionProList;//创建节点
    QDomNodeList OptionProList_List;//创建节点列表
    //读取20个车型的配置信息
    for(i=0;i<20;i++)//循环读取20个车型
    {
        int a=0,b=0,c=0;
        CarTypeInfo_node = CarTypeInfoList_List.item(i);
        CarTypeInfo = CarTypeInfo_node.toElement();
        //车型基本信息
        CarTypeInfo_List =CarTypeInfo.childNodes();
        for(a=0;a<=4;a++)//循环读取五项车型基本信息：CarName G9 VIN PDM IsOptionalStation
        {
            QDomNode node1 = CarTypeInfo_List.item(a);
            QDomElement Element = node1.toElement();
            QString Element_value = Element.text();
            p->CarType[i][a] = Element_value;
        }
        //无选配配置
        ProgramList_node = CarTypeInfo_List.item(5);
        ProgramList = ProgramList_node.toElement();
        ProgramList_List = ProgramList.childNodes();
        for(a=0;a<=19;a++)//读取20个程序的内容
        {
            QDomNode ProgramNum_node = ProgramList_List.item(a);
            QDomElement ProgramNum = ProgramNum_node.toElement();
            QDomNodeList ProgramNum_List = ProgramNum.childNodes();
            for(b=0;b<=3;b++)//循环读取程序： PID BoltNumber BoltCount Sleave
            {
                QDomNode node1 = ProgramNum_List.item(b);
                QDomElement Element1 = node1.toElement();
                QString Element_value = Element1.text();
                p->CarType[i][5+b+4*a] = Element_value;
            }
        }
        //选配配置
        OptionProList = ProgramList.nextSiblingElement();
        OptionProList_List = OptionProList.childNodes();
        for(a=0;a<=4;a++)//读取五个槽的内容
        {
            QDomNode OptionalInfo_node =OptionProList_List.item(a);
            QDomElement OptionalInfo = OptionalInfo_node.toElement();

            QDomNodeList OptionalInfo_List = OptionalInfo.childNodes();
            QDomNode PDM_node = OptionalInfo_List.item(0);
            QDomElement PDM = PDM_node.toElement();
            QString Element_value = PDM.text();
            p->CarType[i][85+101*a] = Element_value;//选择的PDM
            QDomElement ProgramList = PDM.nextSiblingElement();
            QDomNodeList ProgramList_List = ProgramList.childNodes();
            for(b=0;b<=19;b++)//读取20个程序的内容
            {
                QDomNode ProgramNum_node = ProgramList_List.item(b);
                QDomElement ProgramNum = ProgramNum_node.toElement();
                QDomNodeList ProgramNum_List = ProgramNum.childNodes();
                for(c=0;c<=2;c++)//循环读取程序： PID BoltNumber BoltCount
                {
                    QDomNode node1 = ProgramNum_List.item(c);
                    QDomElement Element = node1.toElement();
                    Element_value = Element.text();
                    p->CarType[i][86+c+3*b+101*a] = Element_value;
                }
            }
            QDomElement MustList = ProgramList.nextSiblingElement();
            QDomNodeList MustList_List = MustList.childNodes();
            //必选
            for(b=0;b<=9;b++)//循环读取10个必选内容
            {
                QDomNode OptionalCell_node = MustList_List.item(b);
                QDomElement OptionalCell = OptionalCell_node.toElement();
                QDomNodeList OptionalCell_List = OptionalCell.childNodes();
                for(c=0;c<=1;c++)//读取  Key Value
                {
                    QDomNode node1 = OptionalCell_List.item(c);
                    QDomElement Element = node1.toElement();
                    Element_value = Element.text();
                    p->CarType[i][146+c+2*b+101*a] = Element_value;
                }
            }
            QDomElement OptionList = MustList.nextSiblingElement();
            QDomNodeList OptionList_List = OptionList.childNodes();
            //可选
            for(b=0;b<=9;b++)//循环读取10个可选内容
            {
                QDomNode OptionalCell_node = OptionList_List.item(b);
                QDomElement OptionalCell = OptionalCell_node.toElement();
                QDomNodeList OptionalCell_List = OptionalCell.childNodes();
                for(c=0;c<=1;c++)//读取  Key Value
                {
                    QDomNode node1 = OptionalCell_List.item(c);
                    QDomElement Element = node1.toElement();
                    Element_value = Element.text();
                    p->CarType[i][166+c+2*b+101*a] = Element_value;
                }
            }
        }
    }
    //读取PDM信息
    int ele_num=0;
    QDomNode PDMList_node = list.item(24);
    QDomElement PDMList = PDMList_node.toElement();
    QDomNodeList PDMList_List = PDMList.childNodes();
    int PDM_num = PDMList_List.length();
    p->PDM[ele_num]=QString::number(PDM_num);//读取PDM数量
    for(i=0;i<PDM_num;i++)//按数量循环读取PDM信息
    {
        QDomNode PDMItem_node = PDMList_List.item(i);
        QDomElement PDMItem = PDMItem_node.toElement();
        QDomNodeList PDMItem_List = PDMItem.childNodes();
        QDomNode node1=PDMItem_List.item(0);
        QDomElement Element = node1.toElement();
        QString Element_value = Element.text();
        ele_num++;
        p->PDM[ele_num]=Element_value;//PDM名称
        node1=PDMItem_List.item(1);
        Element = node1.toElement();
        Element_value = Element.text();
        ele_num++;
        p->PDM[ele_num]=Element_value;//PDM数据
        QDomNode Items_node=PDMItem_List.item(2);
        QDomElement Items = Items_node.toElement();
        QDomNodeList Items_List = Items.childNodes();
        int Point_num = Items_List.length();
        ele_num++;
        p->PDM[ele_num]=QString::number(Point_num);//每个PDM的点数
        for(j=0;j<Point_num;j++)//按点数循环读取内容
        {
            QDomNode Point_node=Items_List.item(j);
            QDomElement Point = Point_node.toElement();
            QDomNodeList Point_List = Point.childNodes();
            QDomNode x_node = Point_List.item(0);
            QDomElement x = x_node.toElement();
            QString Element_value = x.text();
            ele_num++;
            p->PDM[ele_num] = Element_value;//X坐标

            QDomNode y_node = Point_List.item(1);
            QDomElement y = y_node.toElement();
            Element_value = y.text();
            ele_num++;
            p->PDM[ele_num] = Element_value;//Y坐标
        }
    }
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
    minitem_vision = minitem.mid(4);
    maxitem_vision = minitem.mid(4);
    for(i=2;i<filenum;i++)
    {
        item=fileInfo->at(i).fileName();
        item_vision = item.mid(4);

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
        qDebug()<<a<<b;
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


