#include "taotong.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
TaoTong::TaoTong(QObject *parent) :
    QObject(parent)
{

    m_thread.start();
    this->moveToThread(&m_thread);
}

void TaoTong::T_start()
{
    qDebug()<<"taotong thread start;";
    count_num1 = 0;
    count_num2 = 0;
    tt1 = 0;
    tt2 = 0;
    tt3 = 0;
    tt4 = 0;
    IO1 = 0;
    IO2 = 0;
    IO3 = 0;
    IO4 = 0;
    IO5 = 0;
    tt_Value[0] = 1;
    tt_Value[1] = 1;
    tt_Value[2] = 1;
    tt_Value[3] = 1;
    IO_Value[0] = 1;
    IO_Value[1] = 1;
    IO_Value[2] = 1;
    IO_Value[3] = 1;
    IO_Value[4] = 1;
    ttSendEnFlag = false;
    IOSendEnFlag = false;
    firstStatusFlag = false;
    k_power = "";
    k_before = "";
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(IO_Collect_timer()));
    m_timer.start(500);
    In_Out_Put = new InOutput;
}
/**************************************/
//IO collect timer
//
/*************************************/
void TaoTong::IO_Collect_timer()
{
    QFile power("/var/power");
    power.open(QIODevice::ReadOnly);
    QTextStream in_power(&power);
    in_power >> k_power;
    if(k_power != k_before)
    {
        k_before = k_power;
        emit sendbattery(k_power);
    }
    power.close();
//    if(vari1_type == "M" )
//    {
//        /***********************************************************/
//        emit sendStopSignal(In_Out_Put->readIOInput("Input6"));
//        /*************************stop button**********************************/
//    }
    if(SYSS == "ING" && TaoTongState)
    {
        tt1 = In_Out_Put->readIOInput("Input1");
        tt2 = In_Out_Put->readIOInput("Input2");
        tt3 = In_Out_Put->readIOInput("Input3");
        tt4 = In_Out_Put->readIOInput("Input4");
        IO1 = In_Out_Put->readIOInput("Input6");
        IO2 = In_Out_Put->readIOInput("Input7");
        IO3 = In_Out_Put->readIOInput("Input8");
        IO4 = In_Out_Put->readIOInput("Input9");
        IO5 = In_Out_Put->readIOInput("Input10");
//        qDebug()<<"****taotong*****value:"<<tt1<<tt2<<tt3<<tt4<<IO1<<IO2<<IO3<<IO4<<IO5<<firstStatusFlag<<ttSendEnFlag<<IOSendEnFlag;
        if( (tt_Value[0] == 1)&&(tt_Value[1] == 1)&&(tt_Value[2] == 1)&&(tt_Value[3] == 1) )
        {
            count_num1 = 0;
        }
        else if((tt1 == tt_Value[0])&&(tt2 == tt_Value[1])&&(tt3 == tt_Value[2])&&(tt4 == tt_Value[3]))
        {
            count_num1++;
            if(count_num1 == 3)
            {
                count_num1 = 0;
                if(!ttSendEnFlag||firstStatusFlag)//taotong enable
                {
                    taotong_num(1);
                    ttSendEnFlag = true;
                    firstStatusFlag = false;
                }
            }
        }
        else
        {
            count_num1 = 0;
            if(ttSendEnFlag||firstStatusFlag)//taotong disable
            {
                taotong_num(0);
                ttSendEnFlag = false;
                firstStatusFlag = false;
            }
        }

        if( (IO_Value[0] == 1)&&(IO_Value[1] == 1)&&(IO_Value[2] == 1)&&(IO_Value[3] == 1)&&(IO_Value[4] == 1) )
        {
            count_num2 = 0;
        }
        else if((IO1 == IO_Value[0])&&(IO2 == IO_Value[1])&&(IO3 == IO_Value[2])&&(IO4 == IO_Value[3])&&(IO5 == IO_Value[4]))
        {
            count_num2++;
            if(count_num2 == 3)
            {
                count_num2 = 0;
                if(!IOSendEnFlag)
                {
                    taotong_num(2);//IO enable
                    IOSendEnFlag = true;
                }
            }
        }
        else
        {
            qDebug()<<"error"<<IO_Value[0]<<IO_Value[1]<<IO_Value[2]<<IO_Value[3]<<IO_Value[4];
        }
    }

}
/*******************************/
//ttValue:setting taotong value
//IOValue:setting Input and Output value
//boltNum:reserver
/*******************************/
void TaoTong::rev_TT_IO_Func(int ttValue, int IOValue, int boltNum)
{
    tt_Value[0] = 1;
    tt_Value[1] = 1;
    tt_Value[2] = 1;
    tt_Value[3] = 1;
    IO_Value[0] = 1;
    IO_Value[1] = 1;
    IO_Value[2] = 1;
    IO_Value[3] = 1;
    IO_Value[4] = 1;
    ttSendEnFlag = false;
    IOSendEnFlag = false;
    firstStatusFlag = true;
    switch (ttValue)
    {
    case 1:
        tt_Value[0] = 0;
        break;
    case 2:
        tt_Value[1] = 0;
        break;
    case 3:
        tt_Value[2] = 0;
        break;
    case 4:
        tt_Value[3] = 0;
        break;
    default:
        break;
    }
    switch (IOValue)
    {
    case 1:
        IO_Value[0] = 0;
        In_Out_Put->writeIOOutput("out5",true);
        break;
    case 2:
        IO_Value[1] = 0;
        In_Out_Put->writeIOOutput("out6",true);
        break;
    case 3:
        IO_Value[2] = 0;
        In_Out_Put->writeIOOutput("out7",true);
        break;
    case 4:
        IO_Value[3] = 0;
        In_Out_Put->writeIOOutput("out8",true);
        break;
    case 5:
        IO_Value[4] = 0;
        In_Out_Put->writeIOOutput("out9",true);
        break;
    default:
        break;
    }
    qDebug()<<"********rev IO value*********"<<tt_Value[0]<<tt_Value[1]<<tt_Value[2]<<tt_Value[3]<<ttSendEnFlag<<boltNum;
    qDebug()<<"********rev In_Out_Put*********************"<<IO_Value[0]<<IO_Value[1]<<IO_Value[2]<<IO_Value[3]<<IO_Value[4]<<IOSendEnFlag;
}
/*******************************/
//IO output reset
/*******************************/
void TaoTong::rev_IO_Reset()
{
    In_Out_Put->writeIOOutput("out5",false);
    In_Out_Put->writeIOOutput("out6",false);
    In_Out_Put->writeIOOutput("out7",false);
    In_Out_Put->writeIOOutput("out8",false);
    In_Out_Put->writeIOOutput("out9",false);
}

