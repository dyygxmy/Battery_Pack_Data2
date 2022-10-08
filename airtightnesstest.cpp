#include "airtightnesstest.h"
#include "unistd.h"

airTightnessTest::airTightnessTest(QObject *parent) :
    QObject(parent)
{
}
void airTightnessTest::comInit()
{
    const char *dev_name;
    dev_name = "/dev/ttymxc0";
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
    resultLen = 85;
    strKanNum = "";
    strProgramnum = "";
    strVL = "";
    strPM = "";
    strResult = "";

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
                if(((data1[0] >='a') && (data1[0]  <= 'z')) ||((data1[0]  >='A') && (data1[0]  <= 'Z'))||((data1[0]  >='0')&&(data1[0] <= '9'))||((char)data1[0] == 0x0d)||((char)data1[0] == 0x0a)||((char)data1[0] == 0x20))
                {
                    buff.append(data1[0]);
                }
            }
            for(i=0;i<buff.size();i++)
            {
                if((unsigned char)buff[i] == 0x0d)
                {
                    if(i == resultLen)
                    {
                        strKanNum = QString(buff.mid(14,2).trimmed());
                        strProgramnum = QString(buff.mid(26,2).trimmed());
                        strVL = QString(buff.mid(55,8).trimmed());
                        strPM = QString(buff.mid(77,6).trimmed());
                        strResult = QString(buff.mid(84,1).trimmed());
                        if(strResult == "G")
                        {
                            strResult = "OK";
                        }
                        else
                        {
                            strResult = "NOK";
                        }
                    }
                    buff.clear();
                    bzero(data1,sizeof(data1));
                }
            }
            if(i > resultLen)
            {
                buff.clear();
                bzero(data1,sizeof(data1));
            }
        }
    }
}
















