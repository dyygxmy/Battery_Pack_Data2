#include "serialComsRead.h"

SerialComsRead::SerialComsRead(QObject *parent) :
    QObject(parent)
{
    m_thread.start();
    this->moveToThread(&m_thread);
    vari2_XL = "";
    vari1 = "";
}
/*******************************************/
//线程开始
/*******************************************/
void SerialComsRead::serialComStart()
{
    qDebug() << "barcode thread start!!!"<<vari1<<vari2_XL;
    comInit();
    QTimer::singleShot(1000,this,SLOT(timerReadComFunc()));
    timerCount = 0;
    scanCodeCount = 0;
    revComBuf = "";
    startScanCodeFlag = false;
    revStartFlag = false;
    timeoutCount = 0;
    boltSN = "";
    packSN = "";
    airResultValidFlag = false;
}

/*******************************************/
//RS232 com initialize
/*******************************************/
bool SerialComsRead::comInit()
{
    QString portName ;
    if(vari1.mid(0,1) == "M")
    {
        portName = "ttymxc0";   //获取串口名
    }
    else
    {
        portName = "ttymxc0";   //获取串口名
    }
    myCom = new QextSerialPort("/dev/" + portName);
    qDebug()<<"this is the carcode"<<Gcarcode;
    myCom->setBaudRate((BaudRateType)Gcarcode);
//    myCom->setBaudRate((BaudRateType)9600);
//    myCom->setBaudRate((BaudRateType)115200);
    //设置数据位
    myCom->setDataBits((DataBitsType)8);
    //设置校验
    myCom->setParity(PAR_NONE);
    //设置停止位
    myCom->setStopBits(STOP_1);
    //设置数据流控制
    myCom->setFlowControl(FLOW_OFF);
    //设置延时
    myCom->setTimeout(10);

    if(myCom->open(QIODevice::ReadWrite))
    {
        qDebug()<<"barcode serial open ok"<<portName;
        connect(myCom, SIGNAL(readyRead()), this, SLOT(slot_read_com3()));
        return true;
    }
    else
    {
        qDebug()<<"barcode serial open fail "<<portName;
        return false;
    }
}

/**************************************/
//timer read com
/**************************************/
void SerialComsRead::timerReadComFunc()
{
    if(revStartFlag) //receive serial data timeout
    {
        timeoutCount ++;
        if(timeoutCount > 3)
        {
            revStartFlag = false;
            timeoutCount = 0;
            revComBuf.clear();
            qDebug()<< "#################################clear revcomBuf";
        }
    }
    if(startScanCodeFlag)
    {
        timerCount++;
        if(timerCount > 2)
        {
            timerCount = 0;
            scanCodeCount++;
            sendSerialCMD();
        }
        if(scanCodeCount > 3)
        {
            emit sendCodeToPLC("Scan Error");
            timerCount = 0;
            scanCodeCount = 0;
            startScanCodeFlag = false;
        }
    }
    slot_read_com3();
    QTimer::singleShot(1000,this,SLOT(timerReadComFunc()));
}
/***********************************/
//serial com send CMD
//
/*************************************/
void SerialComsRead::sendSerialCMD()
{
    QByteArray sendBuf;
    revComBuf.clear();
    revStartFlag = false;
    sendBuf[0] = '+';
    myCom->write(sendBuf,1);
    emit sendCodeToMainwindow("99","NULL");
    qDebug() << "send+++++++++++++++:" << sendBuf <<scanCodeCount;

}
/*******************************************/
//start barcode scanning
/*******************************************/
void SerialComsRead::revAutoBarcodeCmd()
{
    if(!startScanCodeFlag)
    {
        startScanCodeFlag = true;
        timerCount = 0;
        scanCodeCount = 0;
        sendSerialCMD();
    }
}
/*******************************************/
//receive air test cmd
/*******************************************/
void SerialComsRead::revManuulCmd(QString sn,QString code,bool start_en)
{
    if(start_en)
    {
        boltSN = sn;
        packSN = code;
        airResultValidFlag = true;
        revComBuf.clear();
    }
    else
    {
        airResultValidFlag = false;
    }
}

/***********************************/
//receive serial com data
//
/*************************************/
void SerialComsRead::slot_read_com3()
{
    int revLen,i;
    QString serialNums;
    QByteArray recBuf;
    revLen=myCom->bytesAvailable();
    if(revLen>0)
    {
        if(!revStartFlag)
        {
            revStartFlag = true;
        }
        timeoutCount = 0;
        recBuf = myCom->readAll();
        revComBuf.append(recBuf);
        qDebug()<<"receive 11111 ::"<<recBuf.toHex();
        qDebug()<<"receive +++++ ::"<<revLen<<revComBuf.size()<<revComBuf;
        for(i=0;i<revComBuf.size();i++)
        {
            if(vari2_XL == "XL") //air test
            {
                if((unsigned char)revComBuf[i] == 0x0A)
                {
                    if(revComBuf.size() >= 88)
                    {
                        qDebug()<<"this is rec com  buf"<<recBuf.toHex();
                        if(airResultValidFlag)//air test
                        {
                            airTestResult(QString(revComBuf.right(85)));
                        }
                        revComBuf.clear();
                        revStartFlag = false;
                    }
                }
            }
            else
            {
                if((unsigned char)revComBuf[i] == 0x0A)
                {
                    if(startScanCodeFlag)
                    {
                        if(i==52)
                        {
                            serialNums = revComBuf.mid(1,14).trimmed();
                            emit sendCodeToPLC(serialNums);
                            emit sendCodeToMainwindow("98",serialNums);
                            startScanCodeFlag = false;
                            timerCount = 0;
                            scanCodeCount = 0;
                            qDebug() << "atuo tiaoma qiang"<< serialNums;
                        }
                    }
                    else
                    {
                        if(i>1)
                            serialNums = QString(revComBuf.mid(0,i-1));
                        else
                            serialNums = QString(revComBuf.mid(0,i));
                        if(GscanRegExp == "Null")
                        {
                            emit sendCmdToStep(1,serialNums,2);
                            emit sendCodeToMainwindow("1",serialNums);
                        }
                        else if(GscanRegExp == serialNums.mid(0,GscanRegExp.size()))
                        {
                            emit sendCmdToStep(1,serialNums,2);
                            emit sendCodeToMainwindow("1",serialNums);
                        }
                        else
                        {
                            emit sendCodeToMainwindow("huhuhu","301");
                        }
//                        emit sendCmdToStep(1,serialNums,2);
//                        emit sendCodeToMainwindow("1",serialNums);
                        qDebug() << "mannul tiaoma qiang"<< serialNums;
                    }
                    revComBuf.clear();
                    revStartFlag = false;
                }
            }
        }
    }
}
/***********************************/
//receive serial com data
// 4 STAT= 0 KAN= 2 OP= 0 PP= 1 DP=     0  G DT=   0.0 VL=    2.20 PF=-0.271 PM=-0.033 G
//KAN 通道号 上面为1 下面为2
//PP 程序号
//VL 泄漏率（cm3/min）
//PM 测量压力 （bar）
//A=NOK G=OK
/*************************************/
void SerialComsRead::airTestResult(QString buf)
{
    QString STATE_blc,MI_blc,WI_blc,Program,Channel;
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    Channel = buf.mid(12,2).trimmed();//KAN
    Program = buf.mid(24,2).trimmed();//pp
    MI_blc = buf.mid(53,8).trimmed();//VL
    WI_blc = buf.mid(75,6).trimmed();//PM
    STATE_blc = buf.mid(82,1);//A and G
    qDebug()<<"&(&((((((((((((("<<Channel<<Program<<MI_blc<<WI_blc<<STATE_blc;
    if(STATE_blc == "G")
    {
        STATE_blc = "OK";
    }
    else
    {
        STATE_blc = "NOK";
    }
    DATA_STRUCT demo;
    demo.data_model[0] = time.toString("yyyy-MM-dd");
    demo.data_model[1] = time.toString("hh:mm:ss");
    demo.data_model[2] = STATE_blc;
    demo.data_model[3] = MI_blc;
    demo.data_model[4] = WI_blc;
    demo.data_model[5] = boltSN;
    demo.data_model[6] = packSN;
    demo.data_model[7] = "0";
    demo.data_model[8] = "Curve is null";//CurveStr.mid(32);
    demo.data_model[9] = Program;
    demo.data_model[10] = Channel;
    demo.data_model[11] = "1";
    if(STATE_blc == "OK")
    {
        demo.data_model[12] = QString::number(BoltOrder[0]++);
    }
    else
    {
        demo.data_model[12] = QString::number(BoltOrder[0]);
    }
    demo.data_model[13] = "null";//QString::number(torque_max);
    demo.data_model[14] = "null";//QString::number(torque_min);
    demo.data_model[15] = "0";
    QVariant DataVar;
    DataVar.setValue(demo);
    emit sendResultTomainwindow(MI_blc,WI_blc,STATE_blc,Channel.toInt()-1);
    emit sendResultToSQL(DataVar);
}












