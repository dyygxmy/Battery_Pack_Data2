#include "serialComs.h"
#include "qsettings.h"

SerialComs::SerialComs(QObject *parent) :
    QObject(parent)
{
    bzero(&buff,sizeof(buff));
    this->moveToThread(&m_thread);
    m_thread.start();
}

void SerialComs::comInit()
{
    seriallist.clear();
    if(isBarCode)
    {
        const char *dev_name;
        dev_name = "/dev/ttymxc0";
//                dev_name = "/dev/ttyUSB0";
        struct termios tty_attr;
        int baudrate;
        int byte_bits ;
        baudrate = B9600;//B115200;//;
        byte_bits = CS8;
        fd_set rset;
        dev_fd = open(dev_name, O_RDWR, 0);
        if (dev_fd < 0)
        {
            qDebug() << "vin serial open fail "+QString(dev_name);
            return;
        }
        else
        {
            qDebug() <<"vin open serial success "+QString(dev_name);
        }
        if (fcntl(dev_fd, F_SETFL, O_NONBLOCK) < 0)
        {
            qDebug() << "unable to set serial port";
            return;
        }
        memset(&tty_attr, 0, sizeof(struct termios));
        tty_attr.c_iflag = IGNPAR;
        tty_attr.c_cflag = baudrate | HUPCL | byte_bits | CREAD | CLOCAL;
        tty_attr.c_cc[VTIME] = 0;
        tty_attr.c_cc[VMIN] = 1;
        tcsetattr(dev_fd, TCSANOW, &tty_attr);
        QByteArray buff;//[20];
        int select_fd ;


        vinLen = 12;
        vinHead = "1";

        while(1)
        {
            FD_ZERO(&rset);
            FD_SET(dev_fd, &rset);

            select_fd = select(dev_fd+1,&rset,NULL,NULL,NULL);
            if (select_fd < 0)
            {
                qDebug() << "select fail";
            }
            if (FD_ISSET(dev_fd, &rset))
            {
                int  i=0;
                int  nread = 0;
                char data1[1];
                while ((nread = (read(dev_fd, &data1,sizeof(data1)))) > 0)
                {
                    if(((data1[0] >='a') && (data1[0]  <= 'z')) ||((data1[0]  >='A') && (data1[0]  <= 'Z'))||((data1[0]  >='0')&&(data1[0] <= '9'))||((char)data1[0] == 0x0d)||((char)data1[0] == 0x20))
                    {
                        buff.append(data1[0]);
                    }
                }
//                while( (!buff.isEmpty())&&(!buff.startsWith(vinHead)) )
//                {
//                    qDebug()<<"not Start with "<<vinHead<<buff;
//                    buff.replace(0,1,"");
//                }
//                if((buff.size() >= vinLen)&&(buff[0]==vinHead[0]))
                for(i=0;i<buff.size();i++)
                {
                    if((unsigned char)buff[i] == 0x0d)
                    {
                        if(i == vinLen)
                        {
                            QString serialNums = QString(buff.mid(0,i));
                            qDebug() << "tiaoma qiang"<< serialNums;
                            if(GscanRegExp == "Null")
                            {
                                if(DebugMode) //test interface display serialnum
                                {
                                    emit sendDebug(serialNums);
                                }
                                else
                                {
                                    emit sendCmdToStep(1,serialNums,2);
                                }
                            }
                            else if(GscanRegExp == serialNums.mid(0,GscanRegExp.size()))
                            {
                                if(DebugMode) //test interface display serialnum
                                {
                                    emit sendDebug(serialNums);
                                }
                                else
                                {
                                    emit sendCmdToStep(1,serialNums,2);
                                }
                            }
                            else
                            {
                                emit sendStationFinishToMaindow("huhuhu","301");
                            }
//                            if(DebugMode) //test interface display serialnum
//                            {
//                                emit sendDebug(serialNums);
//                            }
//                            else
//                            {
//                                emit sendCmdToStep(1,serialNums,2);
//                            }
                        }
                        buff.clear();
                        bzero(data1,sizeof(data1));
                    }
                }
                if(i > vinLen)
                {
                    buff.clear();
                    bzero(data1,sizeof(data1));
                }
            }
        }
    }
}


