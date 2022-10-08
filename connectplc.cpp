#include "connectplc.h"
#include "GlobalVarible.h"

ConnectPLC::ConnectPLC(QObject *parent) :
    QObject(parent)
{
    this->moveToThread(&plc_thread);
    plc_thread.start();
    channel_Num = 0;
    vinCode = "";
}

void ConnectPLC::PLCStart()
{
    qDebug() << "connect plc thread start<><><><><>channel_Num::"<<channel_Num;
    PLC_IP = "";
    QVariantMap tempMap = parsejson->parseJson();
    if(tempMap["errorCode"] != 1)
    {
        PLC_IP      = tempMap["plcIP"].toString();
    }

    readMesTemp = "";//记录本次读到PLC的数据

    //有配置就取配置，没配置就配置一个给个默认值
    if(QSettings("/config.ini", QSettings::IniFormat).contains("baseinfo/DBWRLen")){
        DBWRLen = 100 + QSettings("/config.ini", QSettings::IniFormat).value("baseinfo/DBWRLen").toInt();
    }else{
        DBWRLen = 40;//读取多少(B)长度的DBW
        QSettings("/config.ini", QSettings::IniFormat).setValue("baseinfo/DBWRLen",DBWRLen);
    }

    isRepeatWritePLC = false;//是重复写入PLC
    countRepeatWrite = 0;//重复写入的次数
    stationAddr = 0;
    setUpCommFlag = 2;
    sendCmd = 0;
    codeNum = 0;
    scanCount = 0;
    length = 0;
    trayIsAction = 0;
    trayBindCount = 0;
    aliveCount = 1;
    tmpPackNum = 0;
    packUplodeStatus1 = true;
    packUplodeStatus2 = true;
    isReadRfidFlag = true;
    plcAckFlag = true;
    sendNOStationFlag = false;
    stationSkip = true;
    glueTightClearFlag = false;
    packUplodeTemp1 = "1";
    packUplodeTemp2 = "1";
    plcRecvFlag = true;
    plcLinkFlag = false;
    startCmdBuf1 = "\x03**\x16\x11\xE0***\x2E*\xC1\x02\x01*\xC2\x02\x03*\xC0\x01\x09";     //set up communication
    if(PLC_IP == "172.168.2.214")
    {
        qDebug()<<"other plc";
        startCmdBuf1 = "\x03**\x16\x11\xE0***\x01*\xC1\x02\x01*\xC2\x02\x03\x02\xC0\x01\x09";
    }
//    startCmdBuf1 = "\x03**\x16\x11\xE0***\x01*\xC1\x02\x01*\xC2\x02\x03\x02\xC0\x01\x09";
    startCmdBuf2 = "\x03**\x19\x02\xF0\x80\x32\x01**\xFF\xFF*\x08**\xF0**\x03*\x03\x01*";  //set up communication
//        startCmdBuf1 = "\x03**\x16\x11\xE0***\x01*\xC1\x02\x10*\xC2\x02\x03\x02\xC0\x01\x0A";
//        startCmdBuf2 = "\x03**\x19\x02\xF0\x80\x32\x01**\xCC\xC1*\x08**\xF0**\x01*\x01\x03\xC0";  //set up communication
    readCmdBuf   = "";
    writeCmdBuf  = "";
    clearPLCFlag = false;
    sendCount = 0;
    readAddr = 0;
    writeAddr = 0;
//    isOneCount = 0;
    trayfinishBoundCount = 0;
    nextTrayflag = false;
    trayProtectFlag = false;
    reqProductTypeAddr = 0;
    reqBoundAddr = 0;
    conditionType = 0;
    packStatusTrueFlag = false;
//    readPLCFlag = false;     //开始读取PLC有效标志位
    statusValidFlag = false; //读到PLC有效标志位
    statusValidFlagTemp = false;
    writePLCFlag = false;

    writeProductTypeFlag = true;
    writeProductFinishFlag = true;
    reqBindingSNFlag = false;
    reqProductTypeFlag = false;
    reqProductTrayFlag = false;
    getProductTypeFlag = false;
    fixtureHoldFlag = false;
    getGlueProductTypeFlag = false;
    getDistroyStatus = false;
//    onlyRunFirst = true;
    cleanAllDWFlag = true;
    reqTightenPositionFlag = false;
    getTightenPositionFlag = false;
    getGluePositionFlag = false;
    getGlueBindSuccess = false;
    reqRobotTightenReadyFlag = false;
    getRobotTightenReadyFlag = false;
    reqBoxPositionFlag = false;
    dutouStatusPosition = false;
    getBoxPositionFlag = false;
    reqBoxFinishFlag = false;
    getBoxFinishFlag = false;
    getStationFinishFlag = false;
    getTightenFinishFlag = false;
    boxErrorFlag = false;
    getTightErrorFlag = false;
    getStationErrorFinishFlag = false;
    reqDutouPositionFlag = false;
    getDutoupositionFlag = false;
    reqDutouRecordDataFlag = false;
    getDutouRecordDataFlag = false;
    getDutouRecordFinishFlag = false;
    getBindingSNFlag = false;
    ackBoundPackFlag = false;
    ackProductTypeFlag = false;
    getUpdownBoxPositionFlag = false;
    getScanBarcodeFlag = false;
    getScanBarcodeVM010Flag = false;
    getRFIDCodeVM010Flag = false;
    getUpdownBoxRecordDataFlag = false;
    getTrayType = false;
    tellTrayReadyFlag = false;
    getTrayReadyFlag = false;
    reqModuleBoxNumFlag = false;
    getModuleBoxNumFlag = false;
    getRfidUpDowmFlag = false;
    reqModuleRecordDataFlag = false;
    getModuleRecordDataFlag = false;
    getModuleRecordFinishFlag = false;
    tellRollerBedReadyFlag = false;
    getRollerBedReadyFlag = false;
    reqModuleTypeFlag = false;
    getModuleTypeFlag = false;
    tellModuleMoveFinishFlag = false;
    getModuleMoveFinishFlag = false;
    reqPLCMoveTrayFlag = false;
    getPLCMoveTrayFlag = false;

    startWorkFlag = false;
    startWorkWriteFlag = false;
    pack1StatusFlag = false;
/**********************************************/
    pack1StatusWriteFlag = true;
/**********************************************/
    PLCWorkAllStep = 0;
    tightenCount = 0;
    boxCount = 0;
    dutouCount = 0;
    moduleCount = 0;
    moduleBindCount = 0;
//    PLC_StepNum = 0;
    stationFinishAddr = 0;
    trayFinishAddr = 0;
    mannulNum = 99;
    moveTrayStep = 99;
    pack1StatusNum = 0;
    startWorkNum = 0;
    glueStartWorkNum = 0;
    glueStartWorkAddr = 0;
    glueTightPackStatusAddr = 0;
    glueTightProductTypeAddr = 0;
    writeToPlcPackSNAddr = 0;
    writeToPlcPack1SNAddr = 0;
    writeToPlcPack2SNAddr = 0;
    regExpVm1 = "";
    regExpVm2 = "";
    lingjianTemp = "";
    VariantDiff = "";
    dataDiff = "";
    dataDiff1 = "";
    shortFlag = false;
    longFlag = false;
    glueStartWorkFlag = false;
    glueTightPackStatusFlag = false;
    glueTightProductTypeFlag = false;
    tightenTimes = 0;
    W500EnableFlag = false;
    curStepNum = 0;
    strBoxCode = "";
    strBoxCodeTemp = "";
    strBoxCodeFlag = false;
    strBoxCoderegExpFlag = false;
    scanResult = 1;
    tighten_Num = 0;
    box_Num = 0;
    dutou_Num = 0;
    module_Num = 0;
    module_typeNum = 0;
    moduleMessage = "";
    readRFIDResult = 0;
    readRFIDCount = 0;

    PLCPackType = "0";//product type
    PLCPackAGVSN = "";
    PLCPack1SN = "";
    PLCPack2SN = "";
    PLCPack1Status = "";
    PLCPack2Status = "";
    dotouPositionNum1 = "";
    dotouPositionNum2 = "";

    w500_IP = "";
    ackCode = 0;
    RFIDCode = "";
    updataTraySN = "";
    updataTrayTemp = "";
    RFIDCodeTemp = "";
    RFIDCodeTemp1 = "";
    pack12Num = 0;
    pack1Result = 0;
    pack2Result = 0;
    countInitValue = 0;
    countValue = 0;
    workSecondePackFlag = false;
    PLC_AllStep = 0;
    tightenTestFlag = false;
    moduleBuf = "";
    plcStopFlag = true;
    parsejson = new ParseJson;
    stepInit();
//    readPLCFlag = true;
    if(PLC_IP != "")
    {
        tcpSocket = new QTcpSocket;
//        connect(tcpSocket, SIGNAL(readyRead()),this, SLOT(revPLCData()));
        connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(disConnectDO()));
        plcConnects();
        QTimer::singleShot(2000,this,SLOT(timerFuncXS()));
    }
    else
    {
        qDebug()<<"PLC IP Addr NULL";
    }

    if(W500EnableFlag)
    {
        if(channel_Num == 1)
        {
            READW500 *W500_1 = new READW500;
            W500_1->W500IP = w500_IP;
            QTimer::singleShot(0,W500_1,SLOT(W500Start()));
            QObject::connect(W500_1,SIGNAL(sendTighteningResult1(int,int)),this,SLOT(revCmdFunc(int,int)),Qt::AutoConnection);
            QObject::connect(this,SIGNAL(startW500CMD(int,QString)),W500_1,SLOT(revW500StartCmd(int,QString)),Qt::AutoConnection);
        }
        else if(channel_Num == 2)
        {
            READW500 *W500_2 = new READW500;
            int intIP_Right=0;
            QStringList list = w500_IP.split(".",QString::SkipEmptyParts);
            if(list.size() > 1)
            {
                QString strIP_Right = list[list.size()-1];
                intIP_Right = list[list.size()-1].toInt()+2;
                w500_IP.remove(w500_IP.size()-strIP_Right.size(),strIP_Right.size());
            }
            else
            {
                w500_IP = "";
            }
            W500_2->W500IP = w500_IP.append(QString::number(intIP_Right));
            QTimer::singleShot(0,W500_2,SLOT(W500Start()));
            QObject::connect(W500_2,SIGNAL(sendTighteningResult2(int,int)),this,SLOT(revCmdFunc(int,int)),Qt::AutoConnection);
            QObject::connect(this,SIGNAL(startW500CMD(int,QString)),W500_2,SLOT(revW500StartCmd(int,QString)),Qt::AutoConnection);
        }
    }
}
/**************************************
 * step parameter init
 *************************************/
void ConnectPLC::stepInit()
{
    QVariantMap tempMap = parsejson->parseJson();
    if(tempMap["errorCode"] != 1)
    {
        int i=0;
        stepParaPLCBufInit(tempMap);
        PLC_IP      = tempMap["plcIP"].toString();
        stationAddr = tempMap["plcDB"].toInt();
        if(channel_Num == 2)
        {
            stationAddr += 1;
            qDebug()<<"stationAddr"<<stationAddr;
        }
        foreach (QVariant tempStep, PLCstepList)
        {
            PLCStepMap = tempStep.toMap();
//            qDebug()<<"this is tempstep"<<parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString());
            if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 2)//tightening type
            {
                tighten_Num = PLCStepMap["tightenTimes"].toInt();
                tighten_Num = readPackStatus(tighten_Num,100);//tighten_Num%100
                tightenTestFlag = true;
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 9)//dutou type
            {
                dutou_Num = PLCStepMap["plugTimes"].toInt();
                dutou_Num = readPackStatus(dutou_Num,100);
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 26)//box type
            {
                box_Num = PLCStepMap["boxNums"].toInt();
                if(tightenTestFlag)
                {
                    box_Num = box_Num%100;
                }
                else
                {
                    box_Num = readPackStatus(box_Num,100);
                }
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 16)//module type
            {
                module_Num = PLCStepMap["moduleNums"].toInt();
                moduleMessage = PLCStepMap["moduleNums"].toString();
                module_Num = readPackStatus(module_Num,10000);
                //module_typeNum = readPackStatus(module_Num,10);
                moduleBuf = QByteArray::number(module_Num);
                module_typeNum=moduleBuf.mid(3,1).toInt();
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 10)//request Record Data type
            {
                W500EnableFlag = true;
                w500_IP = PLCStepMap["w500IP"].toString();
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 4)//request move tray
            {
                stationFinishAddr = PLCStepMap["writeAdd"].toInt();
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 30)//request move tray
            {
                moveTrayStep = i;
                trayFinishAddr = PLCStepMap["writeAdd"].toInt();
                if(PLCWorkAllStep == 0)
                {
                    PLCWorkAllStep = i;
                }
                qDebug()<<"this is tray finish"<<trayFinishAddr;
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 8)//mannul and auto mode
            {
                mannulNum = i; //mannul mode position number
                if(i > 0)
                {
                    PLCWorkAllStep = i;
                }
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 50)//start work
            {
                startWorkNum = i;
                if(PLCWorkAllStep == 0)
                {
                    PLCWorkAllStep = i;
                }
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 201)//glue start work
            {
                glueStartWorkNum = i;
                glueStartWorkAddr = PLCStepMap["writeAdd"].toInt();
                glueStartWorkFlag = true;
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 202)//涂胶拧紧Pack1状态
            {
                glueTightPackStatusAddr = PLCStepMap["writeAdd"].toInt();
                glueTightPackStatusFlag = true;
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 203)//涂胶拧紧请求产品型号
            {
                glueTightProductTypeAddr = PLCStepMap["writeAdd"].toInt();
                glueTightProductTypeFlag = true;
            }

            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 204)//Pack数据
            {
                writeToPlcPackSNAddr = PLCStepMap["writeAdd"].toInt();
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 205)//Pack1SN
            {
                writeToPlcPack1SNAddr = PLCStepMap["writeAdd"].toInt();
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 206)//Pack2SN
            {
                writeToPlcPack2SNAddr = PLCStepMap["writeAdd"].toInt();
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 51)//pack1 status
            {
                pack1StatusNum = i;
                if(PLCWorkAllStep == 0)
                {
                    PLCWorkAllStep = i;
                }
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 27) //scan regexp
            {
                regExpVm1 = PLCStepMap["regexpVM1"].toString();
                regExpVm2 = PLCStepMap["regexpVM2"].toString();
                lingjianTemp = PLCStepMap["lingjian"].toString();
                GscanRegExpCode = lingjianTemp;
                qDebug()<<"GscanRegExpCode"<<GscanRegExpCode;
                VariantDiff = PLCStepMap["VariantDiff"].toString();
                dataDiff = PLCStepMap["dataDiff"].toString();
                dataDiff1 = PLCStepMap["dataDiff1"].toString();
            }
            i++;
        }
    }
    else
    {
        qDebug()<<"PLC control parsejson in Null";
    }
    qDebug()<<"*****PLC***** stationAddr:"<<stationAddr<<"PLC_AllStep"<<PLC_AllStep<<PLCWorkAllStep<<W500EnableFlag<<PLC_IP;
    qDebug()<<"*****PLC***** tighten_Num:"<<tighten_Num<<box_Num<<dutou_Num<<module_Num<<moduleBuf;

    statusValidFlag = false;
    tightenCount = 0;
    dutouCount = 0;
    boxCount = 0;
    moduleCount = 0;
    moduleBindCount = 0;
    workSecondePackFlag = false;
}
/**************************************
 *
 *
 *************************************/
void ConnectPLC::stepParaPLCBufInit(QVariantMap Step)
{
    int num=0,n=0;
    QVariantMap stepMapTemp;
    PLCstepList  = Step["steps"].toList();//获取步骤配置文件里面的步骤数据
    num = PLCstepList.size();
    GStepListSize = PLCstepList.size();
    for(int i=0;i<num;i++)
    {
        stepMapTemp = PLCstepList[n].toMap();
        if(stepMapTemp["type"].toInt() == 0)//移除所有手动步骤("type": 0)
        {
            PLCstepList.removeAt(n);//remove manual step
        }
        else
        {
            n++;
        }
    }
    PLC_AllStep = PLCstepList.size();//有多少个PLC的步骤
}
/*******************************************************/
//nexo disconnect handle
/*******************************************************/
void ConnectPLC::disConnectDO()
{
    qDebug()<<"PLCDisConnect!";
//    plcConnects();
}
/*******************************************************/
//nexo bulid connect
/*******************************************************/
void ConnectPLC::plcConnects()
{
    setUpCommFlag = 2;
    tcpSocket->abort(); //取消已有的连接 不断开下次连接会发以前的数据
//    tcpSocket->close();
//    tcpSocket->disconnectFromHost();
    tcpSocket->connectToHost(PLC_IP,102); //连接到主机，这里从界面获取主机地址和端口号
    tcpSocket->waitForConnected(2000); //waitting 2000ms
    qDebug() << "plc connect waiting>>>>>>>>"<< tcpSocket->state();
    if(tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "plc connect success!!!!!!!!!!!!!";
#if DYTEST5
        //\x03**\x16\x11\xE0***\x2E*\xC1\x02\x01*\xC2\x02\x03*\xC0\x01\x09
        startCmdBuf1 = "\x03**\x16\x11\xE0***\x01*\xC1\x02\x01*\xC2\x02\x03\x02\xC0\x01\x09";
#endif
        ctlPLCHandle(startCmdBuf1,true);
//        plcLinkFlag = true;   //PLC connect   //20180620
//        emit sendPLCConnect(plcLinkFlag);
    }
    else
    {
        plcLinkFlag = false;   //PLC disconnect
        emit sendPLCConnect(plcLinkFlag);
        qDebug() << "plc connect fail" ;
    }
}
/*******************************************************/
//1000ms timer function
/*******************************************************/
void ConnectPLC::timerFuncXS()
{
    if(!plcRecvFlag)
    {
        qDebug()<<"plcRecvFlag==false";
    }
    if(plcLinkFlag && plcRecvFlag)//PLC的连接状态 && 是否在发给PLC数据（不管读还是写）有任意回复变true
    {
        plcRecvFlag = false;
        if(plcAckFlag)//给PLC写数据的时候变false，收到写命令的回复变true
        {
            qDebug()<<"testplcAckFlag";
            writeDataToPLCFunc();
        }
        if(writePLCFlag)
        {
            if(plcAckFlag)
            {
                plcAckFlag = false;
                qDebug()<<"testplcAckFlagwritePLCFlag";
                writePLCFunc(stationAddr,writeAddr,writeLen,strWriteBuf);//
            }
            else
            {
                qDebug()<<"there are not ff";
            }
        }
        else
        {
            readPLCFunc(stationAddr,readAddr,DBWRLen);//读PLC的DB块数据
        }
    }

//    qDebug()<<"aliveCount:"<<aliveCount<<tcpSocket;
    if(aliveCount > 50)
    {
        qDebug()<<"tcpSocketError:"<<tcpSocket->isOpen()<<tcpSocket<<tcpSocket->error()<<tcpSocket->errorString();
        plcLinkFlag = false;
        aliveCount = 0;
//        if(tcpSocket->error() == QAbstractSocket::SocketTimeoutError){//只是超时触发中断连接槽自动连接
//            tcpSocket->abort();
//        }else{//其他情况直接重新连接
//            plcConnects();
//        }
        writePLCFlag = false;

//        tcpSocket->abort();
//        qDebug()<<"tcpSocketIsOpen1:"<<tcpSocket->isOpen();
//        tcpSocket->abort();
//        qDebug()<<"tcpSocketIsOpen2:"<<tcpSocket->isOpen();
//        tcpSocket->abort();
//        qDebug()<<"tcpSocketIsOpen3:"<<tcpSocket->isOpen();
        plcConnects();
    }
    aliveCount ++ ;
    if( (tellTrayReadyFlag)||(tellRollerBedReadyFlag) )
    {
        readRFIDCount++;
        if(readRFIDCount > 250)
        {
            readRFIDCount = 0;
            revRFIDAck("0002", "");//timeout
        }
    }
    if(getScanBarcodeVM010Flag && getRFIDCodeVM010Flag)
    {
        getScanBarcodeVM010Flag = false;
        getRFIDCodeVM010Flag = false;
    }
    QTimer::singleShot(100,this,SLOT(timerFuncXS()));
}
/*******************************************************/
//read plc cmd
/*******************************************************/
void ConnectPLC::readPLCFunc(int station,int addr,int len)
{
    QByteArray tempBuf ="\x03**\x1F\x02\xF0\x80\x32\x01***C*\x0E**\x04\x01\x12\x0A\x10\x02*\x0A*\xC6\x84**\x50";
    for(int i=0;i<tempBuf.size();i++)
    {
        if(tempBuf[i]=='*')
        {
            tempBuf[i] = 0;
        }
    }
    qDebug()<<"read";
    if(plcLinkFlag)
    {
        tempBuf[12] = sendCmd;    //SN
        tempBuf[24] = len;        //read data length
        tempBuf[25] = station>>8; //DB station high byte
        tempBuf[26] = station;    //DB station low byte
        tempBuf[27] = 132;        //wr area  DB(132) M(133) I(129) Q(130)
        tempBuf[29] = (addr<<3)>>8;//read high address
        tempBuf[30] = addr<<3;    //read low address
        readCmdBuf = tempBuf;
        //        qDebug() << "**** read  plc******" <<station<<addr;
        ctlPLCHandle(readCmdBuf,false);
        sendCmd ++;
    }
}

/*******************************************************/
//write plc cmd
/*******************************************************/
void ConnectPLC::writePLCFunc(int station,int addr,int len,char *buf)
{
    QByteArray tempBuf ="\x03**\x24\x02\xF0\x80\x32\x01***C*\x0E*\x08\x05\x01\x12\x0A\x10\x06*\x01*\xC6\x84**\x50*\x04*\x20";//writeCmdBuf;
    int i;
    for(i=0;i<tempBuf.size();i++)
    {
        if(tempBuf[i]=='*')
        {
            tempBuf[i] = 0;
        }
    }
    if(plcLinkFlag)
    {
        tempBuf[12] = sendCmd; //SN
        tempBuf[16] = len + 4; //write data length
        tempBuf[22] = 2;       //data type
        tempBuf[23] = 0;       //data len high byte
        tempBuf[24] = len;     //data len low byte
        tempBuf[25] = station>>8;  //DB station high byte
        tempBuf[26] = station;     //DB station low byte
        tempBuf[27] = 132;     //wr area  DB(132) M(133) I(129) Q(130)
        tempBuf[29] = (addr<<3)>>8;//write high address(addr*8)
        tempBuf[30] = addr<<3; //write low address(addr*8)
        tempBuf[33] = (len<<3)>>8;//data len high byte(len*8)
        tempBuf[34] = len<<3;  //data len low byte(len*8)
        for(i=0;i<len;i++)
        {
            tempBuf[35+i] = buf[i];
            if(channel_Num == 1)
            {
                qDebug() << "*channel_Num == 1*write plc*" <<station<<addr<<len<<tempBuf.mid(35+i).toHex();
            }
            else
            {
                qDebug() << "*channel_Num == 2*write plc*" <<station<<addr<<len<<tempBuf.mid(35+i).toHex();
            }
        }
        //        tempBuf.append(buf.toAscii());//data
        tempBuf[3]  = tempBuf.size(); //cmd length
        writeCmdBuf = tempBuf;
        QPair<int,QPair<int,QByteArray> >taskPair;
        taskPair.first = addr;//写入PLC的起始DBW
        taskPair.second.first = len;//写入PLC的数据的长度
        taskPair.second.second = tempBuf.right(len).toHex();//记录写入PLC的数据内容
        if(!isRepeatWritePLC){
            //OP80 的 DBW150(托盘号) A350-1/-2 的 DBW160(拧紧数据)这两个数据写过去就会被清掉，所以不添加数据检测重发功能
            if(!(Station.mid(0,4) == "A350" && addr == 160) && !(Station == "OP80" && addr == 150)){
                taskList<<taskPair;//发送的数据插入任务列表
            }
        }
        writeLen = len;//写入PLC的数据的长度
        ctlPLCHandle(writeCmdBuf,false);
        sendCmd ++;
    }
}

/***********************************/
//send data to PLC  type = true(* invalue)
/*************************************/
void ConnectPLC::ctlPLCHandle(QByteArray sendBuf,bool type)
{
    int Tx_len=0,i;
    Tx_len = sendBuf.size();
    if(type)
    {
        for(i=0;i<Tx_len;i++)
        {
            if(sendBuf[i]=='*')
            {
                sendBuf[i] = 0;
            }
        }
    }
    //识别是在发PLC哪种类型的命令
    QByteArray sendData = sendBuf.right(writeLen).toHex();
    QByteArray sendWriteDataTemp;
    if(sendBuf.size() == 22){
            qDebug()<<"send first cmd:"<<sendBuf.toHex();//第一次握手
        }else if(sendBuf.size() == 25){
            qDebug()<<"send second cmd:"<<sendBuf.toHex();//第二次握手
        }else if(sendBuf.size() == 31){
        qDebug()<<"send read cmd:"<<sendBuf.toHex();//读指令
        }else if(sendBuf.size() == 35+writeLen){
            qDebug()<<"send write cmd:"<<sendData<<writeLen;//写指令
            sendWriteDataTemp = sendData;
        }else{
        sendWriteDataTemp = sendBuf.right(1*2).toHex();//1个值占2个db，4个byte
        qDebug()<<"outher sendBuf:"<<sendWriteDataTemp<<endl<<sendBuf.toHex();
        }
    tcpSocket->write(sendBuf,Tx_len);
    isRepeatWritePLC = false;
    if(!tcpSocket->waitForBytesWritten(1000)){
            qDebug()<<"send plc cmd faile:"<<tcpSocket->errorString();
        }else{
            if(tcpSocket->waitForReadyRead(1000)){
                //qDebug()<<"read plc data success";
                QString allData = QString(tcpSocket->readAll().toHex());
                readPLCData(allData);
            }else{
                qDebug()<<"read plc data faile:"<<tcpSocket->errorString();
            }
        }
}

/*************************************************/
//receive cmd function StepControl::sendTypeToPLC发过来的步骤信息
/************************************************/
void ConnectPLC::revCmdFunc(int condition_type, int result)
{
    switch (condition_type)
    {
    case 3:
        qDebug()<<"revCmdFunc=getTightenFinishFlag"<<GMI;
        tightenResult = result;
        tightenCount++;
        if(Station.contains("A350")){
            strWriteBufTemp[0] = (GMI>>8);
            strWriteBufTemp[1] = GMI;
            writePLCFunc(stationAddr,160,2,strWriteBufTemp);//
        }
        getTightenFinishFlag = true;
        packStatusTrueFlag = true;
        break;
    case 4:
        qDebug()<<"this is dutouCount add"<<dutouCount;
        dutouCount ++;
        getDutouRecordFinishFlag = true;
        statusValidFlag = true;
        break;
    case 5:
        break;
    case 6:
        getTightErrorFlag = true;
        statusValidFlag = true;
        break;
    case 30: //mannul station funcation
        if(moveTrayStep != 99)//默认是99，如果配置文件中有30（站完成请求PCL放行托盘）即取步骤列表的下标，意思是该工位配置步骤有“站完成请求PCL放行托盘”
        {
            if(Station.mid(0,4) != "OP80"){//非OP80处的“站完成请求PCL放行托盘”
                reqPLCMoveTrayFlag = true;//告诉PLC放行托盘
                PLCStepMap = PLCstepList[moveTrayStep].toMap();//提取“站完成请求PCL放行托盘”步骤的配置信息
                writeAddr = PLCStepMap["writeAdd"].toInt();//获取写入PLC的DBW
                statusValidFlag = false;
            }
        }
        qDebug()<<"$$$$$$$$$$$$$ receive 30 $$$$$$$$$$$$$$"<<writeAddr<<moveTrayStep;
        break;
    case 99:
        statusValidFlag = false;
        tightenCount = 0;
        dutouCount = 0;
        boxCount = 0;
        moduleCount = 0;
        moduleBindCount = 0;
        workSecondePackFlag = false;
        break;
    default:
        break;
    }
}
/*************************************************/
//write data to PLC
/************************************************/
void ConnectPLC::writeDataToPLCFunc()
{
//    qDebug() << "plc 5000ms connects"<<"stationAddr:"<<stationAddr<<"readAddr:"<<readAddr<<"writeAddr:"<<writeAddr;
//    qDebug() << "%%%%%%%writ flag%%%%%%%"<<getProductTypeFlag<<getTightenFinishFlag;
    if(!startWorkWriteFlag) //start work flag
    {
        if(startWorkNum > 0)
        {
            startWorkFlag = true;
            PLCStepMap = PLCstepList[startWorkNum].toMap();
            writeAddr = PLCStepMap["writeAdd"].toInt();
        }
        else
        {
            startWorkWriteFlag = true;
        }
    }
    else if(pack1StatusWriteFlag)//pack1 and pack2 status
    {
        if(pack1StatusNum > 0)//first write pack status
        {
            qDebug()<<"*pack1StatusWriteFlag*";
            pack1StatusFlag = true;
            PLCStepMap = PLCstepList[pack1StatusNum].toMap();
            writeAddr = PLCStepMap["writeAdd"].toInt();
        }
        else
        {
            pack1StatusWriteFlag = false;
        }
    }



    if(clearPLCFlag)//clear PLC writefalse data
    {
        qDebug()<<"*clearPLCFlag*";
        if(glueTightClearFlag)
        {
            glueTightClearFlag = false;
            strWriteBufTemp[0] = 0;
            strWriteBufTemp[1] = 0;
            writePLCFunc(stationAddr,114,2,strWriteBufTemp);
        }
        usleep(200000);
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 0;
        writeLen = 2;
    }
    else if(pack1StatusFlag)
    {
        qDebug()<<"*packstatus*";
        if(glueTightPackStatusFlag)
        {
            strWriteBufTemp[0] = 0;
            strWriteBufTemp[1] = pack1Result;//PLCPack1Status.toInt();
            strWriteBufTemp[2] = 0;
            strWriteBufTemp[3] = pack2Result;//PLCPack2Status.toInt();
            writePLCFunc(stationAddr,glueTightPackStatusAddr,4,strWriteBufTemp);
        }
        usleep(200000);
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = pack1Result;//PLCPack1Status.toInt();
        strWriteBuf[2] = 0;
        strWriteBuf[3] = pack2Result;//PLCPack2Status.toInt();
        writeLen = 4;
    }
    else if(getStationErrorFinishFlag)
    {
        qDebug()<<"getStationErrorFinishFlag"<<stationFinishAddr;
        if(Station == "A350-1" || Station == "A350-2")
        {
            strWriteBufTemp[0] = 0;
            strWriteBufTemp[1] = 1;
            writePLCFunc(stationAddr,106,2,strWriteBufTemp);
        }
        usleep(200000);
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 1;
        writeLen = 2;
        writeAddr = stationFinishAddr;
        getStationErrorFinishFlag = false;
        getProductTypeFlag = true;
        writeProductFinishFlag = false;
    }
    else if(getGlueProductTypeFlag)
    {
        qDebug()<<"*getGlueProductTypeFlag*"<<PLCPackType.toInt();
        writePLCFlag = true;
        strWriteBuf[0] = 0;
//        strWriteBuf[1] = 1;//ackCode;
        strWriteBuf[1] = PLCPackType.toInt();
        writeLen = 2;
        if(glueTightProductTypeFlag)
        {
            strWriteBufTemp[0] = 0;
            strWriteBufTemp[1] = PLCPackType.toInt();
            writePLCFunc(stationAddr,glueTightProductTypeAddr,2,strWriteBufTemp);
        }
    }
    else if(getProductTypeFlag)//get product type
    {
        qDebug()<<"*getProductTypeFlag*"<<PLCPackType.toInt();
        if(glueTightProductTypeFlag)
        {
            strWriteBufTemp[0] = 0;
            strWriteBufTemp[1] = PLCPackType.toInt();
            writePLCFunc(stationAddr,glueTightProductTypeAddr,2,strWriteBufTemp);
        }
        usleep(200000);
        writePLCFlag = true;
        strWriteBuf[0] = 0;
//        strWriteBuf[1] = 1;//ackCode;
        strWriteBuf[1] = PLCPackType.toInt();
        writeLen = 2;
        writeAddr = reqProductTypeAddr;//请求产品型号时，要写入pack状态，再写入产品型号，需要记录产品型号地址
        writeProductFinishFlag = true;
    }
    else if(getDistroyStatus)
    {
        qDebug()<<"*getDistroyStatus*";
        errorHandle();
        getDistroyStatus = false;
    }
    else if(getTightenPositionFlag)//get tightening position number
    {
        emit sendStationFinishToMaindow("huhuhu","201");
        qDebug()<<"*getTightenPositionFlag*";
        writePLCFlag = true;
        if(GPackType == "PHEV")
        {
            if(tightenCount >= (tighten_Num<<1))
            {
                tightenCount = 0;
            }
            if((workSecondePackFlag == true)||(pack12Num == 2))
            {
                countValue = tighten_Num;
            }
            else
            {
                countValue = 0;
            }
            strWriteBuf[0] = 0;
            strWriteBuf[1] = tightenCount+1+countValue;
        }
        else
        {
            if(tightenCount >= tighten_Num)
            {
                tightenCount = 0;
            }
            strWriteBuf[0] = 0;
            strWriteBuf[1] = tightenCount+1;
        }
        writeLen = 2;
    }
    else if(getGluePositionFlag)
    {
        qDebug()<<"*getGluePositionFlag*";
        QByteArray vinTemp = PLCPack1SN.toLatin1();

        for(int i=0;i<vinTemp.size();i++)
        {
            strWriteBufTemp[i] = (unsigned char)vinTemp[i];
        }
        writeLen = (vinTemp.size());
        qDebug()<<"this is send glue"<<strWriteBuf<<vinTemp.size();
        writePLCFunc(stationAddr,128,writeLen,strWriteBufTemp);
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 1;
        writeLen = 2;
    }
    else if(getGlueBindSuccess)
    {
        qDebug()<<"*getGlueBindSuccess*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 1;
        strWriteBuf[2] = 0;
        strWriteBuf[3] = 1;
        writeLen = 4;
    }
    else if(getTightErrorFlag)
    {
        qDebug()<<"*getTightErrorFlag*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 2;
        writeLen = 2;
        writeAddr = 104;
    }
    else if(getTightenFinishFlag)
    {
//        emit sendStationFinishToMaindow("huhuhu","202");
        qDebug()<<"*getTightenFinishFlag*"<<tightenCount<<workSecondePackFlag<<countValue<<GNokFlagTemp;
        writePLCFlag = true;
        if(GPackType == "PHEV")
        {
            if(tightenCount < tighten_Num)
            {
                strWriteBuf[0] = 0;
                strWriteBuf[1] = 1;
                writeLen = 2;
            }
            else
            {
                tightenCount = 0;
               // if((workSecondePackFlag == true)||(pack12Num != 3))   //2018-04-24change
                if((workSecondePackFlag == true)||(pack12Num != 3))
                {
                    workSecondePackFlag = false;
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    strWriteBuf[2] = 0;
                    strWriteBuf[3] = 1;
                    writeLen = 4;
                    qDebug()<<"this is pack GBoltOkNum"<<pack12Num<<GBoltOkNum<<GBoltAllNum<<BoltTotalNum<<tighten_Num<<(BoltTotalNum*tighten_Num);
    //                if(GBoltOkNum < (BoltTotalNum*tighten_Num))
                    if(pack12Num == 1)
                    {
                        if(GBoltOkNum < GBoltAllNum)
                        {
                            packUplodeTemp1 = "2";
                        }
                        else
                        {
                            packUplodeTemp1 = "1";
                        }
                    }
                    else
                    {
                        if(GBoltOkNum < GBoltAllNum)
                        {
                            packUplodeTemp2 = "2";
                        }
                        else
                        {
                            packUplodeTemp2 = "1";
                        }
                    }
                    GBoltOkNum = 0;
                    GBoltAllNum = 0;
                    if(Station == "A050")
                    {
                        if(pack12Num == 3)
                        {
                            if(packUplodeTemp1 == "2")
                            {
                                if(packUplodeTemp2 == "1")
                                {
                                    pack12Num = 2;
                                }
                                else if(packUplodeTemp2 == "2")
                                {
                                    boxErrorFlag = true;
                                }
                            }
                            else if(packUplodeTemp1 == "1")
                            {
                                if(packUplodeTemp2 == "1")
                                {

                                }
                                else if(packUplodeTemp2 == "2")
                                {
                                    pack12Num = 1;
                                }
                            }
                        }
                        else if(pack12Num == 2)
                        {
                            if(packUplodeTemp2 == "2")
                            {
                                boxErrorFlag = true;
                            }
                        }
                        else if(pack12Num == 1)
                        {
                            if(packUplodeTemp1 == "2")
                            {
                                boxErrorFlag = true;
                            }
                        }
//                        if((workSecondePackFlag == true)||(pack12Num == 2))
//                        {
//                            countValue = box_Num;
//                        }
//                        else
//                        {
//                            countValue = 0;
//                        }
                        if(boxErrorFlag)
                        {
                            qDebug()<<"boxErrorFlag";
                            boxErrorFlag = false;
//                            strWriteBuf[0] = 0;
//                            strWriteBuf[1] = 0;
//                            strWriteBuf[2] = 0;
//                            strWriteBuf[3] = 0;
//                            strWriteBuf[4] = 0;
//                            strWriteBuf[5] = 1;
//                            writeLen = 6;
                            strWriteBufTemp[0] = 0;
                            strWriteBufTemp[1] = 1;
                            writePLCFunc(stationAddr,112,2,strWriteBufTemp);
                        }
                    }
                    if(1)
                    {
                        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
                        qDebug()<<"emitStationFinish";
                        emit sendReqStationState(PLCPack1SN,packUplodeTemp1,PLCPack2SN,packUplodeTemp2,time.toString("yyyy-MM-dd hh:mm:ss"));
                    }
                }
                else
                {
                    workSecondePackFlag = true;
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    writeLen = 2;
                    qDebug()<<"this is pack1 GBoltOkNum"<<GBoltOkNum<<GBoltAllNum<<tighten_Num;
    //                if(GBoltOkNum < (BoltTotalNum*tighten_Num))
                    if(GBoltOkNum < GBoltAllNum)
                    {
                        packUplodeTemp1 = "2";
                    }
                    else
                    {
                        packUplodeTemp1 = "1";
                    }
                    GBoltOkNum = 0;
                    GBoltAllNum = 0;
                }
            }
        }       //bev tight
        else
        {
            qDebug()<<"*getTightenFinishFlag*bev*"<<tightenCount<<tighten_Num;
            if(tightenCount < tighten_Num)
            {
                strWriteBuf[0] = 0;
                strWriteBuf[1] = GtightGlueresult;
                writeLen = 2;
            }
            else
            {
                tightenCount = 0;
                strWriteBuf[0] = 0;
                strWriteBuf[1] = GtightGlueresult;
                strWriteBuf[2] = 0;
                strWriteBuf[3] = 1;
                writeLen = 4;
                if(GBoltOkNum < GBoltAllNum)
                {
                    packUplodeTemp1 = "2";
                }
                else
                {
                    packUplodeTemp1 = "1";
                }
                GBoltOkNum = 0;
                GBoltAllNum = 0;
                qDebug()<<"this is bev pack1 GBoltOkNum"<<GBoltOkNum<<GBoltAllNum<<tighten_Num;
                if(1)
                {
                    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
                    qDebug()<<"emitStationFinish";
                    emit sendReqStationState(PLCPack1SN,packUplodeTemp1,PLCPack2SN,"3",time.toString("yyyy-MM-dd hh:mm:ss"));
                }
            }
        }
//        if(Station == "A050new")
        if(GTightResetOnline == "Yes")
        {
            writeAddr = 104;
        }
    }
    else if(getBoxPositionFlag)
    {
        /*qDebug()<<"*******22222***********11111111"<<pack12Num<<boxCount;
        writePLCFlag = true;
        if( boxCount >= (box_Num<<1))
        {
            boxCount = 0;
        }
        if((workSecondePackFlag == true)||(pack12Num == 2))
        {
            countValue = box_Num;
        }
        else
        {
            countValue = 0;
        }
        strWriteBuf[0] = 0;
        strWriteBuf[1] = boxCount+1+countValue;
        writeLen = 2;*/
    }
    else if(getBoxFinishFlag)
    {
        qDebug()<<"overturn BoxFinishFlag"<<boxCount;
        writePLCFlag = true;
        boxCount++;
        if(boxCount < box_Num)
        {
            strWriteBuf[0] = 0;
            strWriteBuf[1] = 1;
            writeLen = 2;
        }
        else
        {
            boxCount = 0;
            if((workSecondePackFlag == true)||(pack12Num != 3))
            {
                workSecondePackFlag = false;
                strWriteBuf[0] = 0;
                strWriteBuf[1] = 1;
                strWriteBuf[2] = 0;
                strWriteBuf[3] = 1;
                writeLen = 4;
            }
            else
            {
                workSecondePackFlag = true;
                strWriteBuf[0] = 0;
                strWriteBuf[1] = 1;
                writeLen = 2;
            }
        }
    }
    else if(getStationFinishFlag)
    {
        qDebug()<<"getStationFinishFlag";
        if(Station == "A350-1" || Station == "A350-2")
        {
            writePLCFlag = true;
            //writeLen = (GwhichStepNum<<1);
            for(int i=0;i<8;i++) //clear valid step value
            {
                strWriteBufTemp[i] = 0;
            }
            writePLCFunc(stationAddr,100,8,strWriteBufTemp);
            usleep(200000);
            writePLCFunc(stationAddr,114,8,strWriteBufTemp);
        }
        else
        {
            writePLCFlag = true;
            writeLen = PLCWorkAllStep<<1;
            for(int i=0;i<writeLen;i++) //clear valid step value
            {
                strWriteBuf[i] = 0;
            }
            writeAddr = 100;
        }
        tightenCount = 0;
        boxCount = 0;
        dutouCount = 0;
        moduleCount = 0;
        moduleBindCount = 0;
        workSecondePackFlag = false;
    }
    else if(getDutoupositionFlag)
    {
        emit sendStationFinishToMaindow("huhuhu","203");
        qDebug()<<"*getDutoupositionFlag*"<<vinCode<<dutouStatusPosition<<dotouPositionNum1<<dotouPositionNum2;
        QByteArray vinTemp = vinCode.toLatin1();
        if(vinTemp == "")
        {
            vinTemp = "Code__NULL";
        }
        writePLCFlag = true;
        if(dutouStatusPosition)
        {
            if(channel_Num == 1)
            {
                strWriteBuf[0] = 0;
                strWriteBuf[1] = dotouPositionNum1.mid(dutouCount,1).toInt();
            }
            else if(channel_Num == 2)
            {
                strWriteBuf[0] = 0;
                strWriteBuf[1] = dotouPositionNum2.mid(dutouCount,1).toInt();
            }
            else
            {
                qDebug()<<"this is channel_Num error";
            }
            if( dotouPositionNum2.mid(dutouCount,1).toInt() > 2)
            {
                vinTemp = PLCPack2SN.toLatin1();
            }
            else
            {
                vinTemp = PLCPack1SN.toLatin1();
            }
        }
        else
        {
            if( dutouCount >= (dutou_Num<<1))
            {
                dutouCount = 0;
            }
            if((workSecondePackFlag == true)||(pack12Num == 2))
            {
                countValue = dutou_Num;
            }
            else
            {
                countValue = 0;
            }
            strWriteBuf[0] = 0;
            strWriteBuf[1] = dutouCount+1+countValue;
            if(workSecondePackFlag)
            {
                vinTemp = PLCPack2SN.toLatin1();
            }
            else
            {
                 vinTemp = PLCPack1SN.toLatin1();
            }
        }
        strWriteBuf[2] = 0;
        strWriteBuf[3] = 0;
        strWriteBuf[4] = 0;
        strWriteBuf[5] = 0;
        strWriteBuf[6] = 0;
        strWriteBuf[7] = 1;
        strWriteBuf[8] = 0;
        strWriteBuf[9] = pack1Result;
        strWriteBuf[10] = 0;
        strWriteBuf[11] = pack2Result;
        strWriteBuf[12] = 'S';
        strWriteBuf[13] = 'K';
        if(vinTemp.size() < 20)
        {
            for(int i = 0; i < vinTemp.size(); i++)
            {
                strWriteBuf[14+i] = (unsigned char)vinTemp[i];
            }
            writeLen = 14+vinTemp.size();
        }
        else
        {
            for(int i = 0; i < 20; i++)
            {
                strWriteBuf[14+i] = (unsigned char)vinTemp[i];
            }
            writeLen = 34;
        }
        qDebug()<<"this is vintemp"<<vinTemp.size()<<vinTemp;
        /*for(int i=0;i<10;i++)
        {
            strWriteBuf[14+i] = (unsigned char)vinTemp[i];
        }
        writeLen = 24;*/
    }
    else if(getDutouRecordFinishFlag)
    {
        qDebug()<<"*getDutouRecordFinishFlag*"<<dutouStatusPosition<<dutouCount<<dotouPositionNum1.size()<<dotouPositionNum2.size();
        writePLCFlag = true;
        if(dutouStatusPosition)
        {
            if(channel_Num == 1)
            {
                if(dutouCount < dotouPositionNum1.size())
                {
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    writeLen = 2;
                }
                else
                {
                    dutouCount = 0;
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    strWriteBuf[2] = 0;
                    strWriteBuf[3] = 1;
                    writeLen = 4;
                }
            }
            else if(channel_Num == 2)
            {
                if(dutouCount < dotouPositionNum2.size())
                {
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    writeLen = 2;
                }
                else
                {
                    dutouCount = 0;
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    strWriteBuf[2] = 0;
                    strWriteBuf[3] = 1;
                    writeLen = 4;
                }
            }
            else
            {
                qDebug()<<"this is channel error!!!";
            }
        }
        else
        {
            if(dutouCount < dutou_Num)
            {
                strWriteBuf[0] = 0;
                strWriteBuf[1] = 1;
                writeLen = 2;
            }
            else
            {
                dutouCount = 0;
                if((workSecondePackFlag == true)||(pack12Num != 3))
                {
                    workSecondePackFlag = false;
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    strWriteBuf[2] = 0;
                    strWriteBuf[3] = 1;
                    writeLen = 4;
                }
                else
                {
                    vinCode = PLCPack2SN;
                    workSecondePackFlag = true;
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    writeLen = 2;
                }
            }
        }

    }
    else if(getTrayReadyFlag)
    {
        qDebug()<<"send tray message to plc";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
//        strWriteBuf[1] = 1;//1;
        if(Station == "VM010-1" || Station == "VM010-2")
        {
            strWriteBuf[1] = GrfidStatusTemp;
        }
        else
        {
            strWriteBuf[1] = readRFIDResult;    //trayflag 1 160 161 162 otherstation
        }
//        strWriteBuf[1] = GrfidStatusTemp;   //traytype 1 2 3  vm010
//        strWriteBuf[1] = readRFIDResult;    //trayflag 1 160 161 162 otherstation
        writeLen = 2;
        /*if(Station == "A090-1" || Station == "A090-2")
        {
            strWriteBufTemp[0] = 0;
            strWriteBufTemp[1] = 0;
            writePLCFunc(stationAddr,118,2,strWriteBufTemp);
        }*/
    }
    else if(getTrayType)
    {
        qDebug()<<"requset getTrayType rfid";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = GrfidStatusTemp;
        strWriteBuf[2] = 0;
        strWriteBuf[3] = 1;
        writeLen = 4;
    }
    else if(getRfidUpDowmFlag)
    {
        qDebug()<<"*getRfidUpDowmFlag*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = GrfidStatusTemp;//1;
        writeLen = 2;
    }
    else if(getModuleBoxNumFlag)
    {
        emit sendStationFinishToMaindow("请求模组入箱位置","204");

        qDebug()<<"*getModuleBoxNumFlag*"<<pack12Num;
        writePLCFlag = true;
        RFIDCodeTemp = RFIDCode;
        RFIDlist.append(RFIDCode);
        if(workSecondePackFlag == true || pack12Num == 2)
        {
            countValue = 2;
        }
        else
        {
            countValue = 0;
        }
        strWriteBuf[0] = 0;
        strWriteBuf[1] = moduleBuf.mid(moduleCount+countValue,1).toInt();//moduleCount + 1;
        writeLen = 2;
    }
    else if(fixtureHoldFlag)
    {
        qDebug()<<"*fixtureHoldFlag*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 1;//1;
        writeLen = 2;
    }
    else if(getModuleRecordFinishFlag)
    {
        emit sendStationFinishToMaindow("托盘数据上传","204");
        qDebug()<<"*getModuleRecordFinishFlag*"<<moduleCount<<updataTrayTemp<<moduleBoundResult<<trayBindCount;
        writePLCFlag = true;
        /*strWriteBufTemp[0] = 0;
        strWriteBufTemp[1] = 1;
        writePLCFunc(stationAddr,118,2,strWriteBufTemp);*/
        if(moduleBoundResult == 1)
        {
            trayfinishBoundCount++;
            if(updataTraySN.mid(0,1) == "P")
            {
                nextTrayflag = true;
            }
            else if(updataTraySN.mid(0,1) == "B")
            {
                if(Station == "A090-1")
                {
                    if(trayfinishBoundCount >= 4)
                    {
                        trayfinishBoundCount = 0;
                        nextTrayflag = true;
                    }
                }
                else if(Station == "A090-2")
                {
                    if(trayfinishBoundCount >= 2)
                    {
                        trayfinishBoundCount = 0;
                        nextTrayflag = true;
                    }
                }
            }
            else
            {
                nextTrayflag = true;
            }
            if(nextTrayflag)
            {
                trayfinishBoundCount = 0;
                emit sendModuleTrayClean(updataTraySN);
                nextTrayflag = false;
                updataTraySN = "";
                if(updataTrayTemp.isEmpty())
                {

                }
                else
                {
                    updataTraySN = updataTrayTemp;
                    updataTrayTemp = "";
                }
            }
        }
        if(GPackType == "PHEV")
        {
            int moduleTemp = 0;
            if(module_typeNum == 0)
            {
                moduleTemp = 1;
            }
            else
            {
                moduleTemp = 2;
            }
            qDebug()<<"this is moduletemp"<<moduleTemp<<moduleBoundResult;
            if(moduleTemp == 1)
            {
                if(!workSecondePackFlag)
                {

//                    workSecondePackFlag = true;
                    moduleCount = 0;
                    qDebug()<<"this is first send module ok"<<pack12Num;
                    if(pack12Num == 1)
                    {
                        if(moduleBoundResult == 1)
                        {
                            workSecondePackFlag = true;
                            strWriteBuf[0] = 0;
                            strWriteBuf[1] = moduleBoundResult;
                            strWriteBuf[2] = 0;
                            strWriteBuf[3] = 1;
                            writeLen = 4;
                        }
                        else
                        {
                            strWriteBuf[0] = 0;
                            strWriteBuf[1] = moduleBoundResult;
                            writeLen = 2;
                        }

                    }
                    else if(pack12Num == 3)
                    {
                        /*workSecondePackFlag = true;
                        strWriteBuf[0] = 0;
                        strWriteBuf[1] = moduleBoundResult;
                        writeLen = 2;*/
                        if(moduleBoundResult == 1)
                        {
                            workSecondePackFlag = true;
                            strWriteBuf[0] = 0;
                            strWriteBuf[1] = moduleBoundResult;
                            writeLen = 2;
                        }
                        else
                        {
                            strWriteBuf[0] = 0;
                            strWriteBuf[1] = moduleBoundResult;
                            writeLen = 2;
                        }
                    }
                    else if(pack12Num == 2)
                    {
                        if(moduleBoundResult == 1)
                        {
                            workSecondePackFlag = true;
                            strWriteBuf[0] = 0;
                            strWriteBuf[1] = moduleBoundResult;
                            strWriteBuf[2] = 0;
                            strWriteBuf[3] = 1;
                            writeLen = 4;
                        }
                        else
                        {
                            strWriteBuf[0] = 0;
                            strWriteBuf[1] = moduleBoundResult;
                            writeLen = 2;
                        }
                    }
                }
                else
                {
                    qDebug()<<"this is second send module ok";
                    moduleCount = 0;
//                    workSecondePackFlag = false;
                    if(moduleBoundResult == 1)
                    {
                        workSecondePackFlag = false;
                        strWriteBuf[0] = 0;
                        strWriteBuf[1] = moduleBoundResult;
                        strWriteBuf[2] = 0;
                        strWriteBuf[3] = 1;
                        writeLen = 4;
                    }
                    else
                    {
                        strWriteBuf[0] = 0;
                        strWriteBuf[1] = moduleBoundResult;
                        writeLen = 2;
                    }
                }
            }
            else if(moduleTemp == 2)
            {
                if(moduleCount < 2)
                {
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = moduleBoundResult;
                    writeLen = 2;
                    qDebug()<<"this is moduletemp1111111111"<<moduleTemp;//wz
                    qDebug()<<"workSecondePackFlag  1111111"<<workSecondePackFlag;

                }
                else
                {
                    moduleCount = 0;
                    qDebug()<<"workSecondePackFlag333222222222"<<workSecondePackFlag;
                    if((workSecondePackFlag == true)||(pack12Num != 3))
                    {
                        workSecondePackFlag = false;
                        if(moduleBoundResult == 1)
                        {
                            strWriteBuf[0] = 0;
                            strWriteBuf[1] = moduleBoundResult;
                            strWriteBuf[2] = 0;
                            strWriteBuf[3] = 1;
                            writeLen = 4;
                        }
                        else
                        {
                            strWriteBuf[0] = 0;
                            strWriteBuf[1] = moduleBoundResult;
                            writeLen = 2;
                        }
                        qDebug()<<"this is moduletemp222222222"<<moduleTemp;
                        qDebug()<<"this is pack12Num222222222"<<pack12Num;
                        qDebug()<<"workSecondePackFlag  222222222"<<workSecondePackFlag;
                    }
                    else
                    {
                        vinCode = PLCPack2SN;
                        workSecondePackFlag = true;
                        strWriteBuf[0] = 0;
                        strWriteBuf[1] = moduleBoundResult;
                        writeLen = 2;
                        qDebug()<<"this is moduletemp333333333"<<moduleTemp;
                        qDebug()<<"workSecondePackFlag  33333333333"<<workSecondePackFlag;


                    }
                }
            }
        }
        else
        {
            if(moduleCount < (moduleMessage.size()-1))
            {
                strWriteBuf[0] = 0;
                strWriteBuf[1] = moduleBoundResult;
                writeLen = 2;
            }
            else
            {
                if(moduleBoundResult == 1 && trayBindCount == moduleCount)
                {
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = moduleBoundResult;
                    strWriteBuf[2] = 0;
                    strWriteBuf[3] = 1;
                    writeLen = 4;
                }
                else
                {
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = moduleBoundResult;
                    writeLen = 2;
                }
                moduleCount = 0;
            }
        }
    }
    else if(getRollerBedReadyFlag)
    {
        qDebug()<<"*getRollerBedReadyFlag*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = readRFIDResult;
        writeLen = 2;
    }
    else if(getBindingSNFlag)
    {
        qDebug()<<"*getBindingSNFlag*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 1;//ackCode;
        writeLen = 2;
        writeAddr = reqBoundAddr;//请求产品型号时，要写入pack状态，再写入产品型号，需要记录产品型号地址
    }
    else if(getUpdownBoxPositionFlag)//请求下箱体位置号
    {
        emit sendStationFinishToMaindow("huhuhu","205");
        qDebug()<<"*getUpdownBoxPositionFlag*"<<countValue<<boxCount<<pack12Num<<packUplodeTemp1<<packUplodeTemp2;
        writePLCFlag = true;
        if( boxCount >= (box_Num<<1))
        {
            boxCount = 0;
        }
        if(pack12Num == 3)
        {
            if(packUplodeTemp1 == "2")
            {
                if(packUplodeTemp2 == "1")
                {
                    pack12Num = 2;
                }
                else if(packUplodeTemp2 == "2")
                {
                    boxErrorFlag = true;
                }
            }
            else if(packUplodeTemp1 == "1")
            {
                if(packUplodeTemp2 == "1")
                {

                }
                else if(packUplodeTemp2 == "2")
                {
                    pack12Num = 1;
                }
            }
        }
        else if(pack12Num == 2)
        {
            if(packUplodeTemp2 == "2")
            {
                boxErrorFlag = true;
            }
        }
        else if(pack12Num == 1)
        {
            if(packUplodeTemp1 == "2")
            {
                boxErrorFlag = true;
            }
        }
        if((workSecondePackFlag == true)||(pack12Num == 2))
        {
            countValue = box_Num;
        }
        else
        {
            countValue = 0;
        }
        if(boxErrorFlag)
        {
            qDebug()<<"boxErrorFlag";
            boxErrorFlag = false;
            strWriteBuf[0] = 0;
            strWriteBuf[1] = 0;
            strWriteBuf[2] = 0;
            strWriteBuf[3] = 0;
            strWriteBuf[4] = 0;
            strWriteBuf[5] = 1;
            writeLen = 6;
        }
        else
        {
            strWriteBuf[0] = 0;
            strWriteBuf[1] = boxCount+1+countValue;
            writeLen = 2;
        }
    }
    else if(getScanBarcodeFlag)
    {
        qDebug()<<"*getScanBarcodeFlag*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = scanResult;
        writeLen = 2;
    }
    else if(getUpdownBoxRecordDataFlag)//请求条码数据绑定
    {
        qDebug()<<"*getUpdownBoxRecordDataFlag*"<<boxCount<<box_Num<<pack12Num<<workSecondePackFlag;
        writePLCFlag = true;
        if(GPackType == "PHEV")
        {
            boxCount++;
            if(boxCount < box_Num)
            {
                strWriteBuf[0] = 0;
                strWriteBuf[1] = 1;
                writeLen = 2;
            }
            else
            {
                boxCount = 0;
                if((workSecondePackFlag == true)||(pack12Num != 3))
                {
                    workSecondePackFlag = false;
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    strWriteBuf[2] = 0;
                    strWriteBuf[3] = 1;
                    writeLen = 4;
                }
                else
                {
                    workSecondePackFlag = true;
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    writeLen = 2;
                }
            }
        }
        else
        {
            strWriteBuf[0] = 0;
            strWriteBuf[1] = 1;
            strWriteBuf[2] = 0;
            strWriteBuf[3] = 1;
            writeLen = 4;
        }

    }
    else if(getModuleMoveFinishFlag)
    {
        qDebug()<<"*getModuleMoveFinishFlag*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 1;
        writeLen = 2;
    }
    else if(getModuleTypeFlag)
    {
        qDebug()<<"*getModuleTypeFlag*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 1;
        writeLen = 2;
    }
    else if(startWorkFlag)
    {
        qDebug()<<"*startWorkFlag*";
        if(glueStartWorkFlag)
        {
            strWriteBufTemp[0] = 0;
            strWriteBufTemp[1] = 1;
            writePLCFunc(stationAddr,glueStartWorkAddr,2,strWriteBufTemp);
        }
        /*if(Station == "A090-1" || Station == "A090-2")
        {
            strWriteBufTemp[0] = 0;
            strWriteBufTemp[1] = 1;
            writePLCFunc(stationAddr,118,2,strWriteBufTemp);
        }*/
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 1;
        writeLen = 2;
    }
    else if(reqPLCMoveTrayFlag)
    {
        qDebug()<<"*reqPLCMoveTrayFlag*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 0;
        strWriteBuf[2] = 0;
        strWriteBuf[3] = 1;
        writeLen = 4;
        writeAddr = 100;
    }
    else if(getPLCMoveTrayFlag)
    {
        if(Station == "VM010-1" || Station == "VM010-2")
        {
            writePLCFlag = true;
            strWriteBuf[0] = 0;
            strWriteBuf[1] = 1;
            writeLen = 2;
            writeAddr = trayFinishAddr;
        }
        else
        {
            qDebug()<<"*getPLCMoveTrayFlag*"<<PLCWorkAllStep;
            writePLCFlag = true;
            writeLen = ((trayFinishAddr%100)<<1);
            qDebug()<<"this is trayFinishAddr"<<writeLen;
    //        writeLen = PLCWorkAllStep<<1;
            for(int i=0;i<writeLen;i++) //clear valid step value
            {
                strWriteBuf[i] = 0;
            }
            writeAddr = 100;
        }
    }
}
/******************************************/
//handle plc step type parameter 与PLC交互到第几步了
/******************************************/
bool ConnectPLC::handlePLCStepTypeFunc(int num,int type)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    GwhichStepNum = num;
    getDistroyStatus = false;
//    PLCStepMap = PLCstepList[num].toMap();
//    int type = parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString());
    qDebug()<<"*handlePLCStepTypeFunc*"<<num<<type;
    switch (type)
    {
    case 1: //request product type
        reqProductTypeFlag = true;
        stationSkip = true;
        if(cleanAllDWFlag)
        {
            cleanAllDW();
        }

        //主窗口界面显示
        emit sendStationFinishToMaindow("正在匹配工艺，请稍等。。。","显示托盘请求状态");

        emit sendReqPackStatus();//向接口获取订单信息
        if(Station == "A090-1" || Station == "A090-2")
        {
            qDebug()<<"!!!!!!!!";
            if(trayIsAction > 0 && updataTraySN.isEmpty())
            {
                 emit sendReadCmdToRFID("0000","0015");
                reqProductTrayFlag = true;
            }
            else
            {
                qDebug()<<"tray is error in A090";
            }
//            reqProductTrayFlag = true;    20190304
        }
        ackProductTypeFlag = true;
        statusValidFlag = true;
        reqProductTypeAddr = PLCStepMap["writeAdd"].toInt();
        break;
    case 2://request tighten position
        reqTightenPositionFlag = true;
        getTightenPositionFlag = true;
        statusValidFlag = true;
        break;
    case 53://request glue position
        getGluePositionFlag = true;
        statusValidFlag = true;
        stationSkip = false;
        break;
    case 54://requset glue bind success
        getGlueBindSuccess = true;
        statusValidFlag = true;
        emit sendAddStationTimer(PLCPack1SN,2);
        break;
    case 3: //Robot tighten ready机器人到达拧紧位置
        reqRobotTightenReadyFlag = true;
        getRobotTightenReadyFlag = true;
        GgetRobotTightenReadyFlag = true;
        if(pack12Num == 1)
        {
            if(Station == "A400A" || Station == "A400B")
            {
                qDebug()<<"this is station pack A400";
                vinCode =PLCPack2SN;
            }
            else
            {
                vinCode =  PLCPack1SN;
            }
//            vinCode =  PLCPack1SN;
//            emit sendCmdToMain(PLCPack1SN,0,tightenCount+1);
//            emit sendChToWork(channel_Num,1,PLCPack1SN);//type=1 tighten2  tell work vin
        }
        else if(pack12Num == 2)
        {
            vinCode =  PLCPack2SN;
//            emit sendCmdToMain(PLCPack2SN,0,tightenCount+1);
//            emit sendChToWork(channel_Num,1,PLCPack2SN);//type=1 tighten2
        }
        else//pack12Num = 3
        {
            if(!workSecondePackFlag)
            {
                vinCode =  PLCPack1SN;
            }
            else
            {
                vinCode =  PLCPack2SN;
            }
        }
        if(GPackType == "PHEV")
        {
            emit sendCmdToMain(vinCode,0,tightenCount+1);
        }
        else if(GPackType == "BEV")
        {
            qDebug()<<"this is bev tightenCount"<<tightenCount;
            emit sendCmdToMain(vinCode,0,tightenCount+5);
        }
        emit sendChToWork(channel_Num,1,vinCode);//type=1 tighten2
        statusValidFlag = true;
        break;
    case 4://station finish
        reqStationFinishFlag = true;
        cleanAllDWFlag = false;
        getRobotTightenReadyFlag = false;
        plcStopFlag = true;
        GgetRobotTightenReadyFlag = false;
        if(sendNOStationFlag || ((Station == "A350-1" || Station == "A350-2") && stationSkip))
        {
            sendNOStationFlag = false;
            stationSkip = true;
        }
        else
        {
            if(pack12Num == 3)
            {
                emit sendReqStationState(PLCPack1SN,packUplodeTemp1,PLCPack2SN,packUplodeTemp2,time.toString("yyyy-MM-dd hh:mm:ss"));
            }
            else if(pack12Num == 1)
            {
                qDebug()<<"this is  pack12Num equ 1 status post"<<packUplodeTemp1;
                if(Station == "A400A" || Station == "A400B")
                {
                    qDebug()<<"this is A400 station status post";
                    emit sendReqStationState(PLCPack1SN,"3",PLCPack2SN,packUplodeTemp1,time.toString("yyyy-MM-dd hh:mm:ss"));
                }
                else
                {
                    emit sendReqStationState(PLCPack1SN,packUplodeTemp1,PLCPack2SN,"3",time.toString("yyyy-MM-dd hh:mm:ss"));
                }
            }
            else if (pack12Num == 2)
            {
                qDebug()<<"this is the second pack operate";
                emit sendReqStationState(PLCPack1SN,"3",PLCPack2SN,packUplodeTemp2,time.toString("yyyy-MM-dd hh:mm:ss"));
            }
            else if(pack12Num == 0)
            {
                qDebug()<<"this is the error pack operate";
                if(Station == "A390A" || Station == "A390B" || Station == "A400A" || Station == "A400B" )
                {
                    if(PLCPack1Status == "4" && PLCPack2Status == "4")
                    {
                        qDebug()<<"this is not operator station without ok";
                         emit sendReqStationState(PLCPack1SN,"3",PLCPack2SN,"3",time.toString("yyyy-MM-dd hh:mm:ss"));
                    }
                    else
                    {
                        emit sendReqStationState(PLCPack1SN,"3",PLCPack2SN,"3",time.toString("yyyy-MM-dd hh:mm:ss"));
                    }
                }
                else
                {
                    emit sendReqStationState(PLCPack1SN,"3",PLCPack2SN,"3",time.toString("yyyy-MM-dd hh:mm:ss"));
                }
            }
        }
//        emit sendReqStationState(PLCPack1SN,"1",PLCPack2SN,"1",time.toString("yyyy-MM-dd hh:mm:ss"));
        emit sendStationFinishToMaindow("huhu","30");
        getStationFinishFlag = true;
        statusValidFlag = true;
        tightenCount = 0;
        boxCount = 0;
        dutouCount = 0;
        moduleCount = 0;
        dutouStatusPosition = false;
        workSecondePackFlag = false;
        break;
    case 5:
        reqBoxPositionFlag = true;
        getBoxPositionFlag = true;
        statusValidFlag = true;
        break;
    case 6:
        reqBoxFinishFlag = true;
        getBoxFinishFlag = true;
        statusValidFlag = true;
        break;
    case 8://mannul mode
        statusValidFlag = false;
//        if(Station == "A050new")
        if(GTightResetOnline == "Yes")
        {
            qDebug()<<"GTightResetOnline == 8";
        }
//        else
//        {
//            PLC_StepNum = 0;
//        }
        return false;
        break;
    case 9://request dutou position
        reqDutouPositionFlag = true;
        getDutoupositionFlag = true;
//        emit startW500CMD(channel_Num,vinCode);
        statusValidFlag = true;
        break;
    case 10://request data2 rocord dutou install result
        reqDutouRecordDataFlag = true;
        getDutouRecordDataFlag = true;
        statusValidFlag = true;
        emit startW500CMD(channel_Num,"read");
        break;
    case 15://tray place ready
        qDebug()<<"××××××××××托盘到位××××××××××××××××";
        tellTrayReadyFlag = true;
//        getTrayReadyFlag = true;
        statusValidFlag = true;
        isReadRfidFlag = true;
        GEKSReRFID = true;
//        emit sendCmdToStep(6,"ready",15);
        emit sendReadCmdToRFID("0000","0015");
        break;
    case 22:
        getRfidUpDowmFlag = true;
        statusValidFlag = true;
        break;
    case 16://request module box num
        reqModuleBoxNumFlag = true;
        getModuleBoxNumFlag = true;
        statusValidFlag = true;
        break;
    case 37:
//        fixtureHoldFlag = true;
        revModulePack(true);
        statusValidFlag = true;
        break;
    case 17:
        reqModuleRecordDataFlag = true;
        getModuleRecordDataFlag = true;
        qDebug()<<"%%%%%%%%%%%%%%%%%%%%%%%";
        emit sendShowPush();//显示当前模组绑定bypass图标
//        if(moduleCount == 0)
//        {
//        if(!RFIDlist.isEmpty())
//        {
            if(!workSecondePackFlag)
            {
                if(pack12Num == 2)
                {
                    qDebug()<<"this is send moudle pack to http"<<updataTraySN<<PLCPack2SN;
                    emit sendReqModulePack(RFIDCode,PLCPack2SN);
                }
                else
                {
                    qDebug()<<"this is send moudle pack to http first"<<updataTraySN<<PLCPack1SN;
                    emit sendReqModulePack(RFIDCode,PLCPack1SN);
                }
            }
            else
            {
                qDebug()<<"this is send moudle pack to http"<<updataTraySN<<PLCPack2SN;
                emit sendReqModulePack(RFIDCode,PLCPack2SN);
            }
            /*RFIDlist.removeFirst();
        }
        else
        {
            qDebug()<<"rfidlist is empty";
        }
        }
        else
        {
            moduleCount ++;
            getModuleRecordFinishFlag = true;
        }*/
        statusValidFlag = true;
        break;
    case 18:
        tellRollerBedReadyFlag = true;
//        getRollerBedReadyFlag = true;
        statusValidFlag = true;
        emit sendReadCmdToRFID("0000","0015");//read RFID addr 0000 len 15
        break;
    case 20:
        tellModuleMoveFinishFlag = true;
        emit sendReqModuleCount();
        getModuleMoveFinishFlag = true;
        statusValidFlag = true;
        break;
    case 21:
        reqModuleTypeFlag = true;
        emit sendReqModule();
        getModuleTypeFlag = true;
        statusValidFlag = true;
        break;
    case 25:
        reqBindingSNFlag = true;
        emit sendBoundPack(); //A010
        ackBoundPackFlag = true;
        statusValidFlag = true;
        reqBoundAddr = PLCStepMap["writeAdd"].toInt();
        break;
    case 26://请求下箱体位置号
        reqUpdownBoxPositionFLag = true;
        getUpdownBoxPositionFlag = true;
        statusValidFlag = true;
        break;
    case 27://请求下箱体条码扫描
        statusValidFlag = true;
        reqScanBarcodeFlag = true;
        //sao ma
        emit sendStartBarcode() ;
        curStepNum = num;
        //        getScanBarcodeFlag = true;
        //        statusValidFlag = true;
        break;
    case 29:
        getTrayType = true;
    case 28://请求条码数据绑定
        //while(!GIsInterfaceUpSuccess)
        {
            reqUpdownBoxRecordDataFlag = true;
            getUpdownBoxRecordDataFlag = true;
            if(scanResult == 172)
            {
                qDebug()<<"28error";
            }
            else
            {
                if(strBoxCode != "Scan Error"){
                    if(pack12Num == 1)
                    {
                        emit sendReqKeybarcode(Line_ID,1,GscanRegExpCode,strBoxCode,PLCPack1SN,Station);
                    }
                    else if(pack12Num == 2)
                    {
                        emit sendReqKeybarcode(Line_ID,2,GscanRegExpCode,strBoxCode,PLCPack2SN,Station);
                    }
                    else//pack12Num = 3
                    {
                        if(!workSecondePackFlag)
                        {
                            emit sendReqKeybarcode(Line_ID,1,GscanRegExpCode,strBoxCode,PLCPack1SN,Station);
                        }
                        else
                        {
                            emit sendReqKeybarcode(Line_ID,2,GscanRegExpCode,strBoxCode,PLCPack2SN,Station);
                        }
                    }
                }
            }
            statusValidFlag = true;
            //sleep(1);
            //continue;
        }
        //GIsInterfaceUpSuccess = false;
        break;
    case 30://收到PLC放行托盘信号进入站完成请求PCL放行托盘
        getPLCMoveTrayFlag = true;
        trayProtectFlag = false;
//        qDebug()<<"this is rfidCode"<<RFIDCode<<GTrayStatus;
        if(Station == "VM010-1" || Station == "VM010-2")
        {
            statusValidFlag = true;
//            codeNum = 0;  //更改为计数清零
            QString DataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            emit sendReqStationState(RFIDCode,GTrayStatus,"","",DataTime);
        }
//        if(Station == "OP80")
//        {
//            qDebug()<<"this is bmce sendModuleTrayClean"<<RFIDCode;
//            emit sendBMCETrayClean(RFIDCode,GPackSN);//上传接口解绑托盘
//        }
        if(RFIDCode == "P-xxx")
        {
            qDebug()<<"this is rfidcode"<<RFIDCode;
        }
        else
        {
            QString DataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            if(Station.mid(0,2) == "VM")
            {
                int Type = 2;//出站
                emit sendUpInAndOutStation(Type,RFIDCode,"",Station,DataTime);//小车出站发到interface上传接口
//                emit sendReqStationState(RFIDCode,GTrayStatus,"","",DataTime);
            }
        }
//                statusValidFlag = true;
        break;
    case 50:
        break;
    default:
        statusValidFlag = false;
        return false;
        break;
    }
    writeAddr = PLCStepMap["writeAdd"].toInt();//获取收到PLC的数据对应的Dat写入的DWB
    return true;
}


void ConnectPLC::recvMasterReadRfid()
{
    if(!isReadRfidFlag)
    {
        qDebug()<<"master read RFID";
        emit sendReadCmdToRFID("0000","0015");
    }
}

void ConnectPLC::reSendReqModulePack()
{
    if(!workSecondePackFlag)
    {
        qDebug()<<"this is send moudle pack to http first111"<<RFIDCode<<PLCPack1SN;
        emit sendReqModulePack(RFIDCode,PLCPack1SN);
    }
    else
    {
        qDebug()<<"this is send moudle pack to http111"<<RFIDCode<<PLCPack2SN;
        emit sendReqModulePack(RFIDCode,PLCPack2SN);
    }
}

void ConnectPLC::revGluePackStatus(QString gluePackStatus)
{
    if(gluePackStatus == "2")
    {
        sendNOStationFlag = true;
        qDebug()<<"gluePackStatusErrorFinishFlag"<<stationFinishAddr;
//        writePLCFlag = true;
        strWriteBufTemp[0] = 0;
        strWriteBufTemp[1] = 1;
        writePLCFunc(stationAddr,106,2,strWriteBufTemp);
        usleep(200000);
        strWriteBufTemp[0] = 0;
        strWriteBufTemp[1] = 1;
        writePLCFunc(stationAddr,stationFinishAddr,2,strWriteBufTemp);
    }
}

void ConnectPLC::recDutouTightStatus1(QString recMessage)
{
   // QString dotouPositionNum = "";
    if(recMessage.size() == 8)
    {
        dutouStatusPosition = true;
        dotouPositionNum1 = "";
        if(recMessage.mid(0,1) == "1" && recMessage.mid(1,1) == "1")
        {
            dotouPositionNum1.append("1");
        }
        if(recMessage.mid(2,1) == "1" && recMessage.mid(3,1) == "1")
        {
            dotouPositionNum1.append("2");
        }
        if(recMessage.mid(4,1) == "1" && recMessage.mid(5,1) == "1")
        {
            dotouPositionNum1.append("3");
        }
        if(recMessage.mid(6,1) == "1" && recMessage.mid(7,1) == "1")
        {
            dotouPositionNum1.append("4");
        }

        if(recMessage == "00000000" || dotouPositionNum1.isEmpty())
        {
            getStationErrorFinishFlag = true;
        }
    }
    else
    {
        qDebug()<<"this is recMessage is error";
    }
    qDebug()<<"dotouPositionNum1"<<dotouPositionNum1<<recMessage;
}

void ConnectPLC::recDutouTightStatus2(QString recMessage)
{
    if(recMessage.size() == 8)
    {
        dutouStatusPosition = true;
        dotouPositionNum2 = "";
        if(recMessage.mid(0,1) == "1" && recMessage.mid(1,1) == "1")
        {
            dotouPositionNum2.append("1");
        }
        if(recMessage.mid(2,1) == "1" && recMessage.mid(3,1) == "1")
        {
            dotouPositionNum2.append("2");
        }if(recMessage.mid(4,1) == "1" && recMessage.mid(5,1) == "1")
        {
            dotouPositionNum2.append("3");
        }if(recMessage.mid(6,1) == "1" && recMessage.mid(7,1) == "1")
        {
            dotouPositionNum2.append("4");
        }
        if(recMessage == "00000000" || dotouPositionNum2.isEmpty())
        {
            getStationErrorFinishFlag = true;
        }
    }
    else
    {
        qDebug()<<"this is recMessage is error";
    }
    qDebug()<<"dotouPositionNum2"<<dotouPositionNum2<<recMessage;
}

void ConnectPLC::recReadyClicked()
{
    if(Station.mid(0,5) == "VM444")
    {
        qDebug()<<"onClickedReadyPlcMessage"<<GrecvStatusMsee;
        strWriteBufTemp[0] = 0;
        strWriteBufTemp[1] = 0;
        strWriteBufTemp[2] = 0;
        strWriteBufTemp[3] = 1;
        writePLCFunc(stationAddr,100,4,strWriteBufTemp);
    }

}
void ConnectPLC::recgetStationTimer(int time)
{
    qDebug()<<"recgetStationTimer"<<time<<GGlueTime;   //与张宁写重了biw
    if(qAbs(time) > GGlueTime)
    {
        sendNOStationFlag = true;
        strWriteBufTemp[0] = 0;
        strWriteBufTemp[1] = 1;
        writePLCFunc(stationAddr,106,2,strWriteBufTemp);
        usleep(200000);
        strWriteBufTemp[0] = 0;
        strWriteBufTemp[1] = 1;
        writePLCFunc(stationAddr,stationFinishAddr,2,strWriteBufTemp);
    }
//    QString timer = time.toString();
//    timer = "=="+timer;
//    QByteArray vinTemp = timer.toLatin1();
//    for(int i = 0; i < vinTemp.size(); i++)
//    {
//        strWriteBufTemp[i] = (unsigned char)vinTemp[i];
//    }
//    writeLen = vinTemp.size();
//    writePLCFunc(stationAddr,150,writeLen,strWriteBufTemp);
}

void ConnectPLC::revPackStatus(QVariant packStatus)
{
    PACK_STATUS statusStruct;
    statusStruct = packStatus.value<PACK_STATUS>();
    qDebug()<<"connectplc statusStruct"<<statusStruct.Result<<statusStruct.Errorcode;
    if( (reqProductTypeFlag)||(reqBindingSNFlag) )
    {
        reqProductTypeFlag = false;
        reqBindingSNFlag = false;
        tmpPackNum = 0;
        if(statusStruct.Result)
        {
            PLCPackType = statusStruct.PackType;//product type
            QString stepPackTypeTemp;
            if(PLCPackType == "1")
            {
                stepPackTypeTemp = "PHEV";
            }
            else
            {
                stepPackTypeTemp = "BEV";
            }

            if(GPackType == stepPackTypeTemp)
            {

            }
            else
            {
                qDebug()<<"this is PLCPackType"<<stepPackTypeTemp;
                GPackType = stepPackTypeTemp;
                stepInit();
                statusValidFlag = true;
            }
            PLCPackAGVSN = statusStruct.AGVSN;
            PLCPack1SN = statusStruct.Pack1SN;
            PLCPack2SN = statusStruct.Pack2SN;
            PLCPack1Status = statusStruct.Pack1Status;
            PLCPack2Status = statusStruct.Pack2Status;
            if(writeToPlcPack1SNAddr == 0)
            {
                qDebug()<<"this is error write packSN to plc";
            }
            else
            {
                 writePackSNToPlc(PLCPack1SN,PLCPack2SN);
            }
            if(Station == "A160")
            {
                if(channel_Num == 1)
                {
                    emit sendReqDutouTightStatus(PLCPackAGVSN,"A140A");
                }
                else if(channel_Num == 2)
                {
                    emit sendReqDutouTightStatus(PLCPackAGVSN,"A150");
                }
                else
                {
                    qDebug()<<"channel_Num@@@@@@@@"<<channel_Num;
                }
            }
            if(Station.mid(0,4) == "A350" && (!PLCPack1SN.isEmpty()) && PLCPackType == "2")
            {
                qDebug()<<"getStationTimer";
                emit sendgetStationTimer(PLCPack1SN,2,"M325A");
            }
            if(Station == "A350-2" || Station == "A350-1")
            {
                if(PLCPack1SN.isEmpty())
                {
                    qDebug()<<"skipEmpty";
                }
                else
                {
                    emit sendGlueWorkFlag(PLCPack1SN);
                }
            }
            vinCode = PLCPack1SN;
            if(Station == "A400A" || Station == "A400B")
            {
                vinCode = PLCPack2SN;
                emit sendStationFinishToMaindow(vinCode,"22");
            }
            else if(Station == "A390A" || Station == "A390B")
            {
                vinCode = PLCPack1SN;
                emit sendStationFinishToMaindow(vinCode,"21");
            }
//            vinCode = PLCPack1SN;
//            emit sendStationFinishToMaindow(vinCode,"21");
            //*************************只做pack包是ok的************************
            if(!PLCPack1SN.isEmpty() && PLCPack1Status == "1")
            {
                tmpPackNum += 1;
            }
            if(!PLCPack2SN.isEmpty() && PLCPack2Status == "1")
            {
                tmpPackNum += 2;
            }
//            if((Station == "A390A" || Station == "A390B") &&(!PLCPack1SN.isEmpty()) && PLCPack1Status == "1")
            if((Station == "A390A" || Station == "A390B"))
            {
                if((!PLCPack1SN.isEmpty()) && PLCPack1Status == "1")
                {
                    pack12Num = 1;
                }
                else
                {
                    pack12Num = 0;
                }
                qDebug()<<"this is A390 station"<<pack12Num;
//                pack12Num = 1;
//                qDebug()<<"this is A390 station"<<pack12Num;
            }
//            else if((Station == "A400A" || Station == "A400B") &&(!PLCPack2SN.isEmpty()) && PLCPack2Status == "1")
            else if((Station == "A400A" || Station == "A400B"))
            {
                if((!PLCPack2SN.isEmpty()) && PLCPack2Status == "1")
                {
                    pack12Num = 1;
                }
                else
                {
                    pack12Num = 0;
                }
                qDebug()<<"this is A400 station"<<pack12Num;
//                pack12Num = 1;
//                qDebug()<<"this is A400 station"<<pack12Num;
            }
            else
            {
                pack12Num = tmpPackNum;
                qDebug()<<"this is other station"<<pack12Num;
            }
            if(Station.mid(0,4) == "A090" && plcStopFlag)
            {
                plcStopFlag = false;
                moduleBindCount = 0;
                cleanBandWvent();
                sleep(2);
                handleBandEvent();
            }
            qDebug() << "$$$$$$$$$$ PLC product type::"<<PLCPackType<<PLCPack1SN<<PLCPack1Status<<PLCPack2SN<<PLCPack2Status<<tmpPackNum<<pack12Num;
            changePackResult();
            pack1StatusWriteFlag = true;
            if(writeProductTypeFlag)
            {
                qDebug()<<"this is ackCodeHandle";
                ackCodeHandle("0");
            }
           /*if(PLCPack1Status == "OK" && PLCPack2Status == "OK")
            {
                qDebug()<<"this two pack is all ok ";
                ackCodeHandle("0");
            }
            else
            {
                qDebug()<<"this two pack is bot all ok";
                QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
                sleep(3);
                emit sendReqStationState(PLCPack1SN,"3",PLCPack2SN,"3",time.toString("yyyy-MM-dd"));
            }*/
        }
        else
        {
            qDebug() << "request PackStatus fail ErrorCode:" <<statusStruct.Errorcode;
//            pack1Result = 3;
//            pack2Result = 3;
//            pack1StatusWriteFlag = true;
//            getStationErrorFinishFlag = true;
//            emit sendStationFinishToMaindow("AGV强制放行","204");
            if(statusStruct.Errorcode.isEmpty())
            {
                PLCPackType = "101";
            }
            else
            {
                PLCPackType = statusStruct.Errorcode;
            }
            getGlueProductTypeFlag = true;
//            statusValidFlag = true;
        }
    }
}

void ConnectPLC::revFixtureStatus(bool isSuccess)
{
    if(isSuccess)
    {
        qDebug()<<"fixtureholdrevtrue";
        fixtureHoldFlag = true;
    }
}


void ConnectPLC::revModulePack(bool isSuccess)
{
    if(isSuccess)
    {
        qDebug()<<"post ModulePack success";
        moduleBoundResult = 1;
        strWriteBufTemp[0] = 0;
        strWriteBufTemp[1] = moduleBoundResult;
        writePLCFunc(stationAddr,(166+(moduleBindCount*2)),2,strWriteBufTemp);
        moduleBindCount ++;
        moduleCount ++;
    }
    else
    {
        qDebug()<<"post ModulePack fail";
        moduleBoundResult = 101;
        strWriteBufTemp[0] = 0;
        strWriteBufTemp[1] = moduleBoundResult;
        writePLCFunc(stationAddr,(166+(moduleBindCount*2)),2,strWriteBufTemp);
    }
    usleep(100000);
    getModuleRecordFinishFlag = true;
}

//void ConnectPLC::revKeybarcode(bool isSuccess)
//{
//    if(isSuccess)
//    {
//        qDebug()<<"post Keybarcode success";
//    }
//    else
//        qDebug()<<"post Keybarcode fail";
//}

//void ConnectPLC::revModule(bool isSuccess, QString module)
//{
//    if(isSuccess)
//    {
//        qDebug()<<"request module success"<<module;
//    }
//    else
//        qDebug()<<"request Module fail";
//}

void ConnectPLC::revStationState(bool isSuccess)
{
    if(isSuccess)
    {
        qDebug()<<"auto post StationState success";
    }
    else
        qDebug()<<"auto post StationState fail";
}

//void ConnectPLC::revModuleCount(bool isSuccess)
//{
//    if(isSuccess)
//    {
//        qDebug()<<"post ModuleCount success";
//    }
//    else
//        qDebug()<<"post ModuleCount fail";
//}
/*********************************************/
//receive box barcode
/*********************************************/
void ConnectPLC::revBoxCodeFunc(QString buf)
{
    qDebug()<<"connect tiaoma qiang";
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString regexpVM11 = "12121212";
    QString regexpVM22 = "1212";
    strBoxCode = buf;
    if(buf == "Scan Error")
    {
        scanResult = 171;
    }
    else
    {
        scanResult = 1;
    }
    if(strBoxCode != strBoxCodeTemp)
    {
        strBoxCodeTemp = strBoxCode;
        strBoxCodeFlag = true;
    }
    else
    {
        qDebug()<<"this is equ scanBuff";
    }
    if(strBoxCodeFlag)
    {
        if(Station == "VM010-1")
        {
            if(regexpVM11.mid(codeNum,1) == "1")
            {
                if(strBoxCode.contains(regExpVm1))
                {
                    strBoxCoderegExpFlag = true;
                }
                else
                {
                    qDebug()<<"this is vm1 scan regexp1 error"<<strBoxCode<<regExpVm1;
                    scanResult = 172;
                }
            }
            else if(regexpVM11.mid(codeNum,1) == "2")
            {
                if(strBoxCode.contains(regExpVm2))
                {
                    strBoxCoderegExpFlag = true;
                }
                else
                {
                    qDebug()<<"this is vm1 scan regexp2 error"<<strBoxCode<<regExpVm2;
                    scanResult = 172;
                }
            }
            else
            {
                qDebug()<<"regexpVM11 error";
            }
        }
        if(Station == "VM010-2")
        {
            if(regexpVM22.mid(codeNum,1) == "1")
            {
                if(strBoxCode.contains(regExpVm1))
                {
                    strBoxCoderegExpFlag = true;
                }
                else
                {
                    qDebug()<<"this is vm2 scan regexp1 error"<<strBoxCode<<regExpVm1;
                    scanResult = 172;
                }
            }
            else if(regexpVM22.mid(codeNum,1) == "2")
            {
                if(strBoxCode.contains(regExpVm2))
                {
                    strBoxCoderegExpFlag = true;
                }
                else
                {
                    qDebug()<<"this is vm2 scan regexp2 error"<<strBoxCode<<regExpVm2;
                    scanResult = 172;
                }
            }
            else
            {
                qDebug()<<"regexpVM22 error";
            }
        }
//        if(Station == "VM010-1" || Station == "VM010-2")
//        {
//            if(regexpVM1.mid(codeNum,1) == "1")
//            {
//                if(strBoxCode.contains(regExpVm1))
//                {
//                    strBoxCoderegExpFlag = true;
//                }
//                else
//                {
//                    qDebug()<<"this is vm scan regexp1 error"<<strBoxCode<<regExpVm1;
//                    scanResult = 172;
//                }
//            }
//            else if(regexpVM2.mid(codeNum,1) == "2")
//            {
//                if(strBoxCode.contains(regExpVm2))
//                {
//                    strBoxCoderegExpFlag = true;
//                }
//                else
//                {
//                    qDebug()<<"this is vm scan regexp2 error"<<strBoxCode<<regExpVm2;
//                    scanResult = 172;
//                }
//            }
//        }
        if(strBoxCode.size() > 46)
        {
            if(Station == "VM010-1")
            {
                longFlag = handScanMessageLong(regexpVM11.mid(codeNum,1),strBoxCode);
            }
            else if(Station == "VM010-2")
            {
                longFlag = handScanMessageLong(regexpVM22.mid(codeNum,1),strBoxCode);
            }
            else if(Station == "A010")
            {
                qDebug()<<"stationIsA010";
                longFlag = handScanMessageLongA010(strBoxCode);
                shortFlag = false;
            }
            else
            {
                qDebug()<<"stationIsNotVM010";
                longFlag = true;
                shortFlag = true;
            }
        }
        else
        {
            if(Station.mid(0,5) == "VM010")
            {
                shortFlag = handScanMessageShort(strBoxCode);
            }
            else
            {
                longFlag = true;
                shortFlag = true;
            }
        }
        if(longFlag || shortFlag)
        {

        }
        else
        {
            scanResult = 172;
        }
    }
    getScanBarcodeFlag = true;
    statusValidFlag = true;
    PLCStepMap = PLCstepList[curStepNum].toMap();
    writeAddr = PLCStepMap["writeAdd"].toInt();

    if(Station == "VM010-1" && strBoxCodeFlag && strBoxCoderegExpFlag)
    {
        strBoxCodeFlag = false;
        strBoxCoderegExpFlag = false;
        int codeCount = 8;
        if(codeNum >=codeCount)
        {
            codeNum = 0;
            scanCount = 0;
        }
        scanList.append(strBoxCode);
        qDebug()<<"scanList"<<scanList.size()<<scanList<<codeCount;
        if(scanList.size() >= codeCount)
        {
            for(int i = 0;i < scanList.size();i++)
            {
                QString strCode = scanList.at(i);
//                emit sendReqKeybarcode(strCode,RFIDCode,time.toString("yyyy-MM-dd hh:mm:ss"),1);
                usleep(100000);
            }
            scanList.clear();
        }
        if(codeNum != 0)
        {
//            emit sendReqKeybarcode(strBoxCode,RFIDCode,time.toString("yyyy-MM-dd hh:mm:ss"),1);
        }
        else
        {
            getScanBarcodeVM010Flag = true;
        }
        codeBuf[codeNum] = strBoxCode;
        codeNum++;
        if(codeNum == codeCount)
        {
            codeNum = 0;
            qDebug()<<"this is send Vm010-1 bind"<<codeCount;
            scanListLong.clear();
            scanListShort.clear();
            if(codeCount == 16)
            {
                 emit sendReqModuleTray(vinCode,codeBuf[0],codeBuf[1],codeBuf[2],codeBuf[3],codeBuf[4],codeBuf[5],codeBuf[6],codeBuf[7],codeBuf[8],codeBuf[9],codeBuf[10],codeBuf[11],codeBuf[12],codeBuf[13],codeBuf[14],codeBuf[15]);
            }
            else if(codeCount == 8)
            {
                 emit sendReqModuleTray(RFIDCode,codeBuf[0],codeBuf[1],codeBuf[2],codeBuf[3],codeBuf[4],codeBuf[5],codeBuf[6],codeBuf[7]);
            }
            else
            {
                qDebug()<<"codeCountError"<<codeCount;
            }
        }
        scanCount++;
        strWriteBufTemp[0] = 0;
        strWriteBufTemp[1] = scanCount;
        writePLCFunc(stationAddr,112,2,strWriteBufTemp);
        if(scanCount >= codeCount)
        {
            scanCount = 0;
        }
    }
    if(Station == "VM010-2" && strBoxCodeFlag && strBoxCoderegExpFlag)
    {
        strBoxCodeFlag = false;
        strBoxCoderegExpFlag = false;
        int codeCount = 4;
        if(codeNum >= codeCount)
        {
            codeNum = 0;
            scanCount = 0;
        }
        scanList.append(strBoxCode);
        qDebug()<<"scanList"<<scanList.size()<<scanList<<codeCount;
        if(scanList.size() >= codeCount)
        {
            qDebug()<<"scanList!!!";
            for(int i = 0;i < scanList.size();i++)
            {
                QString strCode = scanList.at(i);
                qDebug()<<"strCode"<<strCode;
//                emit sendReqKeybarcode(strCode,RFIDCode,time.toString("yyyy-MM-dd hh:mm:ss"),1);
                usleep(100000);
            }
            scanList.clear();
        }
        if(codeNum != 0)
        {
//            emit sendReqKeybarcode(strBoxCode,RFIDCode,time.toString("yyyy-MM-dd hh:mm:ss"),1);
        }
        else
        {
            getScanBarcodeVM010Flag = true;
        }
        codeBuf[codeNum] = strBoxCode;
        codeNum++;
        if(codeNum == codeCount)
        {
            codeNum = 0;
            qDebug()<<"this is send Vm010-2 bind"<<codeCount;
            scanListLong.clear();
            scanListShort.clear();
            if(codeCount == 8)
            {
                emit sendReqModuleTray(RFIDCode,codeBuf[0],codeBuf[1],codeBuf[2],codeBuf[3],codeBuf[4],codeBuf[5],codeBuf[6],codeBuf[7]);
            }
            else if(codeCount == 4)
            {
                codeBuf[4] = "";
                codeBuf[5] = "";
                codeBuf[6] = "";
                codeBuf[7] = "";
                emit sendReqModuleTray(RFIDCode,codeBuf[0],codeBuf[1],codeBuf[2],codeBuf[3],codeBuf[4],codeBuf[5],codeBuf[6],codeBuf[7]);
//                emit sendReqModuleTray(RFIDCode,codeBuf[0],codeBuf[1],codeBuf[2],codeBuf[3]);
            }
            else
            {
                qDebug()<<"codeCountError"<<codeCount;
            }
        }
        scanCount++;
        strWriteBufTemp[0] = 0;
        strWriteBufTemp[1] = scanCount;
        writePLCFunc(stationAddr,112,2,strWriteBufTemp);
        if(scanCount >= codeCount)
        {
            scanCount = 0;
        }
    }
    else
    {
        qDebug()<<"strBoxCodeFlag"<<strBoxCodeFlag<<"strBoxCoderegExpFlag"<<strBoxCoderegExpFlag<<"Station"<<Station;
    }
//    if(Station == "VM010-1" || Station == "VM010-2")
//    {
//        emit sendReqKeybarcode(strBoxCode,RFIDCode,time.toString("yyyy-MM-dd hh:mm:ss"),1);
//        if(codeNum >= 8)
//        {
//            codeNum = 0;
//        }
//        codeBuf[codeNum] = strBoxCode;
//        codeNum ++;
//        if(codeNum == 8)
//        {
//            qDebug()<<"this is send Vm010 bind";
//            emit sendReqModuleTray(RFIDCode,codeBuf[0],codeBuf[1],codeBuf[2],codeBuf[3],codeBuf[4],codeBuf[5],codeBuf[6],codeBuf[7]);
//        }
//    }
}
/*********************************************/
//server ack error code handel
/*********************************************/
void ConnectPLC::ackCodeHandle(QString type)
{
    if(ackBoundPackFlag)
    {
        ackBoundPackFlag = false;
        getBindingSNFlag = true;
        if(type == "0")
        {
            ackCode = 1;
        }
        else
        {
            ackCode = 101;
        }
    }
    else if(ackProductTypeFlag)
    {
        qDebug()<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
        ackProductTypeFlag = false;
        getProductTypeFlag = true;
        writeProductFinishFlag = false;
        if(type == "0")
        {
            ackCode = 1;
        }
        else
        {
            ackCode = 101;
        }
    }
    qDebug()<<"**********123456**********"<<type<<ackCode;
}


void ConnectPLC::recvTrayCleanResult()
{
    qDebug()<<"send type == 1";
    QString DataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    if(Station.mid(0,2) == "VM")
    {
        int Type = 1;//进站
        emit sendUpInAndOutStation(Type,RFIDCode,"",Station,DataTime);//小车进站发到interface上传接口
    }
}

/*********************************************/
//receive RFID ACK
/*********************************************/
void ConnectPLC::revRFIDAck(QString errorCode, QByteArray code)
{
    qDebug()<<"read RFID ACK"<<errorCode<<code<<reqProductTrayFlag;
    isReadRfidFlag = false;
    if(errorCode == "0000")
    {
        if(reqProductTrayFlag)
        {
            updataTraySN = code;
            reqProductTrayFlag = false;
        }
    }
    if(tellTrayReadyFlag)
    {
        tellTrayReadyFlag = false;
        if(errorCode == "0000")//RFID read OK
        {
            RFIDCode = code;
            qDebug()<<"$$$$$updataTraySN"<<updataTraySN<<RFIDCode;
            if(updataTraySN.isEmpty())
            {
                updataTraySN = RFIDCode;
            }
            else
            {
                if(updataTraySN == RFIDCode)
                {

                }
                else
                {
                    updataTrayTemp = RFIDCode;
                }
            }
            if((Station == "VM010-1" || Station == "VM010-2") && (RFIDCodeTemp != RFIDCode))
            {
                RFIDCodeTemp = RFIDCode;
//                codeNum = 0;  //更改为计数清零
                qDebug()<<"this is sendModuleTrayClean"<<RFIDCode;
                emit sendModuleTrayClean(RFIDCode);
            }
            if(RFIDCodeTemp1 != RFIDCode)
            {
                getRFIDCodeVM010Flag = true;
                RFIDCodeTemp1 = RFIDCode;
                trayProtectFlag = true;
                qDebug()<<"this is new Tray";
                QString DataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                if(Station.mid(0,2) == "VM")
                {
                    int Type = 1;//进站
                    emit sendUpInAndOutStation(Type,RFIDCode,"",Station,DataTime);//小车进站发到interface上传接口
                }
            }

            if(Station.mid(0,2) == "OP")
            {
                emit sendBMCEFirstStation(RFIDCode);
            }
            if(Station == "OP80")//把托盘号发给PLC
            {
                QByteArray vinTemp = RFIDCode.toLatin1();
                for(int i = 0; i < vinTemp.size(); i++)
                {
                    strWriteBufTemp[i] = (unsigned char)vinTemp[i];
                }
                writeLen = vinTemp.size();
                writePLCFunc(stationAddr,150,writeLen,strWriteBufTemp);
            }
            GRFIDMessage = code;
            readRFIDResult = 1;
            if(RFIDCode.mid(0,1) == "P")
            {
                GrfidStatusTemp = 1;
            }
            else if(RFIDCode.mid(0,1) == "B")
            {
                GrfidStatusTemp = 2;
            }
            else
            {
                qDebug()<<"this is rfid error";
                GrfidStatusTemp = 3;
            }
            emit sendCmdToStep(6,RFIDCode,15);
        }
        else if(errorCode == "0001")//RFID no connect
        {
            readRFIDResult = 160;
            GrfidStatusTemp = 160;
//            RFIDCode = "disconnect";
            GEKSReRFID = true;
            tellTrayReadyFlag = true;
            if(!trayProtectFlag)
            {
                emit sendStationFinishToMaindow("RFID未连接","204");
            }
            emit sendReadCmdToRFID("0000","0015");//请求 RFID 数据
            //emit sendStates("RFID未连接");
        }
        else if(errorCode == "0002")//RFID no read data / over 3S to timeout
        {
            readRFIDResult = 161;
            GrfidStatusTemp =161;
//            RFIDCode = "timeOut";
            GEKSReRFID = true;
            tellTrayReadyFlag = true;
            if(!trayProtectFlag)
            {
                emit sendStationFinishToMaindow("RFID读取超时","204");
            }
            emit sendReadCmdToRFID("0000","0015");//请求rfid数据
           // emit sendStates("RFID读取超时");
        }
        else//RFID read error data
        {
            readRFIDResult = 162;
            GrfidStatusTemp = 162;
//            RFIDCode = "otherError";
            GEKSReRFID = true;
            tellTrayReadyFlag = true;
            if(!trayProtectFlag)
            {
                emit sendStationFinishToMaindow("RFID连接未知错误","204");
            }
            emit sendReadCmdToRFID("0000","0015");//请求rfid数据
            //emit sendStates("RFID连接未知错误");
        }
//        GrfidStatusTemp = readRFIDResult;
        getTrayReadyFlag = true;
//        emit sendCmdToStep(6,"P-002",15);
//        emit sendCmdToStep(6,RFIDCode,15);
    }
    else if(tellRollerBedReadyFlag)
    {
        tellRollerBedReadyFlag = false;
        if(errorCode == "0000")//read OK
        {
            RFIDCode = code;
            /*if( ((PLCPackType == "1")&&(RFIDCode.mid(0,1) == "P")) ||
                ((PLCPackType == "2")&&(RFIDCode.mid(0,1) == "B")) )
            {
                readRFIDResult = 1;
            }
            else
            {
                readRFIDResult = 114;
            }*/
            if(RFIDCode.mid(0,1) == "P")
            {
                readRFIDResult = 1;
            }
            else if(RFIDCode.mid(0,1) == "B")
            {
                readRFIDResult = 2;
            }
            else
            {
                qDebug()<<"this is rfid error";
                readRFIDResult = 3;
            }
        }
        else if(errorCode == "0001")//RFID no connect
        {
            readRFIDResult = 160;
        }
        else if(errorCode == "0002")//RFID no read data / over 3S to timeout
        {
            readRFIDResult = 161;
        }
        else//RFID read error data
        {
            readRFIDResult = 162;
        }
        getRollerBedReadyFlag = true;
    }
}

void ConnectPLC::revCS351Status(bool)
{

}

//重新读取rfid
void ConnectPLC::revRFIDRead()
{
    qDebug()<<"this is maindow send RFID Read";
    //statusValidFlag = false;
}

/**************************************
 * read pack status
***************************************/
int ConnectPLC::readPackStatus(int numValue,int size)
{
    pack12Num = numValue/size; //pack number
    numValue = numValue%size;//work times per pack
    if(pack12Num == 1)//first pack
    {
        pack1Result = 4;
        pack2Result = 3;
    }
    else if(pack12Num == 2)//second pack
    {
        pack1Result = 3;
        pack2Result = 4;
    }
    else if(pack12Num == 3)//two pack
    {
        pack1Result = 4;
        pack2Result = 4;
    }
    else if(pack12Num == 0)
    {
        pack1Result = 3;
        pack2Result = 3;
    }
    qDebug()<<":pack1Result::"<<pack1Result<<":pack2Result::"<<pack2Result<<":pack12Num::"<<pack12Num;
    return numValue;
}

void ConnectPLC::changePackResult()
{
    qDebug()<<"this is GPackType"<<GPackType<<"GBevPass"<<GBevPass;
    if(GPackType == GBevPass)
    {
        pack1Result = 3;
        pack2Result = 3;
        packUplodeTemp1 = "3";
        packUplodeTemp2 = "3";
        getStationErrorFinishFlag = true;
        writeProductTypeFlag = false;
        emit sendStationFinishToMaindow("AGV强制放行","204");
        qDebug()<<"this is bev getStationErrorFinishFlag";
    }
    else
    {
        if(pack12Num == 1)//first pack
        {
            pack1Result = 4;
            pack2Result = 3;
            writeProductTypeFlag = true;
        }
        else if(pack12Num == 2)//second pack
        {
            pack1Result = 3;
            pack2Result = 4;
            writeProductTypeFlag = true;
        }
        else if(pack12Num == 3)//two pack
        {
            pack1Result = 4;
            pack2Result = 4;
            writeProductTypeFlag = true;
        }
        else if(pack12Num == 0 || (GPackType == "BEV" && GBevPass == "Yes"))
        {
            if(Station == "A350-3" || Station == "A350-4")
            {
                if(PLCPack1SN.isEmpty())
                {
                    pack1Result = 3;
                    pack2Result = 3;
                    getStationErrorFinishFlag = true;
                    writeProductTypeFlag = false;
                    emit sendStationFinishToMaindow("AGV放行","220");
                }
                else
                {
                    pack1Result = 4;
                    pack2Result = 4;
                    writeProductTypeFlag = true;
                }
            }
            else
            {
                pack1Result = 3;
                pack2Result = 3;
                getStationErrorFinishFlag = true;
                writeProductTypeFlag = false;
                emit sendStationFinishToMaindow("AGV放行","220");
                qDebug()<<"this is getStationErrorFinishFlag";
            }
        }}
    qDebug()<<"this is pack1result"<<pack1Result<<pack2Result<<GBevPass;
}

void ConnectPLC::errorHandle()
{
    if(glueStartWorkFlag)
    {
        for(int i=0;i<8;i++) //clear valid step value
        {
            strWriteBufTemp[i] = 0;
        }
        writePLCFunc(stationAddr,100,6,strWriteBufTemp);
        usleep(200000);
        writePLCFunc(stationAddr,114,6,strWriteBufTemp);
    }
    else
    {
        //qDebug()<<"glueStartWorkFlag"<<glueStartWorkFlag;
        writePLCFlag = true;
        writeLen = (PLCWorkAllStep<<1)-2;
        for(int i=0;i<writeLen;i++) //clear valid step value
        {
            strWriteBuf[i] = 0;
        }
        writeAddr = 100;
    }
}

void ConnectPLC::writePackSNToPlc(QString pack1sn, QString pack2sn)
{
    if(pack1sn.isEmpty())
    {
        pack1sn = "pack1SN=NULL";
    }
    if(pack2sn.isEmpty())
    {
        pack2sn = "pack2SN=NULL";
    }
//    QString packSN = "=="+pack1sn+"<>"+pack2sn;
//    QByteArray vinTemp = packSN.toLatin1();
//    for(int i = 0; i < vinTemp.size(); i++)
//    {
//        strWriteBufTemp[i] = (unsigned char)vinTemp[i];
//    }
//    writeLen = vinTemp.size();
//    writePLCFunc(stationAddr,writeToPlcPackSNAddr,writeLen,strWriteBufTemp);
    pack1sn = "=="+pack1sn;
    QByteArray vinTemp = pack1sn.toLatin1();
    for(int i = 0; i < vinTemp.size(); i++)
    {
        strWriteBufTemp[i] = (unsigned char)vinTemp[i];
    }
    writeLen = vinTemp.size();
    writePLCFunc(stationAddr,writeToPlcPack1SNAddr,writeLen,strWriteBufTemp);
    usleep(200000);
    pack2sn = "=="+pack2sn;
    vinTemp = pack2sn.toLatin1();
    for(int i = 0; i < vinTemp.size(); i++)
    {
        strWriteBufTemp[i] = (unsigned char)vinTemp[i];
    }
    writeLen = vinTemp.size();
    writePLCFunc(stationAddr,writeToPlcPack2SNAddr,writeLen,strWriteBufTemp);
}

void ConnectPLC::cleanBandWvent()
{
    strWriteBufTemp[0] = 0;
    strWriteBufTemp[1] = 0;
    strWriteBufTemp[2] = 0;
    strWriteBufTemp[3] = 0;
    strWriteBufTemp[4] = 0;
    strWriteBufTemp[5] = 0;
    strWriteBufTemp[6] = 0;
    strWriteBufTemp[7] = 0;
    strWriteBufTemp[8] = 0;
    strWriteBufTemp[9] = 0;
    strWriteBufTemp[10] = 0;
    strWriteBufTemp[11] = 0;
    strWriteBufTemp[12] = 0;
    strWriteBufTemp[13] = 0;
    strWriteBufTemp[14] = 0;
    strWriteBufTemp[15] = 0;
    writePLCFunc(stationAddr,158,16,strWriteBufTemp);
}

void ConnectPLC::handleBandEvent()
{
    strWriteBufTemp[0] = 0;
    strWriteBufTemp[1] = 1;
    strWriteBufTemp[2] = 0;
    strWriteBufTemp[3] = 1;
    strWriteBufTemp[4] = 0;
    strWriteBufTemp[5] = 1;
    strWriteBufTemp[6] = 0;
    strWriteBufTemp[7] = 1;
    if(GPackType == "PHEV")
    {
        if(pack12Num == 1)
        {
            length = 2;
        }
        else if(pack12Num == 2)
        {
            length = 2;
        }
        else if(pack12Num == 3)
        {
            length = 4;
        }
        else if(pack12Num == 0)
        {
            length = 0;
        }
        else
        {
            qDebug()<<"PHEVpack12NumError";
        }
    }
    else if(GPackType == "BEV")
    {
        if(pack12Num == 1)
        {
            length = 8;
        }
        else if(pack12Num == 0)
        {
            length = 0;
        }
        else
        {
            qDebug()<<"BEVpack12NumError";
        }
    }
    else
    {
        qDebug()<<"GPackTypeError";
    }
    if(length == 0)
    {
        qDebug()<<"lengthiIsError";
    }
    else
    {
        writePLCFunc(stationAddr,158,length,strWriteBufTemp);
    }
}

void ConnectPLC::cleanAllDW()
{
    qDebug()<<"cleanAllDW";
    cleanAllDWFlag = false;
    if(Station == "A350-1" || Station == "A350-2")
    {
//        writePLCFlag = true;
        //writeLen = (GwhichStepNum<<1);
        for(int i=0;i<8;i++) //clear valid step value
        {
            strWriteBufTemp[i] = 0;
        }
        writePLCFunc(stationAddr,100,8,strWriteBufTemp);
        usleep(200000);
        writePLCFunc(stationAddr,114,8,strWriteBufTemp);
    }
    else
    {
        writeLen = PLCWorkAllStep<<1;
        for(int i=0;i<writeLen;i++) //clear valid step value
        {
            strWriteBuf[i] = 0;
        }
        writeAddr = 100;
        writePLCFunc(stationAddr,writeAddr,writeLen,strWriteBuf);
    }
}

bool ConnectPLC::handScanMessageLongA010(QString strBuf)
{
    bool datediffFlag = false;
    bool vactionRegexpFlag = false;
    if(strBuf.contains(VariantDiff))
    {
        vactionRegexpFlag = true;
    }
    else
    {
        vactionRegexpFlag = true;
    }
    if(dataDiff.isEmpty() && dataDiff1.isEmpty())
    {
        datediffFlag = true;
    }
    else
    {
        if(strBuf.size() >46)
        {
            bool thinkingTimeFlag = true;
            QDateTime localtime = QDateTime::currentDateTime();
            QDateTime timeTemp;
            QString aaa = "20";
            QString bbb = strBuf.mid(27,6); //从哪一位开始是时间
            if(strBuf.mid(0,4) == "#5QD")
            {
                bbb = strBuf.mid(30,6);
            }else if(strBuf.mid(0,10) == "#5NG804842"){
                bbb = strBuf.mid(27,6);
            }else{
                thinkingTimeFlag = false;
                datediffFlag = true;
            }
            QString day = bbb.mid(4,2);
            QString month = bbb.mid(2,2);
            QString year = aaa.append(bbb.mid(0,2));
            QString datebuff = year+"-"+month+"-"+day;
            timeTemp = QDateTime::fromString(datebuff, "yyyy-MM-dd");
            int timediff = timeTemp.daysTo(localtime);
            qDebug()<<"timeTemp"<<timeTemp<<datebuff<<timediff<<localtime;
//            if(timediff < GdateDiff)   //时间间隔判断
//            {
//                datediffFlag = true;
//            }
//            else
//            {
//                datediffFlag = false;
//            }
            if(thinkingTimeFlag){
                if(dataDiff.isEmpty())   //检测时间点之前的时间
                {
                    QDateTime RightTime = QDateTime::fromString(dataDiff1,"yyyy-MM-dd");
                    int RightTimeDiff = timeTemp.daysTo(RightTime);
                    qDebug()<<"RightTime"<<RightTime;
                    if(RightTimeDiff <= 0)
                    {
                        datediffFlag = false;
                    }
                    else
                    {
                        datediffFlag = true;
                    }
                }
                else if(dataDiff1.isEmpty())
                {
                    QDateTime leftTime = QDateTime::fromString(dataDiff,"yyyy-MM-dd");
                    int RightTimeDiff = timeTemp.daysTo(leftTime);
                    qDebug()<<"leftTime"<<leftTime;
                    if(RightTimeDiff < 0)
                    {
                        datediffFlag = true;
                    }
                    else
                    {
                        datediffFlag = false;
                    }
                }
                else
                {
                    QDateTime RightTime = QDateTime::fromString(dataDiff1,"yyyy-MM-dd");
                    QDateTime leftTime = QDateTime::fromString(dataDiff,"yyyy-MM-dd");
                    qDebug()<<"leftTime"<<leftTime<<"RightTime"<<RightTime;
                    int RightTimeDiff = timeTemp.daysTo(RightTime);
                    int leftTimeDiff = timeTemp.daysTo(leftTime);
                    int timeTemp = leftTime.daysTo(RightTime);
                    if(timeTemp < 0)
                    {
                        if(RightTimeDiff < 0 && leftTimeDiff > 0)
                        {
                            datediffFlag = false;
                        }
                        else
                        {
                            datediffFlag = true;
                        }
                    }
                    else
                    {
                        if(RightTimeDiff > 0 && leftTimeDiff < 0)
                        {
                            datediffFlag = true;
                        }
                        else
                        {
                            datediffFlag = false;
                        }
                    }
                }
            }
        }
        else
        {
            datediffFlag = true;
        }
    }
    if(datediffFlag && vactionRegexpFlag)
    {
        return true;
    }
    else
    {

        if(!datediffFlag)
        {
            emit sendStationFinishToMaindow("时间校验失败","301");
        }
        else if(!vactionRegexpFlag)
        {
            emit sendStationFinishToMaindow("软件版本检验失败","301");
        }
        return false;
    }
}

bool ConnectPLC::handScanMessageLong(QString scanType, QString strBuf)
{
    bool datediffFlag = false;
    bool vactionRegexpFlag = false;
    QStringList ccc = VariantDiff.split(",");
    if(ccc.isEmpty())
    {
        vactionRegexpFlag = true;
    }
    else
    {
        if(ccc.size()>=2 && strBuf.size()>46)
        {
            qDebug()<<"vactionRegexpFlag"<<strBuf.mid(21,5);
            if(scanType == "1")
            {
                if(ccc[0] == strBuf.mid(21,5))
                {
                    vactionRegexpFlag = true;
                }
                else
                {
                    vactionRegexpFlag = false;
                }
            }
            else if(scanType == "2")
            {
                if(ccc[1] == strBuf.mid(21,5))
                {
                    vactionRegexpFlag = true;
                }
                else
                {
                    vactionRegexpFlag = false;
                }
            }
        }
        else
        {
            vactionRegexpFlag = true;
        }
    }
    if(dataDiff.isEmpty() && dataDiff1.isEmpty())
    {
        datediffFlag = true;
    }
    else
    {
        if(strBuf.size() >46)
        {
            QDateTime localtime = QDateTime::currentDateTime();
            QDateTime timeTemp;
            QString aaa = "20";
            QString bbb = strBuf.mid(38,8); //从哪一位开始是时间
            QString day = bbb.mid(0,2);
            QString month = bbb.mid(3,2);
            QString year = aaa.append(bbb.mid(6,2));
            QString datebuff = year+"-"+month+"-"+day;
            timeTemp = QDateTime::fromString(datebuff, "yyyy-MM-dd");
            int timediff = timeTemp.daysTo(localtime);
            qDebug()<<"timeTemp"<<timeTemp<<datebuff<<timediff<<localtime;
//            if(timediff < GdateDiff)   //时间间隔判断
//            {
//                datediffFlag = true;
//            }
//            else
//            {
//                datediffFlag = false;
//            }
            if(dataDiff.isEmpty())   //检测时间点之前的时间
            {
                QDateTime RightTime = QDateTime::fromString(dataDiff1,"yyyy-MM-dd");
                int RightTimeDiff = timeTemp.daysTo(RightTime);
                qDebug()<<"RightTime"<<RightTime;
                if(RightTimeDiff <= 0)
                {
                    datediffFlag = false;
                }
                else
                {
                    datediffFlag = true;
                }
            }
            else if(dataDiff1.isEmpty())
            {
                QDateTime leftTime = QDateTime::fromString(dataDiff,"yyyy-MM-dd");
                int RightTimeDiff = timeTemp.daysTo(leftTime);
                qDebug()<<"leftTime"<<leftTime;
                if(RightTimeDiff < 0)
                {
                    datediffFlag = true;
                }
                else
                {
                    datediffFlag = false;
                }
            }
            else
            {
                QDateTime RightTime = QDateTime::fromString(dataDiff1,"yyyy-MM-dd");
                QDateTime leftTime = QDateTime::fromString(dataDiff,"yyyy-MM-dd");
                qDebug()<<"leftTime"<<leftTime<<"RightTime"<<RightTime;
                int RightTimeDiff = timeTemp.daysTo(RightTime);
                int leftTimeDiff = timeTemp.daysTo(leftTime);
                int timeTemp = leftTime.daysTo(RightTime);
                if(timeTemp < 0)
                {
                    if(RightTimeDiff < 0 && leftTimeDiff > 0)
                    {
                        datediffFlag = false;
                    }
                    else
                    {
                        datediffFlag = true;
                    }
                }
                else
                {
                    if(RightTimeDiff > 0 && leftTimeDiff < 0)
                    {
                        datediffFlag = true;
                    }
                    else
                    {
                        datediffFlag = false;
                    }
                }
            }
        }
        else
        {
            datediffFlag = true;
        }
    }
    if(datediffFlag && vactionRegexpFlag)
    {
        bool scanIsSamed = false;
        if(scanListLong.isEmpty())
        {
            scanListLong.append(strBuf);
        }
        else
        {
            for(int i = 0;i < scanListLong.size(); i++)
            {
                if(scanListLong[i] == strBuf)
                {
                    qDebug()<<"scan1IsSame1";
                    scanIsSamed = true;
                }
            }
            if(scanIsSamed)
            {
                qDebug()<<"scan1IsSame2";
            }
            else
            {
                scanListLong.append(strBuf);
            }
        }
        if(Station == "VM010-1")
        {
            if(scanListLong.size() > 8)
            {
                qDebug()<<"PscanListLongIsError";
                scanIsSamed = true;
                emit sendStationFinishToMaindow("同类条码超出要求","301");
            }
        }
        else if(Station == "VM010-2")
        {
            if(scanListLong.size() > 4)
            {
                qDebug()<<"BscanListLongIsError";
                scanIsSamed = true;
                emit sendStationFinishToMaindow("同类条码超出要求","301");
            }
        }
        else
        {
            qDebug()<<"shortPackTypeError";
            scanIsSamed = true;
        }
        return !scanIsSamed;
    }
    else
    {

        if(!datediffFlag)
        {
            emit sendStationFinishToMaindow("时间校验失败","301");
        }
        else if(!vactionRegexpFlag)
        {
            emit sendStationFinishToMaindow("软件版本检验失败","301");
        }
        return false;
    }
}


bool ConnectPLC::handScanMessageShort(QString strBuf)
{
    bool scanIsSamed = false;
    if(scanListShort.isEmpty())
    {
        scanListShort.append(strBuf);
    }
    else
    {
        for(int i = 0;i < scanListShort.size(); i++)
        {
            if(scanListShort[i] == strBuf)
            {
                qDebug()<<"scan1IsSame1";
                scanIsSamed = true;
            }
        }
        if(scanIsSamed)
        {
            qDebug()<<"scan1IsSame2";
        }
        else
        {
            scanListShort.append(strBuf);
        }
    }
    if(Station == "VM010-1")
    {
        if(scanListShort.size() > 8)
        {
            qDebug()<<"PscanListShortIsError";
            scanIsSamed = true;
            emit sendStationFinishToMaindow("同类条码超出要求","301");
        }
    }
    else if(Station == "VM010-2")
    {
        if(scanListShort.size() > 4)
        {
            qDebug()<<"BscanListShortIsError";
            scanIsSamed = true;
            emit sendStationFinishToMaindow("同类条码超出要求","301");
        }
    }
    else
    {
        qDebug()<<"shortPackTypeError";
        scanIsSamed = true;
    }
    return !scanIsSamed;
}

//void ConnectPLC::reModuleClean( )
//{
//}


void ConnectPLC::slot_trayReleaseFunc()
{
    //OP80托盘解绑成功后处理“站完成请求PCL放行托盘”
    reqPLCMoveTrayFlag = true;//告诉PLC放行托盘
    PLCStepMap = PLCstepList[moveTrayStep].toMap();//提取“站完成请求PCL放行托盘”步骤的配置信息
    writeAddr = PLCStepMap["writeAdd"].toInt();//获取写入PLC的DBW
    statusValidFlag = false;
}

void ConnectPLC::readPLCData(QString receMessage)
{
    plcRecvFlag = true;
    aliveCount = 0;
    int revLen = receMessage.size()/2;//25+DBWRLen
    int i = 0;
    qDebug()<<"receMessage:"<<receMessage;
    if(22 == revLen){//第一次握手回复的
#if DYTEST5
        if(receMessage.mid(0,14) == "0300001611d000"){
#else
        if(receMessage.mid(8,10) == "11d0002e00"){
#endif
            qDebug()<<"ack-first";//<<receMessage;
            ctlPLCHandle(startCmdBuf2,true);//发送第二次握手指令
        }else{//写指令的返回
            if(receMessage.mid(21*2,2)=="ff"){//写入数据回复的“成功” 成功FF，失败05
                qDebug()<<"ack-w:";//<<sendWriteDataTemp<<endl<<receMessage;
                plcAckFlag = true;
                if(clearPLCFlag)
                {
                    clearPLCFlag = false;
                    writePLCFlag = false;
                    statusValidFlag = false;
                }
                else if(writePLCFlag)
                {
                    writePLCFlag = false;
                    getTightenPositionFlag = false;
                    getGluePositionFlag = false;
                    getGlueBindSuccess = false;
                    getRobotTightenReadyFlag = false;
                    getBoxPositionFlag = false;
                    getBoxFinishFlag = false;
                    getTightenFinishFlag = false;
                    getTightErrorFlag = false;
                    getDistroyStatus = false;
                    //                getStationErrorFinishFlag = false;
                    getBoundInforFlag = false;
                    getDutoupositionFlag = false;
                    getDutouRecordFinishFlag = false;
                    getTrayReadyFlag = false;
                    getModuleBoxNumFlag = false;
                    getRfidUpDowmFlag = false;
                    getModuleRecordFinishFlag = false;
                    fixtureHoldFlag = false;
                    getRollerBedReadyFlag = false;
                    getUpdownBoxPositionFlag = false;
                    getScanBarcodeFlag = false;
                    getUpdownBoxRecordDataFlag = false;
                    getTrayType = false;
                    tellModuleMoveFinishFlag = false;
                    getModuleTypeFlag = false;
                    getModuleMoveFinishFlag = false;
                    getGlueProductTypeFlag = false;
                    if(pack1StatusFlag)
                    {
                        pack1StatusFlag = false;
                        pack1StatusWriteFlag = false;
                    }
                    else//请求产品型号时，要写入pack状态，再写入产品型号
                    {
                        if(writeProductFinishFlag)
                        {
                            getProductTypeFlag = false;
                        }
                        //                    getProductTypeFlag = false;
                        getBindingSNFlag = false;
                    }
                    if(startWorkFlag)
                    {
                        qDebug()<<"this is #################### startWorkFlag"<<startWorkFlag;
                        startWorkFlag = false;
                        startWorkWriteFlag = true;
                    }
                    if(getStationFinishFlag)
                    {
                        getStationFinishFlag = false;
                        statusValidFlag = false;
                    }
                    reqPLCMoveTrayFlag = false;
                    getPLCMoveTrayFlag = false;
                }
            }else if(receMessage.mid(21*2,2)=="05"){
                qDebug()<<"write fail";//<<receMessage.size()<<receMessage;
            }else{
                qDebug()<<"write error";//<<receMessage.size()<<receMessage;
            }
        }
    }else if(27 == revLen){//第二次握手回复
//        if(receMessage == "0300001b02f08032030000ffff000800000000f0000003000300f0"){
        if(receMessage.mid(0,26) == "0300001b02f08032030000ffff"){
            qDebug()<<"ack-second";
            plcLinkFlag = true;
            plcAckFlag = true;
            emit sendPLCConnect(plcLinkFlag);
        }
    }else if(revLen == 25 + DBWRLen){//读指令的返回
        //curServerCycleID = GCycleId;//数据正常交互可以赋值新的循环号
        if(receMessage.mid(21*2,2)=="ff"){//"ff" is success
            qDebug()<<"read PLC success1:"<<receMessage.size()/2 - 25;
            if(taskList.size() > 0){
                qDebug()<<"checkPLCTaskCout1:"<<taskList.size();//数据待验证条数
                for(int i = 0;i<taskList.size();i++){
                    qDebug()<<"checkPLCTaskData1:"<<i<<taskList.at(i).first<<taskList.at(i).second.first<<taskList.at(i).second.second;
                }
                //多条任务时,整理任务数据列表，更新数据
                if(taskList.size() > 1){
//                    QList<int>dbwListA;
//                    QList<int>dbwListB;
                    for(int i = 0;i < taskList.size();i++){
                        QPair<int,QPair<int,QByteArray> >taskPairA;
                        taskPairA = taskList.at(i);
                        int dbwA = taskPairA.first;
                        int lenA = taskPairA.second.first;
                        QList<int>dbwListA;
                        for(int c = 0;c < lenA/2;c++){//任务列表左边一个中包含的所有dbw加入dbwListA
                            dbwListA<<dbwA+2*c;
                        }
                        for(int j = i+1;j < taskList.size();j++){
                            QPair<int,QPair<int,QByteArray> >taskPairB;
                            taskPairB = taskList.at(j);
                            int dbwB = taskPairB.first;
                            int lenB = taskPairB.second.first;
                            QList<int>dbwListB;
                            for(int c = 0;c < lenB/2;c++){//任务列表右边一个中包含的所有dbw加入dbwListB
                                dbwListB<<dbwB+2*c;
                            }
                            int dbwRepeatIndexA = -1;//记录重叠A的位置
                            int dbwRepeatIndexB = -1;//记录重叠B的位置
                            for(int r = 0;r < dbwListB.size();r++){//检查是否有重叠DBW
                                if(dbwListA.contains(dbwListB.at(r))){
                                    dbwRepeatIndexB = r;
                                    if(r == 0){
                                        dbwRepeatIndexA = dbwListA.indexOf(dbwListB.at(r));
                                    }else if(r > 0){
                                        dbwRepeatIndexA = dbwListA.indexOf(dbwListB.at(dbwListB.size() - 1));
                                    }
//                                    qDebug()<<"A&&B value:"<<r<<dbwListA<<dbwListB;
//                                    qDebug()<<"A&&B index:"<<dbwRepeatIndexA<<dbwRepeatIndexB;
                                    break;
                                }else{
                                    continue;
                                }
                            }
                            //处理重叠的项 以B项为准
                            if(dbwRepeatIndexB > -1){
                                if(dbwListA.first() < dbwListB.first()){
                                    if(dbwListA.at(dbwListA.size() - 1) < dbwListB.at(dbwListB.size() - 1)){//A尾与B头重叠
                                        qDebug()<<"type[1]";
                                        taskPairB.first = taskPairA.first;
                                        taskPairB.second.second.insert(0,taskPairA.second.second.mid(0,dbwRepeatIndexA*2*2));
                                        taskPairB.second.first = taskPairB.second.second.size()/2;
                                    }else if(dbwListA.at(dbwListA.size() - 1) == dbwListB.at(dbwListB.size() - 1)){//A包B 尾重叠
                                        qDebug()<<"type[2]";
                                        taskPairB.first = taskPairA.first;
                                        taskPairB.second.second.insert(0,taskPairA.second.second.mid(0,dbwRepeatIndexA*2*2));
                                        taskPairB.second.first = taskPairB.second.second.size()/2;
                                    }else if(dbwListA.at(dbwListA.size() - 1) > dbwListB.at(dbwListB.size() - 1)){//A包B
                                        qDebug()<<"type[3]";
                                        taskPairB.first = taskPairA.first;
                                        taskPairB.second.second.insert(0,taskPairA.second.second.mid(0,dbwRepeatIndexA*2*2));
                                        taskPairB.second.second.append(taskPairA.second.second.mid((dbwRepeatIndexA*2+taskPairB.second.first)*2,
                                                                                                   (taskPairA.second.first-dbwRepeatIndexA*2-taskPairB.second.first)*2));
                                        taskPairB.second.first = taskPairB.second.second.size()/2;
                                    }
                                }else if(dbwListA.first() == dbwListB.first()){
                                    if(dbwListA.at(dbwListA.size() - 1) < dbwListB.at(dbwListB.size() - 1)){//B包A 头重叠
                                        qDebug()<<"type[4]";
                                        //不需要处理
                                    }else if(dbwListA.at(dbwListA.size() - 1) == dbwListB.at(dbwListB.size() - 1)){//AB相等
                                        qDebug()<<"type[5]";
                                        //不需要处理
                                    }else if(dbwListA.at(dbwListA.size() - 1) > dbwListB.at(dbwListB.size() - 1)){//A包B 头重叠
                                        qDebug()<<"type[6]";
                                        taskPairB.second.second.append(taskPairA.second.second.mid(taskPairB.second.first*2,
                                                                                                   (taskPairA.second.first-taskPairB.second.first)*2));
                                        taskPairB.second.first = taskPairB.second.second.size()/2;
                                    }
                                }else if(dbwListA.first() > dbwListB.first()){
                                    if(dbwListA.at(dbwListA.size() - 1) < dbwListB.at(dbwListB.size() - 1)){//B包A
                                        qDebug()<<"type[7]";
                                        //不需要处理
                                    }else if(dbwListA.at(dbwListA.size() - 1) == dbwListB.at(dbwListB.size() - 1)){//B包A 尾重叠
                                        qDebug()<<"type[8]";
                                        //不需要处理
                                    }else if(dbwListA.at(dbwListA.size() - 1) > dbwListB.at(dbwListB.size() - 1)){//A头与B尾重叠
                                        qDebug()<<"type[9]";
                                        taskPairB.second.second.append(taskPairA.second.second.mid(dbwRepeatIndexA*2*2,
                                                                                                   (taskPairA.second.first-dbwRepeatIndexA*2)*2));
                                        taskPairB.second.first = taskPairB.second.second.size()/2;
                                    }
                                }
                                taskList.replace(j,taskPairB);
                                taskList.removeAt(i);
                                break;
                            }
                        }
                    }
                }
                qDebug()<<"checkPLCTaskCout2:"<<taskList.size();//数据待验证条数
                for(int i = 0;i<taskList.size();i++){
                    qDebug()<<"checkPLCTaskData2:"<<i<<taskList.at(i).first<<taskList.at(i).second.first<<taskList.at(i).second.second;
                }
                //每次交互循环只验证一条
                int dbw = taskList.first().first;
                int len = taskList.first().second.first;
                QByteArray writeBuf = taskList.first().second.second;
                QByteArray recWriteBuf = receMessage.mid(25*2+dbw*2,len*2).toLocal8Bit().data();
                if(writeBuf == recWriteBuf){
                    qDebug()<<"checkSuccess:"<<dbw<<len<<writeBuf;
                    countRepeatWrite = 0;
                    taskList.removeFirst();
                }else{
                    qDebug()<<"checkFail:"<<dbw<<len<<writeBuf<<recWriteBuf;
                    writePLCFlag = true;//打开写入PLC开关
                    isRepeatWritePLC = true;
                    countRepeatWrite++;
                    writeAddr = dbw;//写入的DBW
                    writeLen = len;//写入的长度
                    for(int i = 0;i<len;i++){
                        strWriteBuf[i] = writeBuf.mid(2*i,2).toInt(0,16);
                    }
                    if(countRepeatWrite >= 2){//重发2次不成功也删除该任务
                        countRepeatWrite = 0;
                        taskList.removeFirst();
                    }
                }
            }


            if(reqPLCMoveTrayFlag)//告诉PLC放行托盘
            {
                return;
            }

            if(statusValidFlagTemp != statusValidFlag)
            {
                statusValidFlagTemp = statusValidFlag;
                qDebug()<<"statusValidFlag change:"<<statusValidFlag<<i;
            }
            if(mannulNum != 99)
            {
                //                    if((unsigned char)recBuf[26+(mannulNum<<1)] == 1)
                if(qint16(receMessage.mid(25*2+2*(2*mannulNum),4).toInt(0,16)) == 1)
                {
                    mannulModeFlag = 1;//mannul mode
                }
                else
                {
                    mannulModeFlag = 0;//auto mode
                }
            }
            if(Station == "VM050A" || Station == "VM050D")
            {
                GrecvStatusMsee = receMessage.toLocal8Bit().data();//.mid(58,4);//转换PLC的传感器信号
                //qDebug()<<"GrecvStatusMsee[w]:"<<GrecvStatusMsee<<getInt<<recBuf.toHex();
            }

            //encoder
            GrecvStatusMsee = receMessage.toLocal8Bit().data();
            trayIsAction = receMessage.mid(82,2).toInt();
            trayBindCount = receMessage.mid(90,2).toInt();
            GTightStatus = receMessage.mid((receMessage.size() - 6),2).toInt();
            if(readMesTemp.mid(25*2,100*2) != receMessage.mid(25*2,100*2)){//PLC写入字段有变化
                for(i=0;i<PLC_AllStep;i++){
                    //遍历本工位的PLC步骤交互列表
                    int lastVa = readMesTemp.mid(25*2+2*(2*i),4).toInt(0,16);
                    int currentVa = receMessage.mid(25*2+2*(2*i),4).toInt(0,16);
                    PLCStepMap = PLCstepList[i].toMap();
                    int type = parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString());
                    if(lastVa == 0 && currentVa > 0 && !statusValidFlag)//收到PLC发来的数据对应步骤的字段有值
                    {
                        readMesTemp = receMessage;
                        qDebug()<<"@GTightResetOnline@"<<i<<GTightResetOnline;
                        if(GTightResetOnline == "Yes")//拧紧返修 == "Yes"
                        {
                            if(i == 2)
                            {
                                qDebug()<<"this is i equ 2";
                                if(GgetRobotTightenReadyFlag)
//                                if(PLCstepList.at(i).toMap()["singleStep"].toString() == "机器人到达拧紧位置")
                                {
                                    qDebug()<<"this is GrecErrorResult && SYSS"<<GrecErrorResult<<SYSS;//true "Ready"
                                    if(GrecErrorResult && SYSS != "ING")
                                    {
                                        GrecErrorResult = false;
                                        if(!GErrorChannel.isEmpty())//上次拧紧全是不合格的 GErrorChannel:记录了不合格的通道号
                                        {
                                            statusValidFlag = true;
                                            qDebug()<<"tighten is no success";
                                            qDebug()<<"this is send Gnoknumber"<<GErrorChannel<<GErrorChannel.size();
                                            GErrorTight = true;
                                            GBoltOkNum = 0;
                                            GBoltAllNum = 0;
                                            for(int i = 0;i<GErrorChannel.size();i++)
                                            {
                                                qDebug()<<"emit sendOperate(true,GErrorChannel.mid(i,1));";
                                                emit sendOperate(true,GErrorChannel.mid(i,1));//不合格的通道发给拧紧线程给拧紧机发报文
//                                                sleep(1);
                                            }
                                            //emit sendOperate(true,GNokNumber.toInt());
                                            //                                                GsendEnableCount = GErrorChannel.size();//记录本次不合格螺栓数量
                                            GErrorChannel = "";
                                            GNokNumber = "";
                                            break;
                                        }
                                        else//上次拧紧全是合格的
                                        {
                                            if(packStatusTrueFlag)
                                            {
                                                qDebug()<<"tighten is success"<<i;//没见过有这里的打印
                                                if(handlePLCStepTypeFunc(i,type))
                                                {
                                                    GgetRobotTightenReadyFlag = false;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                                else
                                {
                                    qDebug()<<"this is first 3 step"<<i;
                                    if(handlePLCStepTypeFunc(i,type))
                                        break;
                                }
                            }
                            else
                            {
                                qDebug()<<"GTightResetOnline other step"<<i;
                                handlePLCStepTypeFunc(i,type);
                                break;
                            }
                        }
                        else
                        {
                            //第一步请求只能是1，其他位可以是大于1的值
                            if(currentVa > 1)
                            {
                                if(i == 0)
                                {
                                    qDebug()<<"this is error req";
                                }
                                else
                                {
                                    if(handlePLCStepTypeFunc(i,type))
                                        break;
                                }
                            }
                            else
                            {
                                if(handlePLCStepTypeFunc(i,type))
                                    break;
                            }
                        }
                    }
                    else if(lastVa > 0 && currentVa == 0 && statusValidFlag){
                        readMesTemp = receMessage;
                        qDebug()<<"clearPLCFlag:"<<i<<PLC_AllStep-1;
                        if(i < PLC_AllStep - 1 && type != 8){//只要不是最后一个步骤（一般是站完成放行）即做清除
                            clearPLCFlag = true;//打开清除标志位清除数据
                            if(Station == "A350-1" || Station == "A350-2")
                            {
                                if(i == 0)//A350的PLC产品型号清零时还有额外位要清零
                                {
                                    glueTightClearFlag = true;
                                }
                            }
                        }
                    }

//                    if(type == 0 || type == 8 ){
//                        statusValidFlag = false;
//                    }else if((type == 1 || type == 2 || type == 53 || type == 54 ||
//                             type == 3 || type == 4 || type == 5 || type == 6 ||
//                             type == 9 || type == 10 || type == 15 || type == 22 ||
//                             type == 16 || type == 37 || type == 17 || type == 18 ||
//                             type == 20 || type == 21 || type == 25 || type == 26 ||
//                             type == 27 || type == 28) ||
//                             ((Station == "VM010-1" || Station == "VM010-2") && type == 30)
//                             ){
//                        statusValidFlag = true;
//                    }
                }

            }
            //读到PLC有效标志位1(非站完成，清除数据，站完成请求PCL放行托盘，手动模式，备用绑定流水号，未定义交互信号等交互)
        }
        else if(receMessage.mid(21*2,2)=="05"){
            qDebug()<<"read fail"<<receMessage.size()<<receMessage;
        }else{
            qDebug()<<"read error"<<receMessage.size()<<receMessage;
        }
    }else{
        qDebug()<<"outher ask:"<<receMessage.size()<<receMessage;
    }
}
