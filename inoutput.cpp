#include "inoutput.h"

InOutput::InOutput(QObject *parent) :
    QObject(parent)
{

}
/********************************************/
//return 1 = input 0;
//return 0 = input 1;
//In_Num:in1~in10
/********************************************/
bool InOutput::readIOInput(QString In_Num)
{
    qDebug()<<"********wait I/O Input*******"<<In_Num;
    int result = 0;
    switch(In_Num.remove(0,5).toInt())
    {
    case 1:
        {
            QFile IO_File_Path11("/sys/class/gpio/gpio162/value"); //INA1
            IO_File_Path11.open(QIODevice::ReadOnly);
            QTextStream in11(&IO_File_Path11);
            in11 >> result;
            IO_File_Path11.close();
        }
        break;
    case 2:
        {
            QFile IO_File_Path12("/sys/class/gpio/gpio160/value"); //INA2
            IO_File_Path12.open(QIODevice::ReadOnly);
            QTextStream in12(&IO_File_Path12);
            in12 >> result;
            IO_File_Path12.close();
        }
        break;
    case 3:
        {
            QFile IO_File_Path13("/sys/class/gpio/gpio165/value"); //INA3
            IO_File_Path13.open(QIODevice::ReadOnly);
            QTextStream in13(&IO_File_Path13);
            in13 >> result;
            IO_File_Path13.close();
        }
        break;
    case 4:
        {
            QFile IO_File_Path14("/sys/class/gpio/gpio164/value"); //INA4
            IO_File_Path14.open(QIODevice::ReadOnly);
            QTextStream in14(&IO_File_Path14);
            in14 >> result;
            IO_File_Path14.close();
        }
        break;
    case 5:
        {
            QFile IO_File_Path15("/sys/class/gpio/gpio159/value"); //INA5
            IO_File_Path15.open(QIODevice::ReadOnly);
            QTextStream in15(&IO_File_Path15);
            in15 >> result;
            IO_File_Path15.close();
        }
        break;
    case 6:
        {
            QFile IO_File_Path16("/sys/class/gpio/gpio158/value"); //INA6
            IO_File_Path16.open(QIODevice::ReadOnly);
            QTextStream in16(&IO_File_Path16);
            in16 >> result;
            IO_File_Path16.close();
        }
        break;
    case 7:
        {
            QFile IO_File_Path17("/sys/class/gpio/gpio149/value"); //INA7
            IO_File_Path17.open(QIODevice::ReadOnly);
            QTextStream in17(&IO_File_Path17);
            in17 >> result;
            IO_File_Path17.close();
        }
        break;
    case 8:
        {
            QFile IO_File_Path20("/sys/class/gpio/gpio146/value"); //INB0
            IO_File_Path20.open(QIODevice::ReadOnly);
            QTextStream in20(&IO_File_Path20);
            in20 >> result;
            IO_File_Path20.close();
        }
        break;
    case 9:
        {
            QFile IO_File_Path21("/sys/class/gpio/gpio147/value"); //INB1
            IO_File_Path21.open(QIODevice::ReadOnly);
            QTextStream in21(&IO_File_Path21);
            in21 >> result;
            IO_File_Path21.close();
        }
        break;
    case 10:
        {
            QFile IO_File_Path22("/sys/class/gpio/gpio176/value"); //INB2
            IO_File_Path22.open(QIODevice::ReadOnly);
            QTextStream in22(&IO_File_Path22);
            in22 >> result;
            IO_File_Path22.close();
        }
        break;
    default:
        break;
    }
    if(result == 1)
        return false;
    else
        return true;
}
/********************************************/
//value = 1 : output 1;
//value = 0 : output 0;
//out_Num:ou1~out9/r_led/g_led/y_led/w_led

//system("echo 1 > /sys/class/leds/OUTD5/brightness");
//system("echo 1 > /sys/class/leds/OUTD6/brightness");
//system("echo 1 > /sys/class/leds/OUTD7/brightness");
//system("echo 1 > /sys/class/leds/OUTC0/brightness");
//system("echo 1 > /sys/class/leds/OUTC1/brightness");
//system("echo 1 > /sys/class/leds/OUTC2/brightness");        //
//system("echo 1 > /sys/class/leds/OUTC3/brightness");        //
//system("echo 1 > /sys/class/leds/OUTC4/brightness");        //
//system("echo 1 > /sys/class/leds/OUTC5/brightness");        //invalid
//system("echo 1 > /sys/class/leds/OUTC6/brightness");        //

/********************************************/
void InOutput::writeIOOutput(QString out_Num, bool value)
{
    qDebug()<<"********output*********"<<out_Num<<value;
    if(out_Num == "all_led")
    {
        if(value)
        {}
        else
        {
            system("echo 0 > /sys/class/leds/control_led2/brightness &");
            system("echo 0 > /sys/class/leds/control_led3/brightness &");
            system("echo 0 > /sys/class/leds/control_led4/brightness &");
            system("echo 0 > /sys/class/leds/control_led9/brightness &");
            system("echo 0 > /sys/class/leds/OUTC4/brightness &");
        }
    }
    else if(out_Num == "r_led")
    {
        if(value)
            system("echo 1 > /sys/class/leds/control_led2/brightness &");
        else
            system("echo 0 > /sys/class/leds/control_led2/brightness &");
    }
    else if(out_Num == "g_led")
    {
        if(value)
            system("echo 1 > /sys/class/leds/control_led3/brightness &");
        else
            system("echo 0 > /sys/class/leds/control_led3/brightness &");
    }
    else if(out_Num == "y_led")
    {
        if(value)
        {
            system("echo 1 > /sys/class/leds/control_led4/brightness &");
        }
        else
            system("echo 0 > /sys/class/leds/control_led4/brightness &");
    }
    else if(out_Num == "w_led")
    {
        if(value)
            system("echo 1 > /sys/class/leds/control_led9/brightness &");
        else
            system("echo 0 > /sys/class/leds/control_led9/brightness &");
    }
    else if(out_Num == "n_led")
    {
        if(value)
            system("echo 1 > /sys/class/leds/OUTC4/brightness &");
        else
            system("echo 0 > /sys/class/leds/OUTC4/brightness &");
    }
    else if(out_Num == "Output1")
    {
        if(value)
            system("echo 1 > /sys/class/leds/OUTC3/brightness &");
        else
            system("echo 0 > /sys/class/leds/OUTC3/brightness &");
    }
    else if(out_Num == "Output2")
    {
        if(value)
            system("echo 1 > /sys/class/leds/OUTC2/brightness &");
        else
            system("echo 0 > /sys/class/leds/OUTC2/brightness &");
    }
    else if(out_Num == "Output3")
    {
        if(value)
            system("echo 1 > /sys/class/leds/OUTC1/brightness &");
        else
            system("echo 0 > /sys/class/leds/OUTC1/brightness &");
    }
    else if(out_Num == "Output4")   //放行指示灯
    {
        if(value)
            system("echo 1 > /sys/class/leds/OUTC0/brightness &");
        else
            system("echo 0 > /sys/class/leds/OUTC0/brightness &");
    }
    else if(out_Num == "Output5")
    {
        if(value)
            system("echo 1 > /sys/class/leds/OUTD7/brightness &");
        else
            system("echo 0 > /sys/class/leds/OUTD7/brightness &");
    }
    else if(out_Num == "Output6")
    {
        if(value)
            system("echo 1 > /sys/class/leds/OUTD6/brightness &");
        else
            system("echo 0 > /sys/class/leds/OUTD6/brightness &");
    }
    else if(out_Num == "Output7")
    {
        if(value)
            system("echo 1 > /sys/class/leds/OUTD5/brightness &");
        else
            system("echo 0 > /sys/class/leds/OUTD5/brightness &");
    }
    else if(out_Num == "Output8")
    {
        if(value)
            system("echo 1 > /sys/class/leds/OUTC6/brightness &");
        else
            system("echo 0 > /sys/class/leds/OUTC6/brightness &");
    }
    else if(out_Num == "all_out")
    {
        if(value)
        {}
        else
        {
            system("echo 0 > /sys/class/leds/OUTC6/brightness &");
            system("echo 0 > /sys/class/leds/OUTC4/brightness &");
            system("echo 0 > /sys/class/leds/OUTC3/brightness &");
            system("echo 0 > /sys/class/leds/OUTC2/brightness &");
            system("echo 0 > /sys/class/leds/OUTC1/brightness &");
            system("echo 0 > /sys/class/leds/OUTC0/brightness &");
            system("echo 0 > /sys/class/leds/OUTD7/brightness &");
            system("echo 0 > /sys/class/leds/OUTD6/brightness &");
            system("echo 0 > /sys/class/leds/OUTD5/brightness &");
        }
    }
}
\























