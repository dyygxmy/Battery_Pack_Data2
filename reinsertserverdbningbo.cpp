#include "reinsertserverdbningbo.h"
#include "GlobalVarible.h"
ReInsertServerDbNingBo::ReInsertServerDbNingBo(QObject *parent) :
    QObject(parent)
{
    m_thread.start();
    this->moveToThread(&m_thread);
    //connect(&m_thread,SIGNAL(finished()),this,SLOT(printDead()));
}


void ReInsertServerDbNingBo::reinsert_start()
{
    qDebug()<<"ReInsertServerDbNingBo thread start";

    db1=QSqlDatabase::addDatabase("QODBC","ReinsertCurve");
    db1.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=1;SQL_ATTR_CONNECTION_TIMEOUT=1");
    db1.setDatabaseName("sqlcurve");
    db1.setPort(1433);
    db1.setUserName(SqlUserName);
    db1.setPassword(SqlPassword);
    query1 = QSqlQuery(db1);

    db2=QSqlDatabase::addDatabase("QMYSQL","ReinsertMySQL");
    db2.setHostName("localhost");
    db2.setDatabaseName("Tighten");
    db2.setUserName("root");
    db2.setPassword("123456");
    query2 = QSqlQuery(db2);
    query4 = QSqlQuery(db2);

    db3=QSqlDatabase::addDatabase("QODBC","ReinsertData");
    db3.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=1;SQL_ATTR_CONNECTION_TIMEOUT=1");
    db3.setDatabaseName("sqltighten");
    db3.setPort(1433);
    db3.setUserName(SqlUserName);
    db3.setPassword(SqlPassword);
    query3 = QSqlQuery(db3);

    timer_insert();
    //    if(!db1.isOpen())
    //        curvesqlopen();
    //    if(!db2.isOpen())
    //        mysqlopen();
    //    if(!db3.isOpen())
    //        datasqlopen();

    //connect(&m_insert,SIGNAL(timeout()),this,SLOT(timer_insert()));
    //m_insert.start(5000);
}

void ReInsertServerDbNingBo::timer_insert()
{
    //5秒 遍历本地数据库标志位 处理
    while(1)
    {
        qDebug()<< "I am alive "<< WIFI_STATE;
        if(WIFI_STATE)
        {
            if(!db1.isOpen()||!QSqlDatabase::contains("ReinsertCurve"))
                curvesqlopen();
            if(!db2.isOpen()||!QSqlDatabase::contains("ReinsertMySQL"))
                mysqlopen();
            if(!db3.isOpen()||!QSqlDatabase::contains("ReinsertData"))
                datasqlopen();
            bool datasql = false;
            bool curvesql = false;
            int uploadmark = 0;
            QString msg="";
            if(db2.isOpen() && QSqlDatabase::contains("ReinsertMySQL"))
            {
                query2.exec("SELECT RecordID, IDCode, ScrewID, Torque, Angle, Curve, TighteningStatus, TighteningTime,UploadMark,Cycle FROM TighteningDatas Where UploadMark =2 or UploadMark =4 order by RecordID limit 10");
                while(query2.next())
                {
                    datasql = false;
                    curvesql = false;
                    //                QByteArray VIN_PIN_byte = query2.value(1).toString().toAscii();
                    //                for (int i = 0; i < VIN_PIN_byte.length();i += 2)
                    //                {
                    //                    VIN_PIN_byte[i] = ~VIN_PIN_byte[i];       //偶数位取反
                    //                }
                    //                QString vin_pin_sql;
                    //                vin_pin_sql = QString(VIN_PIN_byte.toBase64());

                    QString OKFlag = query2.value(6).toString();
                    int OK = -1;
                    if(OKFlag == "OK")
                        OK = 1;
                    else if(OKFlag == "NOK")
                        OK = 0;

                    if(query2.value(8).toInt()==2)
                    {
                        if(db3.isOpen() && QSqlDatabase::contains("ReinsertData"))
                        {
                            qDebug()<<"*****************come in 2*************************";
                            if(query2.value(1).toString().length()==17)
                                query3.prepare("INSERT INTO dbo.ScrewData(VIN, dataid, torque, angle, ok, gettingtime,logintime)"
                                               "VALUES (?, ?, ?, ?, ?, ?, convert(varchar(20),getdate(),120))");
                            else if(query2.value(1).toString().length()==14)
                                query3.prepare("INSERT INTO dbo.ScrewData(PIN, dataid, torque, angle, ok, gettingtime,logintime)"
                                               "VALUES (?, ?, ?, ?, ?, ?, convert(varchar(20),getdate(),120))");
                            query3.addBindValue(query2.value(1).toString());
                            query3.addBindValue(query2.value(2).toInt());
                            query3.addBindValue(query2.value(3).toString());
                            query3.addBindValue(query2.value(4).toString());
                            query3.addBindValue(OK);
                            query3.addBindValue(query2.value(7).toString());
                            //                        query3.addBindValue("convert(varchar(10),getdate(),120)");
                            //                        query3.addBindValue(query2.value(9).toInt());
                            //                        query3.addBindValue(query2.value(10).toString());
                            //                        query3.addBindValue(query2.value(11).toInt());
                            //                        query3.addBindValue(query2.value(12).toInt());
                            //                        query3.addBindValue(Line_ID);
                            if(query3.exec())
                            {
                                msg= QString("ReInsertServerDbNingBo datasql insert success: Cycle:")+query2.value(9).toString();
                                qDebug()<<msg;
                                if(query4.exec("UPDATE TighteningDatas SET UploadMark = 1,UploadTime = now() WHERE RecordID = "+query2.value(0).toString()))
                                {
                                    msg= QString("ReInsertServerDbNingBo datasql update success: Cycle:")+query2.value(9).toString()+QString("||UploadMark:1");
                                    qDebug()<<msg;
                                }
                                else
                                {
                                    msg= QString("ReInsertServerDbNingBo datasql update fail: Cycle:")+query2.value(9).toString()+QString("||UploadMark:2");
                                    qDebug()<<msg;
                                }
                            }
                            else
                            {
                                msg= QString("ReInsertServerDbNingBo datasql insert fail: Cycle:")+query2.value(9).toString();
                                qDebug()<<msg;
                            }
                        }
                    }
                    else if(query2.value(8).toInt()==4)
                    {
                        if(db3.isOpen()&& QSqlDatabase::contains("ReinsertData"))
                        {
                            qDebug()<<"*****************come in 4*************************";
                            if(query2.value(1).toString().length()==17)
                                query3.prepare("INSERT INTO dbo.ScrewData(VIN, dataid, torque, angle, ok, gettingtime,logintime)"
                                               "VALUES (?, ?, ?, ?, ?, ?, convert(varchar(20),getdate(),120))");
                            else if(query2.value(1).toString().length()==14)
                                query3.prepare("INSERT INTO dbo.ScrewData(PIN, dataid, torque, angle, ok, gettingtime,logintime)"
                                               "VALUES (?, ?, ?, ?, ?, ?, convert(varchar(20),getdate(),120))");
                            query3.addBindValue(query2.value(1).toString());
                            query3.addBindValue(query2.value(2).toInt());
                            query3.addBindValue(query2.value(3).toString());
                            query3.addBindValue(query2.value(4).toString());
                            query3.addBindValue(OK);
                            query3.addBindValue(query2.value(7).toString());
                            //                        query3.addBindValue("convert(varchar(10),getdate(),120)");
                            //                        query3.addBindValue(query2.value(9).toInt());
                            //                        query3.addBindValue(query2.value(10).toString());
                            //                        query3.addBindValue(query2.value(11).toInt());
                            //                        query3.addBindValue(query2.value(12).toInt());
                            //                        query3.addBindValue(Line_ID);
                            if(!query3.exec())
                            {
                                datasql = false;
                                msg= QString("ReInsertServerDbNingBo datasql insert fail: Cycle:")+query2.value(9).toString();
                                qDebug()<<msg;
                            }
                            else
                            {
                                msg= QString("ReInsertServerDbNingBo datasql insert success: Cycle:")+query2.value(9).toString();
                                qDebug()<<msg;
                                datasql = true;
                            }
                        }
                        if(db1.isOpen()&& QSqlDatabase::contains("ReinsertCurve"))
                        {
                            bool ok_curve = false;
                            bool ok_data = false;
                            if(db1.transaction())
                            {
                                query1.prepare("INSERT INTO dbo.Curves(Curve) VALUES (?)");
                                query1.addBindValue(query2.value(5).toString());
                                ok_curve = query1.exec();
                                if(!ok_curve)
                                {
                                    msg= QString("ReInsertServerDbNingBo curvesql Curves insert fail: Cycle:")+query2.value(9).toString();
                                    qDebug()<<msg;
                                    curvesql = false;
                                }
                                else
                                {
                                    msg= QString("ReInsertServerDbNingBo curvesql Curves insert success: Cycle:")+query2.value(9).toString();
                                    qDebug()<<msg;
                                    curvesql = true;
                                }

                                query1.prepare("INSERT INTO dbo.TighteningData(VIN, ScrewID, Torque, Angle, CurveID, OKFlag, GetTime, IpAddress)"
                                               "VALUES (?, ?, ?, ?, @@IDENTITY, ?, ?, ?)");
                                query1.addBindValue(query2.value(1).toString());
                                query1.addBindValue(query2.value(2).toString());
                                query1.addBindValue(query2.value(3).toString());
                                query1.addBindValue(query2.value(4).toString());
                                //query1.addBindValue("@@IDENTITY");
                                query1.addBindValue(OK);
                                query1.addBindValue(query2.value(7).toString());
                                query1.addBindValue("192.168.3.1");
                                //                        query1.addBindValue("convert(varchar(10),getdate(),120)");
                                //                        query1.addBindValue(query2.value(9).toInt());
                                //                        query1.addBindValue(query2.value(10).toString());
                                //                        query1.addBindValue(query2.value(11).toInt());
                                //                        query1.addBindValue(query2.value(12).toInt());
                                //                        query1.addBindValue(Line_ID);

                                ok_data  = query1.exec();
                                if(!ok_data)
                                {
                                    curvesql = false;
                                    msg= QString("ReInsertServerDbNingBo curvesql TighteningDatas insert fail: Cycle:")+query2.value(9).toString();
                                    qDebug()<<msg;
                                }
                                else
                                {
                                    msg= QString("ReInsertServerDbNingBo curvesql TighteningDatas insert success: Cycle:")+query2.value(9).toString();
                                    qDebug()<<msg;
                                    //curvesql = true;
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
                                            qDebug() <<"rollback fail:"<< db1.lastError(); // 回滚
                                    }
                                }
                                else
                                {
                                    if (!db1.rollback())
                                        qDebug() <<"rollback fail:"<< db1.lastError(); // 回滚
                                    curvesql = false;
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
                                    msg= QString("ReInsertServerDbNingBo update success: Cycle:")+query2.value(9).toString()+QString("||UploadMark:")+QString::number(uploadmark);
                                    qDebug()<<msg;
                                }
                                else
                                {
                                    msg= QString("ReInsertServerDbNingBo update fail: Cycle:")+query2.value(9).toString()+QString("||UploadMark:")+QString::number(uploadmark);
                                    qDebug()<<msg;
                                }
                            }
                        }
                    }
                }
                query2.exec("SELECT RecordID, IDCode, ScrewID, Torque, Angle, Curve, TighteningStatus, TighteningTime,UploadMark,Cycle FROM TighteningDatas Where UploadMark =3 order by RecordID limit 5");
                while(query2.next())
                {
                    //              datasql = false;
                    curvesql = false;

                    //                QByteArray VIN_PIN_byte = query2.value(1).toString().toAscii();
                    //                for (int i = 0; i < VIN_PIN_byte.length();i += 2)
                    //                {
                    //                    VIN_PIN_byte[i] = ~VIN_PIN_byte[i];       //偶数位取反
                    //                }
                    //                QString vin_pin_sql;
                    //                vin_pin_sql = QString(VIN_PIN_byte.toBase64());

                    QString OKFlag = query2.value(6).toString();
                    int OK = -1;
                    if(OKFlag == "OK")
                        OK = 1;
                    else if(OKFlag == "NOK")
                        OK = 0;


                    if(db1.isOpen()&& QSqlDatabase::contains("ReinsertCurve"))
                    {
                        bool ok_curve = false;
                        bool ok_data = false;
                        qDebug()<<"*****************come in 3*************************";
                        if(db1.transaction())
                        {
                            query1.prepare("INSERT INTO dbo.Curves(Curve) VALUES (?)");
                            query1.addBindValue(query2.value(5).toString());
                            ok_curve = query1.exec();
                            if(!ok_curve)
                            {
                                curvesql = false;
                                msg= QString("ReInsertServerDbNingBo curvesql Curves insert fail: Cycle:")+query2.value(9).toString();
                                qDebug()<<msg;
                            }
                            else
                            {
                                msg= QString("ReInsertServerDbNingBo curvesql Curves insert success: Cycle:")+query2.value(9).toString();
                                qDebug()<<msg;
                                curvesql = true;

                            }

                            query1.prepare("INSERT INTO dbo.TighteningData(VIN, ScrewID, Torque, Angle, CurveID, OKFlag, GetTime, IpAddress)"
                                           "VALUES (?, ?, ?, ?, @@IDENTITY, ?, ?, ?)");
                            query1.addBindValue(query2.value(1).toString());
                            query1.addBindValue(query2.value(2).toString());
                            query1.addBindValue(query2.value(3).toString());
                            query1.addBindValue(query2.value(4).toString());
                            //query1.addBindValue("@@IDENTITY");
                            query1.addBindValue(OK);
                            query1.addBindValue(query2.value(7).toString());
                            query1.addBindValue("192.168.3.1");
                            //                        query1.addBindValue("convert(varchar(10),getdate(),120)");
                            //                        query1.addBindValue(query2.value(9).toInt());
                            //                        query1.addBindValue(query2.value(10).toString());
                            //                        query1.addBindValue(query2.value(11).toInt());
                            //                        query1.addBindValue(query2.value(12).toInt());
                            //                        query1.addBindValue(Line_ID);

                            ok_data  = query1.exec();
                            if(!ok_data)
                            {
                                curvesql = false;
                                msg= QString("ReInsertServerDbNingBo curvesql TighteningDatas insert fail: Cycle:")+query2.value(9).toString();
                                qDebug()<<msg;
                            }
                            else
                            {
                                msg= QString("ReInsertServerDbNingBo curvesql TighteningDatas insert success: Cycle:")+query2.value(9).toString();
                                qDebug()<<msg;
                                //curvesql = true;

                            }
                            if(ok_curve && ok_data)
                            {
                                if(db1.commit())
                                    curvesql = true;
                                else
                                {
                                    curvesql = false;
                                    qDebug()<<"commit fail: Cycle:"+query2.value(9).toString();
                                }
                            }
                            else
                            {
                                db1.rollback();
                                curvesql = false;
                            }
                        }

                        if(curvesql)
                        {
                            if(query4.exec("UPDATE TighteningDatas SET UploadMark = 1,UploadTime = now() WHERE RecordID = "+query2.value(0).toString()))
                            {
                                msg= QString("ReInsertServerDbNingBo curvesql update success: Cycle:")+query2.value(9).toString()+QString("||UploadMark:1");
                                qDebug()<<msg;
                            }
                            else
                            {
                                msg= QString("ReInsertServerDbNingBo curvesql update fail: Cycle:")+query2.value(9).toString()+QString("||UploadMark:3");
                                qDebug()<<msg;
                            }
                        }
                    }
                }
            }
            sqlclose();
        }
        sleep(5);
    }
}

//数据服务器open
void ReInsertServerDbNingBo::datasqlopen()
{
    if(QSqlDatabase::contains("ReinsertData")){
        db3 = QSqlDatabase::database("ReinsertData");
    }else{
        db3=QSqlDatabase::addDatabase("QODBC","ReinsertData");
        db3.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=1;SQL_ATTR_CONNECTION_TIMEOUT=1");
        db3.setDatabaseName("sqltighten");
        db3.setPort(1433);
        db3.setUserName(SqlUserName);
        db3.setPassword(SqlPassword);
        query3 = QSqlQuery(db3);
    }
    if(!db3.open())
    {
        if(!db3.open())
        {
            // qDebug()<< "reinsert datasql "<<db3.lastError().text();
            qDebug()<< "reinsert datasql open fail 329";
        }else
        {
            qDebug()<< "reinsert datasql open ok 2";
        }
    }else
    {
        qDebug()<< "reinsert datasql open ok 1";
    }
}

//曲线服务器open
void ReInsertServerDbNingBo::curvesqlopen()
{
    if(QSqlDatabase::contains("ReinsertCurve")){
        db1 = QSqlDatabase::database("ReinsertCurve");
    }else{
        db1=QSqlDatabase::addDatabase("QODBC","ReinsertCurve");
        db1.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=1;SQL_ATTR_CONNECTION_TIMEOUT=1");
        db1.setDatabaseName("sqlcurve");
        db1.setPort(1433);
        db1.setUserName(SqlUserName);
        db1.setPassword(SqlPassword);
        query1 = QSqlQuery(db1);
    }

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

//本地mysql open
void ReInsertServerDbNingBo::mysqlopen()
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
        query4 = QSqlQuery(db2);
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
        //qDebug()<< "reinsert localmysql open ok 1";
    }
}

//数据库关闭
void ReInsertServerDbNingBo::sqlclose()
{
    if(db1.isOpen())
        db1.close();
    if(db2.isOpen())
        db2.close();
    if(db3.isOpen())
        db3.close();
}


