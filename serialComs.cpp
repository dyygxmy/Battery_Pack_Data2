#include "serialComs.h"

SerialComs::SerialComs(QObject *parent) :
    QObject(parent)
{
    whichindex = 0;
    bzero(&buff,sizeof(buff));
    this->moveToThread(&m_thread);
    m_thread.start();
}

void SerialComs::comInit()
{
    seriallist.clear();
    if(isRFID != 1)
    {
        const char *dev_name;
        dev_name = "/dev/ttymxc3";
        struct termios tty_attr;
        int baudrate;
        int byte_bits ;
        baudrate = B9600;
        byte_bits = CS8;
        fd_set rset;
        dev_fd = open(dev_name, O_RDWR, 0);
        if (dev_fd < 0)
        {
            qDebug() << "serial open fail";
        }
        if (fcntl(dev_fd, F_SETFL, O_NONBLOCK) < 0)
            qDebug() << "unable to set serial port";
        memset(&tty_attr, 0, sizeof(struct termios));
        tty_attr.c_iflag = IGNPAR;
        tty_attr.c_cflag = baudrate | HUPCL | byte_bits | CREAD | CLOCAL;
        tty_attr.c_cc[VTIME] = 0;
        tty_attr.c_cc[VMIN] = 1;
        tcsetattr(dev_fd, TCSANOW, &tty_attr);
        char buff[20];
        int select_fd ;
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
                int  nread = 0;
                char data1[1];
                while ((nread = (read(dev_fd, &data1,sizeof(data1)))) > 0)
                {
                    if(((data1[0] >='a') && (data1[0]  <= 'z')) ||((data1[0]  >='A') && (data1[0]  <= 'Z'))||((data1[0]  >='0')&&(data1[0] <= '9')))
                    {
                        strcat(buff,data1);
                    }
                }
                if(strlen(buff) == 17)
                {
                    buff[17] = '\0';
                    qDebug() << "tiaoma qiang";
                    QString serialNums = QString(QLatin1String(buff));
                    if(isRFID == 2)
                    {
//                        if(SYSS == "ING")
                            emit  serialCom(serialNums,false,"queue");
//                        else
//                        {
//                            emit serialCom(serialNums,false,"ready");
//                        }
                    }
                    else if(CsIsConnect && !ISmaintenance  && SYSS!="ING")
                    {
                        if(SerialGunMode)
                        {
                            system("echo 1 > /sys/class/leds/control_uart2/brightness");
                        }

                        isequeal = false;
                        if(seriallist.isEmpty())
                        {
                            seriallist.push_back(serialNums);
                            whichindex++;
                        }
                        else
                        {
                            QList<QString>::iterator i;
                            for (i = seriallist.begin(); i != seriallist.end(); ++i)
                            {
                                if(!(QString::compare(serialNums, *i, Qt::CaseSensitive)))
                                {
                                    isequeal = true;
                                    break;
                                }
                            }
                            if(!isequeal)
                            {
                                if(seriallist.size() == 10)
                                {
                                    seriallist[whichindex] = serialNums;
                                }
                                else
                                {
                                    seriallist.push_back(serialNums);
                                }
                                whichindex++;
                                if(whichindex == 10)
                                    whichindex = 0;
                            }
                        }
                        emit serialCom(serialNums,isequeal," ");
                    }

                    if(strlen(buff) == 17)
                    {
                        bzero(buff,sizeof(buff));
                        bzero(data1,sizeof(data1));
                    }
                }
            }
        }
    }
}
