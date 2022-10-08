#include "interface.h"

Interface::Interface(QObject *parent) :
    QObject(parent)
{
    m_thread.start();
    this->moveToThread(&m_thread);
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
    oneTimesRePost = true;
    manager = new QNetworkAccessManager(this);
    Server = "http://"+DataServerIp+":"+ServerPort;
    AGVServer = "http://"+AGVDataServerIp+":"+AGVServerPort;
    statusServer = "http://"+statusDataServerIp+":"+statusServerPort;
    QTimer::singleShot(1000,this,SLOT(uplodeStationStatus()));
//    req_packstatus.setUrl(QUrl("http://"+DataServerIp+":"+ServerPort+"/getstatus/packstatus?"));
    //    req_ErrorMessage.setUrl(QUrl("http://"+DataServerIp+":"+ServerPort+"/ErrorMessage?"));
}

void Interface::uplodeStationStatus()
{
    QString DataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QVariantMap dataMap;
    dataMap.insert("OptionType","0");
    dataMap.insert("StationCode",Station);
    dataMap.insert("Status",GTightStatus);
    dataMap.insert("UpdateTime",DataTime);
    QString url = "/Upload/UploadStationStatus";
    if(StatusTemp != GTightStatus)
    {
        RESULT resultStruct = HTTP_Req2(url,dataMap);
        if(resultStruct.isSuccess)
        {
            QVariantMap result = resultStruct.Result;
            bool Result = result["Result"].toBool();
            if(!Result)
            {
                qDebug()<< "Get uplodeStationStatus fail";
            }
            else if(Result)
            {
                Result = true;
                StatusTemp = GTightStatus;
                QString PackStatus = result["PackStatus"].toString();
                qDebug()<< "Get uplodeStationStatus success:"<<PackStatus;
    //            emit sendGluePackStatus(PackStatus);
            }
        }
    }
    QTimer::singleShot(30000,this,SLOT(uplodeStationStatus()));
}

void Interface::reqTightStatus(QString station, QString tarStation, QString packSN)
{
    bool isSuccess = false;
    QString ErrorCode = "";
    QVariantMap dataMap;
    dataMap.insert("StationID",station);
    dataMap.insert("tarStationID",tarStation);
    dataMap.insert("packSN",packSN);
    QString url = "/getstatus/packstatus?";
    RESULT resultStruct = HTTP_Req(url,dataMap);
//    RESULT resultStruct;
//    resultStruct = returnResult.value<RESULT>();
//    qDebug()<<"&&&&&&&&&&&"<<resultStruct.isSuccess<<resultStruct.Result;
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            ErrorCode = result["Errorcode"].toString();
            qDebug()<< "Get PackStatus fail Result = 0"<<ErrorCode;
        }
        else if(Result)
        {
            isSuccess = true;
//            PACK_STATUS pack;
            QVariant dataTighten;
            dataTighten.setValue(result["huhuhu"].toString());
            qDebug()<<"this is dataTighten status"<<dataTighten;
//            pack.Result = true;
//            pack.PackType = result["Type"].toString();
//            pack.Pack1SN = result["Pack1SN"].toString();
//            pack.Pack2SN = result["Pack2SN"].toString();
//            pack.Pack1Status = result["Pack1Status"].toString();
//            pack.Pack2Status = result["Pack2Status"].toString();

//            QVariant DataResult;
//            DataResult.setValue(pack);
//            emit sendPackStatus(dataTighten);
//            qDebug()<< "Get PackStatus success:"<<pack.PackType<<pack.Pack1SN<<pack.Pack2SN<<pack.Pack1Status<<pack.Pack2Status;
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"Get PackStatus fail";
        PACK_STATUS pack;
        pack.Result = false;
        pack.Errorcode = ErrorCode;

        QVariant DataResult;
        DataResult.setValue(pack);
//        emit sendPackStatus(DataResult);
    }
}
void Interface::revgetTrayPackType(QString TrarySN)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("TrarySN",TrarySN);
    dataMap.insert("StationID",Station);
    QString url = "/getstatus/getTrayPackType?";
    RESULT resultStruct = HTTP_Req(url,dataMap);
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        QString PackType = result["PackType"].toString();
        if(!Result)
        {
            qDebug()<< "Get  getTrayPackType fail";
            QTimer::singleShot(1000,this,SLOT(revgetTrayPackType(TrarySN)));
        }
        else if(Result)
        {
            isSuccess = true;
            qDebug()<< "Get getTrayPackType success:"<<time;
            emit sendReturngetTrayPackType(PackType);
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"Get getTrayPackType fail";
        QTimer::singleShot(1000,this,SLOT(revgetTrayPackType(TrarySN)));
    }
}

void Interface::revAddTrayPackType(QString TrarySN)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("TrarySN",TrarySN);
    dataMap.insert("StationID",Station);
    dataMap.insert("PackType",GPack1SN);
    QString url = "/upload/AddTrayPackType?";
    RESULT resultStruct = HTTP_Req(url,dataMap);
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "Get  AddTrayPackType fail";
            QTimer::singleShot(1000,this,SLOT(revAddTrayPackType(TrarySN)));
        }
        else if(Result)
        {
            isSuccess = true;
            qDebug()<< "Get AddTrayPackType success:";
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"Get AddTrayPackType fail";
        QTimer::singleShot(1000,this,SLOT(revAddTrayPackType(TrarySN)));
    }
}

void Interface::revgetStationTimer(QString PackSN, int Type)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("PackSN",PackSN);
    dataMap.insert("StationID",Station);
    dataMap.insert("Type",Type);
    dataMap.insert("ReqStationID","M325A");
    QString url = "/getstatus/getStationTimer?";
    RESULT resultStruct = HTTP_Req(url,dataMap);
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        QDateTime time = result["DateTime"].toDateTime();
        if(!Result)
        {
            qDebug()<< "Get  getStationTimer fail";
            QTimer::singleShot(1000,this,SLOT(revgetStationTimer(PackSN,Type)));
        }
        else if(Result)
        {
            isSuccess = true;
            qDebug()<< "Get getStationTimer success:"<<time;
            emit sendGetStationTimer(time);
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"Get getStationTimer fail";
        QTimer::singleShot(1000,this,SLOT(revgetStationTimer(PackSN,Type)));
    }
}

void Interface::reqGlueWorkFlag(QString packsn)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("PackSN",packsn);
    dataMap.insert("StationID","A350-1");
    QString url = "/getstatus/getTJOneStatus";
    RESULT resultStruct = HTTP_Req(url,dataMap);
//    RESULT resultStruct;
//    resultStruct = returnResult.value<RESULT>();
//    qDebug()<<"&&&&&&&&&&&"<<resultStruct.isSuccess<<resultStruct.Result;
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "Get GluePackStatus status fail";
        }
        else if(Result)
        {
            isSuccess = true;
            Result = true;
            QString PackStatus = result["PackStatus"].toString();
            qDebug()<< "Get GluePackStatus success:"<<PackStatus;
            emit sendGluePackStatus(PackStatus);
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"Get GluePackStatus fail";
    }
}

void Interface::reqdutouTightStatus(QString sendAGVSN, QString sendStation)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("AGVSN",sendAGVSN);
    dataMap.insert("Station",sendStation);
    QString url = "/query/getTightenScrewInfo";
    RESULT resultStruct = HTTP_Req(url,dataMap);
//    RESULT resultStruct;
//    resultStruct = returnResult.value<RESULT>();
//    qDebug()<<"&&&&&&&&&&&"<<resultStruct.isSuccess<<resultStruct.Result;
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "Get dutou tight status fail";
        }
        else if(Result)
        {
            isSuccess = true;
            Result = true;
            QString recDutouTightStatus = result["Data"].toString();
//            QString channelResult = result["ChannelResult"].toString();
            if(sendStation == "A140A")
            {
                emit senddutouTightStatus1(recDutouTightStatus);

            }
            else if(sendStation == "A150")
            {
                emit senddutouTightStatus2(recDutouTightStatus);
            }
            qDebug()<< "Get dutou TightStatus success:"<<recDutouTightStatus;
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"Get PackStatus fail";
    }
}

void Interface::rereqPackStatus()
{
    bool isSuccess = false;
    QString ErrorCode = "";
    QVariantMap dataMap;
    dataMap.insert("StationID",Station);
    QString url = "/getstatus/packstatus?";
    RESULT resultStruct = HTTP_Req(url,dataMap);
//    RESULT resultStruct;
//    resultStruct = returnResult.value<RESULT>();
//    qDebug()<<"&&&&&&&&&&&"<<resultStruct.isSuccess<<resultStruct.Result;
    if(resultStruct.isSuccess || GIsTestStartFlag)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            ErrorCode = result["Errorcode"].toString();
            PACK_STATUS pack;
            pack.Result = false;
            pack.Errorcode = ErrorCode;
            QVariant DataResult;
            DataResult.setValue(pack);
//            rereqPackStatus();
            emit sendPackStatus(DataResult);
            qDebug()<< "reGet PackStatus fail Result = 0"<<ErrorCode;
        }
        else if(Result||GIsTestStartFlag)
        {
            isSuccess = true;
            PACK_STATUS pack;
            if(GIsTestStartFlag){
                pack.Result = true;
                pack.AGVSN = "18";
                pack.PackType = "1";
                pack.Pack1SN = "288 5UY849A001S";
                pack.Pack2SN = "288 5UY849A002T";
                pack.Pack1Status = "1";
                pack.Pack2Status = "1";
            }else{
                pack.Result = true;
                pack.AGVSN = result["AGVSN"].toString();
                pack.PackType = result["Type"].toString();
                pack.Pack1SN = result["Pack1SN"].toString();
                pack.Pack2SN = result["Pack2SN"].toString();
                pack.Pack1Status = result["Pack1Status"].toString();
                pack.Pack2Status = result["Pack2Status"].toString();
            }

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
           // emit sendStates("Pack1SN:"+pack.Pack1SN+ ", Pack2SN:"+pack.Pack2SN);
            /*emit sendPack1And2Type(result["Pack1SN"].toString().mid(10,1),
                    result["Pack2SN"].toString().mid(10,1));*/

            QVariant DataResult;
            DataResult.setValue(pack);
            emit sendPackStatus(DataResult);
            qDebug()<< "reGet PackStatus success:"<<pack.PackType<<pack.Pack1SN<<pack.Pack2SN<<pack.Pack1Status<<pack.Pack2Status;
        }
    }
//    if(!isSuccess)
    else
    {
        qDebug()<<"reGet PackStatus fail";
        PACK_STATUS pack;
        pack.Result = false;
        pack.Errorcode = ErrorCode;

        QVariant DataResult;
        DataResult.setValue(pack);
        rereqPackStatus();
        emit sendPackStatus(DataResult);
    }
}

void Interface::rerecIsMatchModuleKeyCode(QString PackType, QString PackSN)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("PackSN",PackSN);
    dataMap.insert("Type",PackType.toInt());
    QString url = "/Getstatus/IsMatchModuleKeyCode?";
    RESULT resultStruct = HTTP_Req(url,dataMap);
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            rerecIsMatchModuleKeyCode(PackType,PackSN);
            qDebug()<< "Get  IsMatchModuleKeyCode status fail";
        }
        else if(Result)
        {
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
            isSuccess = true;
//            emit sendGetStationkeyCodeResult(true);
//            qDebug()<< "Get IsMatchModuleKeyCode success:";
        }
    }
    if(!isSuccess)
    {
        rerecIsMatchModuleKeyCode(PackType,PackSN);
        qDebug()<<"Get IsMatchModuleKeyCode fail";
    }
}

void Interface::recIsMatchModuleKeyCode(QString PackType, QString PackSN)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("PackSN",PackSN);
    dataMap.insert("Type",PackType.toInt());
    QString url = "/Getstatus/IsMatchModuleKeyCode?";
    RESULT resultStruct = HTTP_Req(url,dataMap);
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            rerecIsMatchModuleKeyCode(PackType,PackSN);
            qDebug()<< "Get  IsMatchModuleKeyCode status fail";
        }
        else if(Result)
        {
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
            isSuccess = true;
//            emit sendGetStationkeyCodeResult(true);
//            qDebug()<< "Get IsMatchModuleKeyCode success:";
        }
    }
    if(!isSuccess)
    {
        rerecIsMatchModuleKeyCode(PackType,PackSN);
        qDebug()<<"Get IsMatchModuleKeyCode fail";
    }
}

void Interface::reRevgetStationkeyCode(QString PackType, QString PackSN)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("PackSN",PackSN);
    dataMap.insert("Station","A010");
    dataMap.insert("Type",PackType.toInt());
    QString url = "/Getstatus/getStationkeyCode?";
    RESULT resultStruct = HTTP_Req(url,dataMap);
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        int ReturnStatus = result["ReturnStatus"].toInt();
        if(!Result)
        {
//            emit sendGetStationkeyCodeResult(false);
//            qDebug()<< "Get  getStationkeyCode status fail";
            reRevgetStationkeyCode(PackType,PackSN);
        }
        else if(Result)
        {
            isSuccess = true;
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
        }
    }
    if(!isSuccess)
    {
        reRevgetStationkeyCode(PackType,PackSN);
        qDebug()<<"Get getStationkeyCode fail";
    }
}

//revgetStationkeyCode
void Interface::revgetStationkeyCode(QString PackType, QString PackSN)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("PackSN",PackSN);
    dataMap.insert("Station","A010");
    dataMap.insert("Type",PackType.toInt());
    QString url = "/Getstatus/getStationkeyCode?";
    RESULT resultStruct = HTTP_Req(url,dataMap);
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        int ReturnStatus = result["ReturnStatus"].toInt();
        if(!Result)
        {
//            emit sendGetStationkeyCodeResult(false);
//            qDebug()<< "Get  getStationkeyCode status fail";
            reRevgetStationkeyCode(PackType,PackSN);
        }
        else if(Result)
        {
            isSuccess = true;
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
        }
    }
    if(!isSuccess)
    {
        reRevgetStationkeyCode(PackType,PackSN);
        qDebug()<<"Get getStationkeyCode fail";
    }
}
void Interface::revAddStationTimer(QString PackSN, int Type)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("PackSN",PackSN);
    dataMap.insert("StationID",Station);
    dataMap.insert("Type",Type);
    QString url = "/upload/AddStationTimer?";
    RESULT resultStruct = HTTP_Req(url,dataMap);
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "Get  AddStationTimer fail";
        }
        else if(Result)
        {
            isSuccess = true;
            qDebug()<< "Get AddStationTimer success:";
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"Get AddStationTimer fail";
        QTimer::singleShot(1000,this,SLOT(revAddStationTimer(PackSN,Type)));
    }
}

//获取PACK状态
void Interface::reqPackStatus()
{qDebug()<<"point[11]";
    bool isSuccess = false;
    QString ErrorCode = "";
    QVariantMap dataMap;
    dataMap.insert("StationID",Station);
    QString url = "/getstatus/packstatus?";
    RESULT resultStruct = HTTP_Req(url,dataMap);
//    RESULT resultStruct;
//    resultStruct = returnResult.value<RESULT>();
//    qDebug()<<"&&&&&&&&&&&"<<resultStruct.isSuccess<<resultStruct.Result;
    if(resultStruct.isSuccess || GIsTestStartFlag)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            ErrorCode = result["Errorcode"].toString();
            PACK_STATUS pack;
            pack.Result = false;
            pack.Errorcode = ErrorCode;
            QVariant DataResult;
            DataResult.setValue(pack);
//            rereqPackStatus();
            emit sendPackStatus(DataResult);
            qDebug()<< "Get PackStatus fail Result = 0"<<ErrorCode;
        }
        else if(Result||GIsTestStartFlag)
        {
            isSuccess = true;
            PACK_STATUS pack;
            if(GIsTestStartFlag){
                pack.Result = true;
                pack.AGVSN = "18";
                pack.PackType = "1";
                pack.Pack1SN = "288 5UY849A001S";
                pack.Pack2SN = "288 5UY849A002T";
                pack.Pack1Status = "1";
                pack.Pack2Status = "1";
            }else{
                pack.Result = true;
                pack.AGVSN = result["AGVSN"].toString();
                pack.PackType = result["Type"].toString();
                pack.Pack1SN = result["Pack1SN"].toString();
                pack.Pack2SN = result["Pack2SN"].toString();
                pack.Pack1Status = result["Pack1Status"].toString();
                pack.Pack2Status = result["Pack2Status"].toString();
            }

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
           // emit sendStates("Pack1SN:"+pack.Pack1SN+ ", Pack2SN:"+pack.Pack2SN);
            /*emit sendPack1And2Type(result["Pack1SN"].toString().mid(10,1),
                    result["Pack2SN"].toString().mid(10,1));*/

            QVariant DataResult;
            DataResult.setValue(pack);
            emit sendPackStatus(DataResult);
            qDebug()<< "Get PackStatus success:"<<pack.PackType<<pack.Pack1SN<<pack.Pack2SN<<pack.Pack1Status<<pack.Pack2Status;
        }
    }
//    if(!isSuccess)
    else
    {
        qDebug()<<"Get PackStatus fail";
        PACK_STATUS pack;
        pack.Result = false;
        pack.Errorcode = ErrorCode;

        QVariant DataResult;
        DataResult.setValue(pack);
        rereqPackStatus();
//        emit sendPackStatus(DataResult);
    }
}

void Interface::reboundPack()
{
    bool isSuccess = false;
    QString ErrorCode = "";
    QVariantMap dataMap;
    dataMap.insert("StationID",Station);
    QString url = "/binding/packandagv?";
    RESULT resultStruct = HTTP_Req(url,dataMap);
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            ErrorCode = result["Errorcode"].toString();
            PACK_STATUS pack;
            pack.Result = false;
            pack.Errorcode = ErrorCode;
            QVariant DataResult;
            DataResult.setValue(pack);
            emit sendPackStatus(DataResult);
            qDebug()<< "reboundPack fail Result = 0"<<ErrorCode;
        }
        else if(Result)
        {
            isSuccess = true;
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
            qDebug()<< "reboundPack success:"<<pack.PackType<<pack.Pack1SN<<pack.Pack2SN;
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"reboundPack fail";
        PACK_STATUS pack;
        pack.Result = false;
        pack.Errorcode = ErrorCode;

        QVariant DataResult;
        DataResult.setValue(pack);
        reboundPack();
        emit sendPackStatus(DataResult);
    }
}

//A010 绑定流水号 并获取PACK状态
void Interface::boundPack()
{
    bool isSuccess = false;
    QString ErrorCode = "";
    QVariantMap dataMap;
    dataMap.insert("StationID",Station);
    QString url = "/binding/packandagv?";
    RESULT resultStruct = HTTP_Req(url,dataMap);
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            ErrorCode = result["Errorcode"].toString();
            PACK_STATUS pack;
            pack.Result = false;
            pack.Errorcode = ErrorCode;
            QVariant DataResult;
            DataResult.setValue(pack);
            emit sendPackStatus(DataResult);
            qDebug()<< "boundPack fail Result = 0"<<ErrorCode;
        }
        else if(Result)
        {
            isSuccess = true;
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
            qDebug()<< "boundPack success:"<<pack.PackType<<pack.Pack1SN<<pack.Pack2SN;
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"boundPack fail";
        PACK_STATUS pack;
        pack.Result = false;
        pack.Errorcode = ErrorCode;
        QVariant DataResult;
        DataResult.setValue(pack);
        reboundPack();
//        emit sendPackStatus(DataResult);
    }
}


//VM010 获取模组型号
void Interface::reqModule()
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("StationID",Station);
    dataMap.insert("LineID",Line_ID);
    QString url = "/getstatus/moduletype?";
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "reqModule fail Result = 0";
        }
        else if(Result)
        {
            isSuccess = true;
            QString module = result["ModuleType"].toString();

            emit sendModule(true, module);
            qDebug()<< "reqModule success:"<<module;
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"reqModule fail";
        emit sendModule(false, "");
    }
}
void Interface::remoduleTray4(QString TraySN,QString module1 ,QString module2, QString module3, QString module4)
{
    bool isSuccess = false;
    QVariantMap dataMap;
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

    QString url = "/binding/trayandmodule?";
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "moduleTray fail Result = 0";
        }
        else if(Result)
        {
            isSuccess = true;
            emit sendModuleTray(true);
            qDebug()<< "moduleTray success";
        }
    }
//    if(!isSuccess)
    else
    {
        qDebug()<<"moduleTray fail";
        emit sendModuleTray(false);
        remoduleTray4(TraySN,module1 ,module2, module3, module4);
    }
}

//VM040 托盘码和模组码绑定
void Interface::moduleTray(QString TraySN,QString module1 ,QString module2, QString module3, QString module4)
{
    bool isSuccess = false;
    QVariantMap dataMap;
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

    QString url = "/binding/trayandmodule?";
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "moduleTray fail Result = 0";
        }
        else if(Result)
        {
            isSuccess = true;
            emit sendModuleTray(true);
            qDebug()<< "moduleTray success";
        }
    }
//    if(!isSuccess)
    else
    {
        qDebug()<<"moduleTray fail";
        emit sendModuleTray(false);
        remoduleTray4(TraySN,module1 ,module2, module3, module4);
    }
}

void Interface::remoduleTray(QString TraySN,QString module1 ,QString module2, QString module3, QString module4,QString module5 ,QString module6, QString module7, QString module8)
{
    qDebug()<<"this is second VM010 bind";
    bool isSuccess = false;
    QVariantMap dataMap;
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

    QString url = "/binding/trayandmodule?";
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "re8moduleTray fail Result = 0";
        }
        else if(Result)
        {
            isSuccess = true;
            emit sendModuleTray(true);
            qDebug()<< "re8moduleTray success";
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"re8moduleTray fail";
        remoduleTray(TraySN,module1 ,module2,module3,module4,module5 ,module6,module7,module8);
        emit sendModuleTray(false);
    }
}

//VM040 托盘码和模组码绑定16
void Interface::moduleTray(QString TraySN,QString module1 ,QString module2, QString module3, QString module4,QString module5 ,QString module6, QString module7, QString module8,QString module9 ,QString module10, QString module11, QString module12,QString module13 ,QString module14, QString module15, QString module16)
{
    qDebug()<<"this is rec VM040 bind16";
    bool isSuccess = false;
    QVariantMap dataMap;
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

    QString url = "/binding/trayandmodule?";
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "16moduleTray fail Result = 0";
        }
        else if(Result)
        {
            isSuccess = true;
            emit sendModuleTray(true);
            qDebug()<< "16moduleTray success";
        }
    }
    if(!isSuccess)
    {
        remoduleTray16(TraySN,module1 ,module2,module3,module4,module5 ,module6,module7,module8,module9 ,module10,module11,module12,module13 ,module14,module15,module16);
        qDebug()<<"16moduleTray fail";
        emit sendModuleTray(false);
    }
}


void Interface::remoduleTray16(QString TraySN,QString module1 ,QString module2, QString module3, QString module4,QString module5 ,QString module6, QString module7, QString module8,QString module9 ,QString module10, QString module11, QString module12,QString module13 ,QString module14, QString module15, QString module16)
{
    qDebug()<<"this is rerec VM040 bind16";
    bool isSuccess = false;
    QVariantMap dataMap;
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

    QString url = "/binding/trayandmodule?";
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "16moduleTray fail Result = 0";
        }
        else if(Result)
        {
            isSuccess = true;
            emit sendModuleTray(true);
            qDebug()<< "16moduleTray success";
        }
    }
    if(!isSuccess)
    {
        remoduleTray16(TraySN,module1 ,module2,module3,module4,module5 ,module6,module7,module8,module9 ,module10,module11,module12,module13 ,module14,module15,module16);
        qDebug()<<"16moduleTray fail";
        emit sendModuleTray(false);
    }
}

//VM040 托盘码和模组码绑定
void Interface::moduleTray(QString TraySN,QString module1 ,QString module2, QString module3, QString module4,QString module5 ,QString module6, QString module7, QString module8)
{
    qDebug()<<"this is rec VM010 bind";
    bool isSuccess = false;
    QVariantMap dataMap;
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

    QString url = "/binding/trayandmodule?";
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "8moduleTray fail Result = 0";
        }
        else if(Result)
        {
            isSuccess = true;
            emit sendModuleTray(true);
            qDebug()<< "8moduleTray success";
        }
    }
    if(!isSuccess)
    {
        remoduleTray(TraySN,module1 ,module2,module3,module4,module5 ,module6,module7,module8);
        qDebug()<<"8moduleTray fail";
        emit sendModuleTray(false);
    }
}
//BMCE首工位绑定判断状态
void Interface::reqBMCEStatus(QString TraySN)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("StationID",Station);
    dataMap.insert("TraySN",TraySN);
    QString url = "/binding/TrayandBmce?";
    if(Station == "OP10-1" || Station == "OP10-2")
    {
        url = "/binding/TrayandBmce?";
    }
    else
    {
        url = "/getstatus/packstatus?";
    }
    RESULT resultStruct = HTTP_Req(url,dataMap);
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        QString Result = result["Result"].toString();
        if(Result == "0")
        {
            qDebug()<< "BMCE fail Result = 0";
        }
        else
        {
            BMCE_STATUS bmce;
            bmce.AGVSN = result["AGVSN"].toString();
            bmce.Pack1SN = result["Pack1SN"].toString();
            bmce.Pack1Type = result["Pack1Type"].toString();
            bmce.Result = result["Result"].toString();
            bmce.Type = result["Type"].toString();
            bmce.Pack1Status = result["Pack1Status"].toString();
            isSuccess = true;
            if(bmce.Pack1SN == "-1")
            {
                qDebug()<<"BMCE packSN = -1";
                QTimer::singleShot(10000,this,SLOT(reqBMCEStatus(TraySN)));
            }
            else
            {
                QVariant DataResult;
                DataResult.setValue(bmce);
                emit sendBMCEFirstStatus(DataResult);
                qDebug()<< "BMCE success";
            }
        }

    }
    if(!isSuccess)
    {
        qDebug()<<"BMCE fail";
        QTimer::singleShot(1000,this,SLOT(reqBMCEStatus(TraySN)));
    }
}

void Interface::remodulePack(QString TraySN, QString packSN)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("StationID",Station);
    dataMap.insert("LineID",Line_ID);
    dataMap.insert("TraySN",TraySN);
    dataMap.insert("PackSN",packSN);

    QString url = "/binding/packsnandmodule?";
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "remodulePack fail Result = 0";
            emit sendModulePack(false);
        }
        else if(Result)
        {
            isSuccess = true;
            emit sendModulePack(true);
            qDebug()<< "remodulePack success";
        }
    }
//    if(!isSuccess)
    else
    {
        qDebug()<<"remodulePack fail";
        emit sendModulePack(false);
    }
}

//A090 托盘码上传 pack与模组绑定
void Interface::modulePack(QString TraySN,QString packSN)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("StationID",Station);
    dataMap.insert("LineID",Line_ID);
    dataMap.insert("TraySN",TraySN);
    dataMap.insert("PackSN",packSN);

    QString url = "/binding/packsnandmodule?";
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "modulePack fail Result = 0";
            remodulePack(TraySN,packSN);
        }
        else if(Result)
        {
            isSuccess = true;
            emit sendModulePack(true);
            qDebug()<< "modulePack success";
        }
    }
//    if(!isSuccess)
    else
    {
        qDebug()<<"modulePack fail";
        remodulePack(TraySN,packSN);
//        emit sendModulePack(false);
    }
}


//A010 下箱体条码上传 AGV车号与下箱体绑定 关键条码
void Interface::Keybarcode(QString keyBarcode,QString PackSN ,QString time, int OrderSN)
{
    qDebug()<<"Keybarcode";
    bool isSuccess = false;
    QVariantMap dataMap;
    QString scanRegExpCode = GscanRegExpCode;
    dataMap.insert("StationID",Station);
    dataMap.insert("LineID",Line_ID);
    dataMap.insert("KeyCode",keyBarcode);
    dataMap.insert("PackSN",PackSN);
    dataMap.insert("DataTime",time);
    dataMap.insert("OrderSN",OrderSN);
    dataMap.insert("PartsCode",GscanRegExpCode);

   // QString url = "/upload/Keybarcode?";    //true

    QString url = "";
    if(Station.mid(0,2) == "VM"){
        url =  "/Module/mKeybarcode";
    }else{
        url =  "/upload/Keybarcode?";
    }
    //url = "/test?";
    emit sendKeyCodeInfo(Line_ID,OrderSN,scanRegExpCode,keyBarcode,PackSN,Station);//没成功上传到接口，发到本地数据库存储
//    RESULT resultStruct = HTTP_Req(url,dataMap);
//    if(resultStruct.isSuccess)
//    {
//        QVariantMap result = resultStruct.Result;
//        bool Result = result["Result"].toBool();
//        if(!Result)
//        {
////            emit sendKeyCodeInfo(Line_ID,OrderSN,scanRegExpCode,keyBarcode,PackSN,Station);//没成功上传到接口，发到本地数据库存储
//            GIsInterfaceUpSuccess = false;
//            qDebug()<< "Keybarcode fail Result = 0";
//        }
//        else if(Result)
//        {
//            isSuccess = true;
//            GIsInterfaceUpSuccess = true;
//            emit sendKeybarcode(true);
//            qDebug()<< "Keybarcode success";
//        }
//    }
//    if(!isSuccess)
//    {
//        //没成功上传到接口，发到本地数据库存储
////        emit sendKeyCodeInfo(Line_ID,OrderSN,scanRegExpCode,keyBarcode,PackSN,Station);
//        GIsInterfaceUpSuccess = false;
//        qDebug()<<"Keybarcode fail";
//        emit sendKeybarcode(false);
//    }
}


//站完成状态上传
void Interface::stationState(QString Pack1SN,QString Pack1Status,QString Pack2SN,QString Pack2Status, QString time)
{
    qDebug()<<"this is send stationstate Pack1SN"<<Pack1SN<<"Pack1Status"<<Pack1Status<<"Pack2SN"<<Pack2SN<<"Pack2Status"<<Pack2Status;
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("StationID",Station);

    dataMap.insert("Pack1SN",Pack1SN);
    dataMap.insert("Pack1Status",Pack1Status);
    dataMap.insert("Pack2SN",Pack2SN);
    dataMap.insert("Pack2Status",Pack2Status);
    dataMap.insert("Time",time);

    QString url = "";
    if(Station.mid(0,2) == "VM"){
        url = "/Module/mstationstate?";
    }else{
        url = "/upload/stationstate?";
    }
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "stationState fail Result = 0";
            resendPackstatus(Pack1SN,Pack1Status,Pack2SN,Pack2Status,time);
        }
        else if(Result)
        {
            isSuccess = true;
            emit sendStationState(true);
            qDebug()<< "stationState success";
        }
    }
//    if(!isSuccess)
    else
    {
        qDebug()<<"stationState fail";
        emit sendStationState(false);
        sleep(3);
        resendPackstatus(Pack1SN,Pack1Status,Pack2SN,Pack2Status,time);
    }
}
void Interface::resendPackstatus(QString Pack1SN,QString Pack1Status,QString Pack2SN,QString Pack2Status, QString time)
{
    qDebug()<<"this is resend stationstate Pack1SN"<<Pack1SN<<"Pack1Status"<<Pack1Status<<"Pack2SN"<<Pack2SN<<"Pack2Status"<<Pack2Status;
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("StationID",Station);

    dataMap.insert("Pack1SN",Pack1SN);
    dataMap.insert("Pack1Status",Pack1Status);
    dataMap.insert("Pack2SN",Pack2SN);
    dataMap.insert("Pack2Status",Pack2Status);
    dataMap.insert("Time",time);

    QString url = "";
    if(Station.mid(0,2) == "VM"){
        url = "/Module/mstationstate?";
    }else{
        url = "/upload/stationstate?";
    }
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "restationState fail Result = 0";
//            resendPackstatus(Pack1SN,Pack1Status,Pack2SN,Pack2Status,time);
        }
        else if(Result)
        {
            isSuccess = true;
            emit sendStationState(true);
            qDebug()<< "restationState success";
        }
    }
//    if(!isSuccess)
    else
    {
        qDebug()<<"restationState fail";
        emit sendStationState(false);
        sleep(3);
        resendPackstatus(Pack1SN,Pack1Status,Pack2SN,Pack2Status,time);
    }
}

void Interface::reqRepairBatterypack(QString packSN)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("PackSN",packSN);
    QString url = "/query/getRepairBatterypack?";
    RESULT resultStruct = HTTP_Req(url,dataMap);
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        QString Result = result["Result"].toString();
        if(Result == "0")
        {
            qDebug()<< "BMCE handle Result = 0";
        }
        else
        {
            qDebug()<< "BMCE handle success";
            datas = result["Datas"].toList();
            qDebug()<<"this is datas"<<datas;
        }

    }
    if(!isSuccess)
    {
        qDebug()<<"BMCE handle fail";
    }
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
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("StationID",Station);
    dataMap.insert("LineID",Line_ID);

    QString url = "/count/modulecount?";
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "moduleCount fail Result = 0";
        }
        else if(Result)
        {
            isSuccess = true;
            emit sendModuleCount(true);
            qDebug()<< "moduleCount success";
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"moduleCount fail";
        emit sendModuleCount(false);
    }
}

void Interface::reqEmergencyStop(bool isMove)
{
    isEmergencyStop = true;
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("StationID",Station);
    dataMap.insert("Status",isMove);

    QString url = "/AGVS/EmergencyStop?";
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        int ResultCode = result["ResultCode"].toInt();
        if(ResultCode != 0)
        {
            qDebug()<< "send req EmergencyStop fail ResultCode:"<<ResultCode;
        }
        else
        {
            isSuccess = true;
            emit sendEmergencyStopCode(true);
            qDebug()<< "send req EmergencyStop success";
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"send req EmergencyStop fail";
        emit sendEmergencyStopCode(false);
    }
}

RESULT Interface::HTTP_Req(QString url,QVariantMap dataMap)   //上传接口
{
    urlTemp = url;
    dataMapTemp = dataMap;
    bool isSuccess = false;
    QVariantMap result;
    WIFIlock.lockForRead();
    if(1)//服务器
//        if(WIFI_STATE)//服务器
    {
        WIFIlock.unlock();

        QJson::Serializer serializer;
        bool ok;
        QByteArray json = serializer.serialize(dataMap, &ok);
        qDebug()<<"post send"<<json;

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
                 QString Server_BMCE = "http://"+DataServerIp_BMCE+":"+"51565";
                req.setUrl(QUrl(Server_BMCE+url));
            }
            else
            {
                req.setUrl(QUrl(Server+url));
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
                qDebug()<<"*post receive*"<<bytes;//打印小车所有信息


                QJson::Parser parser;
                bool ok;

                result = parser.parse(bytes, &ok).toMap();
//                emit sendStates("Pack1SN:"+result["Pack1SN"].toString()+
//                        ", Pack2SN:"+result["Pack2SN"].toString());
//                emit sendPack1And2Type(result["Pack1SN"].toString().mid(10,1),
//                        result["Pack2SN"].toString().mid(10,1));//获取到的小车型号
                if (!ok) {
                    qDebug()<<"HTTP_Req An error occurred during parsing"<<bytes;
                }
                else {
                        if(result["Result"].toBool())
                        {
                            isSuccess = true;
                        }
                        else
                        {
                            qDebug()<<"this is result"<<result["Result"].toBool();
                            isSuccess = true;
                            /*WIFIlock.unlock();
                            if(oneTimesRePost)
                            {
                                oneTimesRePost = false;
                                sleep(5);
                                RESULT resultStruct = HTTP_Req(urlTemp,dataMapTemp);
                                qDebug()<< "HTTP_Req wifi unconnect fail"<<resultStruct.isSuccess;
                                return resultStruct;
                            }
                            else
                            {
                                oneTimesRePost = true;
                                qDebug()<< "HTTP_Req wifi unconnect rePost";
                                RESULT resultStruct;
                                resultStruct.isSuccess = isSuccess;
                                resultStruct.Result = result;
                                return resultStruct;
                            }*/
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
    else
    {
        WIFIlock.unlock();
        if(oneTimesRePost)
        {
            oneTimesRePost = false;
            sleep(5);
            RESULT resultStruct = HTTP_Req(urlTemp,dataMapTemp);
            qDebug()<< "HTTP_Req wifi unconnect fail"<<resultStruct.isSuccess;
            return resultStruct;
        }
        else
        {
            oneTimesRePost = true;
            qDebug()<< "HTTP_Req wifi unconnect rePost";
            RESULT resultStruct;
            resultStruct.isSuccess = isSuccess;
            resultStruct.Result = result;
            return resultStruct;
        }
//        emit sendreSendReqMoudlePack();
    }    
}
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
        //if(isEmergencyStop)
        //{
            req.setUrl(QUrl(AGVServer+url));
        //    isEmergencyStop = false;
        //}
        //else
        //    req.setUrl(QUrl(Server+url));
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

void Interface::BMCETrayClean(QString TarySN, QString Pack1SN)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("AGVSN",TarySN);
    dataMap.insert("PackSN",Pack1SN);
    QString url = "/ubinding/packandagv?";
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "BMCEClean fail Result = 0";
        }
        else if(Result)
        {
            isSuccess = true;
            qDebug()<< "BMCEClean success";
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"BMCEClean fail";
    }
}

void Interface::modulePackClean(QString TraySN) //A010-1/-2返回托盘数据
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("TraySN",TraySN);
    QString url = "/binding/MoveTrayandModuleData?";
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "modulePackClean fail Result = 0";
        }
        else if(Result)
        {
            isSuccess = true;
            qDebug()<< "modulePackClean success";
        }
        emit sendTrayCleanResoult();
    }
    if(!isSuccess)
    {
        qDebug()<<"modulePackClean fail";
        emit sendTrayCleanResoult();
    }
}


void Interface::onUpInAndOutStation(int Type, QString Pack1SN, QString Pack2SN, QString StationID, QString DataTime)
{

    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("Type",Type);
    dataMap.insert("Pack1SN",Pack1SN);
    dataMap.insert("Pack2SN",Pack2SN);
    dataMap.insert("StationID",StationID);
    dataMap.insert("DataTime",DataTime);
    QString url = "";
    if(Station.mid(0,2) == "VM"){
        url = "/Module/mIOdata?";
    }else{
        url = "/upload/IOdata?";
    }
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "UpInAndOutStation fail Result = 0";
            QTimer::singleShot(1000,this,SLOT(onUpInAndOutStation(Type,Pack1SN,Pack2SN,StationID,DataTime)));
        }
        else if(Result)
        {
            isSuccess = true;
            qDebug()<< "UpInAndOutStation success";
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"UpInAndOutStation fail";
        QTimer::singleShot(1000,this,SLOT(onUpInAndOutStation(Type,Pack1SN,Pack2SN,StationID,DataTime)));
    }
}

void Interface::revM020fromMainwindow(QString agvSN)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("AGVSN",agvSN);
    dataMap.insert("StationID",Station);
    QString url = "/binding/HandBindPackSNtoAGVS";
    RESULT resultStruct = HTTP_Req(url,dataMap);
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            qDebug()<< "HandBindPackSNtoAGVS fail Result = 0";
            sendm020ToMainWindow("0","","","");
        }
        else if(Result)
        {
            isSuccess = true;
            QString pack1SN = result["Pack1SN"].toString();
            QString pack2SN = result["Pack2SN"].toString();
            QString packType = result["Type"].toString();
            qDebug()<< "HandBindPackSNtoAGVS success";
            sendm020ToMainWindow("1",pack1SN,pack2SN,packType);
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"HandBindPackSNtoAGVS fail";
        QTimer::singleShot(1000,this,SLOT(revM020fromMainwindow(agvSN)));
    }
}

RESULT Interface::HTTP_Req2(QString url,QVariantMap dataMap)   //上传接口
{
    urlTemp = url;
    dataMapTemp = dataMap;
    bool isSuccess = false;
    QVariantMap result;
    WIFIlock.lockForRead();
    if(1)//服务器
//        if(WIFI_STATE)//服务器
    {
        WIFIlock.unlock();

        QJson::Serializer serializer;
        bool ok;
        QByteArray json = serializer.serialize(dataMap, &ok);
        qDebug()<<"post send"<<json;

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
                 QString Server_BMCE = "http://"+DataServerIp_BMCE+":"+"51565";
                req.setUrl(QUrl(Server_BMCE+url));
            }
            else
            {
                req.setUrl(QUrl(statusServer+url));
            }
        }
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
                qDebug()<<"*post receive*"<<bytes;//打印小车所有信息


                QJson::Parser parser;
                bool ok;

                result = parser.parse(bytes, &ok).toMap();
                if (!ok) {
                    qDebug()<<"HTTP_Req An error occurred during parsing"<<bytes;
                }
                else {
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
    else
    {
        WIFIlock.unlock();
        if(oneTimesRePost)
        {
            oneTimesRePost = false;
            sleep(5);
            RESULT resultStruct = HTTP_Req(urlTemp,dataMapTemp);
            qDebug()<< "HTTP_Req wifi unconnect fail"<<resultStruct.isSuccess;
            return resultStruct;
        }
        else
        {
            oneTimesRePost = true;
            qDebug()<< "HTTP_Req wifi unconnect rePost";
            RESULT resultStruct;
            resultStruct.isSuccess = isSuccess;
            resultStruct.Result = result;
            return resultStruct;
        }
    }
}
