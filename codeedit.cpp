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

    shageGOE = new QGraphicsOpacityEffect(this);

    init();
    initTableWidget();
    initListWidget();

    mesBox = new DTMessageBox(this);
    setRFIDUI(false);
    
    indexItem = "PHEVENCODER" ;
    indexRow  = 0 ;

    ui->listWidget->setSpacing(10);
    ui->stackedWidget_left->setCurrentIndex(0);
    ui->btnEncoder->setFocusPolicy(Qt::NoFocus);
    ui->shageLabel->hide();
    mesBox->hide();
    ui->radioBtnEn->setChecked(true);
    ui->listWidget->setCurrentRow(0);

    connect(&RFIDTimer,SIGNAL(timeout()),this,SLOT(RFIDTimeout()));

    connect(ui->listWidget, SIGNAL(currentTextChanged(QString)),
            this, SLOT(slotListChange(QString)));

    connect(mesBox, SIGNAL(signalUpdateDate(QVariantMap)),
            this, SLOT(saveDataToTable(QVariantMap)));
    connect(mesBox, SIGNAL(signalCancel()), this, SLOT(slotCloseMesBox()));


}

CodeEdit::~CodeEdit()
{
    delete ui;
}

void CodeEdit::init()
{
    QString portName = "ttyUSB0";   //获取串口
    if(Station.mid(0,5) == "VM010")
    {
        qDebug()<<"stationIsVM010";
    }
    else
    {
        portName = "ttymxc3";   //获取串口名
    }

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
    qDebug()<<"writeComFunc"<<buf;
//    len = buf.size();
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
//    sendBuf[0] = 16;
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
//接收串口信息信号槽
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
                    qDebug()<<"EKSrev"<<revRsDataBuf.size()<<"**"<<revRsDataBuf;
                    qDebug() << "rev cmd:"<<revRsDataBuf.mid(7,8);
                    EKSBuff = revRsDataBuf.mid(10,5);
                    if(revRsDataBuf.mid(7,3) == "RD*")
                    {
                        ui->label_key_read->setText(tr("红色"));
                        judgeKey(redKey);
                    }
                    else if(revRsDataBuf.mid(7,3) == "BK*")
                    {
                        ui->label_key_read->setText(tr("黑色"));
                        judgeKey(blackKey);
                    }
                    else if(revRsDataBuf.mid(7,3) == "GN*")
                    {
                        ui->label_key_read->setText(tr("绿色"));
                        judgeKey(greenKey);
                    }
                    ui->lineEdit_EKS->setText(EKSBuff);
                    revRsDataBuf.clear();
                    sendTempBuf[0] = 0x10;
                    comSendFunc(sendTempBuf,1);
                }
            }
            else if(revRsDataBuf.mid(1,2) == "RF")
            {
                qDebug()<<"EKSrev2"<<revRsDataBuf.size()<<"*"<<revRsDataBuf<<revRsDataBuf.toHex();
                if(((unsigned char)revRsDataBuf[0]+3) == revRsDataBuf.size())
                {
                    qDebug()<<"EKSrev1"<<revRsDataBuf.size()<<"*"<<revRsDataBuf<<revRsDataBuf.toHex();
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
                        EKSBuff = "";
                        judgeKey(noKey);
                    }
                    else
                    {
                        qDebug()<<"key Write fail!"<<(unsigned char)revRsDataBuf[6];
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
                qDebug()<<"rec eks error"<<revRsDataBuf.toHex();
            }
        }
    }
}

/*
 * 没钥匙 0
 * 红钥匙 1
 * 绿钥匙 2
 * 黑钥匙 3
*/
void CodeEdit::judgeKey(keys key)
{
    if(tmpKey != key)
    {
        qDebug()<<"key change to "<<key;
        //        if(tmpKey != noKey)
        //            emit sendLogin(false);
        tmpKey = key;
        emit saveEKSStatus();
        emit sendKey(tmpKey);
    }
}

/******************************************************************/
//EKS 写-红
/******************************************************************/
void CodeEdit::on_pushButton_red_clicked()
{
    isWrite = true;
    QByteArray sendTempBuf;
    sendTempBuf[0] = 0x02;
    comSendFunc(sendTempBuf,1);
    firstFlag = true;
    writeFlag = true;
//    writeBuf = "EKS-RD**";
    writeBuf = "RD*";
    writeBuf.append(ui->lineEdit_EKS->text());
}

/******************************************************************/
//EKS 写-红
/******************************************************************/
void CodeEdit::on_pushButton_green_clicked()
{
    isWrite = true;
    QByteArray sendTempBuf;
    sendTempBuf[0] = 0x02;
    comSendFunc(sendTempBuf,1);
    firstFlag = true;
    writeFlag = true;
//    writeBuf = "EKS-GN**";
    writeBuf = "GN*";
    writeBuf.append(ui->lineEdit_EKS->text());
}

/******************************************************************/
//EKS 写-黑
/******************************************************************/
void CodeEdit::on_pushButton_black_clicked()
{
    isWrite = true;
    QByteArray sendTempBuf;
    sendTempBuf[0] = 0x02;
    comSendFunc(sendTempBuf,1);
    firstFlag = true;
    writeFlag = true;
//    writeBuf = "EKS-BK**";
    writeBuf = "BK*";
    writeBuf.append(ui->lineEdit_EKS->text());
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
    qDebug() << "EKS send" <<len<<"**"<<send_Buf.toHex()<<send_Buf;
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
#if 0
QString CodeEdit::hexConvertToStr(QByteArray data)
{
    int hByte,lByte,byte;
    QString strData = "";
    QByteArray strTemp = "";
    for(int i=0;i<data.size();i+=2)
    {
        hByte = (data.mid(i,1).toInt())<<4;
        lByte = data.mid(i+1,1).toInt();
        byte = hByte + lByte;
        strTemp[0] = byte;
        strData.append(strTemp.mid(0,1));
    }
    return strData;
}
#endif

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

void CodeEdit::slotListChange(QString item)
{
    indexItem = item ;
    int i = ui->tableEncoder->rowCount()-1;
    for(int row=i;row>=0;row--)
    {
        ui->tableEncoder->removeRow(row);
    }
    foreach (QVariant single, map[item].toList())
    {
        QVariantMap singleMap = single.toMap();
        QVariantList stepList;
        stepList.append(singleMap);
        int row = ui->tableEncoder->rowCount();
        ui->tableEncoder->setRowCount(row + 1);//总行数增加1
        ui->tableEncoder->setRowHeight(row,43);//行

        if(item.contains("IO"))
        {
            ui->tableEncoder->setItem(row,0,new QTableWidgetItem(singleMap["IOPOSITION"].toString()));
        }
        else if(item.contains("ENCODER"))
        {
            ui->tableEncoder->setItem(row,0,new QTableWidgetItem(singleMap["YENCODER"].toString()));
            ui->tableEncoder->setItem(row,1,new QTableWidgetItem(singleMap["YENTol"].toString()));
            ui->tableEncoder->setItem(row,2,new QTableWidgetItem(singleMap["AENCODER"].toString()));
            ui->tableEncoder->setItem(row,3,new QTableWidgetItem(singleMap["AENTol"].toString()));
            ui->tableEncoder->setItem(row,4,new QTableWidgetItem(singleMap["OENCODER"].toString()));
            ui->tableEncoder->setItem(row,5,new QTableWidgetItem(singleMap["OENTol"].toString()));
        }
    }
}

void CodeEdit::initListWidget()
{
//    ui->listWidget->clear();
    for(int i = 0;i<map.keys().size();i++)
    {
        QString item = map.keys().at(i);
        if(item.compare("errorCode")   != 0 && item.compare("ENCODERFLAG=true") != 0 &&
           item.compare("PHEVENCODER") != 0 && item.compare("PHEVIOPOSITION") != 0 &&
           item.compare("BEVENCODER")  != 0 && item.compare("BEVIOPOSITION") != 0)
        {
            ui->listWidget->addItem(new QListWidgetItem(item));
        }
    }

    for(int row = 0; row<ui->listWidget->count(); row++)
    {
        if(ui->listWidget->item(row)->text().contains("ENCODER"))
        {
            ui->listWidget->item(row)->setHidden(false);
        }
        else if(ui->listWidget->item(row)->text().contains("IO"))
        {
            ui->listWidget->item(row)->setHidden(true);
        }

    }
}

//初始化表格信息
void CodeEdit::initTableWidget()
{
    ui->tableEncoder->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableEncoder->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableEncoder->setColumnCount(6);
    QStringList header ;
    header << "角度" <<"公差"<< "Y轴"<<"公差"<< "NULL" << "公差" ;

    ui->tableEncoder->setHorizontalHeaderLabels(header);
    ui->tableEncoder->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    ParseJson * parsejson = new ParseJson;
    map = parsejson->parseJsonEncoder();
    delete parsejson;


    foreach (QVariant single, map["PHEVENCODER"].toList())
    {
        QVariantMap singleMap = single.toMap();
        QVariantList stepList;
        stepList.append(singleMap);
        int row = ui->tableEncoder->rowCount();
        ui->tableEncoder->setRowCount(row + 1);//总行数增加1
        ui->tableEncoder->setRowHeight(row,43);//行

        ui->tableEncoder->setItem(row,0,new QTableWidgetItem(singleMap["YENCODER"].toString()));
        ui->tableEncoder->setItem(row,1,new QTableWidgetItem(singleMap["YENTol"].toString()));
        ui->tableEncoder->setItem(row,2,new QTableWidgetItem(singleMap["AENCODER"].toString()));
        ui->tableEncoder->setItem(row,3,new QTableWidgetItem(singleMap["AENTol"].toString()));
        ui->tableEncoder->setItem(row,4,new QTableWidgetItem(singleMap["OENCODER"].toString()));
        ui->tableEncoder->setItem(row,5,new QTableWidgetItem(singleMap["OENTol"].toString()));

        qDebug()<< "read"<<singleMap["AENCODER"].toString()<< singleMap["AENTol"].toString()
                <<singleMap["OENCODER"].toString() << singleMap["OENTol"].toString()
                <<singleMap["YENCODER"].toString() << singleMap["YENTol"].toString();
    }

    //上下居中 & 左右居中
    ui->tableEncoder->verticalHeader()
            ->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    //表头样式表
    ui->tableEncoder->horizontalHeader()
            ->setStyleSheet("QHeaderView::section {background-color: rgb(51, 153, 255);"
                            "color: rgb(248, 248, 255);border: 0px; font:14pt}");
    //序号列样式表
    ui->tableEncoder->verticalHeader()
            ->setStyleSheet("QHeaderView::section{border-bottom:1px groove #bdac96;"
                            "border-top:0px;border-left:0px;border-right:1px groove #e9dbc4;"
                            "background-color: rgb(250, 250, 250);font: 12pt}");
    //固定第一行&第一列宽度
    ui->tableEncoder->horizontalHeader()->setFixedHeight(43);
    ui->tableEncoder->verticalHeader()->setFixedWidth(50);
}

//Encoder
void CodeEdit::on_btnEncoder_clicked()
{
    //切换stackewidget为表格页
    ui->stackedWidget_left->setCurrentIndex(1);
}

//添加
void CodeEdit::on_btnAdd_clicked()
{
    //获取当前行
    indexRow = ui->tableEncoder->currentRow();

    //添加参数当前行、listwidget当前项、编辑类型为添加
    setMap.clear();
    setMap.insert("row",indexRow+1) ;
    setMap.insert("item",indexItem) ;
    setMap.insert("type","add");

    //显示修改弹框，并传参数
    showShageWidget();
    mesBox->show();
    mesBox->setParameter(setMap);
}

//插入
void CodeEdit::on_btnInsert_clicked()
{
    indexRow = ui->tableEncoder->currentRow();
    setMap.clear();
    setMap.insert("row",indexRow) ;
    setMap.insert("item",indexItem) ;
    setMap.insert("type","insert");

    //如果没有选中项，则插入无效
    if(ui->tableEncoder->selectedItems().count() == 0 )
    {
        return;
    }
    else {
        mesBox->setParameter(setMap);
        showShageWidget();
        mesBox->show();
    }

}

//编辑
void CodeEdit::on_btnEdit_clicked()
{
    //点击编辑，编辑弹框中默认显示为表格当前数据

    indexRow = ui->tableEncoder->currentRow();
    if(ui->tableEncoder->selectedItems().count() == 0)
    {
        return;
    }
    else {
        setMap.clear();
        setMap.insert("row" ,indexRow) ;
        setMap.insert("item",indexItem) ;
        setMap.insert("type","edit");
        if(indexItem.contains("ENCODER"))
        {
            setMap.insert("YEN" ,ui->tableEncoder->item(indexRow,0)->text());
            setMap.insert("YTol",ui->tableEncoder->item(indexRow,1)->text());
            setMap.insert("AEN" ,ui->tableEncoder->item(indexRow,2)->text());
            setMap.insert("ATol",ui->tableEncoder->item(indexRow,3)->text());
        }
        else if(indexItem.contains("IO"))
        {
            setMap.insert("posIO" ,ui->tableEncoder->item(indexRow,0)->text());
        }

        mesBox->setParameter(setMap);
        showShageWidget();
        mesBox->show();
    }

}

//删除
void CodeEdit::on_btnDelete_clicked()
{
    //如果没有所选行，不执行删除，不加判断可能会导致闪退
    indexRow = ui->tableEncoder->currentRow();
    if(ui->tableEncoder->selectedItems().count() == 0)
    {
        return;
    }
    else {
        qDebug()<<"row" << indexRow;
        ui->tableEncoder->removeRow(indexRow);
    }
}

/*
 * json框架
 *
 * QVariantMap
 * {
 * QVariantList[QVariantMap][QVariantMap][QVariantMap]
 * }
 */
void CodeEdit::on_btnSave_clicked()
{
    int countRow = ui->tableEncoder->rowCount();

    QVariantList list;

    //循环读取表格数据
    for(int row=0;row<countRow;row++)
    {
        if(indexItem.contains("ENCODER"))
        {
            QVariantMap mapson;
            mapson.insert("boltID",row);
            mapson.insert("YENCODER",ui->tableEncoder->item(row,0)->text());
            mapson.insert("YENTol",ui->tableEncoder->item(row,1)->text());
            mapson.insert("AENCODER",ui->tableEncoder->item(row,2)->text());
            mapson.insert("AENTol",ui->tableEncoder->item(row,3)->text());

        mapson.insert("OENCODER",ui->tableEncoder->item(row,4)->text());
        mapson.insert("OENTol",ui->tableEncoder->item(row,5)->text());

            list.append(mapson);
        }

        else if(indexItem.contains("IO"))
        {
            QVariantMap mapsonIO;
            mapsonIO.insert("boltID",row);
            mapsonIO.insert("IOPOSITION",ui->tableEncoder->item(row,0)->text());

            list.append(mapsonIO);
        }

    }

    //读取json的数据到map，删除map中对应键值的数据，写入新的数据
    map.remove(indexItem) ;
    map.insert("ENCODERFLAG=true",true);
    map.insert(indexItem,list);

    saveToJson(map);

}

//将messageBox修改后的值写入表格
void CodeEdit::saveDataToTable(QVariantMap mesMap)
{
    mesBox->setVisible(false);
    ui->shageLabel->hide();

    //将编辑弹框传过来的数据写入表格
    int row = mesMap.value("row").toInt();

    QString editType = mesMap.value("type").toString();

    if(editType == "listWidget")
    {
        QString item = mesMap.value("item").toString();
        ui->listWidget->addItem(new QListWidgetItem(item));
        map.insert(item,"NULL");
        saveToJson(map);
    }
    else
    {
        //如果是添加或插入要增加行。
        if(editType == "add")
        {
            //在当前行下方添加为添加
            ui->tableEncoder->insertRow(indexRow+1);
        }
        else if(editType == "insert")
        {
            //在当前行上方添加为插入
            ui->tableEncoder->insertRow(indexRow);
        }

        if(indexItem.contains("ENCODER"))
        {
            ui->tableEncoder->setItem(row,0,new QTableWidgetItem(mesMap.value("YEN").toString()));
            ui->tableEncoder->setItem(row,1,new QTableWidgetItem(mesMap.value("YTol").toString()));
            ui->tableEncoder->setItem(row,2,new QTableWidgetItem(mesMap.value("AEN").toString()));
            ui->tableEncoder->setItem(row,3,new QTableWidgetItem(mesMap.value("ATol").toString()));
            ui->tableEncoder->setItem(row,4,new QTableWidgetItem(mesMap.value("OEN").toString()));
            ui->tableEncoder->setItem(row,5,new QTableWidgetItem(mesMap.value("OTol").toString()));
        }

        else if(indexItem.contains("IO"))
        {
            ui->tableEncoder->setItem(row,0,new QTableWidgetItem(mesMap.value("posIO").toString()));
            qDebug()<<mesMap.value("posIO").toString()<<"......................................";
        }

    }

}

//遮罩层
void
CodeEdit::showShageWidget()
{
    shageGOE->setOpacity(0.5);
    ui->shageLabel->setGraphicsEffect(shageGOE);
    ui->shageLabel->show();
    ui->shageLabel->setGeometry(0,0,1366,768);
}

//切换stackwidget
void CodeEdit::on_pushButton_rfid_clicked()
{
    ui->stackedWidget_left->setCurrentIndex(0);
}

//关闭编辑弹框
void CodeEdit::slotCloseMesBox()
{
    mesBox->setVisible(false);
    ui->shageLabel->hide();
}

//筛选ENCODER配置项
void CodeEdit::on_radioBtnEn_clicked()
{
    ui->tableEncoder->clear();
    indexItem = "PHEVENCODER";
    ui->listWidget->setCurrentRow(0);
    for(int row = 0; row<ui->listWidget->count(); row++)
    {
        if(ui->listWidget->item(row)->text().contains("ENCODER"))
        {
            ui->listWidget->item(row)->setHidden(false);
        }
        else if(ui->listWidget->item(row)->text().contains("IO"))
        {
            ui->listWidget->item(row)->setHidden(true);
        }

    }

    ui->tableEncoder->setColumnCount(6);
    QStringList header ;
    header << "Y轴" <<"公差"<< "角度"<<"公差"<< "NULL" << "公差" ;

    ui->tableEncoder->setHorizontalHeaderLabels(header);

    foreach (QVariant single, map["PHEVENCODER"].toList())
    {
        QVariantMap singleMap = single.toMap();
        QVariantList stepList;
        stepList.append(singleMap);
        int row = ui->tableEncoder->rowCount();
        ui->tableEncoder->setRowCount(row + 1);//总行数增加1
        ui->tableEncoder->setRowHeight(row,43);//行

        ui->tableEncoder->setItem(row,0,new QTableWidgetItem(singleMap["YENCODER"].toString()));
        ui->tableEncoder->setItem(row,1,new QTableWidgetItem(singleMap["YENTol"].toString()));
        ui->tableEncoder->setItem(row,2,new QTableWidgetItem(singleMap["AENCODER"].toString()));
        ui->tableEncoder->setItem(row,3,new QTableWidgetItem(singleMap["AENTol"].toString()));
        ui->tableEncoder->setItem(row,4,new QTableWidgetItem(singleMap["OENCODER"].toString()));
        ui->tableEncoder->setItem(row,5,new QTableWidgetItem(singleMap["OENTol"].toString()));

    }
}

//筛选IO配置项
void CodeEdit::on_radioBtnIO_clicked()
{
    ui->tableEncoder->clear();

    indexItem = "PHEVIOPOSITION";
    for(int row = 0; row<ui->listWidget->count(); row++)
    {
        if(ui->listWidget->item(row)->text().contains("ENCODER"))
        {
            ui->listWidget->item(row)->setHidden(true);
        }
        else
        {
            ui->listWidget->item(row)->setHidden(false);
        }
    }

    ui->tableEncoder->setColumnCount(1);
    ui->tableEncoder->setRowCount(0);
    ui->tableEncoder->setHorizontalHeaderItem(0,new QTableWidgetItem("位置号"));

    foreach (QVariant single, map["PHEVIOPOSITION"].toList())
    {
        QVariantMap singleMap = single.toMap();
        QVariantList stepList;
        stepList.append(singleMap);
        int row = ui->tableEncoder->rowCount();
        ui->tableEncoder->setRowCount(row + 1);//总行数增加1
        ui->tableEncoder->setRowHeight(row,43);//行

        ui->tableEncoder->setItem(row,0,new QTableWidgetItem(singleMap["IOPOSITION"].toString()));
    }
    //虽然第0行被隐藏了，但是你大爷依然是你大爷，它依然是第0行
    ui->listWidget->setCurrentRow(1);
}

//添加listWidgetItem
void CodeEdit::on_btnAddList_clicked()
{
    //添加参数当前行、listwidget当前项、编辑类型为添加
    setMap.clear();
    setMap.insert("type","listWidget");

    //显示修改弹框，并传参数
    showShageWidget();
    mesBox->show();
    mesBox->setParameter(setMap);

}

//删除listWidgetItem
void CodeEdit::on_btnDeleteList_clicked()
{
    //判断ListWidgetItem的item是否为空
    if(ui->listWidget->count()== 0 || ui->listWidget->currentRow() < 3)
    {
        ui->btnDeleteList->setEnabled(true);
    }
    else {
        QString str = ui->listWidget->currentItem()->text();

        ui->listWidget->takeItem(ui->listWidget->currentRow());
        map.remove(str);
        saveToJson(map);
    }

}

void CodeEdit::saveToJson(QVariantMap saveMap)
{
    //序列化json
    bool ok;
    QJson::Serializer serializer;
    QByteArray json = serializer.serialize(saveMap, &ok);

    //写入文件
    if (ok)
    {
        QFile stepJson("/encoder.json");
        if(stepJson.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            QTextStream text_stream(&stepJson);
            text_stream << json << endl;
            stepJson.flush();
            stepJson.close();
        }
        else
            qDebug()<<"encoder.json write open fail";
    }
    else
        qCritical() << "Something went wrong:" << serializer.errorMessage();

}
