#include "interface.h"

Interface::Interface(QObject *parent) :
    QObject(parent)
{
    this->moveToThread(&m_thread);
    m_thread.start();

    QTimer::singleShot(5000,this,SLOT(dyUpdateTask()));
}

void Interface::init()
{
    qDebug() <<"Interface start!!";
    isEmergencyStop = false;
    TIMEOUT = (10 * 1000);
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    QString DataServerIp = configIniRead->value("baseinfo/DataServerIp").toString();
    QString ServerPort = configIniRead->value("baseinfo/ServerPort").toString();
    QString AGVDataServerIp = configIniRead->value("baseinfo/AGVServerIp").toString();
    QString AGVServerPort = configIniRead->value("baseinfo/AGVServerPort").toString();
    QString statusDataServerIp = configIniRead->value("baseinfo/statusServerIp").toString();
    QString statusServerPort = configIniRead->value("baseinfo/statusServerPort").toString();
    delete configIniRead;
    packTypeTemp = 0;
    StatusTemp = 8;
    oneTimesRePost = true;
    manager = new QNetworkAccessManager(this);
    Server = "http://"+DataServerIp+":"+ServerPort;
    AGVServer = "http://"+AGVDataServerIp+":"+AGVServerPort;
    statusServer = "http://"+statusDataServerIp+":"+statusServerPort;
    //    QTimer::singleShot(0,this,SLOT(uplodeStationStatus()));
    connect(&timerTest,SIGNAL(timeout()),this,SLOT(uplodeStationStatus()));
    timerTest.start(1000);
    //    req_packstatus.setUrl(QUrl("http://"+DataServerIp+":"+ServerPort+"/getstatus/packstatus?"));
    //    req_ErrorMessage.setUrl(QUrl("http://"+DataServerIp+":"+ServerPort+"/ErrorMessage?"));

}

//工位（拧紧枪）状态 上传采集大屏工位状态
void Interface::uplodeStationStatus()
{
    if(StatusTemp != GTightStatus){
        StatusTemp = GTightStatus;
        QString DataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QVariantMap dataMap;
        QPair<QString,QVariantMap>taskPair;
        dataMap.insert("OptionType","0");
        dataMap.insert("StationCode",Station);
        dataMap.insert("Status",GTightStatus);
        dataMap.insert("UpdateTime",DataTime);
        taskPair.first = "/Upload/UploadStationStatus";
        taskPair.second = dataMap;
        taskList<<taskPair;
        qDebug()<<"uplodeStationStatus:"<<taskPair;
    }
}

void Interface::revGetTrayAndModuleStatus(QString traySN, int LineID)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("TraySN",traySN);
    dataMap.insert("LineID",LineID);
    taskPair.first = "/getstatus/getTrayAndModuleStatus?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"revGetTrayAndModuleStatus:"<<taskPair;
}

void Interface::reqTightStatus(QString station, QString tarStation, QString packSN)
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("StationID",station);
    dataMap.insert("tarStationID",tarStation);
    dataMap.insert("packSN",packSN);
    taskPair.first = "/getstatus/packstatus?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"reqTightStatus:"<<taskPair;
}

//模组线获取电池包类型
void Interface::revgetTrayPackType(QString TrarySN)
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("TrarySN",TrarySN);
    dataMap.insert("StationID",Station);
    taskPair.first = "/getstatus/getTrayPackType?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"revgetTrayPackType:"<<taskPair;
}

void Interface::revAddTrayPackType(QString TrarySN)
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("TrarySN",TrarySN);
    dataMap.insert("StationID",Station);
    dataMap.insert("PackType",GPack1SN);
    taskPair.first = "/upload/AddTrayPackType?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"revAddTrayPackType:"<<taskPair;
}

void Interface::revgetStationTimer(QString PackSN, int Type,QString ReqStationID)
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("PackSN",PackSN);
    dataMap.insert("StationID",Station);
    dataMap.insert("Type",Type);
    dataMap.insert("ReqStationID",ReqStationID);
    taskPair.first = "/getstatus/getStationTimer?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"revgetStationTimer:"<<taskPair;
}

void Interface::reqGlueWorkFlag(QString packsn)
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("PackSN",packsn);
    dataMap.insert("StationID",Station);
    taskPair.first = "/getstatus/getTJOneStatus";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"reqGlueWorkFlag:"<<taskPair;
}

void Interface::reqdutouTightStatus(QString sendAGVSN, QString sendStation)
{
    this->sendStation = sendStation;
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("AGVSN",sendAGVSN);
    dataMap.insert("Station",sendStation);//发送接口的工位名有调整，非本地工位名
    taskPair.first = "/query/getTightenScrewInfo";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"reqdutouTightStatus:"<<taskPair;
}

//void Interface::rereqPackStatus()
//{
//    bool isSuccess = false;
//    QString ErrorCode = "";
//    QVariantMap dataMap;
//    dataMap.insert("StationID",Station);
//    QString url = "/getstatus/packstatus?";
//    RESULT resultStruct = HTTP_Req(url,dataMap);
//    //    RESULT resultStruct;
//    //    resultStruct = returnResult.value<RESULT>();
//    //    qDebug()<<"&&&&&&&&&&&"<<resultStruct.isSuccess<<resultStruct.Result;
//    if(resultStruct.isSuccess || GIsTestStartFlag)
//    {
//        QVariantMap result = resultStruct.Result;
//        bool Result = result["Result"].toBool();
//        if(!Result)
//        {
//            ErrorCode = result["Errorcode"].toString();
//            PACK_STATUS pack;
//            pack.Result = false;
//            pack.Errorcode = ErrorCode;
//            QVariant DataResult;
//            DataResult.setValue(pack);
//            //            rereqPackStatus();
//            emit sendPackStatus(DataResult);
//            qDebug()<< "reGet PackStatus fail Result = 0"<<ErrorCode;
//        }
//        else if(Result||GIsTestStartFlag)
//        {
//            isSuccess = true;
//            PACK_STATUS pack;
//            if(GIsTestStartFlag){
//                pack.Result = true;
//                pack.AGVSN = "18";
//                pack.PackType = "1";
//                pack.Pack1SN = "288 5UY849A001S";
//                pack.Pack2SN = "288 5UY849A002T";
//                pack.Pack1Status = "1";
//                pack.Pack2Status = "1";
//            }else{
//                pack.Result = true;
//                pack.AGVSN = result["AGVSN"].toString();
//                pack.PackType = result["Type"].toString();
//                pack.Pack1SN = result["Pack1SN"].toString();
//                pack.Pack2SN = result["Pack2SN"].toString();
//                pack.Pack1Status = result["Pack1Status"].toString();
//                pack.Pack2Status = result["Pack2Status"].toString();
//            }

//            //获取到的小车型号
//            if(pack.Pack1SN.isEmpty())
//            {
//                qDebug()<<"pack1sn isempty";
//                GPack1SN = "9";
//            }
//            else
//            {
//                GPack1SN = pack.Pack1SN.mid(10,1);
//            }
//            if(pack.Pack2SN.isEmpty())
//            {
//                qDebug()<<"pack2sn isempty";
//                GPack2SN = "9";
//            }
//            else
//            {
//                GPack2SN = pack.Pack2SN.mid(10,1);
//            }
//            // emit sendStates("Pack1SN:"+pack.Pack1SN+ ", Pack2SN:"+pack.Pack2SN);
//            /*emit sendPack1And2Type(result["Pack1SN"].toString().mid(10,1),
//                    result["Pack2SN"].toString().mid(10,1));*/

//            QVariant DataResult;
//            DataResult.setValue(pack);
//            emit sendPackStatus(DataResult);
//            qDebug()<< "reGet PackStatus success:"<<pack.PackType<<pack.Pack1SN<<pack.Pack2SN<<pack.Pack1Status<<pack.Pack2Status;
//        }
//    }
//    //    if(!isSuccess)
//    else
//    {
//        qDebug()<<"reGet PackStatus fail";
//        PACK_STATUS pack;
//        pack.Result = false;
//        pack.Errorcode = ErrorCode;

//        QVariant DataResult;
//        DataResult.setValue(pack);
//        rereqPackStatus();
//        emit sendPackStatus(DataResult);
//    }
//}

//void Interface::rerecIsMatchModuleKeyCode(QString PackType, QString PackSN)
//{
//    bool isSuccess = false;
//    QVariantMap dataMap;
//    dataMap.insert("PackSN",PackSN);
//    dataMap.insert("Type",PackType.toInt());
//    QString url = "/Getstatus/IsMatchModuleKeyCode?";
//    RESULT resultStruct = HTTP_Req(url,dataMap);
//    if(resultStruct.isSuccess)
//    {
//        QVariantMap result = resultStruct.Result;
//        bool Result = result["Result"].toBool();
//        if(!Result)
//        {
//            rerecIsMatchModuleKeyCode(PackType,PackSN);
//            qDebug()<< "Get  IsMatchModuleKeyCode status fail";
//        }
//        else if(Result)
//        {
//            if(result["ReturnStatus"].toBool())
//            {
//                qDebug()<< "Get ReturnStatus success:";
//                emit sendGetStationkeyCodeResult(true);
//            }
//            else
//            {
//                qDebug()<< "Get ReturnStatus false:";
//                emit sendGetStationkeyCodeResult(false);
//            }
//            isSuccess = true;
//            //            emit sendGetStationkeyCodeResult(true);
//            //            qDebug()<< "Get IsMatchModuleKeyCode success:";
//        }
//    }
//    if(!isSuccess)
//    {
//        rerecIsMatchModuleKeyCode(PackType,PackSN);
//        qDebug()<<"Get IsMatchModuleKeyCode fail";
//    }
//}

void Interface::recIsMatchModuleKeyCode(QString PackType, QString PackSN)
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("PackSN",PackSN);
    dataMap.insert("Type",PackType.toInt());
    taskPair.first = "/Getstatus/IsMatchModuleKeyCode?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"recIsMatchModuleKeyCode:"<<taskPair;
}

//void Interface::reRevgetStationkeyCode(QString PackType, QString PackSN)
//{
//    bool isSuccess = false;
//    QVariantMap dataMap;
//    dataMap.insert("PackSN",PackSN);
//    dataMap.insert("Station","A010");
//    dataMap.insert("Type",PackType.toInt());
//    QString url = "/Getstatus/getStationkeyCode?";
//    RESULT resultStruct = HTTP_Req(url,dataMap);
//    if(resultStruct.isSuccess)
//    {
//        QVariantMap result = resultStruct.Result;
//        bool Result = result["Result"].toBool();
//        int ReturnStatus = result["ReturnStatus"].toInt();
//        if(!Result)
//        {
//            //            emit sendGetStationkeyCodeResult(false);
//            //            qDebug()<< "Get  getStationkeyCode status fail";
//            reRevgetStationkeyCode(PackType,PackSN);
//        }
//        else if(Result)
//        {
//            isSuccess = true;
//            if(ReturnStatus == 1)
//            {
//                emit sendGetStationkeyCodeResult(true);
//                qDebug()<< "Get getStationkeyCode success:";
//            }
//            else
//            {
//                emit sendGetStationkeyCodeResult(false);
//                qDebug()<< "Get  getStationkeyCode status fail";
//            }
//        }
//    }
//    if(!isSuccess)
//    {
//        reRevgetStationkeyCode(PackType,PackSN);
//        qDebug()<<"Get getStationkeyCode fail";
//    }
//}

//revgetStationkeyCode
void Interface::revgetStationkeyCode(QString PackType, QString PackSN)
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("PackSN",PackSN);
    dataMap.insert("Station","A010");
    dataMap.insert("Type",PackType.toInt());
    taskPair.first = "/Getstatus/getStationkeyCode?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"revgetStationkeyCode:"<<taskPair;
}
void Interface::revAddStationTimer(QString PackSN, int Type)
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("PackSN",PackSN);
    dataMap.insert("StationID",Station);
    dataMap.insert("Type",Type);
    taskPair.first = "/upload/AddStationTimer?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"revAddStationTimer:"<<taskPair;
}

//获取PACK状态
void Interface::reqPackStatus()
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("StationID",Station);
    taskPair.first = "/getstatus/packstatus?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"reqPackStatus:"<<taskPair;
}

//void Interface::reboundPack()
//{
//    bool isSuccess = false;
//    QString ErrorCode = "";
//    QVariantMap dataMap;
//    dataMap.insert("StationID",Station);
//    QString url = "/binding/packandagv?";
//    RESULT resultStruct = HTTP_Req(url,dataMap);
//    if(resultStruct.isSuccess)
//    {
//        QVariantMap result = resultStruct.Result;
//        bool Result = result["Result"].toBool();
//        if(!Result)
//        {
//            ErrorCode = result["Errorcode"].toString();
//            PACK_STATUS pack;
//            pack.Result = false;
//            pack.Errorcode = ErrorCode;
//            QVariant DataResult;
//            DataResult.setValue(pack);
//            emit sendPackStatus(DataResult);
//            qDebug()<< "reboundPack fail Result = 0"<<ErrorCode;
//        }
//        else if(Result)
//        {
//            isSuccess = true;
//            PACK_STATUS pack;
//            pack.Result = true;
//            pack.PackType = result["Type"].toString();
//            pack.Pack1SN = result["Pack1SN"].toString();
//            pack.Pack2SN = result["Pack2SN"].toString();
//            pack.Pack1Status = "1";
//            pack.Pack2Status = "1";

//            QVariant DataResult;
//            DataResult.setValue(pack);
//            emit sendPackStatus(DataResult);
//            qDebug()<< "reboundPack success:"<<pack.PackType<<pack.Pack1SN<<pack.Pack2SN;
//        }
//    }
//    if(!isSuccess)
//    {
//        qDebug()<<"reboundPack fail";
//        PACK_STATUS pack;
//        pack.Result = false;
//        pack.Errorcode = ErrorCode;

//        QVariant DataResult;
//        DataResult.setValue(pack);
//        reboundPack();
//        emit sendPackStatus(DataResult);
//    }
//}

//A010 绑定流水号 并获取PACK状态
void Interface::boundPack()
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("StationID",Station);
    taskPair.first = "/binding/packandagv?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"boundPack:"<<taskPair;
}


//VM010 获取模组型号
void Interface::reqModule()
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("StationID",Station);
    dataMap.insert("LineID",Line_ID);
    taskPair.first = "/getstatus/moduletype?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"reqModule:"<<taskPair;
}
//void Interface::remoduleTray4(QString TraySN,QString module1 ,QString module2, QString module3, QString module4)
//{
//    bool isSuccess = false;
//    QVariantMap dataMap;
//    if(Station == "M100B")
//    {
//        dataMap.insert("StationID","VM040B");
//    }
//    else
//    {
//        dataMap.insert("StationID",Station);
//    }
//    dataMap.insert("LineID",Line_ID);
//    dataMap.insert("TraySN",TraySN);
//    if(TraySN.mid(0,1) == "P")
//    {
//        packTypeTemp = 1;
//    }
//    else
//    {
//        packTypeTemp = 2;
//    }
//    dataMap.insert("Type",packTypeTemp);
//    dataMap.insert("M1",module1);
//    dataMap.insert("M2",module2);
//    dataMap.insert("M3",module3);
//    dataMap.insert("M4",module4);

//    QString url = "/binding/trayandmodule?";
//    RESULT resultStruct = HTTP_Req(url,dataMap);

//    if(resultStruct.isSuccess)
//    {
//        QVariantMap result = resultStruct.Result;
//        bool Result = result["Result"].toBool();
//        if(!Result)
//        {
//            qDebug()<< "moduleTray fail Result = 0";
//        }
//        else if(Result)
//        {
//            isSuccess = true;
////            emit sendModuleTray(true);
//            qDebug()<< "moduleTray success";
//        }
//    }
//    //    if(!isSuccess)
//    else
//    {
//        qDebug()<<"moduleTray fail";
////        emit sendModuleTray(false);
//        remoduleTray4(TraySN,module1 ,module2, module3, module4);
//    }
//}

//VM040 托盘码和模组码绑定
void Interface::moduleTray(QString TraySN,QString module1 ,QString module2, QString module3, QString module4)
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    if(Station == "M100B")
    {
        dataMap.insert("StationID","VM040B");
    }
    else
    {
        dataMap.insert("StationID",Station);
    }
    dataMap.insert("LineID",Line_ID);
    dataMap.insert("TraySN",TraySN);
    if(TraySN.mid(0,1) == "P")
    {
        packTypeTemp = 1;
    }
    else
    {
        packTypeTemp = 2;
    }
    dataMap.insert("Type",packTypeTemp);
    dataMap.insert("M1",module1);
    dataMap.insert("M2",module2);
    dataMap.insert("M3",module3);
    dataMap.insert("M4",module4);
    taskPair.first = "/binding/trayandmodule?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"moduleTray4:"<<taskPair;
}

//void Interface::remoduleTray(QString TraySN,QString module1 ,QString module2, QString module3, QString module4,QString module5 ,QString module6, QString module7, QString module8)
//{
//    qDebug()<<"this is second VM010 bind";
//    bool isSuccess = false;
//    QVariantMap dataMap;
//    if(Station == "M100B")
//    {
//        dataMap.insert("StationID","VM040B");
//    }
//    else
//    {
//        dataMap.insert("StationID",Station);
//    }
//    dataMap.insert("LineID",Line_ID);
//    dataMap.insert("TraySN",TraySN);
//    if(TraySN.mid(0,1) == "P")
//    {
//        packTypeTemp = 1;
//    }
//    else
//    {
//        packTypeTemp = 2;
//    }
//    dataMap.insert("Type",packTypeTemp);
//    dataMap.insert("M1",module1);
//    dataMap.insert("M2",module2);
//    dataMap.insert("M3",module3);
//    dataMap.insert("M4",module4);
//    dataMap.insert("M5",module5);
//    dataMap.insert("M6",module6);
//    dataMap.insert("M7",module7);
//    dataMap.insert("M8",module8);

//    QString url = "/binding/trayandmodule?";
//    RESULT resultStruct = HTTP_Req(url,dataMap);

//    if(resultStruct.isSuccess)
//    {
//        QVariantMap result = resultStruct.Result;
//        bool Result = result["Result"].toBool();
//        if(!Result)
//        {
//            qDebug()<< "re8moduleTray fail Result = 0";
//        }
//        else if(Result)
//        {
//            isSuccess = true;
////            emit sendModuleTray(true);
//            qDebug()<< "re8moduleTray success";
//        }
//    }
//    if(!isSuccess)
//    {
//        qDebug()<<"re8moduleTray fail";
//        remoduleTray(TraySN,module1 ,module2,module3,module4,module5 ,module6,module7,module8);
////        emit sendModuleTray(false);
//    }
//}

//VM040 托盘码和模组码绑定16
void Interface::moduleTray(QString TraySN,QString module1 ,QString module2, QString module3, QString module4,QString module5 ,QString module6, QString module7, QString module8,QString module9 ,QString module10, QString module11, QString module12,QString module13 ,QString module14, QString module15, QString module16)
{
    qDebug()<<"this is rec VM040 bind16";
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    if(Station == "M100B")
    {
        dataMap.insert("StationID","VM040B");
    }
    else
    {
        dataMap.insert("StationID",Station);
    }
    dataMap.insert("LineID",Line_ID);
    dataMap.insert("TraySN",TraySN);
    if(TraySN.mid(0,1) == "P")
    {
        packTypeTemp = 1;
    }
    else
    {
        packTypeTemp = 2;
    }
    dataMap.insert("Type",packTypeTemp);
    dataMap.insert("M1",module1);
    dataMap.insert("M2",module2);
    dataMap.insert("M3",module3);
    dataMap.insert("M4",module4);
    dataMap.insert("M5",module5);
    dataMap.insert("M6",module6);
    dataMap.insert("M7",module7);
    dataMap.insert("M8",module8);
    dataMap.insert("M9",module9);
    dataMap.insert("M10",module10);
    dataMap.insert("M11",module11);
    dataMap.insert("M12",module12);
    dataMap.insert("M13",module13);
    dataMap.insert("M14",module14);
    dataMap.insert("M15",module15);
    dataMap.insert("M16",module16);

    taskPair.first = "/binding/trayandmodule?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"moduleTray16:"<<taskPair;
}


//void Interface::remoduleTray16(QString TraySN,QString module1 ,QString module2, QString module3, QString module4,QString module5 ,QString module6, QString module7, QString module8,QString module9 ,QString module10, QString module11, QString module12,QString module13 ,QString module14, QString module15, QString module16)
//{
//    qDebug()<<"this is rerec VM040 bind16";
//    bool isSuccess = false;
//    QVariantMap dataMap;
//    if(Station == "M100B")
//    {
//        dataMap.insert("StationID","VM040B");
//    }
//    else
//    {
//        dataMap.insert("StationID",Station);
//    }
//    dataMap.insert("LineID",Line_ID);
//    dataMap.insert("TraySN",TraySN);
//    if(TraySN.mid(0,1) == "P")
//    {
//        packTypeTemp = 1;
//    }
//    else
//    {
//        packTypeTemp = 2;
//    }
//    dataMap.insert("Type",packTypeTemp);
//    dataMap.insert("M1",module1);
//    dataMap.insert("M2",module2);
//    dataMap.insert("M3",module3);
//    dataMap.insert("M4",module4);
//    dataMap.insert("M5",module5);
//    dataMap.insert("M6",module6);
//    dataMap.insert("M7",module7);
//    dataMap.insert("M8",module8);
//    dataMap.insert("M9",module9);
//    dataMap.insert("M10",module10);
//    dataMap.insert("M11",module11);
//    dataMap.insert("M12",module12);
//    dataMap.insert("M13",module13);
//    dataMap.insert("M14",module14);
//    dataMap.insert("M15",module15);
//    dataMap.insert("M16",module16);

//    QString url = "/binding/trayandmodule?";
//    RESULT resultStruct = HTTP_Req(url,dataMap);

//    if(resultStruct.isSuccess)
//    {
//        QVariantMap result = resultStruct.Result;
//        bool Result = result["Result"].toBool();
//        if(!Result)
//        {
//            qDebug()<< "16moduleTray fail Result = 0";
//        }
//        else if(Result)
//        {
//            isSuccess = true;
////            emit sendModuleTray(true);
//            qDebug()<< "16moduleTray success";
//        }
//    }
//    if(!isSuccess)
//    {
//        remoduleTray16(TraySN,module1 ,module2,module3,module4,module5 ,module6,module7,module8,module9 ,module10,module11,module12,module13 ,module14,module15,module16);
//        qDebug()<<"16moduleTray fail";
////        emit sendModuleTray(false);
//    }
//}

//VM040 托盘码和模组码绑定
void Interface::moduleTray(QString TraySN,QString module1 ,QString module2, QString module3, QString module4,QString module5 ,QString module6, QString module7, QString module8)
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    if(Station == "M100B")
    {
        dataMap.insert("StationID","VM040B");
    }
    else
    {
        dataMap.insert("StationID",Station);
    }
    dataMap.insert("LineID",Line_ID);
    dataMap.insert("TraySN",TraySN);
    if(TraySN.mid(0,1) == "P")
    {
        packTypeTemp = 1;
    }
    else
    {
        packTypeTemp = 2;
    }
    dataMap.insert("Type",packTypeTemp);
    dataMap.insert("M1",module1);
    dataMap.insert("M2",module2);
    dataMap.insert("M3",module3);
    dataMap.insert("M4",module4);
    dataMap.insert("M5",module5);
    dataMap.insert("M6",module6);
    dataMap.insert("M7",module7);
    dataMap.insert("M8",module8);
    taskPair.first = "/binding/trayandmodule?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"moduleTray8:"<<taskPair;
}
//BMCE首工位绑定判断状态
void Interface::reqBMCEStatus(QString TraySN)
{
    //待上传数据放在任务表
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("StationID",Station);
    dataMap.insert("TraySN",TraySN);
    if(Station == "OP10-1" || Station == "OP10-2")
    {
        taskPair.first = "/binding/TrayandBmce?";
    }
    else
    {
        taskPair.first = "/getstatus/packstatus?";
    }
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"reqBMCEStatus:"<<taskPair;
}

//void Interface::remodulePack(QString TraySN, QString packSN)
//{
//    bool isSuccess = false;
//    QVariantMap dataMap;
//    dataMap.insert("StationID",Station);
//    dataMap.insert("LineID",Line_ID);
//    dataMap.insert("TraySN",TraySN);
//    dataMap.insert("PackSN",packSN);

//    QString url = "/binding/packsnandmodule?";
//    RESULT resultStruct = HTTP_Req(url,dataMap);

//    if(resultStruct.isSuccess)
//    {
//        QVariantMap result = resultStruct.Result;
//        bool Result = result["Result"].toBool();
//        if(!Result)
//        {
//            qDebug()<< "remodulePack fail Result = 0";
//            emit sendModulePack(false);
//        }
//        else if(Result)
//        {
//            isSuccess = true;
//            emit sendModulePack(true);
//            qDebug()<< "remodulePack success";
//        }
//    }
//    //    if(!isSuccess)
//    else
//    {
//        qDebug()<<"remodulePack fail";
//        emit sendModulePack(false);
//    }
//}

//A090 托盘码上传 pack与模组绑定
void Interface::modulePack(QString TraySN,QString packSN)
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("StationID",Station);
    dataMap.insert("LineID",Line_ID);
    dataMap.insert("TraySN",TraySN);
    dataMap.insert("PackSN",packSN);
    taskPair.first = "/binding/packsnandmodule?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"modulePack:"<<taskPair;
}


//A010 下箱体条码上传 AGV车号与下箱体绑定 关键条码
void Interface::Keybarcode(QString keyBarcode,QString PackSN ,QString time, int OrderSN)
{
//    QString scanRegExpCode = GscanRegExpCode;
//    QVariantMap dataMap;
//    QPair<QString,QVariantMap>taskPair;
//    dataMap.insert("StationID",Station);
//    dataMap.insert("LineID",Line_ID);
//    dataMap.insert("KeyCode",keyBarcode);
//    dataMap.insert("PackSN",PackSN);
//    dataMap.insert("DataTime",time);
//    dataMap.insert("OrderSN",OrderSN);
//    dataMap.insert("PartsCode",GscanRegExpCode);
//    if(Station.mid(0,2) == "VM"){
//        taskPair.first =  "/Module/mKeybarcode";
//    }else{
//        taskPair.first =  "/upload/Keybarcode?";
//    }
//    taskPair.second = dataMap;
//    taskList<<taskPair;
//    qDebug()<<"Keybarcode:"<<taskPair;
//    emit sendKeyCodeInfo(Line_ID,OrderSN,scanRegExpCode,keyBarcode,PackSN,Station);//发到本地数据库存储
}


//站完成状态上传
void Interface::stationState(QString Pack1SN,QString Pack1Status,QString Pack2SN,QString Pack2Status, QString time)
{
    //待上传数据放在任务表
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("StationID",Station);
    dataMap.insert("Pack1SN",Pack1SN);
    dataMap.insert("Pack1Status",Pack1Status);
    dataMap.insert("Pack2SN",Pack2SN);
    dataMap.insert("Pack2Status",Pack2Status);
    dataMap.insert("Time",time);
    dataMap.insert("EmployeeNo",EKSBuff);
    if(Station.mid(0,2) == "VM"){
        taskPair.first = "/Module/mstationstate?";
    }else{
        taskPair.first = "/upload/stationstate?";
    }
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"stationState:"<<taskPair;
}
//void Interface::resendPackstatus(QString Pack1SN,QString Pack1Status,QString Pack2SN,QString Pack2Status, QString time)
//{
//    qDebug()<<"this is resend stationstate Pack1SN"<<Pack1SN<<"Pack1Status"<<Pack1Status<<"Pack2SN"<<Pack2SN<<"Pack2Status"<<Pack2Status;
//    bool isSuccess = false;
//    QVariantMap dataMap;
//    dataMap.insert("StationID",Station);

//    dataMap.insert("Pack1SN",Pack1SN);
//    dataMap.insert("Pack1Status",Pack1Status);
//    dataMap.insert("Pack2SN",Pack2SN);
//    dataMap.insert("Pack2Status",Pack2Status);
//    dataMap.insert("Time",time);

//    QString url = "";
//    if(Station.mid(0,2) == "VM"){
//        url = "/Module/mstationstate?";
//    }else{
//        url = "/upload/stationstate?";
//    }
//    RESULT resultStruct = HTTP_Req(url,dataMap);

//    if(resultStruct.isSuccess)
//    {
//        QVariantMap result = resultStruct.Result;
//        bool Result = result["Result"].toBool();
//        if(!Result)
//        {
//            qDebug()<< "restationState fail Result = 0";
//            //            resendPackstatus(Pack1SN,Pack1Status,Pack2SN,Pack2Status,time);
//        }
//        else if(Result)
//        {
//            isSuccess = true;
//            emit sendStationState(true);
//            qDebug()<< "restationState success";
//        }
//    }
//    //    if(!isSuccess)
//    else
//    {
//        qDebug()<<"restationState fail";
//        emit sendStationState(false);
//        sleep(3);
//        resendPackstatus(Pack1SN,Pack1Status,Pack2SN,Pack2Status,time);
//    }
//}

void Interface::reqRepairBatterypack(QString packSN)
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("PackSN",packSN);
    taskPair.first = "/query/getRepairBatterypack?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"reqRepairBatterypack:"<<taskPair;
}

void Interface::requestAgvLeave()
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("StationID",Station);

    QString url = "/AGVS/RequestAgvLeave?";
    RESULT resultStruct = HTTP_Req1(url,dataMap);

    if(!(resultStruct.isSuccess))
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["ResultCode"].toBool();
        if(Result==0)
        {
            qDebug()<< "RequestAgvLeave success Result = 0";
            isSuccess = true;
            emit sendAgvLeaveState(true);
        }
        else if(Result==1)
        {

            qDebug()<< "sendAgvLeaveState There is no car!!!!";
        }
        else if(Result==2)
        {

            qDebug()<< "sendAgvLeaveState There is no car signal of ready!!!!";
        }
        else if(Result==3)
        {

            qDebug()<< "sendAgvLeaveState The car is manual control !!!!";
        }
        else
        {

            qDebug()<< "sendAgvLeaveState other error !!!!";
        }

    }
    if(isSuccess)
    {
        qDebug()<<"sendAgvLeaveState fail";
        emit sendAgvLeaveState(false);
    }
}


void Interface::moduleCount()
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("StationID",Station);
    dataMap.insert("LineID",Line_ID);
    taskPair.first = "/count/modulecount?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"moduleCount:"<<taskPair;
}

void Interface::reqEmergencyStop(bool isMove)
{
    isEmergencyStop = true;
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("StationID",Station);
    dataMap.insert("Status",isMove);
    taskPair.first = "/AGVS/EmergencyStop?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"reqEmergencyStop:"<<taskPair;
}

//普通使用的IP：DataServerIp  端口：ServerPort	OP工位（IP：DataServerIp 端口：51565）
RESULT Interface::HTTP_Req(QString url,QVariantMap dataMap)   //上传接口
{
    urlTemp = url;
    dataMapTemp = dataMap;
    bool isSuccess = false;
    QVariantMap result;
    qDebug()<<"readyPost[0]:"<<url;
        QJson::Serializer serializer;
        bool ok;
        QByteArray json = serializer.serialize(dataMap, &ok);
        qDebug()<<"post send"<<url<<json;

        if(isEmergencyStop)
        {
            req.setUrl(QUrl(AGVServer+url));
            isEmergencyStop = false;
        }
        else
        {
            if(Station.mid(0,2) == "OP")
            {
                QSettings *configIniRead_BMCE = new QSettings("/config.ini", QSettings::IniFormat);
                QString DataServerIp_BMCE = configIniRead_BMCE->value("baseinfo/DataServerIp").toString();
                QString Server_BMCE;
                if(url == "/Upload/UploadStationStatus"){
                    Server_BMCE = "http://"+DataServerIp_BMCE+":"+"51564";
                }else{
                    Server_BMCE = "http://"+DataServerIp_BMCE+":"+"51565";
                }
                req.setUrl(QUrl(Server_BMCE+url));
            }
            else
            {
                if(url == "/Upload/UploadStationStatus"){
                    req.setUrl(QUrl(statusServer+url));
                }else{
                    req.setUrl(QUrl(Server+url));
                }
            }
        }
        //            req.setUrl(QUrl(Server+url));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        req.setHeader(QNetworkRequest::ContentLengthHeader, json.length());

        if (ok)
        {
            QNetworkReply *reply = manager->post(req,json);

            reply->ignoreSslErrors();
            QTime t_time;
            t_time.start();

            bool timeout = false;
            qDebug()<<"point[701]";
//            qDebug()<<reply->waitForBytesWritten(5*1000);
//            qDebug()<<"point[702]";
//            qDebug()<<reply->waitForReadyRead(5*1000);
//            bool use = true;

//            QDateTime time1 = QDateTime::currentDateTime();
//            int count = 0;
            int timeCountTemp = 0;
            while (!reply->isFinished()) {
//                QDateTime time2 = QDateTime::currentDateTime();
                int currentTime = t_time.elapsed();
//                int currentTime = time1.msecsTo(time2);
                if(currentTime%15 == 0 && timeCountTemp != currentTime){
                    timeCountTemp = currentTime;
//                    use = false;
//                    count = 0;
//                    qDebug()<<"processEvents[000]"<<currentTime;
                    QApplication::processEvents();
//                    qDebug()<<"processEvents[111]"<<currentTime;
                }

                if (currentTime >= TIMEOUT) {
                    timeout = true;
                    qDebug()<< "reply timeout fail:"<<url;
                    break;
                }

//                count++;
            }

            qDebug()<<"point[703]";


            if (!timeout && reply->error() == QNetworkReply::NoError) {
                QByteArray bytes = reply->readAll();
                qDebug()<<"*post receive*"<<url<<bytes;//打印小车所有信息
                QJson::Parser parser;
                bool ok;

                result = parser.parse(bytes, &ok).toMap();
                if (!ok) {
                    qDebug()<<"HTTP_Req An error occurred during parsing"<<bytes;
                }
                else {
                    qDebug()<<"point[601]";
                    //只要接口有回复就清除掉该接口任务
                    QPair<QString,QVariantMap>taskPairTemp;
//                    readWriteLock_taskList.lockForRead();
                    bool getUrl = false;
                    foreach(taskPairTemp,taskList){
                        if(taskPairTemp.first == url){
                            getUrl = true;
                            break;
                        }
                    }
                    if(getUrl){
                        //                    readWriteLock_taskList.unlock();
                        //                    qDebug()<<"point[602]";
                        //                    readWriteLock_taskList.lockForWrite();
                        int removeIndex = taskList.indexOf(taskPairTemp);
                        qDebug()<<"point[603]:"<<removeIndex<<taskPairTemp;
                        taskList.removeAt(removeIndex);//removeAt没有越界概念，-1也不会影响运行
                        //                    readWriteLock_taskList.unlock();
                        qDebug()<<"point[604]";
                    }

                    if(result["Result"].toBool())
                    {
                        isSuccess = true;
                    }
                    else
                    {
                        qDebug()<<"this is result"<<result["Result"].toBool();
                        isSuccess = true;
                    }
                }
            }
            else if(reply->error() != QNetworkReply::NoError)
            {
                qDebug()<<"HTTP_Req handle errors here";
                QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
                qDebug( "HTTP_Req found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
                qDebug()<<reply->errorString();
            }
            reply->deleteLater();
        }
        else
        {
            qDebug() << "HTTP_Req Something went wrong:" << serializer.errorMessage();
        }
        RESULT resultStruct;
        resultStruct.isSuccess = isSuccess;
        resultStruct.Result = result;

        return resultStruct;
}

//普通使用的IP：AGVServerIp  端口：AGVServerPort
RESULT Interface::HTTP_Req1(QString url,QVariantMap dataMap)   //上传接口
{
    bool isSuccess = false;
    QVariantMap result;
    WIFIlock.lockForRead();
    if(WIFI_STATE)//服务器
    {
        WIFIlock.unlock();

        QJson::Serializer serializer;
        bool ok;
        QByteArray json = serializer.serialize(dataMap, &ok);
        qDebug()<<"Req json111"<<json;
        qDebug()<<"AGVServerIp"<<AGVServer;
        req.setUrl(QUrl(AGVServer+url));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        req.setHeader(QNetworkRequest::ContentLengthHeader, json.length());

        if (ok)
        {
            QNetworkReply *reply = manager->post(req,json);

            reply->ignoreSslErrors();
            QTime t_time;
            t_time.start();

            bool timeout = false;

            while (!reply->isFinished()) {
                QApplication::processEvents();
                if (t_time.elapsed() >= TIMEOUT) {
                    timeout = true;
                    qDebug()<< "reply timeout fail.";
                    break;
                }
            }

            if (!timeout && reply->error() == QNetworkReply::NoError) {
                QByteArray bytes = reply->readAll();
                qDebug()<<"post receive:"<<bytes;//打印小车所有信息

                QJson::Parser parser;
                bool ok;

                result = parser.parse(bytes, &ok).toMap();
                //                emit sendStates("Pack1SN:"+result["Pack1SN"].toString()+
                //                        ", Pack2SN:"+result["Pack2SN"].toString());
                if (!ok) {
                    qDebug()<<"HTTP_Req An error occurred during parsing"<<bytes;
                }
                else {
                    isSuccess = true;
                }
            }
            else if(reply->error() != QNetworkReply::NoError)
            {
                qDebug()<<"HTTP_Req handle errors here";
                QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
                qDebug( "HTTP_Req found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
                qDebug()<<reply->errorString();
            }

            reply->deleteLater();
        }
        else
        {
            qDebug() << "HTTP_Req Something went wrong:" << serializer.errorMessage();
        }
    }
    else
    {
        WIFIlock.unlock();
        qDebug()<< "HTTP_Req wifi unconnect fail";
    }

    RESULT resultStruct;
    resultStruct.isSuccess = isSuccess;
    resultStruct.Result = result;
    return resultStruct;
}


//站完成后OP80工位解绑托盘
void Interface::BMCETrayClean(QString TarySN, QString Pack1SN)
{
    //待上传数据放在任务表
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("AGVSN",TarySN);
    dataMap.insert("PackSN",Pack1SN);
    taskPair.first = "/ubinding/packandagv?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"BMCETrayClean:"<<taskPair;
}

void Interface::modulePackClean(QString TraySN) //A010-1/-2返回托盘数据
{
    bool isSuccess = false;
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("TraySN",TraySN);
    taskPair.first = "/binding/MoveTrayandModuleData?";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"modulePackClean:"<<taskPair;
}


//进站/出站信息上传
void Interface::onUpInAndOutStation(int Type, QString Pack1SN, QString Pack2SN, QString StationID, QString DataTime)
{
    if("-1" == Pack1SN){
        qDebug()<<"onUpInAndOutStation Pack1SN == -1!";
    }else{
        QVariantMap dataMap;
        QPair<QString,QVariantMap>taskPair;
        dataMap.insert("Type",Type);
        dataMap.insert("Pack1SN",Pack1SN);
        dataMap.insert("Pack2SN",Pack2SN);
        dataMap.insert("StationID",StationID);
        dataMap.insert("DataTime",DataTime);
        if(Station.mid(0,2) == "VM"){
            taskPair.first = "/Module/mIOdata?";
        }else{
            taskPair.first = "/upload/IOdata?";
        }
        taskPair.second = dataMap;
        taskList<<taskPair;
        qDebug()<<"onUpInAndOutStation:"<<taskPair;
    }
}

void Interface::revM020fromMainwindow(QString agvSN)
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("AGVSN",agvSN);
    dataMap.insert("StationID",Station);
    taskPair.first = "/binding/HandBindPackSNtoAGVS";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"revM020fromMainwindow:"<<taskPair;
}

//普通使用的IP：statusServerIp  端口：statusServerPort	OP工位（IP：端口：）
//RESULT Interface::HTTP_Req2(QString url,QVariantMap dataMap)   //上传接口
//{
//    urlTemp = url;
//    dataMapTemp = dataMap;
//    bool isSuccess = false;
//    QVariantMap result;
//    WIFIlock.lockForRead();
//    if(1)//服务器
//        //        if(WIFI_STATE)//服务器
//    {
//        WIFIlock.unlock();

//        QJson::Serializer serializer;
//        bool ok;
//        QByteArray json = serializer.serialize(dataMap, &ok);
//        qDebug()<<"post send"<<json;

//        if(isEmergencyStop)
//        {
//            req.setUrl(QUrl(AGVServer+url));
//            isEmergencyStop = false;
//        }
//        else
//        {
//            if(Station.mid(0,2) == "OP")
//            {
//                QSettings *configIniRead_BMCE = new QSettings("/config.ini", QSettings::IniFormat);
//                QString DataServerIp_BMCE = configIniRead_BMCE->value("baseinfo/DataServerIp").toString();
//                QString Server_BMCE = "http://"+DataServerIp_BMCE+":"+"51564";
//                req.setUrl(QUrl(Server_BMCE+url));
//            }
//            else
//            {
//                req.setUrl(QUrl(statusServer+url));
//            }
//        }
//        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
//        req.setHeader(QNetworkRequest::ContentLengthHeader, json.length());

//        if (ok)
//        {
//            qDebug()<<"point[801]";
//            QNetworkReply *reply = manager->post(req,json);

//            reply->ignoreSslErrors();
//            QTime t_time;
//            t_time.start();

//            bool timeout = false;
//            qDebug()<<"point[802]";
//            while (!reply->isFinished()) {
//                QApplication::processEvents();
//                if (t_time.elapsed() >= TIMEOUT) {
//                    timeout = true;
//                    qDebug()<< "reply timeout fail.";
//                    break;
//                }
//            }
//            qDebug()<<"point[803]";

//            if (!timeout && reply->error() == QNetworkReply::NoError) {
//                QByteArray bytes = reply->readAll();
//                qDebug()<<"*post receive*"<<bytes;//打印小车所有信息


//                QJson::Parser parser;
//                bool ok;

//                result = parser.parse(bytes, &ok).toMap();
//                if (!ok) {
//                    qDebug()<<"HTTP_Req An error occurred during parsing"<<bytes;
//                }
//                else {
//                    if(result["Result"].toBool())
//                    {
//                        isSuccess = true;
//                    }
//                    else
//                    {
//                        qDebug()<<"this is result"<<result["Result"].toBool();
//                        isSuccess = true;
//                    }
//                }
//            }
//            else if(reply->error() != QNetworkReply::NoError)
//            {
//                qDebug()<<"HTTP_Req handle errors here";
//                QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
//                //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
//                qDebug( "HTTP_Req found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
//                qDebug()<<reply->errorString();
//            }

//            reply->deleteLater();
//        }
//        else
//        {
//            qDebug() << "HTTP_Req Something went wrong:" << serializer.errorMessage();
//        }
//        RESULT resultStruct;
//        resultStruct.isSuccess = isSuccess;
//        resultStruct.Result = result;

//        return resultStruct;
//    }
//    else
//    {
//        WIFIlock.unlock();
//        if(oneTimesRePost)
//        {
//            oneTimesRePost = false;
//            sleep(5);
//            RESULT resultStruct = HTTP_Req(urlTemp,dataMapTemp);
//            qDebug()<< "HTTP_Req wifi unconnect fail"<<resultStruct.isSuccess;
//            return resultStruct;
//        }
//        else
//        {
//            oneTimesRePost = true;
//            qDebug()<< "HTTP_Req wifi unconnect rePost";
//            RESULT resultStruct;
//            resultStruct.isSuccess = isSuccess;
//            resultStruct.Result = result;
//            return resultStruct;
//        }
//    }
//}

//上传员工工位操作信息(产量/节拍统计)
void Interface::UploadEmpOptionInfo(QString PackSN,QString PackType, QString InSationTime, QString OutSationTime, double StandardColck,int PackStatus)
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("PackSN",PackSN);
    dataMap.insert("Station",Station);
    dataMap.insert("PackType",PackType);
    dataMap.insert("InSationTime",InSationTime);
    dataMap.insert("OutSationTime",OutSationTime);
    QString vaKey = QString("baseinfo/StandardColck")+"_"+GPackType;
    StandardColck = QSettings("/config.ini", QSettings::IniFormat).value(vaKey).toDouble();//StandardColck不用传来的数据，直接在配置文件中提取标准节拍上传
    dataMap.insert("StandardColck",StandardColck);
    dataMap.insert("EmployeeNo",EKSBuff);
    dataMap.insert("PackStatus",PackStatus);
    taskPair.first = "/upload/UploadEmpOptionInfo";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"UploadEmpOptionInfo:"<<taskPair;
}

//上传员工EKS插拔信息
void Interface::uploadEKSInOutStatus(QString InEKSTime, QString OutEKSTime, QString EmployeeNo)
{
    QVariantMap dataMap;
    QPair<QString,QVariantMap>taskPair;
    dataMap.insert("Station",Station);
    dataMap.insert("InEKSTime",InEKSTime);
    dataMap.insert("OutEKSTime",OutEKSTime);
    dataMap.insert("EmployeeNo",EmployeeNo);
    taskPair.first = "/upload/UploadEmpEKSOptionInfo";
    taskPair.second = dataMap;
    taskList<<taskPair;
    qDebug()<<"uploadEKSInOutStatus:"<<taskPair;
}


void Interface::dyUpdateTask()
{
    int cycle = 500;
    QDateTime time1 = QDateTime::currentDateTime();
    bool isSuccess = false;
    QPair<QString,QVariantMap>taskPair;
//    if(taskList.size() > 0){
        qDebug()<<"taskList:"<<taskList.size()<<taskList;
//    }
//    readWriteLock_taskList.lockForRead();
    foreach(taskPair,taskList){//遍历任务列表
//        qDebug()<<"dyUpdateTask:"<<taskList.indexOf(taskPair)<<taskPair;
//        QVariantMap dataMap;
//        QPair<QString,QString>dataPair;
//        foreach(dataPair,taskPair.second){//遍历单条个任务中的数据
//            dataMap.insert(dataPair.first,dataPair.second);
//        }
        QString url = taskPair.first;
        RESULT resultStruct = HTTP_Req(url,taskPair.second);
        if(url == "/binding/MoveTrayandModuleData?"){
            emit sendTrayCleanResoult();
        }
        if(resultStruct.isSuccess)
        {
            QVariantMap result = resultStruct.Result;
            bool Result = result["Result"].toBool();
            if(!Result)//Result != 1
            {
                qDebug()<< "postResult=0:"<<url;
                if(url == "/binding/packandagv?"){
                    QString ErrorCode = result["Errorcode"].toString();
                    PACK_STATUS pack;
                    pack.Result = false;
                    pack.Errorcode = ErrorCode;
                    QVariant DataResult;
                    DataResult.setValue(pack);
                    emit sendPackStatus(DataResult);
                }else if(url == "/getstatus/packstatus?"){
                    QString ErrorCode = result["Errorcode"].toString();
                    PACK_STATUS pack;
                    pack.Result = false;
                    pack.Errorcode = ErrorCode;
                    QVariant DataResult;
                    DataResult.setValue(pack);
                    emit sendPackStatus(DataResult);
                }else if(url == "/getstatus/getTrayAndModuleStatus?"){
                    emit sendTrayAndModuleStatusResoult(0);
                }else if(url == "/binding/HandBindPackSNtoAGVS"){
                    emit sendm020ToMainWindow("0","","","");
                }
            }
            else if(Result)//Result == 1
            {
                isSuccess = true;
                qDebug()<< "postSuccess:"<<url;
                if(url == "/ubinding/packandagv?" && Station.mid(0,4) == "OP80"){//托盘解绑
                    emit sendTrayRelease();//OP80托盘解绑后处理“站完成请求PCL放行托盘”
                }else if(url == "/Module/mstationstate?" || url == "/upload/stationstate?"){//站完成
                    emit sendStationState(true);
                }else if(url == "/binding/packsnandmodule?"){
                    emit sendModulePack(true);//上传成功给PLC写个结果
                }else if(url == "/binding/packandagv?"){
                    PACK_STATUS pack;
                    pack.Result = true;
                    pack.PackType = result["Type"].toString();
                    pack.Pack1SN = result["Pack1SN"].toString();
                    pack.Pack2SN = result["Pack2SN"].toString();
                    pack.Pack1Status = "1";
                    pack.Pack2Status = "1";
                    QVariant DataResult;
                    DataResult.setValue(pack);
                    emit sendPackStatus(DataResult);
                }else if(url == "/getstatus/packstatus?" || url == "/binding/TrayandBmce?"){//请求产品型号
                    if(Station.mid(0,2) == "OP"){
                        BMCE_STATUS bmce;
                        bmce.AGVSN = result["AGVSN"].toString();
                        bmce.Pack1SN = result["Pack1SN"].toString();
                        bmce.Pack1Type = result["Pack1Type"].toString();
                        bmce.Result = result["Result"].toString();
                        bmce.Type = result["Type"].toString();
                        bmce.Pack1Status = result["Pack1Status"].toString();
                        if(bmce.Pack1SN == "-1")
                        {
                            qDebug()<<"BMCE packSN = -1";
                        }
                        else
                        {
                            QVariant DataResult;
                            DataResult.setValue(bmce);
                            emit sendBMCEFirstStatus(DataResult);
                            qDebug()<< "BMCE success";
                        }
                    }else{
                        PACK_STATUS pack;
                        pack.Result = true;
                        pack.AGVSN = result["AGVSN"].toString();
                        pack.PackType = result["Type"].toString();
                        pack.Pack1SN = result["Pack1SN"].toString();
                        pack.Pack2SN = result["Pack2SN"].toString();
                        pack.Pack1Status = result["Pack1Status"].toString();
                        pack.Pack2Status = result["Pack2Status"].toString();
                        //获取到的小车型号
                        if(pack.Pack1SN.isEmpty())
                        {
                            qDebug()<<"pack1sn isempty";
                            GPack1SN = "9";
                        }
                        else
                        {
                            GPack1SN = pack.Pack1SN.mid(10,1);
                        }
                        if(pack.Pack2SN.isEmpty())
                        {
                            qDebug()<<"pack2sn isempty";
                            GPack2SN = "9";
                        }
                        else
                        {
                            GPack2SN = pack.Pack2SN.mid(10,1);
                        }
                        QVariant DataResult;
                        DataResult.setValue(pack);
                        emit sendPackStatus(DataResult);
                    }
                }else if(url == "/Getstatus/getStationkeyCode?"){
                    int ReturnStatus = result["ReturnStatus"].toInt();
                    if(ReturnStatus == 1)
                    {
                        emit sendGetStationkeyCodeResult(true);
                        qDebug()<< "Get getStationkeyCode success:";
                    }
                    else
                    {
                        emit sendGetStationkeyCodeResult(false);
                        qDebug()<< "Get  getStationkeyCode status fail";
                    }
                }else if(url == "/Getstatus/IsMatchModuleKeyCode?"){
                    if(result["ReturnStatus"].toBool())
                    {
                        qDebug()<< "Get ReturnStatus success:";
                        emit sendGetStationkeyCodeResult(true);
                    }
                    else
                    {
                        qDebug()<< "Get ReturnStatus false:";
                        emit sendGetStationkeyCodeResult(false);
                    }
                }else if(url == "/query/getTightenScrewInfo"){
                    QString recDutouTightStatus = result["Data"].toString();
                    if(sendStation == "A140A")
                    {
                        emit senddutouTightStatus1(recDutouTightStatus);

                    }
                    else if(sendStation == "A150")
                    {
                        emit senddutouTightStatus2(recDutouTightStatus);
                    }
                }else if(url == "/getstatus/getTJOneStatus"){
                    QString PackStatus = result["PackStatus"].toString();
                    qDebug()<< "Get GluePackStatus success:"<<PackStatus;
                    emit sendGluePackStatus(PackStatus);
                }else if(url == "/getstatus/getStationTimer?"){
                    QVariantMap result = resultStruct.Result;
                    QDateTime time = result["DateTime"].toDateTime();
                    QDateTime serverTime = result["ServerTime"].toDateTime();
                    int timeChange = time.secsTo(serverTime);
                    emit sendGetStationTimer(timeChange);//send PLC
                    emit sendGetStationTimerResult(result["DateTime"].toString(),
                                        result["ServerTime"].toString());//send stepCtrl
                }else if(url == "/getstatus/getTrayPackType?"){
                    QString PackType = result["PackType"].toString();
                    emit sendReturngetTrayPackType(PackType);
                }else if(url == "/getstatus/getTrayAndModuleStatus?"){
                    int TrayStatus = result["TrayStatus"].toInt();
                    qDebug()<< "Get getTrayAndModuleStatus success:"<<TrayStatus;
                    emit sendTrayAndModuleStatusResoult(TrayStatus);
                }else if(url == "/binding/HandBindPackSNtoAGVS"){
                    QString pack1SN = result["Pack1SN"].toString();
                    QString pack2SN = result["Pack2SN"].toString();
                    QString packType = result["Type"].toString();
                    sendm020ToMainWindow("1",pack1SN,pack2SN,packType);
                }else if(url == "/upload/UploadEmpEKSOptionInfo"){
                    emit sendEKSUploadResult(true);
                }
            }
        }else{//请求失败无返回结果
            qDebug()<<"postFail:"<<url;
            if(url == "/getstatus/getTrayAndModuleStatus?"){
                emit sendTrayAndModuleStatusResoult(0);
            }
        }
    }
//    readWriteLock_taskList.unlock();

    QDateTime time2 = QDateTime::currentDateTime();
    int timeDiff = time1.msecsTo(time2);
    if(timeDiff > cycle){
        QTimer::singleShot(0,this,SLOT(dyUpdateTask()));
    }else{
        QTimer::singleShot(cycle-timeDiff,this,SLOT(dyUpdateTask()));
    }
}
