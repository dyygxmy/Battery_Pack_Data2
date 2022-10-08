#include "serialComsRead.h"

SerialComsRead::SerialComsRead(QObject *parent) :
    QObject(parent)
{
    this->moveToThread(&m_thread);
    m_thread.start();
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
    scanType = "c5";
    needscanRegExp = false;
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
    qDebug()<<"this is cominit";
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
        //        return true;
    }
    else
    {
        qDebug()<<"barcode serial open fail "<<portName;
        //        return false;
    }
    if(Station.mid(0,5) == "VM010")
    {
        QString portName1 ;
        qDebug()<<"this is com3init";
        if(vari1.mid(0,1) == "M")
        {
            portName1 = "ttymxc3";   //获取串口名
        }
        else
        {
            portName1 = "ttymxc3";   //获取串口名
        }
        myCom3 = new QextSerialPort("/dev/" + portName1);
        qDebug()<<"this is the carcode"<<Gcarcode;
        myCom3->setBaudRate((BaudRateType)Gcarcode);
        //    myCom->setBaudRate((BaudRateType)9600);
        //    myCom->setBaudRate((BaudRateType)115200);
        //设置数据位
        myCom3->setDataBits((DataBitsType)8);
        //设置校验
        myCom3->setParity(PAR_NONE);
        //设置停止位
        myCom3->setStopBits(STOP_1);
        //设置数据流控制
        myCom3->setFlowControl(FLOW_OFF);
        //设置延时
        myCom3->setTimeout(10);

        if(myCom3->open(QIODevice::ReadWrite))
        {
            qDebug()<<"barcode serial3 open ok"<<portName;
            connect(myCom3, SIGNAL(readyRead()), this, SLOT(slot_read_com33()));
            return true;
        }
        else
        {
            qDebug()<<"barcode serial3 open fail "<<portName;
            return false;
        }
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
//    slot_read_com3();
    QTimer::singleShot(1000,this,SLOT(timerReadComFunc()));
}
/***********************************/
//serial com send CMD
//
/*************************************/
void SerialComsRead::sendSerialCMD()
{
    qDebug()<<"test11111";
    QByteArray sendBuf;
    revComBuf.clear();
    revStartFlag = false;
    sendBuf[0] = '+';
    myCom->write(sendBuf,1);
    sleep(1);
    if(Station.mid(0,5) == "VM010")
    {
        myCom3->write(sendBuf,1);
    }
    emit sendCodeToMainwindow("99","NULL");
    qDebug() << "send+++++++++++++++:" << sendBuf <<scanCodeCount;

}
/*******************************************/
//start barcode scanning 请求下箱体条码扫描
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
//receive serial com3 data VM010 特有
//
/*************************************/
void SerialComsRead::slot_read_com33()
{
    int revLen,i;
    QString serialNums;
    QByteArray recBuf;
    qDebug()<<"test22222";
    revLen=myCom3->bytesAvailable();
    if(revLen>0)
    {
        if(!revStartFlag)
        {
            revStartFlag = true;
        }
        timeoutCount = 0;
        recBuf = myCom3->readAll();
        revComBuf.append(recBuf);
        qDebug()<<"receive 33333 ::"<<recBuf.toHex();
        qDebug()<<"receive 44444 ::"<<revLen<<revComBuf.size()<<revComBuf;
        if(revComBuf.size() > 0){
            if((unsigned char)revComBuf[revComBuf.size() - 1] < 0x20){
                revComBuf.remove(revComBuf.size()-1,1);//去掉最后一位回车符
                QString msg = revComBuf;
                msg = msg.trimmed();
                if(msg.size() > 0){
                    comBufFunc(revComBuf);
                }
                revComBuf.clear();
            }
        }
    }
}

/***********************************/
//receive serial com data 普通扫码枪使用
//
/*************************************/
void SerialComsRead::slot_read_com3()
{
    QByteArray recBuf;
    int revLen = myCom->bytesAvailable();
    if(revLen>0)
    {
        if(!revStartFlag)
        {
            revStartFlag = true;
        }
        timeoutCount = 0;
        recBuf = myCom->readAll();

        QByteArray head;
        QByteArray end;
        bool isEnd = false;
        for(int i=0;i<recBuf.size();i++){
            if((unsigned char)recBuf[i]<0x20){
                isEnd = true;
                head = recBuf.mid(0,i).trimmed();
                end = recBuf.mid(i,recBuf.size()-i).trimmed();
                break;
            }
        }
        if(isEnd){//遇到结尾符
            revComBuf.append(head);
            if(head.size()>0){//防止开机第一次扫码开头是上次的结尾符的情况
                comBufFunc(revComBuf);
            }
            revComBuf = end;
        }else{//没遇到结尾符
            revComBuf.append(recBuf);
        }


//        revComBuf.append(head);
//        qDebug()<<"receive 11111 ::"<<recBuf.toHex();
//        qDebug()<<"receive 22222 ::"<<revLen<<revComBuf.size()<<revComBuf;
//        if(revComBuf.size() > 0){
//            if((unsigned char)revComBuf[revComBuf.size() - 1] < 0x20){
////                revComBuf.remove(revComBuf.size()-1,1);//去掉最后一位回车符
//                QString msg = revComBuf;
//                msg = msg.trimmed();
//                if(msg.size() > 0){
//                    comBufFunc(revComBuf);
//                }
//                revComBuf.clear();
//            }
//        }
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
    buf = buf.simplified();
    QStringList listLeak = buf.split(QRegExp(" |="));//按空格或=来分割
    for(int i = 0;i<listLeak.size();i++){//去掉空字符项
        if(listLeak.at(i).isEmpty()){
            listLeak.removeAt(i);
        }
    }
    QString STATE_blc,MI_blc,WI_blc,Program,Channel;
    for(int i = 0;i<listLeak.size()-2;i++){
        if(listLeak.at(i) == "KAN"){
            Channel = listLeak.at(i+1);//KAN
        }
        if(listLeak.at(i) == "PP"){
            Program = listLeak.at(i+1);//pp
        }
        if(listLeak.at(i) == "VL"){
            MI_blc = listLeak.at(i+1);//VL
        }
        if(listLeak.at(i) == "PM"){
            WI_blc = listLeak.at(i+1);//PM
            STATE_blc = listLeak.at(i+2);//A and G
        }
    }

    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
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
    emit sendResultToStep(STATE_blc);
    emit sendResultToSQL(DataVar);
}

//处理接收到的扫码
void SerialComsRead::comBufFunc(QByteArray revComBuf)
{
    QString serialNums = revComBuf;
    serialNums = serialNums.trimmed();
    qDebug()<<"comBufFuncserialNums:"<<serialNums;
    qDebug()<<"recComBufStation:"<<Station;
    if(Station == "VM010-1" || Station == "VM010-2")
    {
        emit sendCodeToPLC(serialNums);
        emit sendCodeToMainwindow("98",serialNums);
        startScanCodeFlag = false;
        timerCount = 0;
        scanCodeCount = 0;
        qDebug() << "vm010 atuo tiaoma qiang"<< serialNums;
    }
    if(vari2_XL == "XL") //air test
    {
        if(revComBuf.size() >= 88)
        {
            qDebug()<<"this is rec com  buf"<<revComBuf;
            if(airResultValidFlag)//air test
            {
                airTestResult(serialNums);
            }
            revStartFlag = false;
        }
    }
    else
    {
        if(startScanCodeFlag)//请求下箱体条码扫描
        {
            emit sendCodeToPLC(serialNums);
            emit sendCodeToMainwindow("98",serialNums);
            startScanCodeFlag = false;
            timerCount = 0;
            scanCodeCount = 0;
            qDebug() << "atuo tiaoma qiang"<< serialNums;
        }
        else//其他扫码
        {
            qDebug()<<"thisIsScan";
            bool scanRegExpFlag = false;
            bool datediffFlag = false;
            bool vactionRegexpFlag = false;
            bool OP80CodeContainsPackSN = true;//OP80独有扫码零件号包含订单号
            if(scanType == "c5")
            {
                if(GscanRegExp == "Null")
                {
                    qDebug()<<"point[900]";
                    if((Station.mid(0,2) == "OP" && GCurrentStep == "条码枪扫码") || Station.mid(0,2) != "OP"){
                        emit sendCmdToStep(1,serialNums,2);
                        emit sendCodeToMainwindow("1",serialNums);
                    }else{
                        emit sendCodeToMainwindow("别扫码,当前正在进行"+GCurrentStep,"scanStepError");
                        if(Pack1SN == "-1"){
                            emit sendCodeToMainwindow("无订单信息，请检查！","scanStepError");
                        }
                    }
                    needscanRegExp = false;
                }
                else
                {
                    needscanRegExp = true;
                }
            }
            else if(scanType == "c6")
            {
                if(GscanRegExp2 == "Null")
                {
                    qDebug()<<"point[901]";
                    if((Station.mid(0,2) == "OP" && GCurrentStep == "条码枪扫码") || Station.mid(0,2) != "OP"){
                        emit sendCmdToStep(1,serialNums,2);
                        emit sendCodeToMainwindow("1",serialNums);
                    }else{
                        emit sendCodeToMainwindow("别扫码,当前正在进行"+GCurrentStep,"scanStepError");
                        if(Pack1SN == "-1"){
                            emit sendCodeToMainwindow("无订单信息，请检查！","scanStepError");
                        }
                    }
                    needscanRegExp = false;
                }
                else
                {
                    needscanRegExp = true;
                }
            }
            else
            {
                qDebug()<<"scanTypeiserror"<<scanType;
            }

            if(needscanRegExp)
            {
                QStringList aaa = GscanRegExp.split(",");//当前扫码步骤所有规则1
                QStringList bbb = GscanRegExp2.split(",");//当前扫码步骤所有规则2
                QStringList ccc = GVRegexp.split(",");//当前扫码步骤所有版本检验
                QString currentCode;//当前扫码
                QString currentNeedCode;//当前需要的码
                if(ccc.isEmpty())
                {
                    vactionRegexpFlag = true;
                }
                else//有版本检验
                {
                    if(ccc.size()>=2 && serialNums.size()>46)
                    {
                        qDebug()<<"vactionRegexpFlag"<<serialNums.mid(21,5);
                        if(scanType == "c5")
                        {
                            if(ccc[0] == serialNums.mid(21,5))
                            {
                                vactionRegexpFlag = true;
                            }
                            else
                            {
                                vactionRegexpFlag = false;
                            }
                        }
                        else if(scanType == "c6")
                        {
                            if(ccc[1] == serialNums.mid(21,5))
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

                if(scanType == "c5")
                {
                    for(int i = 0; i < aaa.size(); i++)
                    {
                        currentNeedCode = aaa[i];
                        currentCode = serialNums.mid(0,currentNeedCode.size());
                        qDebug()<<"serialNums_aaa:"<<i<<currentCode<<currentNeedCode;
                        if(currentCode == currentNeedCode)//扫码中的规则==配置的规则
                        {
                            qDebug()<<"serialNums_aaa ture";
                            scanRegExpFlag = true;
                            break;
                        }
                    }
                }
                else if(scanType == "c6")
                {
                    for(int i = 0; i < bbb.size(); i++)
                    {
                        currentNeedCode = bbb[i];
                        currentCode = serialNums.mid(0,currentNeedCode.size());
                        qDebug()<<"bbb"<<i<<currentCode<<currentNeedCode;
                        if(currentCode == currentNeedCode)//扫码中的规则==配置的规则
                        {
                            qDebug()<<"bbb ture";
                            scanRegExpFlag = true;
                            break;
                        }
                    }
                }
                if(GdateDiff.isEmpty() && GdateDiff2.isEmpty())
                {
                    datediffFlag = true;
                }
                else
                {
                    if(serialNums.size() >46)
                    {
                        QDateTime localtime = QDateTime::currentDateTime();
                        QDateTime timeTemp;
                        QString aaa = "20";
                        QString bbb = serialNums.mid(38,8); //从哪一位开始是时间
                        QString day = bbb.mid(0,2);
                        QString month = bbb.mid(3,2);
                        QString year = aaa.append(bbb.mid(6,2));
                        if(serialNums.mid(0,4) == "#5QD" || serialNums.mid(0,10) == "#5NG804842")
                        {
                            if(serialNums.mid(0,4) == "#5QD"){
                                bbb = serialNums.mid(30,6);
                            }else if(serialNums.mid(0,10) == "#5NG804842"){
                                bbb = serialNums.mid(27,6);
                            }
                            day = bbb.mid(4,2);
                            month = bbb.mid(2,2);
                            year = aaa.append(bbb.mid(0,2));
                        }
                        QString datebuff = year+"-"+month+"-"+day;
                        timeTemp = QDateTime::fromString(datebuff, "yyyy-MM-dd");
                        int timediff = timeTemp.daysTo(localtime);
                        qDebug()<<"timeTemp"<<timeTemp<<datebuff<<timediff<<localtime;
                        if(GdateDiff.isEmpty())   //检测时间点之前的时间
                        {
                            QDateTime RightTime = QDateTime::fromString(GdateDiff2,"yyyy-MM-dd");
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
                        else if(GdateDiff2.isEmpty())
                        {
                            QDateTime leftTime = QDateTime::fromString(GdateDiff,"yyyy-MM-dd");
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
                            QDateTime RightTime = QDateTime::fromString(GdateDiff2,"yyyy-MM-dd");
                            QDateTime leftTime = QDateTime::fromString(GdateDiff,"yyyy-MM-dd");
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

                if(Station == "OP80" && !serialNums.contains(Pack1SN)){//OP80工位但扫码不包含订单号匹配失败
                    OP80CodeContainsPackSN = false;
                }

                //首先是在正常扫码界面步骤
                if((Station.mid(0,2) == "OP" && GCurrentStep == "条码枪扫码") || Station.mid(0,2) != "OP"){
                    //满足条件(规则匹配成功&&日期校验正常&&版本检验正常&&OP80扫码零件号包含订单号)将扫码信息发到步骤线程和主窗口
                    if(scanRegExpFlag && datediffFlag && vactionRegexpFlag && OP80CodeContainsPackSN)
                    {
                        qDebug()<<"point[902]";
                        emit sendCmdToStep(1,serialNums,2);
                        emit sendCodeToMainwindow("1",serialNums);
                    }
                    else
                    {
                        if(!scanRegExpFlag){
                            emit sendCodeToMainwindow(QString("条码校验失败!\n操作工扫码 ")+ currentCode +QString("\n现应该扫码 ")+ currentNeedCode,"301");
                        }else if(!datediffFlag){
                            emit sendCodeToMainwindow("时间校验失败！","301");
                        }else if(!vactionRegexpFlag){
                            emit sendCodeToMainwindow("软件版本检验失败！","301");
                        }else if(!OP80CodeContainsPackSN){
                            emit sendCodeToMainwindow("该零件非本次订单使用！","301");
                        }
                    }
                }else{//非扫码步骤
                    emit sendCodeToMainwindow("别扫码,当前正在进行"+GCurrentStep,"scanStepError");
                    if(Pack1SN == "-1"){
                        emit sendCodeToMainwindow("无订单信息，请检查！","scanStepError");
                    }
                }
            }
            qDebug() << "mannul tiaoma qiang"<< serialNums;
        }
        //                revComBuf.clear();
        revStartFlag = false;
        //            }
    }
    //    }
}


void SerialComsRead::revPackStatus(QVariant packStatus)
{
    BMCE_STATUS recBMCE;
    recBMCE = packStatus.value<BMCE_STATUS>();
    Pack1SN = recBMCE.Pack1SN;
    //    qDebug()<<"SerialComsRead::revPackStatus:"<<Pack1SN;
}
