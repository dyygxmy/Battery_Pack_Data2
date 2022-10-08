#include "codeedit.h"
#include "ui_codeedit.h"
#define TIME_OUT 20

CodeEdit::CodeEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CodeEdit)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setGeometry(QRect(0, 0, 1366, 768));
    init();
    setRFIDUI(false);
    connect(&RFIDTimer,SIGNAL(timeout()),this,SLOT(RFIDTimeout()));
}

CodeEdit::~CodeEdit()
{
    delete ui;
}

void CodeEdit::init()
{
    QString portName = "ttyUSB0";   //获取串口
    portName = "ttymxc3";   //获取串口名

    myCom = new QextSerialPort("/dev/" + portName);
    connect(myCom, SIGNAL(readyRead()), this, SLOT(slot_read_com()));

    //设置波特率
    myCom->setBaudRate((BaudRateType)9600);

    //设置数据位
    myCom->setDataBits((DataBitsType)8);

    //设置校验
    myCom->setParity(PAR_EVEN);
    //    myCom->setParity(PAR_NONE);
    //设置停止位
    myCom->setStopBits(STOP_1);

    //设置数据流控制
    myCom->setFlowControl(FLOW_OFF);
    //设置延时
    myCom->setTimeout(TIME_OUT);

    if(myCom->open(QIODevice::ReadWrite)){
        qDebug()<<"EKS serial open ok" << portName;

    }
    else
    {
        qDebug()<<"EKS serial open fail " << portName;
        return;
    }
    revRsDataLen = 0;
    revDataAckFlag = 0;
    revDataAliveFlag = 0;
    midCount = 0;
    sendDataLen = 0;
    firstFlag = false;
    writeBuf = "";
    readFlag = false;
    writeFlag = false;
    writeAckFlag=false;

    connect(&readTimer,SIGNAL(timeout()),this,SLOT(readKey()));
    readTimer.start(2000);
    tmpKey = noKey;
}


//连接eks钥匙
void CodeEdit::readKey()
{
    isWrite = false;
    QByteArray sendTempBuf;
    revRsDataBuf.clear();
    sendTempBuf[0] = 0x02;
    comSendFunc(sendTempBuf,1);
    firstFlag = true;
    readFlag = true;
}

/******************************************************************/
//read data function
/******************************************************************/
void CodeEdit::readCmdFunc(int addr, unsigned char len)
{
    QByteArray sendBuf = "";
    unsigned char sum = 0;
    sendBuf[0] = 0x07;
    sendBuf[1] = 'T';
    sendBuf[2] = 'L';
    sendBuf[3] = 0x01;
    sendBuf[4] = addr>>8;
    sendBuf[5] = addr;
    sendBuf[6] = len;
    sendBuf[7] = 0x10;
    sendBuf[8] = 0x03;
    for(int i=0;i<sendBuf.size();i++)
    {
        sum ^= sendBuf[i];
    }
    sendBuf[9] = sum;
    comSendFunc(sendBuf,sendBuf.size());
    revRsDataBuf.clear();
}

/******************************************************************/
//write data function
/******************************************************************/
void CodeEdit::writeCmdFunc(int addr,unsigned char len,QByteArray buf)
{
    QByteArray sendBuf = "";
    unsigned char sum = 0;
    sendBuf[1] = 'T';
    sendBuf[2] = 'P';
    sendBuf[3] = 0x01;
    sendBuf[4] = addr>>8;
    sendBuf[5] = addr;
    sendBuf[6] = len;
    sendBuf.append(buf);
    sendBuf[buf.size()+7] = 0x10;
    sendBuf[buf.size()+8] = 0x03;
    sendBuf[0] = buf.size()+7;
    for(int i=0;i<sendBuf.size();i++)
    {
        sum ^= sendBuf[i];
    }
    sendBuf[buf.size()+9] = sum;
    comSendFunc(sendBuf,sendBuf.size());
    writeAckFlag=true;
    revRsDataBuf.clear();
}

void CodeEdit::clearUi()
{
    ui->label_key_read->clear();
    ui->label_key_write->clear();
    on_pushButton_RFIDClear_clicked();
}

/******************************************************************/
//RS232 recvice zigbee data
/******************************************************************/
void CodeEdit::slot_read_com()
{
    //du读
    QByteArray revBuf,sendTempBuf="";
    int revLen=myCom->bytesAvailable();
    if(revLen > 0)
    {
        revBuf = myCom->readAll();
        revRsDataBuf.append(revBuf);
//        qDebug()<<"EKS rev len:"<<revLen<<"rev data:"<<revBuf.toHex();
//        qDebug()<<"revRsDataBuf len:"<<revRsDataBuf.size()<<"revRsDataBuf data:"<<revRsDataBuf.toHex();
        if((unsigned char)revRsDataBuf[0] == 0x10)
        {
            revRsDataBuf.clear();
            if(firstFlag)
            {
                firstFlag = false;
                if(readFlag)
                {
                    readFlag = false;
                    readCmdFunc(0x0000,8);
                }
                else if(writeFlag)
                {
                    writeFlag = false;
                    writeCmdFunc(0x0000, 8,writeBuf);
                }
            }
        }
        else if((unsigned char)revRsDataBuf[0] == 0x02)
        {
            revRsDataBuf.clear();
            sendTempBuf[0] = 0x10;
            comSendFunc(sendTempBuf,1);
        }
        else
        {
            if(revRsDataBuf.mid(1,2) == "RL")
            {
                if(((unsigned char)revRsDataBuf[0]+3) == revRsDataBuf.size())
                {
                    qDebug()<<"EKS rev len:"<<revRsDataBuf.size()<<"EKS rev data:"<<revRsDataBuf.toHex();
                    qDebug() << "rev cmd:"<<revRsDataBuf.mid(7,8);
                    qDebug()<<"Read Key: " + revRsDataBuf.mid(7,8);
                    if(revRsDataBuf.mid(7,8) == "EKS-RD**")
                    {
                        ui->label_key_read->setText(tr("红色"));
                        judgeKey(redKey);
                    }
                    else if(revRsDataBuf.mid(7,8) == "EKS-BK**")
                    {
                        ui->label_key_read->setText(tr("黑色"));
                        judgeKey(blackKey);
                    }
                    else if(revRsDataBuf.mid(7,8) == "EKS-GN**")
                    {
                        ui->label_key_read->setText(tr("绿色"));
                        judgeKey(greenKey);
                    }

                    revRsDataBuf.clear();
                    sendTempBuf[0] = 0x10;
                    comSendFunc(sendTempBuf,1);
                }
            }
            else if(revRsDataBuf.mid(1,2) == "RF")
            {
                if(((unsigned char)revRsDataBuf[0]+3) == revRsDataBuf.size())
                {
                    qDebug()<<"EKS rev len:"<<revRsDataBuf.size()<<"EKS rev data:"<<revRsDataBuf.toHex();
                    if((unsigned char)revRsDataBuf[6] == 0x00)
                    {
                        qDebug()<<"key Write Success!";
                        if(isWrite)
                            ui->label_key_write->setText(tr("写入成功"));
                    }
                    else if((unsigned char)revRsDataBuf[6] == 0x02)
                    {
                        if(isWrite)
                            ui->label_key_write->setText(tr("没有钥匙"));
                        else
                            ui->label_key_read->setText(tr("没有钥匙"));
                        qDebug()<<"Error: NO Key !";
                        judgeKey(noKey);
                    }
                    else
                    {
                        qDebug()<<"key Write fail!";
                        if(isWrite)
                            ui->label_key_write->setText(tr("写入失败"));
                    }
                    revRsDataBuf.clear();
                    sendTempBuf[0] = 0x10;
                    comSendFunc(sendTempBuf,1);
                }
            }
            else
            {
                qDebug()<<"rec eks error"<<revRsDataBuf;
            }
        }
    }
}

void CodeEdit::judgeKey(keys key)
{
    if(tmpKey != key)
    {
        qDebug()<<"key change to "<<key;
        //        if(tmpKey != noKey)
        //            emit sendLogin(false);
        tmpKey = key;
        emit sendKey(tmpKey);
    }
}

/******************************************************************/
//write red data
/******************************************************************/
void CodeEdit::on_pushButton_red_clicked()
{
    isWrite = true;
    QByteArray sendTempBuf;
    sendTempBuf[0] = 0x02;
    comSendFunc(sendTempBuf,1);
    firstFlag = true;
    writeFlag = true;
    writeBuf = "EKS-RD**";
}

/******************************************************************/
//write green data
/******************************************************************/
void CodeEdit::on_pushButton_green_clicked()
{
    isWrite = true;
    QByteArray sendTempBuf;
    sendTempBuf[0] = 0x02;
    comSendFunc(sendTempBuf,1);
    firstFlag = true;
    writeFlag = true;
    writeBuf = "EKS-GN**";
}

/******************************************************************/
//write black data
/******************************************************************/
void CodeEdit::on_pushButton_black_clicked()
{
    isWrite = true;
    QByteArray sendTempBuf;
    sendTempBuf[0] = 0x02;
    comSendFunc(sendTempBuf,1);
    firstFlag = true;
    writeFlag = true;
    writeBuf = "EKS-BK**";
}

void CodeEdit::on_pushButton_read_clicked()
{
    readKey();
}

void CodeEdit::on_pushButton_clicked()
{
    this->close();
    readTimer.start(2000);
}
/********************************************/
//seriel send data
/********************************************/
void CodeEdit::comSendFunc(QByteArray send_Buf, int len)
{
    myCom->write(send_Buf,len);
    myCom->waitForBytesWritten(1000);
    qDebug() << "EKS send len:" << len <<"send data:"<< send_Buf.toHex();
}

void CodeEdit::receiveSuccess(QString action, QByteArray data)
{
    RFIDTimer.stop();
    if(action == "write")
    {
        ui->label_rfidReturn->setText(tr("写入成功！"));
        qDebug()<<"write success";
    }
    else if(action == "read")
    {
        ui->label_rfidReturn->setText(tr("读取成功！"));
        ui->lineEdit_RFID->setText(QByteArray::fromHex(data));
        //        ui->lineEdit_RFID->setText(hexConvertToStr(data));
        qDebug()<<"read success"<<QByteArray::fromHex(data);
    }
}

void CodeEdit::RFIDTimeout()
{
    qDebug()<<"RFID Time out";
    ui->label_rfidReturn->setText(tr("操作超时！"));
    RFIDTimer.stop();
    emit sendRFIDReset();
}

//rfid Ui
void CodeEdit::setRFIDUI(bool isEnable)
{
    if(isEnable)
    {
        isRFIDConnected = true;
        ui->label_RFIDState->setStyleSheet("background-color: rgb(25, 125, 44);");
        ui->lineEdit_RFID->setStyleSheet("font: 14pt \"Arial\";border-width:1px; border-style:solid; border-color:rgb(51, 153, 255);");
        ui->pushButton_RFIDRead->setStyleSheet("font: 14pt \"黑体\";color: rgb(248, 248, 255);border-style: solid;background-color: rgb(51,153,255);");
        ui->pushButton_RFIDWrite->setStyleSheet("font: 14pt \"黑体\";color: rgb(248, 248, 255);border-style: solid;background-color: rgb(51,153,255);");
        ui->pushButton_RFIDClear->setStyleSheet("font: 14pt \"黑体\";color: rgb(248, 248, 255);border-style: solid;background-color: rgb(51,153,255);");
        ui->lineEdit_RFID->setEnabled(true);
        ui->pushButton_RFIDRead->setEnabled(true);
        ui->pushButton_RFIDWrite->setEnabled(true);
        ui->pushButton_RFIDClear->setEnabled(true);
    }
    else
    {
        on_pushButton_RFIDClear_clicked();
        isRFIDConnected = false;
        ui->label_RFIDState->setStyleSheet("background-color: rgb(100, 100, 100);");
        ui->lineEdit_RFID->setStyleSheet("font: 14pt \"Arial\";border-width:1px; border-style:solid; border-color:rgb(100, 100, 100);");
        ui->pushButton_RFIDRead->setStyleSheet("font: 14pt \"黑体\";color: rgb(248, 248, 255);border-style: solid;background-color: rgb(100, 100, 100);");
        ui->pushButton_RFIDWrite->setStyleSheet("font: 14pt \"黑体\";color: rgb(248, 248, 255);border-style: solid;background-color: rgb(100, 100, 100);");
        ui->pushButton_RFIDClear->setStyleSheet("font: 14pt \"黑体\";color: rgb(248, 248, 255);border-style: solid;background-color: rgb(100, 100, 100);");
        ui->lineEdit_RFID->setEnabled(false);
        ui->pushButton_RFIDRead->setEnabled(false);
        ui->pushButton_RFIDWrite->setEnabled(false);
        ui->pushButton_RFIDClear->setEnabled(false);
    }
}
//QString CodeEdit::hexConvertToStr(QByteArray data)
//{
//    int hByte,lByte,byte;
//    QString strData = "";
//    QByteArray strTemp = "";
//    for(int i=0;i<data.size();i+=2)
//    {
//        hByte = (data.mid(i,1).toInt())<<4;
//        lByte = data.mid(i+1,1).toInt();
//        byte = hByte + lByte;
//        strTemp[0] = byte;
//        strData.append(strTemp.mid(0,1));
//    }
//    return strData;
//}

void CodeEdit::on_pushButton_RFIDWrite_clicked()
{
    if(isRFIDConnected)
    {
        RFIDTimer.start(3000);
        emit sendRFIDWrite("0000",(ui->lineEdit_RFID->text().toAscii().toHex()+"0D").toUpper());
        ui->lineEdit_RFID->clear();
        ui->label_rfidReturn->clear();
        qDebug()<<"sendRFIDWrite"<<ui->lineEdit_RFID->text();
    }
}

void CodeEdit::on_pushButton_RFIDRead_clicked()
{
    if(isRFIDConnected)
    {
        RFIDTimer.start(3000);
        emit sendRFIDRead("0000","0015");
        ui->label_rfidReturn->clear();
        qDebug()<<"sendRFIDRead";
    }
}

void CodeEdit::on_pushButton_RFIDClear_clicked()
{
    ui->lineEdit_RFID->clear();
    ui->label_rfidReturn->clear();
}
