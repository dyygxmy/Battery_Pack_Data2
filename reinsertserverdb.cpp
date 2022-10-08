#include "reinsertserverdb.h"

ReInsertServerDb::ReInsertServerDb(QObject *parent) :
    QObject(parent)
{
    m_thread.start();
    this->moveToThread(&m_thread);
    //connect(&m_thread,SIGNAL(finished()),this,SLOT(printDead()));
}


void ReInsertServerDb::reinsert_start()
{
    qDebug()<<"reInsertServerDb thread start";
    //    db1=QSqlDatabase::addDatabase("QODBC","ReinsertCurve");
    //    db1.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=1;SQL_ATTR_CONNECTION_TIMEOUT=1");
    //    db1.setDatabaseName("sqlcurve");
    //    db1.setPort(1433);
    //    db1.setUserName(SqlUserName);
    //    db1.setPassword(SqlPassword);
    //    query1 = QSqlQuery(db1);

    //    db2=QSqlDatabase::addDatabase("QMYSQL","ReinsertMySQL");

    //    db2.setHostName("localhost");
    //    db2.setDatabaseName("Tighten");
    //    db2.setUserName("root");
    //    db2.setPassword("123456");
    //    query2 = QSqlQuery(db2);
    //    query4 = QSqlQuery(db2);

    //    db3=QSqlDatabase::addDatabase("QODBC","ReinsertData");

    //    db3.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=1;SQL_ATTR_CONNECTION_TIMEOUT=1");
    //    db3.setDatabaseName("sqltighten");
    //    db3.setPort(1433);
    //    db3.setUserName(SqlUserName);
    //    db3.setPassword(SqlPassword);
    //    query3 = QSqlQuery(db3);
    //connect(&m_insert,SIGNAL(timeout()),this,SLOT(timer_insert()));
    // m_insert.start(5000);
    timer_insert();
}

void ReInsertServerDb::timer_insert()
{
    //5秒 遍历本地数据库标志位 处理
    forever
//    while(1)
    {
        WIFIlock.lockForRead();
        qDebug()<< "I Am Alive " << WIFI_STATE;
        if(WIFI_STATE)
        {
            WIFIlock.unlock();
            //            if(!db1.isOpen()||!QSqlDatabase::contains("ReinsertCurve"))
            //                curvesqlopen();
            //            if(!db2.isOpen()||!QSqlDatabase::contains("ReinsertMySQL"))
            //                mysqlopen();
            //            if(!db3.isOpen()||!QSqlDatabase::contains("ReinsertData"))
            //                datasqlopen();
            bool datasql = false;
            bool curvesql = false;
            int uploadmark = 0;
            QString msg="";

            {
                QSqlDatabase db1 = QSqlDatabase::addDatabase("QODBC","ReinsertCurve");
                db1.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=1;SQL_ATTR_CONNECTION_TIMEOUT=1");
                db1.setDatabaseName("sqlcurve");
                db1.setPort(1433);
                db1.setUserName(SqlUserName);
                db1.setPassword(SqlPassword);
                QSqlQuery query1 = QSqlQuery(db1);
                if(QSqlDatabase::contains("ReinsertCurve"))
                {
                    if(!db1.open())
                    {
                        if(!db1.open())
                        {
                            //qDebug()<<"reinsert curvesql"<<db1.lastError().text();
                            qDebug()<< "reinsert curvesql open fail 362" << db1.lastError().text();
                        }
                        else
                        {
                            qDebug()<< "reinsert curvesql open ok 2";
                        }

                    }else
                    {
                        //qDebug()<< "reinsert curvesql open ok 1";
                    }
                }
                else
                {
                    qDebug()<<"add Database ReinsertCurve fail";
                }

                QSqlDatabase db2 = QSqlDatabase::addDatabase("QMYSQL","ReinsertMySQL");
                db2.setHostName("localhost");
                db2.setDatabaseName("Tighten");
                db2.setUserName("root");
                db2.setPassword("123456");
                QSqlQuery query2 = QSqlQuery(db2);
                QSqlQuery query4 = QSqlQuery(db2);
                if(QSqlDatabase::contains("ReinsertMySQL"))
                {
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
                        //qDebug()<< "reinsert localmysql open ok 1";
                    }
                }
                else
                {
                    qDebug()<<"add Database ReinsertMySQL fail";
                }

                QSqlDatabase db3 = QSqlDatabase::addDatabase("QODBC","ReinsertData");
                db3.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=1;SQL_ATTR_CONNECTION_TIMEOUT=1");
                db3.setDatabaseName("sqltighten");
                db3.setPort(1433);
                db3.setUserName(SqlUserName);
                db3.setPassword(SqlPassword);
                QSqlQuery query3 = QSqlQuery(db3);
                if(QSqlDatabase::contains("ReinsertData"))
                {
                    if(!db3.open())
                    {
                        if(!db3.open())
                        {
                            qDebug()<< "reinsert datasql open fail 329"<<db3.lastError().text();
                            //                            qDebug()<< "reinsert datasql open fail 329";
                        }else
                        {
                            qDebug()<< "reinsert datasql open ok 2";
                        }
                    }else
                    {
                        qDebug()<< "reinsert datasql open ok 1";
                    }
                }
                else
                {
                    qDebug()<<"add Database ReinsertData fail";
                }

                if(db2.isOpen() && QSqlDatabase::contains("ReinsertMySQL"))
                {
                    //上传标志位为2和4的10条
                    if(!query2.exec("SELECT RecordID, IDCode, ScrewID, Torque, Angle, Curve, TighteningStatus, TighteningTime,UploadMark,Cycle,Operator,Program,Channel FROM TighteningDatas Where UploadMark =2 or UploadMark =4 order by RecordID limit 10"))
                    {
                        qDebug() << "Mysql error need to be repaired";
                        system(QString("/usr/local/mysql/bin/myisamchk -c -r -o -f /usr/local/mysql/var/Tighten/"+Localtable+".MYI").toLocal8Bit().data());
                        qDebug() << "repaire Mysql";
                    }
                    while(query2.next())
                    {
                        //qDebug() << "code 82";
                        datasql = false;
                        curvesql = false;
                        QByteArray VIN_PIN_byte = query2.value(1).toString().toAscii();
                        for (int i = 0; i < VIN_PIN_byte.length();i += 2)
                        {
                            VIN_PIN_byte[i] = ~VIN_PIN_byte[i];       //偶数位取反
                        }
                        QString vin_pin_sql;
                        vin_pin_sql = QString(VIN_PIN_byte.toBase64());

                        if(query2.value(8).toInt()==2)
                        {
                            qDebug()<<"*****************come in 2*************************";
                            if(db3.isOpen() && QSqlDatabase::contains("ReinsertData"))
                            {
                                query3.prepare("INSERT INTO Data.TighteningData(IDCode, ScrewID, Torque, Angle, TighteningStatus, TighteningTime, Cycle, Operator, Program, Channel, Line_ID)"
                                               "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                                query3.addBindValue(vin_pin_sql);
                                query3.addBindValue(query2.value(2).toInt());
                                query3.addBindValue(query2.value(3).toString());
                                query3.addBindValue(query2.value(4).toString());
                                query3.addBindValue(query2.value(6).toString());
                                query3.addBindValue(query2.value(7).toString());
                                query3.addBindValue(query2.value(9).toInt());
                                query3.addBindValue(query2.value(10).toString());
                                query3.addBindValue(query2.value(11).toInt());
                                query3.addBindValue(query2.value(12).toInt());
                                query3.addBindValue(Line_ID);
                                if(query3.exec())
                                {
                                    //qDebug() << "code 113";
                                    msg= QString("reinsertserverdb datasql insert success: Cycle:")+query2.value(9).toString();
                                    qDebug()<<msg;
                                    if(query4.exec("UPDATE TighteningDatas SET UploadMark = 1,UploadTime = now() WHERE RecordID = "+query2.value(0).toString()))
                                    {
                                        msg= QString("reinsertserverdb datasql update success: Cycle:")+query2.value(9).toString()+QString("||UploadMark:1");
                                        qDebug()<<msg;
                                    }
                                    else
                                    {
                                        msg= QString("reinsertserverdb datasql update fail: Cycle:")+query2.value(9).toString()+QString("||UploadMark:2");
                                        qDebug()<<msg;
                                    }
                                }
                                else
                                {
                                    msg= QString("reinsertserverdb datasql insert fail: Cycle:")+query2.value(9).toString();
                                    qDebug()<<msg<<query3.lastError();
                                }
                            }
                            else
                            {
                                qDebug()<<"db3 is not open or ReinsertData connect fail 1";
                            }
                        }
                        else if(query2.value(8).toInt()==4)
                        {
                            qDebug()<<"*****************come in 4*************************";
                            if(db3.isOpen()&& QSqlDatabase::contains("ReinsertData"))
                            {
                                query3.prepare("INSERT INTO Data.TighteningData(IDCode, ScrewID, Torque, Angle, TighteningStatus, TighteningTime, Cycle, Operator, Program, Channel, Line_ID)"
                                               "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                                query3.addBindValue(vin_pin_sql);
                                query3.addBindValue(query2.value(2).toInt());
                                query3.addBindValue(query2.value(3).toString());
                                query3.addBindValue(query2.value(4).toString());
                                query3.addBindValue(query2.value(6).toString());
                                query3.addBindValue(query2.value(7).toString());
                                query3.addBindValue(query2.value(9).toInt());
                                query3.addBindValue(query2.value(10).toString());
                                query3.addBindValue(query2.value(11).toInt());
                                query3.addBindValue(query2.value(12).toInt());
                                query3.addBindValue(Line_ID);
                                if(!query3.exec())
                                {
                                    //qDebug() << "code 151";
                                    datasql = false;
                                    msg= QString("reinsertserverdb datasql insert fail: Cycle:")+query2.value(9).toString();
                                    qDebug()<<msg<<query3.lastError();
                                }
                                else
                                {
                                    msg= QString("reinsertserverdb datasql insert success: Cycle:")+query2.value(9).toString();
                                    qDebug()<<msg;
                                    datasql = true;
                                }
                            }
                            else
                            {
                                qDebug()<<"db3 is not open or ReinsertData connect fail 2";
                            }
                            if(query2.value(5).toString()!="null")
                            {
                                if(db1.isOpen()&& QSqlDatabase::contains("ReinsertCurve"))
                                {
                                    bool ok_curve = false;
                                    bool ok_data = false;
                                    if(db1.transaction())
                                    {
                                        query1.prepare("INSERT INTO Curve.Curves(Curve) VALUES (?)");
//                                        query1.addBindValue(tr("偶数位取反"));
                                        query1.addBindValue(query2.value(5).toString());
//                                        qDebug()<<query2.value(5).toString();
                                        ok_curve = query1.exec();
                                        if(!ok_curve)
                                        {
                                            msg= QString("reinsertserverdb curvesql Curves insert fail: Cycle:")+query2.value(9).toString();
                                            qDebug()<<msg<<query1.lastError();
                                            curvesql = false;
                                        }
                                        else
                                        {
                                            msg= QString("reinsertserverdb curvesql Curves insert success: Cycle:")+query2.value(9).toString();
                                            qDebug()<<msg;
                                            curvesql = true;
                                        }

                                        if(ok_curve)
                                        {
                                            query1.prepare("INSERT INTO Data.TighteningDatas(IDCode, ScrewID, Torque, Angle, Curve, TighteningStatus, TighteningTime, Cycle, Operator, Program, Channel, Line_ID)"
                                                           "VALUES (?, ?, ?, ?, @@IDENTITY, ?, ?, ?, ?, ?, ?, ?)");
                                            query1.addBindValue(vin_pin_sql);
                                            query1.addBindValue(query2.value(2).toInt());
                                            query1.addBindValue(query2.value(3).toString());
                                            query1.addBindValue(query2.value(4).toString());
                                            //query1.addBindValue("@@IDENTITY");
                                            query1.addBindValue(query2.value(6).toString());
                                            query1.addBindValue(query2.value(7).toString());
                                            //query1.addBindValue("convert(varchar(10),getdate(),120)");
                                            query1.addBindValue(query2.value(9).toInt());
                                            query1.addBindValue(query2.value(10).toString());
                                            query1.addBindValue(query2.value(11).toInt());
                                            query1.addBindValue(query2.value(12).toInt());
                                            query1.addBindValue(Line_ID);
                                            ok_data  = query1.exec();
                                            if(!ok_data)
                                            {
                                                curvesql = false;
                                                msg= QString("reinsertserverdb curvesql TighteningDatas insert fail: Cycle:")+query2.value(9).toString();
                                                qDebug()<<msg<<query1.lastError();
                                            }
                                            else
                                            {
                                                msg= QString("reinsertserverdb curvesql TighteningDatas insert success: Cycle:")+query2.value(9).toString();
                                                qDebug()<<msg;
                                                //curvesql = true;
                                            }
                                        }
                                        if(ok_curve && ok_data)
                                        {
                                            if(db1.commit())
                                            {
                                                qDebug()<<"commit success";
                                                curvesql = true;
                                            }
                                            else
                                            {
                                                curvesql = false;
                                                qDebug()<<"commit fail: Cycle:"+query2.value(9).toString()<<db1.lastError();
                                                if (!db1.rollback())
                                                    qDebug() <<"rollback fail 1:"<< db1.lastError(); // 回滚
                                            }
                                        }
                                        else if( ok_curve && !ok_data )
                                        {
                                            if (!db1.rollback())
                                                qDebug() <<"rollback fail 2:"<< db1.lastError(); // 回滚
                                            curvesql = false;
                                        }
                                        else
                                        {
                                            curvesql = false;
                                        }
                                    }
                                }
                                else
                                {
                                    qDebug()<<"db1 is not open or ReinsertCurve connect fail 1";
                                }
                            }
                            if(curvesql && datasql)
                                uploadmark = 1;//all successful
                            else if(curvesql && !datasql)
                                uploadmark = 2;//data not successful
                            else if(!curvesql && datasql)
                                uploadmark = 3;
                            else if(!curvesql && !datasql)
                                uploadmark = 4;

                            if(uploadmark!=4)
                            {
                                if(query4.exec("UPDATE TighteningDatas SET UploadMark = "+QString::number(uploadmark)+",UploadTime = now() WHERE RecordID = "+query2.value(0).toString()))
                                {
                                    msg= QString("reinsertserverdb update success: Cycle:")+query2.value(9).toString()+QString("||UploadMark:")+QString::number(uploadmark);
                                    qDebug()<<msg;
                                }
                                else
                                {
                                    qDebug()<<query4.lastError();
                                    msg= QString("reinsertserverdb update fail: Cycle:")+query2.value(9).toString()+QString("||UploadMark:")+QString::number(uploadmark);
                                    qDebug()<<msg;
                                }
                            }
                        }
                    }
                    //上传标志位为3的5条
                    query2.exec("SELECT RecordID, IDCode, ScrewID, Torque, Angle, Curve, TighteningStatus, TighteningTime,UploadMark,Cycle,Operator,Program,Channel FROM TighteningDatas Where UploadMark =3 and Curve !='null' order by RecordID limit 5");
                    while(query2.next())
                    {
                        datasql = false;
                        curvesql = false;
                        QByteArray VIN_PIN_byte = query2.value(1).toString().toAscii();
                        for (int i = 0; i < VIN_PIN_byte.length();i += 2)
                        {
                            VIN_PIN_byte[i] = ~VIN_PIN_byte[i];       //偶数位取反
                        }
                        QString vin_pin_sql;
                        vin_pin_sql = QString(VIN_PIN_byte.toBase64());
                        qDebug()<<"*****************come in 3*************************";
                        if(db1.isOpen()&& QSqlDatabase::contains("ReinsertCurve"))
                        {
                            bool ok_curve = false;
                            bool ok_data = false;
                            if(db1.transaction())
                            {
                                query1.prepare("INSERT INTO Curve.Curves(Curve) VALUES (?)");
                                query1.addBindValue(query2.value(5).toString());
                                ok_curve = query1.exec();
                                if(!ok_curve)
                                {
                                    msg= QString("reinsertserverdb curvesql Curves insert fail: Cycle:")+query2.value(9).toString();
                                    qDebug()<<msg<<query1.lastError();
                                    curvesql = false;
                                }
                                else
                                {
                                    msg= QString("reinsertserverdb curvesql Curves insert success: Cycle:")+query2.value(9).toString();
                                    qDebug()<<msg;
                                    curvesql = true;
                                }

                                if(ok_curve)
                                {
                                    query1.prepare("INSERT INTO Data.TighteningDatas(IDCode, ScrewID, Torque, Angle, Curve, TighteningStatus, TighteningTime, Cycle, Operator, Program, Channel, Line_ID)"
                                                   "VALUES (?, ?, ?, ?, @@IDENTITY, ?, ?, ?, ?, ?, ?, ?)");
                                    query1.addBindValue(vin_pin_sql);
                                    query1.addBindValue(query2.value(2).toInt());
                                    query1.addBindValue(query2.value(3).toString());
                                    query1.addBindValue(query2.value(4).toString());
                                    //query1.addBindValue("@@IDENTITY");
                                    query1.addBindValue(query2.value(6).toString());
                                    query1.addBindValue(query2.value(7).toString());
                                    //query1.addBindValue("convert(varchar(10),getdate(),120)");
                                    query1.addBindValue(query2.value(9).toInt());
                                    query1.addBindValue(query2.value(10).toString());
                                    query1.addBindValue(query2.value(11).toInt());
                                    query1.addBindValue(query2.value(12).toInt());
                                    query1.addBindValue(Line_ID);
                                    ok_data  = query1.exec();
                                    if(!ok_data)
                                    {
                                        curvesql = false;
                                        msg= QString("reinsertserverdb curvesql TighteningDatas insert fail: Cycle:")+query2.value(9).toString();
                                        qDebug()<<msg<<query1.lastError();
                                    }
                                    else
                                    {
                                        msg= QString("reinsertserverdb curvesql TighteningDatas insert success: Cycle:")+query2.value(9).toString();
                                        qDebug()<<msg;
                                        //curvesql = true;
                                    }
                                }
                                if(ok_curve && ok_data)
                                {
                                    if(db1.commit())
                                    {
                                        qDebug()<<"commit success";
                                        curvesql = true;
                                    }
                                    else
                                    {
                                        curvesql = false;
                                        qDebug()<<"commit fail: Cycle:"+query2.value(9).toString()<<db1.lastError();
                                        if (!db1.rollback())
                                            qDebug() <<"rollback fail 1:"<< db1.lastError(); // 回滚
                                    }
                                }
                                else if( ok_curve && !ok_data )
                                {
                                    if (!db1.rollback())
                                        qDebug() <<"rollback fail 2:"<< db1.lastError(); // 回滚
                                    curvesql = false;
                                }
                                else
                                {
                                    curvesql = false;
                                }
                            }
                            if(curvesql)
                            {
                                if(query4.exec("UPDATE TighteningDatas SET UploadMark = 1,UploadTime = now() WHERE RecordID = "+query2.value(0).toString()))
                                {
                                    msg= QString("reinsertserverdb curvesql update success: Cycle:")+query2.value(9).toString()+QString("||UploadMark:1");
                                    qDebug()<<msg;
                                }
                                else
                                {
                                    msg= QString("reinsertserverdb curvesql update fail: Cycle:")+query2.value(9).toString()+QString("||UploadMark:3");
                                    qDebug()<<msg;
                                }
                            }
                        }
                        else
                        {
                            qDebug()<<"db1 is not open or ReinsertCurve connect fail 2";
                        }
                    }
                }
                else
                {
                    qDebug()<<"db2 is not open or ReinsertMySQL connect fail";
                }
                if(db1.isOpen())
                    db1.close();
                if(db2.isOpen())
                    db2.close();
                if(db3.isOpen())
                    db3.close();
            }
            QSqlDatabase::removeDatabase("ReinsertCurve"); // correct
            QSqlDatabase::removeDatabase("ReinsertMySQL");
            QSqlDatabase::removeDatabase("ReinsertData");
        }
        else
        {
            WIFIlock.unlock();
        }
        sleep(5);
    }
}

//数据服务器open
//void ReInsertServerDb::datasqlopen()
//{
//    //    qDebug() << "data server open 329";
//    if(QSqlDatabase::contains("ReinsertData")){
//        db3 = QSqlDatabase::database("ReinsertData");
//    }else{
//        db3=QSqlDatabase::addDatabase("QODBC","ReinsertData");
//        db3.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=1;SQL_ATTR_CONNECTION_TIMEOUT=1");
//        db3.setDatabaseName("sqltighten");
//        db3.setPort(1433);
//        db3.setUserName(SqlUserName);
//        db3.setPassword(SqlPassword);
//        query3 = QSqlQuery(db3);
//    }
//    if(!db3.open())
//    {
//        if(!db3.open())
//        {
//            qDebug()<< "reinsert datasql "<<db3.lastError().text();
//        }else
//        {
//            qDebug()<< "reinsert datasql open ok 2";
//        }
//    }else
//    {
//        //qDebug()<< "reinsert datasql open ok 1";
//    }
//    //    qDebug() << "data server open 349";
//}

//曲线服务器open
//void ReInsertServerDb::curvesqlopen()
//{
//    if(QSqlDatabase::contains("ReinsertCurve")){
//        db1 = QSqlDatabase::database("ReinsertCurve");
//    }else{
//        db1=QSqlDatabase::addDatabase("QODBC","ReinsertCurve");
//        db1.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=1;SQL_ATTR_CONNECTION_TIMEOUT=1");
//        db1.setDatabaseName("sqlcurve");
//        db1.setPort(1433);
//        db1.setUserName(SqlUserName);
//        db1.setPassword(SqlPassword);
//        query1 = QSqlQuery(db1);
//    }

//    if(!db1.open())
//    {
//        if(!db1.open())
//        {
//            qDebug()<<"reinsert curvesql "<<db1.lastError().text();
//        }
//        else
//        {
//            qDebug()<< "reinsert curvesql open ok 2";
//        }

//    }else
//    {
//        //qDebug()<< "reinsert curvesql open ok 1";
//    }
//}

//本地mysql open
//void ReInsertServerDb::mysqlopen()
//{
//    if(QSqlDatabase::contains("ReinsertMySQL")){
//        db2 = QSqlDatabase::database("ReinsertMySQL");
//    }else{
//        db2=QSqlDatabase::addDatabase("QMYSQL","ReinsertMySQL");
//        db2.setHostName("localhost");
//        db2.setDatabaseName("Tighten");
//        db2.setUserName("root");
//        db2.setPassword("123456");
//        query2 = QSqlQuery(db2);
//        query4 = QSqlQuery(db2);
//    }

//    if(!db2.open())
//    {
//        if(!db2.open())
//        {
//            qDebug()<< "reinsert localmysql "<< db2.lastError().text();
//        }else
//        {
//            qDebug()<< "reinsert localmysql open ok 2";
//        }
//    }else
//    {
//        //qDebug()<< "reinsert localmysql open ok 1";
//    }
//}

//数据库关闭
//void ReInsertServerDb::sqlclose()
//{
//    if(db1.isOpen())
//        db1.close();
//    if(db2.isOpen())
//        db2.close();
//    if(db3.isOpen())
//        db3.close();
//}


