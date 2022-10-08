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
    delete configIniRead;
    packTypeTemp = 0;
    oneTimesRePost = true;
    manager = new QNetworkAccessManager(this);
    Server = "http://"+DataServerIp+":"+ServerPort;
    AGVServer = "http://"+AGVDataServerIp+":"+AGVServerPort;
//    req_packstatus.setUrl(QUrl("http://"+DataServerIp+":"+ServerPort+"/getstatus/packstatus?"));
    //    req_ErrorMessage.setUrl(QUrl("http://"+DataServerIp+":"+ServerPort+"/ErrorMessage?"));
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

//获取PACK状态
void Interface::reqPackStatus()
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
            GPack1SN = pack.Pack1SN.mid(10,1);
            GPack2SN = pack.Pack2SN.mid(10,1);
           // emit sendStates("Pack1SN:"+pack.Pack1SN+ ", Pack2SN:"+pack.Pack2SN);
            /*emit sendPack1And2Type(result["Pack1SN"].toString().mid(10,1),
                    result["Pack2SN"].toString().mid(10,1));*/

            QVariant DataResult;
            DataResult.setValue(pack);
            emit sendPackStatus(DataResult);
            qDebug()<< "Get PackStatus success:"<<pack.PackType<<pack.Pack1SN<<pack.Pack2SN<<pack.Pack1Status<<pack.Pack2Status;
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
        emit sendPackStatus(DataResult);
    }
}

//VM040 托盘码和模组码绑定
void Interface::moduleTray(QString TraySN,QString module1 ,QString module2, QString module3, QString module4)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("StationID",Station);
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
    if(!isSuccess)
    {
        qDebug()<<"moduleTray fail";
        emit sendModuleTray(false);
    }
}

//VM040 托盘码和模组码绑定
void Interface::moduleTray(QString TraySN,QString module1 ,QString module2, QString module3, QString module4,QString module5 ,QString module6, QString module7, QString module8)
{
    bool isSuccess = false;
    QVariantMap dataMap;
    dataMap.insert("StationID",Station);
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
        qDebug()<<"8moduleTray fail";
        emit sendModuleTray(false);
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
        }
        else if(Result)
        {
            isSuccess = true;
            qDebug()<< "modulePack success";
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"modulePack fail";
    }
}


//A010 下箱体条码上传 AGV车号与下箱体绑定 关键条码
void Interface::Keybarcode(QString keyBarcode,QString PackSN ,QString time, int OrderSN)
{
    bool isSuccess = false;
    QVariantMap dataMap;
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
    RESULT resultStruct = HTTP_Req(url,dataMap);
    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            GIsInterfaceUpSuccess = false;
            qDebug()<< "Keybarcode fail Result = 0";
        }
        else if(Result)
        {
            isSuccess = true;
            GIsInterfaceUpSuccess = true;
            qDebug()<< "Keybarcode success";
        }
    }
    if(!isSuccess)
    {
        //没成功上传到接口，发到本地数据库存储
        GIsInterfaceUpSuccess = false;
        qDebug()<<"Keybarcode fail";
    }
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
        }
        else if(Result)
        {
            isSuccess = true;
            emit sendStationState(true);
            qDebug()<< "stationState success";
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"stationState fail";
        emit sendStationState(false);
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
        qDebug()<<"Req json222"<<json;

        if(isEmergencyStop)
        {
            req.setUrl(QUrl(AGVServer+url));
            isEmergencyStop = false;
        }
        else{
            if(url == "/Upload/UploadStationStatus?"){
                QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
                QString statusServerIp = configIniRead->value("baseinfo/statusServerIp").toString();
                QString statusServerPort = configIniRead->value("baseinfo/statusServerPort").toString();
                QString statusServer = "http://"+statusServerIp+":"+statusServerPort;
                req.setUrl(QUrl(statusServer+url));
            }else{
                req.setUrl(QUrl(Server+url));
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
                qDebug()<<"************************************ Get packstatus"<<bytes;//打印小车所有信息


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
                qDebug()<<"************************************ Get packstatus"<<bytes;//打印小车所有信息

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


void Interface::reStationStatus()
{
    bool isSuccess = false;
    QString ErrorCode = "";
    QString thisTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QVariantMap dataMap;
    dataMap.insert("OptionType",0);
    dataMap.insert("StationCode",Station);
    dataMap.insert("Type",1);
    dataMap.insert("Status",0);
    dataMap.insert("UpdateTime",thisTime);

    QString url = "/Upload/UploadStationStatus?";
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            ErrorCode = result["Errorcode"].toString();
            qDebug()<< "UploadStationStatus fail Result = 0"<<ErrorCode;
        }
        else if(Result)
        {
            isSuccess = true;
            qDebug()<< "UploadStationStatus success:";
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"UploadStationStatus fail";
    }
}


void Interface::getStationTimer(QString PackSN, QString Type, QString StationID, QString ReqStationID)
{
    bool isSuccess = false;
    QString ErrorCode = "";
    QVariantMap dataMap;
    dataMap.insert("PackSN",PackSN);
    dataMap.insert("Type",Type);
    dataMap.insert("StationID",StationID);
    dataMap.insert("ReqStationID",ReqStationID);

    QString url = "/getstatus/getStationTimer?";
    RESULT resultStruct = HTTP_Req(url,dataMap);

    if(resultStruct.isSuccess)
    {
        QVariantMap result = resultStruct.Result;
        bool Result = result["Result"].toBool();
        if(!Result)
        {
            ErrorCode = result["Errorcode"].toString();
            qDebug()<< "getStationTimer fail Result = 0"<<ErrorCode;
        }
        else if(Result)
        {
            isSuccess = true;
            emit sendGetStationTimerResult(result["DateTime"].toString(),
                    result["ServerTime"].toString());
            qDebug()<< "getStationTimer success:";
        }
    }
    if(!isSuccess)
    {
        qDebug()<<"getStationTimer fail";
    }
}
