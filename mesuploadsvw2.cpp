#include "mesuploadsvw2.h"

MESUploadSVW2::MESUploadSVW2(QObject *parent) :
    QObject(parent)
{
    this->moveToThread(&m_thread);
    m_thread.start();
}

void MESUploadSVW2::init()
{
    if(vari2_XL == "XL")
    {
        strTarget = "LeakData";
    }
    else
    {
        strTarget = "TighteningData";
    }
    qDebug() <<"MESUpload thread start!!"<<vari2_XL<<strTarget;
    TIMEOUT = (5 * 1000);
    isFirst = true;
    GetTimes = 0;
    GetTimesTemp = 0;
    wrongRecordID = -1;
    wrongTimes = 0;
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    QString DataServerIp = configIniRead->value("baseinfo/DataServerIp").toString();
    QString ServerPort = configIniRead->value("baseinfo/ServerPort").toString();
    delete configIniRead;
    manager = new QNetworkAccessManager(this);
    manager1 = new QNetworkAccessManager(this);
    manager3 = new QNetworkAccessManager(this);
    //    QNetworkRequest req;
    QString url = "";
    QString url_keycode = "";
    QString url_trayCode = "/binding/trayandmodule?";
    if(Station.mid(0,2) == "VM"){
        url = "/Module/mtightendata?";
        url_keycode = "/Module/mKeybarcode";
    }else{
        url = "/upload/tightendata?";
        url_keycode = "/upload/Keybarcode?";
    }
    req.setUrl(QUrl("http://"+DataServerIp+":"+ServerPort+url));
    req3.setUrl(QUrl("http://"+DataServerIp+":"+ServerPort+url_keycode));
    req1.setUrl(QUrl("http://"+DataServerIp+":"+ServerPort+url_trayCode));
    //    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
    //    connect (&InsertTimer,SIGNAL(timeout()),this,SLOT(dataInsert()));
    //    InsertTimer.start(1000);
    QTimer::singleShot(1000,this,SLOT(dataInsert()));
    QTimer::singleShot(1000,this,SLOT(keyCodeInsert()));
    if(Station.contains("VM010",Qt::CaseInsensitive)||Station.contains("VM040",Qt::CaseInsensitive)){
        QTimer::singleShot(1000,this,SLOT(trayCodeInsert()));
    }
}

void MESUploadSVW2::dataInsert()
{
    WIFIlock.lockForRead();
    if(WIFI_STATE)//服务器
    {
        WIFIlock.unlock();
        if(GetTimes == 0)       //Time
        {
            GetTimes++;
            QVariantMap dataMap;
            dataMap.insert("Type","Get");
            dataMap.insert("Target","GetCurrentTime");
            QJson::Serializer serializer;
            bool ok;
            QByteArray json = serializer.serialize(dataMap, &ok);

            req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            req.setHeader(QNetworkRequest::ContentLengthHeader, json.length());

            if (ok)
            {
                qDebug() << "Get CurrentTime";
                QNetworkReply *reply = manager->post(req,json);
                reply->ignoreSslErrors();
                QTime t_time;
                t_time.start();

                bool timeout = false;

                while (!reply->isFinished()) {
                    QApplication::processEvents();
                    if (t_time.elapsed() >= TIMEOUT) {
                        timeout = true;
                        qDebug()<<"CurrentTime reply timeout";
                        break;
                    }
                }
                if (!timeout && reply->error() == QNetworkReply::NoError)
                {
                    QByteArray bytes = reply->readAll();
                    //        qDebug()<<bytes;

                    QJson::Parser parser;
                    bool ok;

                    QVariantMap result = parser.parse(bytes, &ok).toMap();
                    if (!ok) {
                        qDebug()<<"Get CurrentTime An error occurred during parsing"<<bytes;
                    }
                    else
                    {
                        if(result["Result"].toBool())
                        {
                            if(result["Type"].toString()=="Get" && result["Target"].toString()=="GetCurrentTime")
                            {
                                GUpdateTimeVal = result["CurrentTime"].toString();
                                GUpdateTimeFlag = true;//打开开关，同步时间
//                                QString datetime = result["CurrentTime"].toString();
//                                system((QString("date -s \"") +datetime+QString("\" &")).toLatin1().data());
//                                //将系统时间写入RTC
//                                system("hwclock -w &");
//                                qDebug()<<"GetCurrentTime success "<<datetime;
                            }
                            else
                                qDebug()<<"What's wrong"<<bytes;
                        }
                        else
                        {
                            qDebug()<<"GetCurrentTime fail ";   //<<result["Error"].toString();
                            if(result["ErrorCode"].toString()== "1")
                            {
                                qDebug()<<"Server Json parse fail"<<json;
                            }
                        }
                    }
                }
                else if(reply->error() != QNetworkReply::NoError)
                {
                    qDebug()<<"GetCurrentTime handle errors here";
                    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                    //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
                    qDebug( "GetCurrentTime found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
                    qDebug()<<reply->errorString();
                }

                reply->deleteLater();
            }
            else
            {
                qDebug() << "Something went wrong:" << serializer.errorMessage();
            }
        }
        else if(GetTimes == 51 )      //Andon
        {
            GetTimes++;
            QVariantMap dataMap;
            dataMap.insert("Type","Set");
            dataMap.insert("Target","Andon");

            QVariantMap data;
            data.insert("StationStatus",StationStatus);
            data.insert("Data2IP",WirelessIp);

            dataMap.insert("Parameter",data);

            QJson::Serializer serializer;
            bool ok;
            QByteArray json = serializer.serialize(dataMap, &ok);

            req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            req.setHeader(QNetworkRequest::ContentLengthHeader, json.length());

            if (ok)
            {
                qDebug()<<"Set Andon";
                QNetworkReply *reply = manager->post(req,json);
                reply->ignoreSslErrors();
                QTime t_time;
                t_time.start();

                bool timeout = false;

                while (!reply->isFinished()) {
                    QApplication::processEvents();
                    if (t_time.elapsed() >= TIMEOUT) {
                        timeout = true;
                        qDebug()<<"Andon reply timeout";
                        break;
                    }
                }
                if (!timeout && reply->error() == QNetworkReply::NoError)
                {
                    QByteArray bytes = reply->readAll();
                    //        qDebug()<<bytes;

                    QJson::Parser parser;
                    bool ok;

                    QVariantMap result = parser.parse(bytes, &ok).toMap();
                    if (!ok) {
                        qDebug()<<"Set Andon An error occurred during parsing"<<bytes;
                    }
                    else
                    {
                        if(!result["Result"].toBool())
                        {
                            qDebug()<<"Set Andon fail "; //<<result["Error"].toString();
                            if(result["ErrorCode"].toString()== "1")
                            {
                                qDebug()<<"Server Json parse fail"<<json;
                            }
                        }
                    }
                }
                else if(reply->error() != QNetworkReply::NoError)
                {
                    qDebug()<<"Andon handle errors here";
                    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                    //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
                    qDebug( "Andon found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
                    qDebug()<<reply->errorString();
                }

                reply->deleteLater();
            }
            else
            {
                qDebug() << "Something went wrong:" << serializer.errorMessage();
            }
        }
        else       //Insert
        {
            qDebug()<<"come in tight insert";
            GetTimes++;
            if(GetTimes == 120)
                GetTimes = 0;

            if(!db2.isOpen() || !QSqlDatabase::contains("ReinsertMySQL") )
                mysql_open();
            if(db2.isOpen() && QSqlDatabase::contains("ReinsertMySQL"))
            {
                //开机就会一直打印
                //qDebug()<<"select no data"<<query2.exec("select RecordID from SVW2TighteningDatas");//ture
                //qDebug()<<"select false no test"<<query2.exec("select Angle_Max from SVW2TighteningDatas");//false
                if(!query2.exec("select Angle_Max from SVW2TighteningDatas")){
                    query2.exec("alter table SVW2TighteningDatas add Angle_Max double");//创建最大角度
                }
                if(!query2.exec("select Angle_Min from SVW2TighteningDatas")){
                    query2.exec("alter table SVW2TighteningDatas add Angle_Min double");//创建最小角度
                }
                if(!query2.exec("select Torque_Max from SVW2TighteningDatas")){
                    query2.exec("alter table SVW2TighteningDatas add Torque_Max double");//创建最大扭矩
                }
                if(!query2.exec("select Torque_Min from SVW2TighteningDatas")){
                    query2.exec("alter table SVW2TighteningDatas add Torque_Min double");//创建最小扭矩
                }

                if(!query2.exec("SELECT RecordID, IDCode, ScrewID, Torque, Angle, Curve, TighteningStatus, TighteningTime, Cycle, Operator, Program, JobStatus, Order_id, `MaxValue`, `MinValue`, Channel , Angle_Max , Angle_Min , Torque_Max, Torque_Min FROM "+Localtable +" Where UploadMark =4 and Curve !='null' order by RecordID limit 1"))
                {
                    //                    qDebug()<<"SELECT fail "<<query2.lastError();
                }
                else
                {
                    if(query2.next())
                    {
                        QString ID = query2.value(0).toString();
                        QVariantMap dataMap;
                        if(vari2_XL == "XL")
                        {
                            dataMap.insert("Type","Set");
                            dataMap.insert("Target",strTarget);//LeakData
                            QVariantMap data;
                            data.insert("ID", query2.value(0).toInt());
                            data.insert("TimeStamp", query2.value(7).toString().left(10)+" "+query2.value(7).toString().right(8));
                            data.insert("Result", query2.value(6).toString());//Result
                            data.insert("Code","0");
                            data.insert("PackSN", query2.value(1).toString());
                            if(GPackType == "PHEV")
                            {
                                data.insert("ProductType","PHEV");    //insert
                            }
                            else
                            {
                                data.insert("ProductType","BEV");    //insert
                            }
                            data.insert("AGVSN","");
                            data.insert("TestType","Leck");
                            data.insert("Station", Station);
                            data.insert("Channel", query2.value(15).toInt());
                            data.insert("Cycle", query2.value(8).toInt());
                            data.insert("Program", query2.value(10).toInt());
                            data.insert("ScrewID", query2.value(2).toString());
                            data.insert("VL", query2.value(3).toDouble());
                            data.insert("PM", query2.value(4).toDouble());
                            data.insert("IpAddress", WirelessIp);
                            data.insert("Line_ID", Line_ID);
                            QVariantMap TestData;
                            TestData.insert("VL", query2.value(3).toDouble());
                            TestData.insert("PM", query2.value(4).toDouble());
                            TestData.insert("Operate","root");
                            TestData.insert("Remark","");
                            //                            TestData.insert("Channel", query2.value(15).toInt());
                            //                            TestData.insert("Cycle", query2.value(8).toInt());
                            QVariantMap Steps;
                            Steps.insert("StepNo","1");
                            Steps.insert("ItemName","Leckrate Test");
                            Steps.insert("ExtName","");
                            QVariantMap leakrate_Cooling;
                            leakrate_Cooling.insert("Unit","ccm_min");
                            leakrate_Cooling.insert("UG","0");
                            leakrate_Cooling.insert("OG","5");
                            leakrate_Cooling.insert("Value",query2.value(3).toDouble());
                            leakrate_Cooling.insert("Result",query2.value(6).toString());
                            //                            Steps.insert("leakrate_Cooling",leakrate_Cooling);
                            QVariantMap leakrate_Box;
                            leakrate_Box.insert("Unit","ccm_min");
                            leakrate_Box.insert("UG","0");
                            leakrate_Box.insert("OG","100");
                            leakrate_Box.insert("Value",query2.value(3).toDouble());
                            leakrate_Box.insert("Result",query2.value(6).toString());
                            if(query2.value(15).toInt() == 1)
                            {
                                qDebug()<<"leakrate_Cooling";
                                Steps.insert("leakrate_Cooling",leakrate_Cooling);
                            }
                            else if(query2.value(15).toInt() == 2)
                            {
                                qDebug()<<"leakrate_Box";
                                Steps.insert("leakrate_Box",leakrate_Box);
                            }
                            else
                            {
                                qDebug()<<"channel error";
                            }
                            //                            Steps.insert("leakrate_Box",leakrate_Box);
                            TestData.insert("Steps",Steps);
                            //                            TestData.insert("ScrewID", query2.value(2).toString());
                            //                            TestData.insert("VL", query2.value(3).toDouble());//
                            //                            TestData.insert("PM", query2.value(4).toDouble());
                            //                            TestData.insert("Cycle", query2.value(8).toInt());
                            //                            TestData.insert("Program", query2.value(10).toInt());
                            //                            TestData.insert("Channel", query2.value(15).toInt());
                            //                            TestData.insert("IpAddress", WirelessIp);
                            //                            TestData.insert("Line_ID", Line_ID);
                            data.insert("TestData",TestData);
                            dataMap.insert("Parameter",data);
                        }
                        else
                        {
                            //                            QVariantMap dataMap;
                            dataMap.insert("Type","Set");
                            dataMap.insert("Target",strTarget);//LeakData
                            QVariantMap data;
                            data.insert("Version",1);
                            data.insert("ID", query2.value(0).toInt());
                            data.insert("IDCode", query2.value(1).toString());
                            data.insert("ScrewID", query2.value(2).toString());
                            data.insert("Torque", query2.value(3).toDouble());//
                            data.insert("Angle", query2.value(4).toDouble());
                            data.insert("Curve", query2.value(5).toString());
                            data.insert("TighteningStatus", query2.value(6).toString());//Result
                            data.insert("TighteningTime", query2.value(7).toString().left(10)+" "+query2.value(7).toString().right(8));
                            data.insert("Cycle", query2.value(8).toInt());
                            data.insert("Operator", query2.value(9).toString());
                            data.insert("Program", query2.value(10).toInt());
                            data.insert("JobStatus", query2.value(11).toInt());
                            data.insert("IIO", query2.value(12).toInt());
                            QString maxValue = QString("%1").arg(query2.value(13).toDouble(), 0, 'g', 2);
                            QString minValue = QString("%1").arg(query2.value(14).toDouble(), 0, 'g', 2);
                            data.insert("MaxValue", maxValue);
                            data.insert("MinValue", minValue);

                            data.insert("Channel", query2.value(15).toInt());

                            //上传接口的数据，按接口的字段名来
                            data.insert("Angle_Max",query2.value(16));//Angle_Max
                            data.insert("Angle_Min",query2.value(17));//Angle_Min
                            data.insert("Torque_Max",query2.value(18));//Torque_Max
                            data.insert("Torque_Min",query2.value(19));//Torque_Min

                            data.insert("Line_ID", Line_ID);
                            data.insert("Station", Station);
                            data.insert("IpAddress", WirelessIp);
                            data.insert("Workshop", "Assembly");
                            data.insert("WorkShift", "");
                            dataMap.insert("Parameter",data);
                        }
                        QJson::Serializer serializer;
                        bool ok;
                        QByteArray json = serializer.serialize(dataMap, &ok);

                        //                        qDebug()<<"@@@@@@@@@@"<<vari2_XL<<strTarget<<json;

                        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
                        req.setHeader(QNetworkRequest::ContentLengthHeader, json.length());

                        if (ok)
                        {
                            qDebug() << "post RecordID:"<<query2.value(0).toInt()<<"Cycle:"<<query2.value(8).toInt();
                            QNetworkReply *reply = manager->post(req,json);
                            reply->ignoreSslErrors();
                            QTime t_time;
                            t_time.start();

                            bool timeout = false;

                            while (!reply->isFinished()) {
                                QApplication::processEvents();
                                if (t_time.elapsed() >= TIMEOUT) {
                                    timeout = true;
                                    qDebug()<<"insert reply timeout";
                                    break;
                                }
                            }
                            if (!timeout && reply->error() == QNetworkReply::NoError)
                            {
                                QByteArray bytes = reply->readAll();
                                qDebug()<<bytes;

                                QJson::Parser parser;
                                bool ok;

                                QVariantMap result = parser.parse(bytes, &ok).toMap();
                                if (!ok) {
                                    qDebug()<<"post Insert An error occurred during parsing"<<bytes;
                                }
                                else
                                {
                                    if(result["Result"].toBool())
                                    {
                                        if(result["Type"].toString()=="Set" && result["Target"].toString()==strTarget)
                                        {
                                            QString RecordID = result["ID"].toString();
                                            QString Cycle = result["Cycle"].toString();
                                            if(query2.exec("UPDATE "+Localtable+" SET UploadMark = 1,UploadTime = now() WHERE RecordID = "+RecordID))
                                            {
                                                qDebug()<<"update 1 success: RecordID:"<<RecordID<<"Cycle:"<<Cycle;
                                            }
                                            else
                                            {
                                                qDebug()<<"update 1 fail: RecordID:"<<RecordID<<"Cycle:"<<Cycle<<query2.lastError();
                                            }
                                        }
                                        else
                                            qDebug()<<"What's wrong"<<bytes;
                                    }
                                    else
                                    {
                                        if(result["ErrorCode"].toString()== "1")
                                        {
                                            qDebug()<<"Server Json parse fail"<<json;
                                            if(query2.exec("UPDATE "+Localtable+" SET UploadMark = 7, UploadTime = now() WHERE RecordID = "+ID))
                                            {
                                                qDebug()<<"update 7 success: RecordID:"<<ID;
                                            }
                                            else
                                            {
                                                qDebug()<<"update 7 fail: RecordID:"<<ID<<query2.lastError();
                                            }
                                        }
                                        else if(bytes == "")
                                        {
                                            qDebug()<<"reply is null";
                                        }
                                        else
                                        {
                                            QString RecordID = result["ID"].toString();
                                            QString Cycle = result["Cycle"].toString();
                                            qDebug()<<"Reinsert fail RecordID:"<<RecordID<<"Cycle:"<<Cycle;
                                            if(wrongRecordID != RecordID.toInt())
                                            {
                                                wrongRecordID = RecordID.toInt();
                                                wrongTimes++;
                                            }
                                            else
                                            {
                                                wrongTimes++;
                                                if(wrongTimes == 10)
                                                {
                                                    if(query2.exec("UPDATE "+Localtable+" SET UploadMark = 6, UploadTime = now() WHERE RecordID = "+RecordID))
                                                    {
                                                        wrongRecordID = -1;
                                                        wrongTimes = 0;
                                                        qDebug()<<"update 6 success: RecordID:"<<RecordID<<"Cycle:"<<Cycle;
                                                    }
                                                    else
                                                    {
                                                        qDebug()<<"update 6 fail: RecordID:"<<RecordID<<"Cycle:"<<Cycle<<query2.lastError();
                                                    }
                                                }
                                                else if(wrongTimes >10)
                                                    wrongTimes = 0;
                                            }
                                        }
                                    }
                                }
                            }
                            else if(reply->error() != QNetworkReply::NoError)
                            {
                                qDebug()<<"insert handle errors here";
                                QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                                //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
                                qDebug( "insert found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
                                qDebug()<<reply->errorString();
                            }

                            reply->deleteLater();
                        }
                        else
                        {
                            qDebug() << "Something went wrong:" << serializer.errorMessage();
                        }
                    }
                }

            }
            else
            {
                qDebug()<<"db2 is not open or ReinsertMySQL connect fail";
            }
        }
    }
    else
        WIFIlock.unlock();
    QTimer::singleShot(1000,this,SLOT(dataInsert()));
}


void MESUploadSVW2::keyCodeInsert()
{
    WIFIlock.lockForRead();
    if(WIFI_STATE)//服务器
    {
        WIFIlock.unlock();
        if(GetTimesTemp == 0)       //Time
        {
            GetTimesTemp++;
        }
        else if(GetTimesTemp == 51 )      //Andon
        {
            GetTimesTemp++;
        }
        else       //Insert
        {
            qDebug()<<"come in scan insert";
            GetTimesTemp++;
            if(GetTimesTemp == 120)
                GetTimesTemp = 0;

            if(!db2.isOpen() || !QSqlDatabase::contains("ReinsertMySQL") )
                mysql_open();
            if(db2.isOpen() && QSqlDatabase::contains("ReinsertMySQL"))
            {
 //-------------------------------------------------------------------------------------------------------------------------------------------------------------
                if(!query2.exec("SELECT * FROM KeyCodeInfo Where Flag = '0' order by ID"))
                {
                    qDebug()<<"SELECT fail "<<query2.lastError();
                    if(query2.lastError().text().contains("MySQL server has gone away",Qt::CaseInsensitive)){//数据库丢失了（系统死机前兆）
                        emit send_mysqlerror();
                    }
                }
                else
                {
                    if(query2.next())
                    {
                        QVariantMap dataMap;
                        //上传接口的数据，按接口的字段名来
                        dataMap.insert("ID",query2.value(0).toInt());
                        dataMap.insert("LineID",query2.value(1));
                        //dataMap.insert("Flag",query2.value(2).toInt());
                        dataMap.insert("PartsCode",query2.value(3).toString());
                        dataMap.insert("KeyCode",query2.value(4).toString());
                        dataMap.insert("PackSN",query2.value(5).toString());
                        dataMap.insert("StationID",query2.value(6).toString());
                        dataMap.insert("DataTime",query2.value(8).toString());
                        //dataMap.insert("LocalTime",query2.value(8).toString());
                        dataMap.insert("PartsID",query2.value(9));

                        QJson::Serializer serializer;
                        bool ok;
                        QByteArray json = serializer.serialize(dataMap, &ok);
                        req3.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
                        req3.setHeader(QNetworkRequest::ContentLengthHeader, json.length());

                        if (ok)
                        {
                            qDebug() << "post ID:"<<query2.value(0).toInt()<<"KeyCode:"<<query2.value(4).toString();
                            QNetworkReply *reply = manager3->post(req3,json);
                            reply->ignoreSslErrors();
                            QTime t_time;
                            t_time.start();

                            bool timeout = false;

                            while (!reply->isFinished()) {
                                QApplication::processEvents();
                                if (t_time.elapsed() >= TIMEOUT) {
                                    timeout = true;
                                    qDebug()<<"insert reply timeout";
                                    break;
                                }
                            }
                            if (!timeout && reply->error() == QNetworkReply::NoError)
                            {
                                QByteArray bytes = reply->readAll();
                                qDebug()<<bytes;

                                QJson::Parser parser;
                                bool ok;

                                QVariantMap result = parser.parse(bytes, &ok).toMap();
                                if (!ok) {
                                    qDebug()<<"post Insert An error occurred during parsing KeyCodeInfo"<<bytes;
                                }
                                else
                                {
                                    if(result["Result"].toBool())
                                    {
                                        QString ID = result["ID"].toString();
                                        QString KeyCode = result["KeyCode"].toString();
                                        query2.prepare("UPDATE KeyCodeInfo SET Flag = 1,UpdateTime = now() WHERE ID = :ID");
                                        query2.bindValue(":ID",ID);
                                        if(query2.exec())
                                        {
                                            qDebug()<<"update 1 success: ID:"<<ID<<"KeyCode:"<<KeyCode;
                                        }
                                        else
                                        {
                                            qDebug()<<"update 1 fail: ID:"<<ID<<"KeyCode:"<<KeyCode<<query2.lastError();
                                        }

                                    }
                                    else
                                    {
                                        if(result["ErrorCode"].toString()== "1")
                                        {
                                        }
                                        else if(bytes == "")
                                        {
                                            qDebug()<<"reply is null";
                                        }
                                        else
                                        {
                                            QString ID = result["ID"].toString();
                                            QString KeyCode = result["KeyCode"].toString();
                                            qDebug()<<"Reinsert fail ID:"<<ID<<"KeyCode:"<<KeyCode;
                                            if(wrongRecordID != ID.toInt())
                                            {
                                                wrongRecordID = ID.toInt();
                                                wrongTimes++;
                                            }
                                            else
                                            {
                                                wrongTimes++;
                                                if(wrongTimes == 10)
                                                {
                                                    qDebug()<<"wrongTimes"<<wrongTimes;
                                                }
                                                else if(wrongTimes >10)
                                                    wrongTimes = 0;
                                            }
                                        }
                                    }
                                }
                            }
                            else if(reply->error() != QNetworkReply::NoError)
                            {
                                qDebug()<<"insert handle errors here";
                                QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                                //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
                                qDebug( "insert found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
                                qDebug()<<reply->errorString();
                            }

                            reply->deleteLater();
                        }
                        else
                        {
                            qDebug() << "Something went wrong:" << serializer.errorMessage();
                        }
                    }
                }
            }
            else
            {
                qDebug()<<"db2 is not open or ReinsertMySQL connect fail";
            }
        }
    }
    else
        WIFIlock.unlock();
    QTimer::singleShot(2000,this,SLOT(keyCodeInsert()));
}

void MESUploadSVW2::trayCodeInsert()
{
    WIFIlock.lockForRead();
    if(WIFI_STATE)//服务器
    {
        WIFIlock.unlock();
        if(GetTimesTemp == 0)       //Time
        {
            GetTimesTemp++;
        }
        else if(GetTimesTemp == 51 )      //Andon
        {
            GetTimesTemp++;
        }
        else       //Insert
        {
            qDebug()<<"come in tray insert";
            GetTimesTemp++;
            if(GetTimesTemp == 120)
                GetTimesTemp = 0;

            if(!db2.isOpen() || !QSqlDatabase::contains("ReinsertMySQL") )
                mysql_open();
            if(db2.isOpen() && QSqlDatabase::contains("ReinsertMySQL"))
            {
 //-------------------------------------------------------------------------------------------------------------------------------------------------------------
                if(!query2.exec("SELECT * FROM TrayCodeInfo Where Flag = '0' order by ID"))
                {
                    qDebug()<<"SELECT fail "<<query2.lastError();
                }
                else
                {
                    if(query2.next())
                    {
                        QVariantMap dataMap;
                        //上传接口的数据，按接口的字段名来
                        dataMap.insert("ID",query2.value(0).toInt());
                        dataMap.insert("StationID",query2.value(1).toString());
                        dataMap.insert("LineID",query2.value(2).toInt());
                        dataMap.insert("TraySN",query2.value(3).toString());
                        dataMap.insert("Type",query2.value(4).toString());
                        dataMap.insert("PartsCode",query2.value(5).toString());
                        dataMap.insert("DateTime",query2.value(6).toString());
                        dataMap.insert("M1",query2.value(7).toString());
                        dataMap.insert("M2",query2.value(8).toString());
                        dataMap.insert("M3",query2.value(9).toString());
                        dataMap.insert("M4",query2.value(10).toString());
                        dataMap.insert("M5",query2.value(11).toString());
                        dataMap.insert("M6",query2.value(12).toString());
                        dataMap.insert("M7",query2.value(13).toString());
                        dataMap.insert("M8",query2.value(14).toString());
                        dataMap.insert("Flag",query2.value(15).toInt());

                        QJson::Serializer serializer;
                        bool ok;
                        QByteArray json = serializer.serialize(dataMap, &ok);
                        req1.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
                        req1.setHeader(QNetworkRequest::ContentLengthHeader, json.length());

                        if (ok)
                        {
                            qDebug() << "post ID:"<<query2.value(0).toInt()<<"KeyCode:"<<query2.value(3).toString();
                            QNetworkReply *reply = manager1->post(req1,json);
                            reply->ignoreSslErrors();
                            QTime t_time;
                            t_time.start();

                            bool timeout = false;

                            while (!reply->isFinished()) {
                                QApplication::processEvents();
                                if (t_time.elapsed() >= TIMEOUT) {
                                    timeout = true;
                                    qDebug()<<"insert reply timeout";
                                    break;
                                }
                            }
                            if (!timeout && reply->error() == QNetworkReply::NoError)
                            {
                                QByteArray bytes = reply->readAll();
                                qDebug()<<bytes;

                                QJson::Parser parser;
                                bool ok;

                                QVariantMap result = parser.parse(bytes, &ok).toMap();
                                if (!ok) {
                                    qDebug()<<"post Insert An error occurred during parsing KeyCodeInfo"<<bytes;
                                }
                                else
                                {
                                    if(result["Result"].toBool())
                                    {
                                        QString ID = result["ID"].toString();
                                        QString KeyCode = result["TraySN"].toString();
                                        query2.prepare("UPDATE TrayCodeInfo SET Flag = 1 WHERE ID = :ID");
                                        query2.bindValue(":ID",ID);
                                        if(query2.exec())
                                        {
                                            qDebug()<<"update 1 success: ID:"<<ID<<"KeyCode:"<<KeyCode;
                                        }
                                        else
                                        {
                                            qDebug()<<"update 1 fail: ID:"<<ID<<"KeyCode:"<<KeyCode<<query2.lastError();
                                        }

                                    }
                                    else
                                    {
                                        if(result["ErrorCode"].toString()== "1")
                                        {
                                        }
                                        else if(bytes == "")
                                        {
                                            qDebug()<<"reply is null";
                                        }
                                        else
                                        {
                                            QString ID = result["ID"].toString();
                                            QString KeyCode = result["TraySN"].toString();
                                            qDebug()<<"Reinsert fail ID:"<<ID<<"KeyCode:"<<KeyCode;
                                            if(wrongRecordID != ID.toInt())
                                            {
                                                wrongRecordID = ID.toInt();
                                                wrongTimes++;
                                            }
                                            else
                                            {
                                                wrongTimes++;
                                                if(wrongTimes == 10)
                                                {
                                                    qDebug()<<"wrongTimes"<<wrongTimes;
                                                }
                                                else if(wrongTimes >10)
                                                    wrongTimes = 0;
                                            }
                                        }
                                    }
                                }
                            }
                            else if(reply->error() != QNetworkReply::NoError)
                            {
                                qDebug()<<"insert handle errors here";
                                QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                                //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
                                qDebug( "insert found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
                                qDebug()<<reply->errorString();
                            }

                            reply->deleteLater();
                        }
                        else
                        {
                            qDebug() << "Something went wrong:" << serializer.errorMessage();
                        }
                    }
                }
            }
            else
            {
                qDebug()<<"db2 is not open or ReinsertMySQL connect fail";
            }
        }
    }
    else
        WIFIlock.unlock();
    QTimer::singleShot(2000,this,SLOT(trayCodeInsert()));
}

// 本地mysql open
void MESUploadSVW2::mysql_open()
{
    if(QSqlDatabase::contains("ReinsertMySQL")){
        db2 = QSqlDatabase::database("ReinsertMySQL");
    }else{
        db2=QSqlDatabase::addDatabase("QMYSQL","ReinsertMySQL");
        db2.setHostName("localhost");
        db2.setDatabaseName("Tighten");
        db2.setUserName("root");
        db2.setPassword("123456");
        query2 = QSqlQuery(db2);
    }

    if(!db2.open())
    {
        if(!db2.open())
        {
            qDebug()<< "reinsert localmysql "<< db2.lastError().text();
        }else
        {
            qDebug()<< "reinsert localmysql open ok 2";
        }
    }else
    {
        qDebug()<< "reinsert localmysql open ok 1";
    }
}
