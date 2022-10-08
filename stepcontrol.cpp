#include "stepcontrol.h"
#include "GlobalVarible.h"

StepControl::StepControl(QObject *parent) :
    QObject(parent)
{
    step_Thread.start();
    this->moveToThread(&step_Thread);
    CS351StatusFlag = false;
    vari1 = "";
    vari2 = "";
}
/*****************************/
//step thread init
/*****************************/
void StepControl::stepStart()
{
    qDebug()<<"step control thread start"<<vari1<<vari2;
    temp333 = false;
    allStep = 0;
    stepNum = 1;
    stepType = 0;
    packNum = 1;
    colorTrue = true;
    recCMDBuf = "";
    tightenResult = "";
    stepType1Flag = false;
    tightStartFlag = true;
    needReScaning = true;
    tighteningFinishFlag = false;
    getCodeFlag = true;
    isFirstPackflag = true;
    PLC_Handle_Flag = false;
    displayPictureFlag = true;
    allowPackType = false;
    AddStationTimerFlag = false;
    holdOnFlag = false;
    PDMShowfalg = false;
    holdOnCount = 0;
    displayCount = 0;
    codeNum = 0;
    scanIsSamed = false;
    timerCount = 5;
    timerCount1 = 0;
    timerCount2 = 0;
    startYellowFlash = true;
    startGreenFlash = true;
    stepPackType = "";//product type
    stepPack1SN = "P-001";
    stepPack2SN = "";
    bmceFrontStatus = "";
    stepPack1Status = "1";
    stepPack2Status = "";
    oldPackSN = "";
    m020ShowMaindownFlag = true;
    stepGetProductTypeFlag = false;
    getTrayPlaceReadyFlag = false;
    reqScanBarcodeFlag = true;
    keyStatusFlag = false;
    vinCode = "NULL";
    vinCodeTemp = "NULL";
    vinCodeTempVM = "NULL";
    trayLineFlag = false;
    trayReadyFlag = true;
    moveButtonFlag = false;
    stationFinishFlag = true;
    airTestFlag = true;
    airTestFinish = false;
    airProgram = 0;
    IO_Module_Value = 0;
    waitIOInputFlag = false;
    stopValueFlag = false;
    reqAckFlag = false;
    isPopUpFlag = false;
    sendStationFinishFlag = false;
    count1 = 0;
    stepList.clear();
    IOput = new InOutput(this);
    tempVariant = true;
    stepPack1SNTemp="AAAAAA";
    stepPack2SNTemp="AAAAAA";
    GPackTypeTemp = "PHEV";
    gostRunFlag=false;
    if(vari2 == "BJB")
    {
        inputModule *input_Module;
        input_Module = new inputModule;
        input_Module->IO_Module_IP = IOModuleIP;
        QTimer::singleShot(0,input_Module,SLOT(IO_Module_Init()));

        QObject::connect(input_Module,SIGNAL(sendIOValueToStep(int)),this,SLOT(revIOModuleValue(int)),Qt::AutoConnection);
    }
    //    connect(this,SIGNAL(sendReadIOModuleCmd(QString,bool)),IOput,SLOT(revReadIOCMD(QString,bool)));

    parsejson = new ParseJson;
//    QVariantMap tempMap = parsejson->parseJson();
//    if(tempMap["errorCode"] != 1)
//        stepParaBufInit(tempMap);
//    else
//        qDebug()<<"stepcontrol parsejson in Null";
    readStepParaInit();
    emit sendInitEncoder();

    connect(&timeFlicker, SIGNAL(timeout()), this, SLOT(timerflick()));
    QTimer::singleShot(5,this,SLOT(stepSelect()));//100ms
}
/*****************************/
//
/*****************************/
void StepControl::stepParaBufInit(QVariantMap Step)
{
    stepList = Step["steps"].toList();
    allStep = stepList.size();
    qDebug()<<"**********allStep num:"<<allStep;
}

/*****************************/
//read step parameter
/*****************************/
void StepControl::readStepParaInit()
{
    QVariantMap tempMap = parsejson->parseJson();
    if(tempMap["errorCode"] != 1)
        stepParaBufInit(tempMap);
    else
        qDebug()<<"stepcontrol parsejson in Null";
}

int StepControl::conditionToCode(QString condition)
{
    if(condition =="当前PACK_SN")
        return 1;
    else if(condition == "=")
        return 1;
    else if(condition == ">")
        return 2;
    else if(condition == "<")
        return 3;
    else
        return 0;
}
/*****************************/
//receive step function
/*****************************/
void StepControl::revCMDFunc(int step_type, QString strBuf,int num)
{
    qDebug()<<"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxx";
    qDebug()<<"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  rev cmd step:"<<step_type<<strBuf;
    qDebug()<<"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxx";
    switch (step_type) {
    case 0:
        if(strBuf == "connect")
        {
            PLCConnectFlag = true;
        }
        else if(strBuf == "disconnect")
        {
            PLCConnectFlag = false;
        }
    case 1:
        if(codeNum >= GScanTimes)
        {
            codeNum = 0;
        }
        if(strBuf.size() > 46)  //激光
        {
            if(scanList1.isEmpty())
            {
                scanList1.append(strBuf);
            }
            else
            {
                for(int i = 0;i < scanList1.size(); i++)
                {
                    if(scanList1[i] == strBuf)
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
                    scanList1.append(strBuf);
                }
            }
            if(vinCode.mid(0,1) == "P")
            {
                if(scanList1.size() > 4)
                {
                    qDebug()<<"PscanList1IsError";
                    scanIsSamed = true;
                    emit sendBarcodeDisplay("同类条码超出要求","301");
                }
            }
            else if(vinCode.mid(0,1) == "B")
            {
                if(scanList1.size() > 8)
                {
                    qDebug()<<"BscanList1IsError";
                    scanIsSamed = true;
                    emit sendBarcodeDisplay("同类条码超出要求","301");
                }
            }
            else if(Station == "M100B")
            {
                if(stepPackType == "1") //phev
                {
                    if(scanList1.size() > 8)
                    {
                        qDebug()<<"BscanList1IsError";
                        scanIsSamed = true;
                        emit sendBarcodeDisplay("同类条码超出要求","301");
                    }
                }
                else    //bev
                {
                    if(scanList1.size() > 16)
                    {
                        qDebug()<<"BscanList1IsError";
                        scanIsSamed = true;
                        emit sendBarcodeDisplay("同类条码超出要求","301");
                    }
                }
            }
            else
            {
                qDebug()<<"longPackTypeError";
//                scanIsSamed = true;   //20190108changed
            }
        }
        else    //印刷
        {
            if(scanList2.isEmpty())
            {
                scanList2.append(strBuf);
            }
            else
            {
                for(int i = 0;i < scanList2.size(); i++)
                {
                    if(scanList2[i] == strBuf)
                    {
                        qDebug()<<"scan2IsSame1";
                        scanIsSamed = true;
                    }
                }
                if(scanIsSamed)
                {
                    qDebug()<<"scan2IsSame2";
                }
                else
                {
                    scanList2.append(strBuf);
                }
            }
            if(vinCode.mid(0,1) == "P")
            {
                if(scanList2.size() > 4)
                {
                    qDebug()<<"PscanList2IsError";
                    scanIsSamed = true;
                    emit sendBarcodeDisplay("同类条码超出要求","301");
                }
            }
            else if(vinCode.mid(0,1) == "B")
            {
                if(scanList2.size() > 8)
                {
                    qDebug()<<"BscanList2IsError";
                    scanIsSamed = true;
                    emit sendBarcodeDisplay("同类条码超出要求","301");
                }
            }
            else if(Station == "M100B")
            {
                if(stepPackType == "1") //phev
                {
                    if(scanList2.size() > 8)
                    {
                        qDebug()<<"BscanList2IsError";
                        scanIsSamed = true;
                        emit sendBarcodeDisplay("同类条码超出要求","301");
                    }
                }
                else    //bev
                {
                    if(scanList2.size() > 16)
                    {
                        qDebug()<<"BscanList2IsError";
                        scanIsSamed = true;
                        emit sendBarcodeDisplay("同类条码超出要求","301");
                    }
                }
            }
            else
            {
                qDebug()<<"shortPackTypeError";
//                scanIsSamed = true;   //20190108changed
            }
        }
        if(scanList.isEmpty())
        {
            codeBuf[codeNum] = strBuf;
            codeNum ++;
            scanList.append(strBuf);
        }
        else
        {
            for(int i = 0;i < scanList.size(); i++)
            {
                if(scanList[i] == strBuf)
                {
                    qDebug()<<"scanIsSame1";
                    scanIsSamed = true;
                }
//                else
//                {
//                    codeBuf[codeNum] = strBuf;
//                    codeNum ++;
//                    scanList.append(strBuf);
//                }
            }
            if(scanIsSamed)
            {
                qDebug()<<"scanIsSame2";
                scanIsSamed = false;
                emit sendBarcodeDisplay("条码扫描重复！！！","301");
                PDMShowfalg = true;
            }
            else
            {

                codeBuf[codeNum] = strBuf;
                codeNum ++;
                scanList.append(strBuf);
                if(PDMShowfalg)
                {
                    PDMShowfalg = false;
                    emit sendBarcodeDisplay(BarcodePdm,"1");
                }
            }
        }
//        codeBuf[codeNum] = strBuf;
//        codeNum ++;
        break;
    case 2:
        tighteningFinishFlag = true;
        break;
    case 3:
        airTestFinish = true;
        break;
    case 6:
        if(num == 15)
        {
            getTrayPlaceReadyFlag = true;
            trayLineFlag = true;
            vinCode = strBuf;
        }
        break;
    default:
        break;
    }
}

/*****************************/
//stepType:1 = read car code
//         2 = tightening bolt and receive bolt tightening Data
//         3 = waitting IO input
//         4 = IO output
//         5 = jump
//         6 = PLC
//         7 = pictrue
/*****************************/
void StepControl::stepSelect()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    qDebug()<<"manual step"<<CS351StatusFlag;
//    if(vari2 == "XL")
//    {
//            emit sendLEDFlashAllOFF(0,"light_red",IOput->readIOInput("input5"));
//    }
    if( (allStep >= stepNum)&&(CS351StatusFlag) )
    {
        QVariantMap stepMap = stepList[stepNum-1].toMap();
        stepType = parsejson->stepNameToTypeCode(stepMap["singleStep"].toString());
        qDebug()<<"*stepNum*"<<stepNum <<"stepType:"<<stepType << stationFinishFlag<<"GWorkStatus"<<GWorkStatus;
        switch (stepType)
        {
        case 100://有挂起步骤
            //tighteningFinishFlag = false;
            if(stepMap["holdOn"].toString() == "Yes")
            {
                holdOnFlag = true;
            }
            else
            {
                holdOnFlag = false;
            }
            if(GisTightenGoOn)       //按了"继续"
            {
                stepNum++;
                GisTightenGoOn = 0;
                displayPictureFlag = true;
            }
            else
            {
                if(displayPictureFlag)
                {
                    qDebug()<<"holdOn steps"<<stepNum<<stepMap["PDM"].toString();
                    displayPictureFlag = false;
                    if(stepMap["PDM"].toString().isEmpty())
                    {
                        emit sendSuspend();
                    }
                    else
                    {
                        emit sendBarcodeDisplay(stepMap["PDM"].toString(),"7");//display picture
                        emit sendSuspend();
                    }
                }
            }
            //emit ;//需要在拧紧前暂停
            break;
        case 99:
            if(m020ShowMaindownFlag)
            {
                m020ShowMaindownFlag = false;
                sendM020ShowToMainwindow();
            }
            break;
        case 111:
            if(!handleJumpStation(stepMap["stationTrue"].toString()))   //该工位不合格
            {
                stepNum++;
            }
            else    //该工位合格
            {
                stepNum = stepMap["jumpStep"].toInt();
            }
        case 101://receive VIN
            if(isFirstPackflag)
            {
                if(stepMap["scanPass"].toString().contains(GPack1SN))
                {
                    allowPackType = true;
                }
                else
                {
                    allowPackType = false;
                }
            }
            else
            {
                if(stepMap["scanPass"].toString().contains(GPack2SN))
                {
                    allowPackType = true;
                }
                else
                {
                    allowPackType = false;
                }
            }
            if((stepMap["scanPass"].toString() == "All" || allowPackType) && needReScaning)
            {
                if(reqScanBarcodeFlag)
                {
                    reqScanBarcodeFlag = false;
                    stepType1Flag = false;
                    codeNum = 0;
                    scanList.clear();
                    scanList1.clear();
                    scanList2.clear();
                    BarcodePdm = stepMap["PDM"].toString();
                    emit sendBarcodeDisplay(stepMap["PDM"].toString(),"1"); //display please scanning
                }
    //            if ((stepMap["scanTimes"].toInt()==4)&&(codeNum>0))  //wz
    //            {
    //                qDebug()<<"************VM040*codeNum"<<codeNum;
    //                emit sendReqKeybarcode(codeBuf[codeNum],vinCode,time.toString("yyyy-MM-dd hh:mm:ss"),1);
    //            }
                GscanRegExp = stepMap["regexp"].toString();
                GscanRegExp2 = stepMap["regexp2"].toString();
                GdateDiff = stepMap["datediff"].toInt();
                GVRegexp = stepMap["VRegexp"].toString();
                GscanRegExpCode = stepMap["regexpCode"].toString();//条码匹配规则
                GScanTimes = stepMap["scanTimes"].toInt();
                qDebug()<<"GdateDiff"<<GdateDiff<<"GVRegexp"<<GVRegexp;
                if(codeNum >= stepMap["scanTimes"].toInt())
                {
                    qDebug()<<"************88888888*"<<codeNum<<GScanTimes;

                    reqScanBarcodeFlag = true;
//                    stepNum++;    //放早了会出问题
                    if(codeNum < GScanTimes)
                    {
                        qDebug()<<"point[21]"<<codeBuf[0];
                        emit sendReqKeybarcode(codeBuf[0],vinCode,time.toString("yyyy-MM-dd hh:mm:ss"),1);
                    }
                    else
                    {
                        for(int i = 0;i < GScanTimes;i++)
                        {
                            qDebug()<<"point[22]"<<codeBuf[i];
                            usleep(100000);
                            emit sendReqKeybarcode(codeBuf[i],vinCode,time.toString("yyyy-MM-dd hh:mm:ss"),1);
                        }
    //                    emit sendReqKeybarcode(codeBuf[0],vinCode,time.toString("yyyy-MM-dd hh:mm:ss"),1);
    //                    emit sendReqKeybarcode(codeBuf[1],vinCode,time.toString("yyyy-MM-dd hh:mm:ss"),1);
    //                    emit sendReqKeybarcode(codeBuf[2],vinCode,time.toString("yyyy-MM-dd hh:mm:ss"),1);
    //                    emit sendReqKeybarcode(codeBuf[3],vinCode,time.toString("yyyy-MM-dd hh:mm:ss"),1);
                        if(GScanTimes == 4)
                        {
                              emit sendReqModuleTray(vinCode,codeBuf[0],codeBuf[1],codeBuf[2],codeBuf[3]);
                        }
                        else if(GScanTimes == 8)
                        {
                            emit sendReqModuleTray(vinCode,codeBuf[0],codeBuf[1],codeBuf[2],codeBuf[3],codeBuf[4],codeBuf[5],codeBuf[6],codeBuf[7]);

                        }
                        else if(GScanTimes == 16)
                        {
                            emit sendReqModuleTray(vinCode,codeBuf[0],codeBuf[1],codeBuf[2],codeBuf[3],codeBuf[4],codeBuf[5],codeBuf[6],codeBuf[7],codeBuf[8],codeBuf[9],codeBuf[10],codeBuf[11],codeBuf[12],codeBuf[13],codeBuf[14],codeBuf[15]);
                        }

                    }
                    stepNum++;
                    codeNum = 0;
                    scanList.clear();
                    scanList1.clear();
                    scanList2.clear();
                }
            }
            else
            {
//                needReScaning = true;
                qDebug()<<"this is scan error"<<isFirstPackflag<<stepMap["scanPass"].toString()<<GPack1SN<<GPack2SN;
                stepNum++;
            }
            break;
        case 102://Tightening function
            if(isFirstPackflag)
            {
                if(stepMap["TightenPass"].toString().contains(GPack1SN))
                {
                    allowPackType = true;
                }
                else
                {
                    allowPackType = false;
                }
            }
            else
            {
                if(stepMap["TightenPass"].toString().contains(GPack2SN))
                {
                    allowPackType = true;
                }
                else
                {
                    allowPackType = false;
                }
            }
//            if(GPack1SN == stepMap["TightenPass"].toString()||
//                    GPack2SN == stepMap["TightenPass"].toString()||
//                    stepMap["TightenPass"].toString().isEmpty())
            if(stepMap["TightenPass"].toString() == "All" || allowPackType)
            {
                qDebug()<<"This is the model that needs tightening.";
                if(stepMap["isOFF"].toBool())
                {
                    stepNum++;
                    qDebug()<<"****** Tightening step OFF **********"<<stepNum;
                }
                else
                {
                    if(tightStartFlag)
                    {
                        GBoltOkNum = 0;
                        qDebug()<<"this is send GstepOkFlag";
                        tightStartFlag = false;
                    }
                    if(getCodeFlag)//receive code
                    {
                        GisTightFlag = true;
                        getCodeFlag = false;
                        emit sendCodeToTightening(vinCode,false,stepMap["Signature"].toString());
                    }
                    else if(tighteningFinishFlag)//receive tightening finish
                    {
                        if(isFirstPackflag)
                        {
                            qDebug()<<"this is step first pack status"<<GBoltOkNum;
                            if(GBoltOkNum < BoltTotalNum)
                            {
                                stepPack1Status = "2";
                            }
                            else
                            {
                                stepPack1Status = "1";
                            }
                            GTrayStatus = stepPack1Status;
                        }
                        else
                        {
                            qDebug()<<"this is step second pack status"<<GBoltOkNum;
                            if(GBoltOkNum < BoltTotalNum)
                            {
                                stepPack2Status = "2";
                            }
                            else
                            {
                                stepPack2Status = "1";
                            }
                        }
//                        GProgramIndex = 0;
                        getCodeFlag = true;
                        tightStartFlag = true;
                        tighteningFinishFlag = false;
                        GisTightFlag = false;
                        stepNum++;
                        qDebug()<<"****** Tightening finish step **********"<<stepNum;
                    }
                }
            }
            else
            {
                qDebug()<<"tight error"<<stepMap["TightenPass"].toString()<<GPack1SN<<isFirstPackflag<<GPack2SN;
                stepNum++;
            }
            break;
        case 103://wait IO input /read IO input status
            if(!waitIOInputFlag)
            {
                waitIOInputFlag = true;
                emit sendBarcodeDisplay("","4"); //display IO input
            }
            if(vari2 == "BJB")
            {
                int IO_Value_Temp = stepMap["IO"].toString().remove(0,5).toInt()-1;
                qDebug()<<"******************************IO_Module_Value:"<<IO_Module_Value<<IO_Value_Temp;
                if((IO_Module_Value>>IO_Value_Temp)&0x0001)
                {
                    stepNum++;
                    qDebug()<<"****** wait IO input step **********"<<stepNum;
                    waitIOInputFlag = false;
                }
            }
            else
            {
                if(IOput->readIOInput(stepMap["IO"].toString()) == stepMap["isTrue"].toBool())
                {
                    stepNum++;
                    qDebug()<<"****** wait IO input step **********"<<stepNum;
                    waitIOInputFlag = false;
                }
            }
            break;
        case 104://IO output
            IOput->writeIOOutput(stepMap["IO"].toString(),stepMap["isTrue"].toBool());
            stepNum++;
            qDebug()<<"****** IO output green step **********"<<stepNum;
            break;
        case 105://Jump
            if(jumpStepFunc(conditionToCode(stepMap["condition1"].toString()),
                            conditionToCode(stepMap["operator"].toString()),
                            stepMap["condition2"].toInt()))//conditon ok
            {
                stepNum = stepMap["jump"].toInt();
                qDebug()<<"****** jump step 222 **********"<<stepNum;
            }
            else
            {
                stepNum++;
                packNum = 1;
            }
            break;
        case 106://display PMD
            if(isFirstPackflag)
            {
                if(stepMap["PDMPass"].toString().contains(GPack1SN))
                {
                    allowPackType = true;
                }
                else
                {
                    allowPackType = false;
                }
            }
            else
            {
                if(stepMap["PDMPass"].toString().contains(GPack2SN))
                {
                    allowPackType = true;
                }
                else
                {
                    allowPackType = false;
                }
            }
            if(stepMap["PDMPass"].toString() == "All" || allowPackType)
            {
                if(displayPictureFlag)
                {
                    displayPictureFlag = false;
                    emit sendBarcodeDisplay(stepMap["PDM"].toString(),"7");//display picture
                }
                displayCount++;
                if(displayCount>stepMap["PDMShowTime"].toInt())//display 5000ms
                {
                    displayCount = 0;
                    displayPictureFlag = true;
                    stepNum++;
                }
            }
            else
            {
                qDebug()<<"PDM error"<<stepMap["PDMPass"].toString()<<GPack1SN<<isFirstPackflag<<GPack2SN;
                stepNum++;
            }
            break;
        case 108:   //uplode tray meaages
        {
            QString packTemp = stepMap["whichPack"].toString();
            qDebug()<<"this is GRFIDMessage"<<packTemp<<GRFIDMessage;
            if(packTemp == "pack1")
            {
                emit sendReqModulePack(GRFIDMessage,stepPack1SN);
            }
            else if(packTemp == "pack2")
            {
                emit sendReqModulePack(GRFIDMessage,stepPack2SN);
            }
        }
            break;
        case 107://air tightness test
        {
            bool tempStartChannel1 = IOput->readIOInput("input9");
            bool tempStartChannel2 = IOput->readIOInput("input8");
            bool sendEnableButten = IOput->readIOInput("input7");//启动按钮
            startGreenFlash = true;
            qDebug()<<"tempStartChannel1"<<tempStartChannel1<<"tempStartChannel2"<<tempStartChannel2<<"sendEnableButten"<<sendEnableButten;
            bool tempSenondSend = false;
            if(tempVariant)
            {
                qDebug()<<"varient change";
                temp111 = tempStartChannel1;
                temp222 = tempStartChannel2;
                tempVariant = false;
            }
            if(airTestFlag)
            {
                airTestFinish = false;
                airTestFlag = false;
                qDebug()<<"air tightness test";
                emit sendBarcodeDisplay(stepMap["PDM"].toString(),"8");//display air tighten test
                emit sendCodeToSerialcom(stepMap["boltSN"].toString(),vinCode,true);//vinCode);
                airProgram = stepMap["program"].toInt();

//                IOput->writeIOOutput("Output1",true);//air test program
//                IOput->writeIOOutput("Output2",false);//air test program
//                IOput->writeIOOutput("Output3",false);//air test program
//                IOput->writeIOOutput("Output5",true);//air test start
            }
            if(GPackType == "PHEV")
            {
                if(!tempStartChannel1 && !tempStartChannel2 && sendEnableButten && (!IOput->readIOInput("input2")) && (!IOput->readIOInput("input3")))
                {
                    timeFlicker.stop();
                    IOput->writeIOOutput("Output4",true);//启动按钮亮
                    emit sendLEDFlashAllOFF(0,"light_green",true);//泄漏中绿亮
                    qDebug()<<"this is send leak first enable";
                    IOput->writeIOOutput("Output8",false);//air test start 1
                    IOput->writeIOOutput("Output3",false);//air test start 2
                    IOput->writeIOOutput("Output2",true);//air test program 11
                    IOput->writeIOOutput("Output1",false);//air test program 12
                    IOput->writeIOOutput("n_led",false);//air test program 13
                    IOput->writeIOOutput("Output7",true);//air test program 21
                    IOput->writeIOOutput("Output6",true);//air test program 22
                    IOput->writeIOOutput("Output5",true);//air test program 23
                    usleep(300000);
                    IOput->writeIOOutput("Output8",true);//air test start 1
                    IOput->writeIOOutput("Output3",true);//air test start 2
                    usleep(3000000);
                    IOput->writeIOOutput("Output2",false);//air test program 11
                    IOput->writeIOOutput("Output1",false);//air test program 12
                    IOput->writeIOOutput("n_led",false);//air test program 13
                    IOput->writeIOOutput("Output7",false);//air test program 21
                    IOput->writeIOOutput("Output6",false);//air test program 22
                    IOput->writeIOOutput("Output5",false);//air test program 23
                    temp333 = true;
                    //tempSenondSend = true;
                }
                if(temp111 != tempStartChannel1 || temp222 != tempStartChannel2)
                {
                    qDebug()<<"tempSenondSend";
                    temp111 = tempStartChannel1;
                    temp222 = tempStartChannel2;
                    tempSenondSend = true;
                }
                if(tempStartChannel1)
                {
    //                IOput->writeIOOutput("Output8",false);//air test start 1
                    IOput->writeIOOutput("Output3",false);//air test start 2
                }
                if(tempStartChannel2)
                {
    //                IOput->writeIOOutput("Output3",false);//air test start 2
                    IOput->writeIOOutput("Output8",false);//air test start 1
                }
                if(!tempStartChannel1 && !tempStartChannel2 && tempSenondSend && temp333)
                {
                    qDebug()<<"this is send leak second enable";
    //                IOput->writeIOOutput("Output8",false);//air test start 1
                    IOput->writeIOOutput("Output3",false);//air test start 2
                    IOput->writeIOOutput("Output7",true);//air test program 21
                    IOput->writeIOOutput("Output6",false);//air test program 22
                    IOput->writeIOOutput("Output5",false);//air test program 23
                    usleep(300000);
                    IOput->writeIOOutput("Output3",true);//air test start 2
                    usleep(300000);
                    IOput->writeIOOutput("Output7",false);//air test program 21
                    IOput->writeIOOutput("Output6",false);//air test program 22
                    IOput->writeIOOutput("Output5",false);//air test program 23
                    tempSenondSend = false;
                    temp333 = false;
                }
            }
            else
            {
                qDebug()<<"this is bev airtest";
                system("echo 1 > /sys/class/leds/control_led8/brightness &");
                if(!tempStartChannel1 && !tempStartChannel2 && sendEnableButten && (!IOput->readIOInput("input2")))
                {
                    timeFlicker.stop();
                    IOput->writeIOOutput("Output4",true);//启动按钮亮
                    emit sendLEDFlashAllOFF(0,"light_green",true);//泄漏中绿亮
                    qDebug()<<"this is send leak first enable";
                    IOput->writeIOOutput("Output8",false);//air test start 1
                    IOput->writeIOOutput("Output3",false);//air test start 2
                    IOput->writeIOOutput("Output7",false);//air test program 21
                    IOput->writeIOOutput("Output6",true);//air test program 22
                    IOput->writeIOOutput("Output5",false);//air test program 23
                    usleep(300000);
                    IOput->writeIOOutput("Output8",false);//air test start 1
                    IOput->writeIOOutput("Output3",true);//air test start 2
                    usleep(3000000);
                    IOput->writeIOOutput("Output7",false);//air test program 21
                    IOput->writeIOOutput("Output6",false);//air test program 22
                    IOput->writeIOOutput("Output5",false);//air test program 23
                }
            }

            if(airTestFinish)
            {
                qDebug()<<"airTestFinish";//泄漏结束，显示站完成，等待放行
//                IOput->writeIOOutput("Output7",false);//air test program 21
//                IOput->writeIOOutput("Output3",false);//air test start 2
//                IOput->writeIOOutput("Output2",false);//air test program 11
                emit sendCodeToSerialcom("boltSN","packSN",false);//vinCode);
                //emit sendLEDFlashAllOFF(0,"0");//发给win关掉红黄绿灯
                //IOput->writeIOOutput("Output5",false);//start air test
                system("echo 0 > /sys/class/leds/control_led8/brightness &"); //outc7
                airTestFinish = false;
                airTestFlag = true;
                tempVariant = true;
                stepNum++;
            }
//            break;
        }
            break;
        case 149://operator pack1
            if(!stepPack1SN.isEmpty() && stepPack1Status == "1")
            {
                needReScaning = true;
                if(Station == "M030A")
                {
                    emit sendgetStationkeyCode(stepPackType,stepPack1SN);
                }
                if(Station == "M030B")
                {
                    if(GPack1SN == GPack1SNTemp)
                    {
                        needReScaning = false;
                    }
                    else
                    {
                        GPack1SNTemp = GPack1SN;
                        needReScaning = true;
                    }
                }
                if(Station == "M100B")
                {
                    emit sendIsMatchModuleKeyCode(stepPackType,stepPack1SN);
                }
                vinCode = stepPack1SN;
                isFirstPackflag = true;
                emit sendBarcodeDisplay(vinCode,"21"); //send pack1 SN to mainwindow
                stepNum++;
            }
            else
            {
                qDebug()<<"this is GWorkStatus status pack1"<<GWorkStatus;
                if(GWorkStatus)
                {
                    if(Station == "M100B")
                    {
                        emit sendIsMatchModuleKeyCode(stepPackType,stepPack1SN);
                    }
                    vinCode = stepPack1SN;
                    isFirstPackflag = true;
                    needReScaning = true;
                    emit sendBarcodeDisplay(vinCode,"21"); //send pack1 SN to mainwindow
                    stepNum++;
                }
                else
                {
                    stepNum = jumpPack1();
                    qDebug()<<"this is jump pack1"<<stepNum<<GWorkStatus;
                }
            }
            break;
        case 150://operator pack2
            if(!stepPack2SN.isEmpty() && stepPack2Status == "1")
            {
                needReScaning = true;
                if(Station == "M030A")
                {
                    emit sendgetStationkeyCode(stepPackType,stepPack2SN);
                }
                if(Station == "M030B")
                {
                    if(GPack2SN == GPack2SNTemp)
                    {
                        needReScaning = false;
                    }
                    else
                    {
                        GPack2SNTemp = GPack2SN;
                        needReScaning = true;
                    }
                }
                if(Station == "M100B")
                {
                    emit sendIsMatchModuleKeyCode(stepPackType,stepPack2SN);
                }
                vinCode = stepPack2SN;
                isFirstPackflag = false;
                emit sendBarcodeDisplay(vinCode,"22"); //send pack2 SN to mainwindow
                stepNum++;
            }
            else
            {
                qDebug()<<"this is GWorkStatus status pack2"<<GWorkStatus;
                if(GWorkStatus)
                {
                    if(Station == "M100B")
                    {
                        emit sendIsMatchModuleKeyCode(stepPackType,stepPack2SN);
                    }
                    vinCode = stepPack2SN;
                    isFirstPackflag = false;
                    needReScaning = true;
                    emit sendBarcodeDisplay(vinCode,"22"); //send pack2 SN to mainwindow
                    stepNum++;
                }
                else
                {
                    qDebug()<<"this is one packnum"<<stepNum<<allStep<<GWorkStatus;
                    stepNum = allStep;
                }
            }
//            vinCode = stepPack2SN;
//            emit sendBarcodeDisplay(vinCode,"22"); //send pack2 SN to mainwindow
//            stepNum++;
            break;
        case 151://request product type
            if(!stepGetProductTypeFlag)
            {qDebug()<<"point[10]";
                if(timerCount >= 4) //delay 2000ms read
                {
                    timerCount = 0;
                    if(!reqAckFlag)
                    {
                        reqAckFlag = true;
                        emit sendReqPackStatus();
//                        emit sendReqTightStatus("A410","A390-A","1");
                    }
                }
                if(vari2 == "XL" && startYellowFlash)
                {
                    startYellowFlash = false;
                    emit sendLEDFlashAllOFF(500,"flash_yellow",true);//黄闪
                }
                timerCount++;
            }
            else
            {
                startYellowFlash = true;
                if(vari2 == "XL" && startGreenFlash)
                {
                    startGreenFlash = false;
                    emit sendLEDFlashAllOFF(500,"flash_green",true);//待测试绿闪
                    timeFlicker.start(500);//计时启动
                }
                reqAckFlag = false;
                stepGetProductTypeFlag = false;
                AddStationTimerFlag = true;
                stepNum++;
                timerCount = 5;
                GProgramIndex = 0;
                stationFinishFlag = true;
                qDebug()<<"this is handle pack"<<stepNum;
            }
            break;
        case 152://station finish
            if(GPackType == "PHEV")
            {
                if(stepMap["finishPass"].toString().contains(GPack1SN) || stepMap["finishPass"].toString().contains(GPack2SN))
                {
                    allowPackType = true;
                }
                else
                {
                    allowPackType = false;
                }
            }
            else if(GPackType == "BEV")
            {
                if(stepMap["finishPass"].toString().contains(GPack1SN))
                {
                    allowPackType = true;
                }
                else
                {
                    allowPackType = false;
                }
                if(Station == "M325A" && AddStationTimerFlag)
                {
                    qDebug()<<"AddStationTimerFlag";
                    AddStationTimerFlag = false;
                    emit sendAddStationTimer(stepPack1SN,2);
                }
            }
            if(holdOnFlag)
            {
                holdOnCount++;
                if(holdOnCount >= 10800)
                {
                    qDebug()<<"holdOnFlag"<<holdOnCount;
                    stepNum--;
                    holdOnCount = 0;
                }
            }
            if(stationFinishFlag)//
            {
                stationFinishFlag = false;
                moveButtonFlag = false;
                emit sendBarcodeDisplay("","30"); //display station finish
            }
            qDebug()<<"allowPackType"<<stepMap["finishPass"].toString()<<GPack1SN<<GPack2SN<<IOput->readIOInput("input5");
            if(stepMap["finishPass"].toString() == "All" || allowPackType)
            {
                if(moveButtonFlag || IOput->readIOInput("input5"))
                {
                    stationFinishFlag = true;
                    moveButtonFlag = false;
                    if(vari2 == "XL")
                    {
                        if(IOput->readIOInput("input2") && IOput->readIOInput("input3"))
                        {
                            sendStationFinishFlag = true;
                            holdOnCount = 0;
                            count1 = 5;
                            emit sendBarcodeDisplay("","31"); //display car pull in
                            stepNum++;
                            timeFlicker.stop();
                        }
                        else
                        {
                            emit sendBarcodeDisplay("","206"); //display car pull in
                        }
                    }
                    else
                    {
                        sendStationFinishFlag = true;
                        holdOnCount = 0;
                        count1 = 5;
                        emit sendBarcodeDisplay("","31"); //display car pull in
                        stepNum++;
                    }

    //                QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    //                emit sendReqStationState(stepPack1SN,"1",stepPack2SN,"1",time.toString("yyyy-MM-dd"));

                }
            }
            else
            {
                qDebug()<<"this is station error"<<stepMap["finishPass"].toString()<<GPack1SN<<GPack2SN;
                stationFinishFlag = true;
                moveButtonFlag = false;
                if(vari2 == "XL")
                {
                    if(IOput->readIOInput("input2") && IOput->readIOInput("input3"))
                    {
                        sendStationFinishFlag = true;
                        count1 = 5;
                        holdOnCount = 0;
                        emit sendBarcodeDisplay("","31"); //display car pull in
                        stepNum++;
                        timeFlicker.stop();
                    }
                    else
                    {
                        emit sendBarcodeDisplay("","206"); //display car pull in
                    }
                }
                else
                {
                    sendStationFinishFlag = true;
                    count1 = 5;
                    holdOnCount = 0;
                    emit sendBarcodeDisplay("","31"); //display car pull in
                    stepNum++;
                }
            }
            break;
        case 15://托盘到位

            if(trayReadyFlag)
            {
                trayReadyFlag = false;
                emit sendBarcodeDisplay("","15"); //display please scanning
//                if(Station == "VM010-1" || Station == "VM010-2"){
//                    emit sendModuleClean();
//                }
            }
            if(getTrayPlaceReadyFlag)
            {
                emit sendBarcodeDisplay("RFID code",vinCode);
                if(Station.mid(0,2) == "VM")
                {
                    if(vinCode.mid(0,1) == "P")
                    {
                        GPackTypeTemp = "PHEV";
                    }
                    else if(vinCode.mid(0,1) == "B")
                    {
                        GPackTypeTemp = "BEV";
                    }
                    else
                    {
                        GPackTypeTemp = "PHEV";
                        qDebug()<<"this is other GPackType";
                    }
                    if(GPackType == GPackTypeTemp)
                    {

                    }
                    else
                    {
                        GPackType = GPackTypeTemp;
                        readStepParaInit();
                        emit sendInitEncoder();
                    }
                    if(vinCodeTempVM == vinCode)
                    {

                    }
                    else
                    {
                        vinCodeTempVM = vinCode;
//                        stepNum++;
                        trayReadyFlag = true;
                        if(GProDuctType != "")
                        {
                            qDebug()<<"need sendAddTrayPackType";
                            emit sendAddTrayPackType(vinCode);
                            stepNum++;
                        }
                        else
                        {
                            qDebug()<<"need getTrayPackType";
                            emit sendgetTrayPackType(vinCode);
                        }
                    }
                    GProgramIndex = 0;
                }
                getTrayPlaceReadyFlag = false;
            }
            break;
        case 30://站完成请求PLC放行托盘
            emit sendTypeToPLC(30,1);//send to connectplc
            //托盘放行后初始给从站的rfid的信息
            sendToRfid = "<reply><resultCode>0000</resultCode><reset/></reply>";
            if(Station.mid(0,2) == "OP")
            {
                sendStationFinishFlag = true;
                count1 = 5;
            }
            if(Station.mid(0,2) == "VM")
            {
              QString DataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
              emit sendReqStationState(vinCode,GTrayStatus,"","",DataTime);
            }
            emit sendBarcodeDisplay("","30"); //display please scanning
            stepNum++;
            break;
        default:
            break;
        }
    }
    if((allStep+1) <= stepNum)
    {
        timerCount1++;
        if(timerCount1 >= 5) //delay 5000ms next start
        {
            timerCount1 = 0;
            stepNum = 1;
            getCodeFlag = true;
            getTrayPlaceReadyFlag = false;
            reqScanBarcodeFlag = true;
            IO_Module_Value = 0;
        }
    }

    if(vari1 == "M")
    {
        revStopButtonSignal(IOput->readIOInput("Input6"));//Input6--急停按钮
        if(vari2 == "XL"){
            IOput->writeIOOutput("r_led",!(IOput->readIOInput("Input6")));
            //emit sendLEDFlashAllOFF(0,"light_red",!(IOput->readIOInput("Input6")));//
        }
    }
    if(count1 == 5)
    {
        if(sendStationFinishFlag)
        {
//            if(vari2 == "XL")
//            {
//                if(IOput->readIOInput("input2") && IOput->readIOInput("input3"))
//                {
//                    timeFlicker.stop();
//                    IOput->writeIOOutput("Output4",false);
//                    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
//                    qDebug()<<"this is station finish packstatus"<<stepPack1Status<<stepPack2Status;
//                    if(stepPack2SN.isEmpty())
//                    {
//                        emit sendReqStationState(stepPack1SN,stepPack1Status,stepPack2SN,"3",time.toString("yyyy-MM-dd"));
//                    }
//                    else if(stepPack1SN.isEmpty())
//                    {
//                        emit sendReqStationState(stepPack1SN,"3",stepPack2SN,stepPack2Status,time.toString("yyyy-MM-dd"));
//                    }
//                    else
//                    {
//                        emit sendReqStationState(stepPack1SN,stepPack1Status,stepPack2SN,stepPack2Status,time.toString("yyyy-MM-dd"));
//                    }
//                }
//                else
//                {
//                    qDebug()<<"this is error airtest go"<<IOput->readIOInput("input2")<<IOput->readIOInput("input3");
//                }
//            }
//            else
//            {
                AddStationTimerFlag = true;
                QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
                QString DataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                if(Station.mid(0,2) == "OP")
                {
                int Type = 2;//出站
                emit sendUpInAndOutStation(Type,stepPack1SN,"",Station,DataTime);//bmce出站发到interface上传接口
                }
                qDebug()<<"this is station finish packstatus"<<stepPack1Status<<stepPack2Status;
                if(stepPack2SN.isEmpty())
                {
                    emit sendReqStationState(stepPack1SN,stepPack1Status,stepPack2SN,"3",time.toString("yyyy-MM-dd hh:mm:ss"));
                }
                else if(stepPack1SN.isEmpty())
                {
                    emit sendReqStationState(stepPack1SN,"3",stepPack2SN,stepPack2Status,time.toString("yyyy-MM-dd hh:mm:ss"));
                }
                else
                {
                    emit sendReqStationState(stepPack1SN,stepPack1Status,stepPack2SN,stepPack2Status,time.toString("yyyy-MM-dd hh:mm:ss"));
                }
//            }
                GWorkStatus = false;
        }
        count1 = 0;
    }
    count1 ++;
    QTimer::singleShot(500,this,SLOT(stepSelect()));//100ms
}



void StepControl::recBMCEFirstStatus(QVariant bmceStatus)
{
    BMCE_STATUS recBMCE;
    recBMCE = bmceStatus.value<BMCE_STATUS>();
    qDebug()<<"stepcontrol bmce station"<<recBMCE.Result<<recBMCE.Type<<recBMCE.Pack1Type<<recBMCE.Pack1SN<<recBMCE.Pack1Status;
    if(recBMCE.Result == "1")
    {
        stepPack1SN = recBMCE.Pack1SN;
        GPackSN = recBMCE.Pack1SN;
        GPack1SN = recBMCE.Pack1Type;
        vinCode = recBMCE.Pack1SN;
        if(recBMCE.Pack1Status.isEmpty())
        {
            recBMCE.Pack1Status = "1";
        }
        if(Station == "OP10")
        {
            recBMCE.Pack1Status = "1";
        }
        bmceFrontStatus = recBMCE.Pack1Status;
        stepPack1Status = recBMCE.Pack1Status;
        if(Station == "OP70-1" || Station == "OP70-2")
        {
            emit sendRepairBatterypack(stepPack1SN);
        }
        emit sendBarcodeDisplay("RFID code",vinCode);
        QString packTypeTemp;
        packTypeTemp = recBMCE.Type;
        if(vinCode.isEmpty())
        {
            emit sendBarcodeDisplay("获取零件号异常","204");
        }
        else
        {
            if(packTypeTemp == "1")
            {
                GPackTypeTemp = "PHEV";
            }
            else if(packTypeTemp == "2")
            {
                GPackTypeTemp = "BEV";
            }
            else
            {
                GPackTypeTemp = "PHEV";
                qDebug()<<"this is other GPackType";
            }
            if(GPackType == GPackTypeTemp)
            {

            }
            else
            {
                GPackType = GPackTypeTemp;
                readStepParaInit();
                emit sendInitEncoder();
            }
            if(vinCodeTemp == vinCode)  //当接收到的零件号是之前做过的，就不重新做
            {
//                emit sendBarcodeDisplay("请勿重复操作该模组","204");
                qDebug()<<"vinCodeTemp == vinCode";
            }
            else
            {
                vinCodeTemp = vinCode;
                QString DataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                int Type = 1;//进站
                emit sendUpInAndOutStation(Type,recBMCE.Pack1SN,"",Station,DataTime);//BMCE进站发到interface上传接口
                if(bmceFrontStatus == "1")
                {
                    stepNum++;
                }
                else if(bmceFrontStatus == "2")
                {
                    if(Station == "OP10")
                    {
                        stepNum++;
                    }
                    else
                    {
                         emit sendBarcodeDisplay("","122");
                    }
                }
                else
                {
                    if(Station == "OP10-2")
                    {
                       qDebug()<<"OP10-2FrontStatuserror"<<bmceFrontStatus;
                       stepNum++;
                    }
                    else
                    {
                        qDebug()<<"bmceFrontStatuserror"<<bmceFrontStatus;
                    }
                }
            }
            getTrayPlaceReadyFlag = false;
            trayReadyFlag = true;
            GProgramIndex = 0;
        }
    }
    else if(recBMCE.Result == "2")
    {
         emit sendBarcodeDisplay("托盘与零件不匹配","204");
    }
    else if(recBMCE.Result == "3")
    {
        emit sendBarcodeDisplay("暂无订单","204");
    }

}
/*****************************/
//jump function
//opSign 1 = ">"
//       2 = "="
//       3 = "<"
/*****************************/
bool StepControl::jumpStepFunc(int var, int opSign, int value)
{
    int conditionValue=0;
    bool result = false;
    if(var == 1)//task
    {
        conditionValue = ++packNum;
    }
    qDebug()<<"*******jump parameter****"<<var<<opSign<<value<<conditionValue;

    switch (opSign)
    {
    case 1:
        if(conditionValue == value)
            result =  true;
        break;
    case 2:
        if(conditionValue > value)
            result =  true;
        break;
    case 3:
        if(conditionValue < value)
            result = true;
        break;
    default:
        break;
    }
    return result;
}
/*********************************************/
//station information
/*********************************************/
void StepControl::revPackStatus(QVariant packStatus)
{
    PACK_STATUS statusStruct;
    statusStruct = packStatus.value<PACK_STATUS>();
    reqAckFlag = false;
    if(statusStruct.Result)
    {
        stepPackType = statusStruct.PackType;//product type
        QString stepPackTypeTemp;
        if(stepPackType == "1")
        {
            stepPackTypeTemp = "PHEV";
        }
        else
        {
            stepPackTypeTemp = "BEV";
        }
        GRFIDMessage = stepPackTypeTemp;
        if(GPackType == stepPackTypeTemp)
        {

        }
        else
        {
            qDebug()<<"this is stepPackType"<<stepPackType;
            GPackType = stepPackTypeTemp;
            readStepParaInit();
            emit sendInitEncoder();
        }
        stepPack1SN = statusStruct.Pack1SN;
        stepPack2SN = statusStruct.Pack2SN;
        stepPack1Status = statusStruct.Pack1Status;
        stepPack2Status = statusStruct.Pack2Status;
        bool packstauOk = packStatusOk(stepPack1SN,stepPack2SN,stepPack1Status,stepPack2Status);
        qDebug()<<"this is pack statues"<<packstauOk;
//        if(stepPack1Status == "OK" && stepPack2Status == "OK")
//        {
//            qDebug()<<"this two pack is all ok ";
//            if(oldPackSN != stepPack1SN)
//            {
//                stepGetProductTypeFlag = true;
//                vinCode = stepPack1SN;
//                oldPackSN = stepPack1SN;
//            }
//        }
//        else
//        {
//            qDebug()<<"this two pack is not all ok";
//            emit sendBarcodeDisplay("","111");
//            QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
//            sleep(3);
//            emit sendReqStationState(stepPack1SN,"3",stepPack2SN,"3",time.toString("yyyy-MM-dd"));
//            emit sendBarcodeDisplay("","31");
//        }
//        if(oldPackSN != stepPack1SN)
//        {
//            stepGetProductTypeFlag = true;
//            vinCode = stepPack1SN;
//            oldPackSN = stepPack1SN;
//        }
        if ((stepPack1SNTemp==stepPack1SN)&&(stepPack2SNTemp==stepPack2SN))
               {
                   qDebug()<<"stepPack1SNTemp==stepPack1SN";
               }
               else
               {
                   stepPack1SNTemp = stepPack1SN;
                   stepPack2SNTemp = stepPack2SN;
//                   if(stepPack1Status == "1" && stepPack2Status == "1")
                   if(packstauOk)
                   {
//                       qDebug()<<"this two pack is ok ";
//                       if(oldPackSN != stepPack1SN)
//                       {
//                           qDebug()<<"this is stepGetProductTypeFlag";
//                           stepGetProductTypeFlag = true;
//                           vinCode = stepPack1SN;
//                           oldPackSN = stepPack1SN;
//                       }
                       qDebug()<<"this is stepGetProductTypeFlag";
                       stepGetProductTypeFlag = true;
                       vinCode = stepPack1SN;
                   }
                   else
                   {
                       qDebug()<<"this pack is not ok";
                       emit sendBarcodeDisplay("","111");
                       //QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
                       //sleep(3);
                       //emit sendReqStationState(stepPack1SN,"3",stepPack2SN,"3",time.toString("yyyy-MM-dd"));
                       //emit sendBarcodeDisplay("","31");
                   }
//                   if(oldPackSN != stepPack1SN)
//                   {
//                       stepGetProductTypeFlag = true;
//                       vinCode = stepPack1SN;
//                       oldPackSN = stepPack1SN;
//                   }
               }
        qDebug() << "from http rev step product type::"<<stepPackType<<stepPack1SN<<stepPack1Status<<stepPack2SN<<stepPack2Status;
    }
    else
    {
        qDebug() << "request PackStatus fail ErrorCode:" <<statusStruct.Errorcode;;
    }
}
/***************************************************
 *receive station finished ACK
 * ************************************************/
void StepControl::revStationState(bool isSuccess)
{
    if(isSuccess)
    {
        sendStationFinishFlag = false;
        qDebug()<<"manual post StationState success";
    }
    else
    {
        qDebug()<<"manual post StationState fail";
    }
}
void StepControl::revModuleTray(bool isSuccess)
{
    if(isSuccess)
    {
        qDebug()<<"post ModuleTray success";
    }
    else
        qDebug()<<"post ModuleTray fail";
}
/***************************************************
 *receive RequestAgvLeave finished ACK
 * ************************************************/
void StepControl::revAgvLeaveState(bool isSuccess)
{
    if(isSuccess)
    {
        sendStationFinishFlag = false;
        qDebug()<<"manual post StationState success";
    }
    else
    {
        qDebug()<<"manual post StationState fail";
    }
}
/*********************************************/
//receive key status    获取键位(目前没卵用)
/*********************************************/
void StepControl::revKeyStatus(bool status)
{
    keyStatusFlag = status;
}
/*********************************************/
//receive move button
/*********************************************/
void StepControl::revMoveButton()
{
    moveButtonFlag = true;
    airTestFinish = true;
}
/*********************************************/
//receive CS351 status
/*********************************************/
void StepControl::revCS351Status(bool status)
{
    CS351StatusFlag = status;
}
/*********************************************/
//receive IO module value
/*********************************************/
void StepControl::revIOModuleValue(int value)
{
    IO_Module_Value = value;
}
/*********************************************/
//receive stop button
/*********************************************/
void StepControl::revStopButtonSignal(bool value)
{
//    qDebug()<<"IO_module#######"<<value<<stopValueFlag;
    if(stopValueFlag != value)
    {
        stopValueFlag = value;
        if(value)//stop signal restart
        {
            qDebug()<<"recv stop signal restart";
            emit sendUIEmergencyStop(true);
        }
        else //stop signal work
        {
            if(!isPopUpFlag)//pop up box
            {
                isPopUpFlag = true;
                 if(vari2 != "XL"){
                     IOput->writeIOOutput("y_led",true);
                 }
                emit sendReqEmergencyStop(false);//send stop cmd to PC
                emit sendUIEmergencyStop(false);
                qDebug()<<"tell PC stop signal work";
            }
        }
    }
}

void StepControl::revEmergencyStop(bool isSuccess)
{
    if(isSuccess)
    {
        qDebug()<<"post EmergencyStop success";
    }
    else
        qDebug()<<"post EmergencyStop fail";
}

void StepControl::informEmergencyStop(bool isReset)
{
    if(isReset)
    {
        isPopUpFlag = false;
        emit sendReqEmergencyStop(true);
        qDebug()<<"tell PC stop signal restart";
    }
    else
    {
        IOput->writeIOOutput("y_led",false);
        qDebug()<<"trouble clearing";
    }
}
void StepControl::revRFIDStepInit()
{
    stepNum = allStep;
}

void StepControl::getErrorGo(int type)
{
    if(type == 1)
    {
        qDebug()<<"bmceErrorGo";
//        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
//        //sleep(3);
//        emit sendReqStationState(stepPack1SN,"3",stepPack2SN,"3",time.toString("yyyy-MM-dd hh:mm:ss"));
        stepNum = allStep;
        stepPack1Status = "3";
    }
    else
    {
        qDebug()<<"this two pack is not all ok";
        //emit sendBarcodeDisplay("","111");
        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
        //sleep(3);
        emit sendReqStationState(stepPack1SN,"3",stepPack2SN,"3",time.toString("yyyy-MM-dd hh:mm:ss"));
        emit sendBarcodeDisplay("","31");
    }
}
void StepControl::sendDrectGoSlot()
{
    emit sendRequestAgvLeave();
    gostRunFlag = true;
    stepPack1SNTemp="AAAAAA";
    stepPack2SNTemp="AAAAAA";
}
void StepControl::getErrorContinue()
{
    if(Station.mid(0,2) == "OP")
    {
        stepNum++;
    }
    else
    {
        qDebug()<<"this is continue operate pack"<<GWorkStatus;
        stepGetProductTypeFlag = true;
        vinCode = stepPack1SN;
    }
}

void StepControl::recEKSatatueEvent(int status)
{
    qDebug()<<"this is rec eks status"<<status;
    if(status == 1) //on_pushButton_deep1_clicked
    {
        int tempStep = allStep;
        for(int i = 1; i < allStep; i++)
        {
            QVariantMap stepMap = stepList[i-1].toMap();
            stepType = parsejson->stepNameToTypeCode(stepMap["singleStep"].toString());
            if(stepType == 150)
            {
                tempStep = i;
            }
        }
        if(tempStep > stepNum)
        {
            stepNum = tempStep;
        }
        else
        {
            qDebug()<<"this step is pack2";
        }
        stepPack1Status = "2";
        GProgramIndex = 0;
        getCodeFlag = true;
        tightStartFlag = true;
        tighteningFinishFlag = false;
        getTrayPlaceReadyFlag = false;
        reqScanBarcodeFlag = true;
        IO_Module_Value = 0;
        stationFinishFlag = true;

    }
    else if(status == 2) //on_pushButton_deep2_clicked
    {
        int tempStep = allStep;
        for(int i = 1; i < allStep; i++)
        {
              QVariantMap stepMap = stepList[i-1].toMap();
              stepType = parsejson->stepNameToTypeCode(stepMap["singleStep"].toString());
              if(stepType == 150)
              {
                  tempStep = i;
              }
        }
        if(tempStep < stepNum)
        {
            stepNum = allStep;
        }
        else
        {
            qDebug()<<"this step is pack1";
        }
        stepPack2Status = "2";
        GProgramIndex = 0;
        getCodeFlag = true;
        tightStartFlag = true;
        tighteningFinishFlag = false;
        getTrayPlaceReadyFlag = false;
        reqScanBarcodeFlag = true;
        IO_Module_Value = 0;
        stationFinishFlag = true;

    }
    else if(status == 3) //on_pushButton_deepUp_clicked
    {
        int tempStep = allStep;
        for(int i = 1; i < allStep; i++)
        {
            QVariantMap stepMap = stepList[i-1].toMap();
            stepType = parsejson->stepNameToTypeCode(stepMap["singleStep"].toString());
            if(stepType == 150)
            {
                tempStep = i;
            }
        }
//        if(tempStep > stepNum)
//        {
//            stepPack1Status = "2";
//        }
//        else
//        {
//            stepPack2Status = "2";
//        }
        stepNum--;
        GProgramIndex = 0;
        getCodeFlag = true;
        tightStartFlag = true;
        tighteningFinishFlag = false;
        getTrayPlaceReadyFlag = false;
        reqScanBarcodeFlag = true;
        IO_Module_Value = 0;
        stationFinishFlag = true;

    }
    else if(status == 4) //on_pushButton_deepDown_clicked
    {
        int tempStep = allStep;
        for(int i = 1; i < allStep; i++)
        {
            QVariantMap stepMap = stepList[i-1].toMap();
            stepType = parsejson->stepNameToTypeCode(stepMap["singleStep"].toString());
            if(stepType == 150)
            {
                tempStep = i;
            }
        }
        if(tempStep > stepNum)
        {
            stepPack1Status = "2";
        }
        else
        {
            stepPack2Status = "2";
        }
        stepNum++;
        GProgramIndex = 0;
        getCodeFlag = true;
        tightStartFlag = true;
        tighteningFinishFlag = false;
        getTrayPlaceReadyFlag = false;
        reqScanBarcodeFlag = true;
        IO_Module_Value = 0;
        stationFinishFlag = true;

    }
    else
    {
        qDebug()<<"this is other on_pushButton_deepDown_clicked";
    }
}

bool StepControl::packStatusOk(QString pack1Num, QString pack2Num, QString pack1Stau, QString pack2Stau)
{
    int pack12Num = 0;
    if(!pack1Num.isEmpty())
    {
        pack12Num += 1;
    }
    if(!pack2Num.isEmpty())
    {
        pack12Num += 2;
    }
    qDebug()<<"this is pack12Num"<<pack12Num;
    if(pack12Num == 1)
    {
        if(pack1Stau == "1")
        {
            return true;
        }
        else
        {
            GWhichPackError = 1;
            return false;
        }
    }
    else if(pack12Num == 2)
    {
        if(pack2Stau == "1")
        {
            return true;
        }
        else
        {
            GWhichPackError = 2;
            return false;
        }
    }
    else if(pack12Num == 3)
    {
        if(pack1Stau == "1" && pack2Stau == "1")
        {
            return true;
        }
        else
        {
            if(pack1Stau != "1" && pack2Stau == "1")
            {
                GWhichPackError = 1;
            }
            else if(pack2Stau != "1" && pack1Stau == "1")
            {
                GWhichPackError = 2;
            }
            else if(pack1Stau != "1" && pack2Stau != "1")
            {
                GWhichPackError = 3;
            }
            return false;
        }
    }
    else
    {
        GWhichPackError = 4;
        return false;
    }
}

int StepControl::jumpPack1()
{
    int tempStep = allStep;
    for(int i = 1; i < allStep; i++)
    {
        QVariantMap stepMap = stepList[i-1].toMap();
        stepType = parsejson->stepNameToTypeCode(stepMap["singleStep"].toString());
        if(stepType == 150)
        {
            tempStep = i;
        }
    }
    return tempStep;
}

void StepControl::timerflick()
{
    if(colorTrue){
        colorTrue = false;
    }else{
        colorTrue = true;
    }
    IOput->writeIOOutput("Output4",colorTrue);//启动按钮闪
}

bool StepControl::handleJumpStation(QString station)
{
//    datas;
    if(datas.isEmpty())
    {
        qDebug()<<"this is error datas";
    }
    else
    {
        for(int i = 0; i < datas.size(); i++)
        {
            QVariantMap stepMap = stepList[i].toMap();
            if(station == stepMap["Station"].toString())
            {
                qDebug()<<"this is station true"<<station<<stepMap["Status"];
                if(stepMap["Status"].toString() == "1")
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

        }
    }

}

void StepControl::revGetStationkeyCodeResult(bool flag)
{
    qDebug()<<"revGetStationkeyCodeResult"<<flag;
    if(flag)
    {
        needReScaning = false;
    }
    else
    {
        needReScaning = true;
    }

}

void StepControl::revreturngetTrayPackType(QString PackType)
{
    GPack1SN = PackType;
    stepNum++;
}







