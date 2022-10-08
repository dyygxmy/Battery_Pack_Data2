#include "inputevents.h"
#include "GlobalVarible.h"
#include <QDebug>
InputEvents::InputEvents(QObject *parent) :
    QObject(parent)
{
    numcount = 0;
    issingle = false;
    keyvalue = 0;
    this->moveToThread(&m_thread);
    m_thread.start();

}

void InputEvents::InputEventStart()
{
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(mtimerarrve3()));
    const char *dev_name;
    dev_name = "/dev/input/event0";
    while(1)
    {
        keys_fd = open(dev_name, O_RDWR, 0);
        if (keys_fd < 0)
        {
            continue;
        }
        else
        {
            break;
        }
    }
    if (fcntl(keys_fd, F_SETFL, O_NONBLOCK) < 0)
        printf("Unable set to NONBLOCK mode");
    m_timer.start(100);
}

void InputEvents::mtimerarrve3()
{
    if(!ISWARNING)
    {
        if(read(keys_fd,&t,sizeof(t))==sizeof(t))
        {
            if(t.type == EV_KEY)
                if(t.value==0 || t.value==1)
                {
                    switch(t.code)
                    {
                    case 114:   //钥匙
                        keyvalue = t.value;

                        if(t.value == 1)
                        {
                            if(DebugMode)
                            {
                                system("echo 1 > /sys/class/leds/OUTC4/brightness");
                            }
                            else
                            {
                                numcount++;
                                if(numcount == 20)
                                {
                                    numcount = 0;
                                    emit sendconfigwarning(false);//nok all bolt
                                }
                            }
                        }
                        else if(t.value == 0)
                        {
                            if(DebugMode)
                            {
                                system("echo 0 > /sys/class/leds/OUTC4/brightness");
                            }
                            else
                            {
                                numcount = 0;
                                emit sendconfigwarning(true);//nok one bolt
                            }
                        }
                        break;
                    case 116:
                        break;
                    default:
                        break;
                    }
                }
        }
        else
        {
            if(keyvalue)
            {
                numcount++;
                if(numcount == 20)
                {
                    emit sendconfigwarning(false);//nok all bolt
                    numcount = 0;
                    keyvalue = 0;
                }

            }
        }
    }
    else
    {
        read(keys_fd,&t,sizeof(t));
    }
}
