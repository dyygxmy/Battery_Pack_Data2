#include "sqlthread.h"

  SqlThread::SqlThread(QObject *parent) :
      QObject(parent)
{
      m_thread.start();
      this->moveToThread(&m_thread);
}

void SqlThread::sqlinit()
{
    //QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    db2=QSqlDatabase::addDatabase("QMYSQL","mysqlconnections");
    db2.setHostName("localhost");
    db2.setDatabaseName("Tighten");
    db2.setUserName("root");
    db2.setPassword("123456");
    query2 = QSqlQuery(db2);
    //delete configIniRead;
}
/*
     数据库关闭
*/
void SqlThread::sqlclose()
{
    //     if(db1.isOpen())
    //       db1.close();
    if(db2.isOpen())
        db2.close();
//    if(db3.isOpen())
//        db3.close();
}
//本地mysql open
void SqlThread::mysqlopen()
{
    if(QSqlDatabase::contains("mysqlconnections")){
        db2 = QSqlDatabase::database("mysqlconnections");
    }else{
        db2=QSqlDatabase::addDatabase("QMYSQL","mysqlconnections");
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
            qDebug()<< "SqlThread localmysql "<< db2.lastError().text();
            //**************打不开重启mysql********************
            emit send_mysqlerror();

        }else
        {
            qDebug()<< "SqlThread localmysql ok 2";
        }
    }else
    {
        qDebug()<< "SqlThread localmysql ok 1";
    }
}

void SqlThread::sqlinsert(QString *data_models)
{

    QString *data_model = data_models;
    qDebug() << "here is sqlthread";
    /*******************************
     * 数据模型
    data_model[0] = DATE_blc;
    data_model[1] = TIME_blc;
    data_model[2] = STATE_blc;
    data_model[3] = MI_blc;
    data_model[4] = WI_blc;
    data_model[5] = SCREWID_SQL;
    data_model[6] = VIN_PIN_SQL;
    data_model[7] = Cycle_CSR;
    data_model[8] = CURVE_STR;
    data_model[9] = Program;
    data_model[10] = Channel;
    *******************************/
    //qDebug() << data_model[0] << data_model[1] <<data_model[2] <<data_model[3] <<data_model[4] <<data_model[5] <<data_model[6] ;
    //bool datasql = false;
   // bool curvesql = false;
    int uploadmark = 0;
    QString msg="";
    if(!db2.isOpen()||!QSqlDatabase::contains("mysqlconnections"))
        mysqlopen();
    int screwid_sql = data_model[5].toInt();
    int Cycle = data_model[7].toInt();
    int Program = data_model[9].toInt();
    int Channel = data_model[10].toInt();
    qDebug() << "Program:" << Program;

//    if(curvesql && datasql)
//        uploadmark = 1;//all successful
//    else if(curvesql && !datasql)
//        uploadmark = 2;//data not successful
//    else if(!curvesql && datasql)
//        uploadmark = 3;
//    else if(!curvesql && !datasql)
        uploadmark = 4;

   //**************************本地数据库*******************************************
    query2.exec("SELECT MAX(RecordID) FROM TighteningDatas");
    query2.next();
    int RecordIDMax = query2.value(0).toInt();

    query2.exec("SELECT COUNT(*) FROM TighteningDatas");
    query2.next();
    int numRows = query2.value(0).toInt();
    if (numRows < 10000)
    {
        query2.prepare("INSERT INTO TighteningDatas(RecordID, IDCode, ScrewID, Torque, Angle, Curve, TighteningStatus, TighteningTime, LocalDateTime, Operator, UploadMark, UploadTime, Cycle, Program, Channel)"
                       "VALUES (?, ?, ?, ?, ?, ?, ?, ?, now(), ?, ?, ?, ?, ?, ?)");
    }
    else
    {
        query2.prepare("UPDATE TighteningDatas SET RecordID =?, IDCode =?, ScrewID =?, Torque =?, Angle =?, Curve =?, TighteningStatus =?, TighteningTime =?, LocalDateTime =now(), Operator =?, UploadMark =?, UploadTime =?, Cycle =?, Program =?, Channel =?   WHERE RecordID = (select Min(t.RecordID) from (select RecordID from TighteningDatas)as t)");
    }
    query2.addBindValue(RecordIDMax+1);
    query2.addBindValue(data_model[6]);
    query2.addBindValue(screwid_sql);
    query2.addBindValue(data_model[3]);
    query2.addBindValue(data_model[4]);

    if(data_model[8].size() < 32)
        query2.addBindValue(data_model[8]);
    else
        query2.addBindValue(data_model[8].mid(32));
    query2.addBindValue(data_model[2]);
    query2.addBindValue(data_model[0]+" "+data_model[1]);
    //query2.addBindValue(now());
    query2.addBindValue(Operator);
    query2.addBindValue(uploadmark);
    query2.addBindValue("");
    query2.addBindValue(Cycle);
    query2.addBindValue(Program);
    query2.addBindValue(Channel);

    bool inserttmp = query2.exec();
    int numRowsAffected=query2.numRowsAffected();

    if(!inserttmp)
    {
        qDebug() << query2.lastError().text();
        if(!db2.isOpen())
            mysqlopen();
        if(!query2.exec())
        {
            if(!query2.exec())
            {
                //插入3次不成功存文件
                msg= QString("SqlThread mysql insert false: IDCode:")+data_model[6]+QString("||ScrewID:")+QString::number(screwid_sql)+QString("||Torque:")+data_model[3]+QString("||Angle:")+data_model[4]+QString("||TighteningStatus:")+data_model[2]+QString("||TighteningTime:")+data_model[0]+QString("||")+data_model[1]+QString("||LocalDateTime:")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd")+QString("||Operator:")+Operator+QString("||UploadMark:")+QString::number(uploadmark)+QString("||Cycle:")+data_model[7]+QString("||Program:")+data_model[9]+QString("||Channel:")+data_model[10];
                qDebug()<<msg;
                qDebug()<<"need to repair table TighteningDatas";
                //重启mysql 服务记录 缓存文件
                emit send_mysqlerror();
            }
            else
            {
                msg= QString("SqlThread mysql insert success3: IDCode:")+data_model[6]+QString("||ScrewID:")+QString::number(screwid_sql)+QString("||Torque:")+data_model[3]+QString("||Angle:")+data_model[4]+QString("||TighteningStatus:")+data_model[2]+QString("||TighteningTime:")+data_model[0]+QString("||")+data_model[1]+QString("||LocalDateTime:")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd")+QString("||Operator:")+Operator+QString("||UploadMark:")+QString::number(uploadmark)+QString("||Cycle:")+data_model[7]+QString("||Program:")+data_model[9]+QString("||Channel:")+data_model[10];
                qDebug()<<msg;
            }
        }
        else
        {
           msg= QString("SqlThread mysql insert success2: IDCode:")+data_model[6]+QString("||ScrewID:")+QString::number(screwid_sql)+QString("||Torque:")+data_model[3]+QString("||Angle:")+data_model[4]+QString("||TighteningStatus:")+data_model[2]+QString("||TighteningTime:")+data_model[0]+QString("||")+data_model[1]+QString("||LocalDateTime:")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd")+QString("||Operator:")+Operator+QString("||UploadMark:")+QString::number(uploadmark)+QString("||Cycle:")+data_model[7]+QString("||Program:")+data_model[9]+QString("||Channel:")+data_model[10];
            qDebug()<<msg;
        }
    }
    else if(inserttmp && numRowsAffected ==0)
    {
        msg= QString("SqlThread mysql insert false: IDCode:")+data_model[6]+QString("||ScrewID:")+QString::number(screwid_sql)+QString("||Torque:")+data_model[3]+QString("||Angle:")+data_model[4]+QString("||TighteningStatus:")+data_model[2]+QString("||TighteningTime:")+data_model[0]+QString("||")+data_model[1]+QString("||LocalDateTime:")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd")+QString("||Operator:")+Operator+QString("||UploadMark:")+QString::number(uploadmark)+QString("||Cycle:")+data_model[7]+QString("||Program:")+data_model[9]+QString("||Channel:")+data_model[10];
        qDebug()<<msg;
        qDebug()<<"need to optimize table TighteningDatas";
        emit send_mysqlerror();

    }
    else if(inserttmp && numRowsAffected >0)
    {
        msg= QString("SqlThread mysql insert success1: IDCode:")+data_model[6]+QString("||ScrewID:")+QString::number(screwid_sql)+QString("||Torque:")+data_model[3]+QString("||Angle:")+data_model[4]+QString("||TighteningStatus:")+data_model[2]+QString("||TighteningTime:")+data_model[0]+QString("||")+data_model[1]+QString("||LocalDateTime:")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd")+QString("||Operator:")+Operator+QString("||UploadMark:")+QString::number(uploadmark)+QString("||Cycle:")+data_model[7]+QString("||Program:")+data_model[9]+QString("||Channel:")+data_model[10];
        qDebug()<<msg;

    }
    sqlclose();

}

void SqlThread::receiveConfigureAll(int howmuchProNnum,int whichar,int whichpronumis,int whichoption)
{
    QString screwid ="";
    QString protmp = "";
    QString vin_pin_sql = VIN_PIN_SQL;
    QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
    QString data_model[11];

    for( int k = whichpronumis;k <21;k++)
    {

        if(!howmuchProNnum)  //非选配
        {
            //qDebug() << "come all 2";
            lsNum = config->value(QString("carinfo").append(QString::number(whichar)).append("/LSNumber").append(QString::number(k))).toString();
            protmp = config->value(QString("carinfo").append(QString::number(whichar)).append("/ProNum").append(QString::number(k))).toString();
            screwid = config->value(QString("carinfo").append(QString::number(whichar)).append("/LuoSuanNum").append(QString::number(k))).toString();

            if(enablenumberLeft)
            {
                qDebug() << enablenumberLeft << "enablenum";
                lsNum = QString::number(enablenumberLeft);
            }

        }
        else
        {
            lsNum = config->value(QString("carinfo").append(QString::number(whichar)).append("/OPLSNumber").append(QString::number(whichoption)).append(QString::number(k))).toString();
            protmp = config->value(QString("carinfo").append(QString::number(whichar)).append("/OPProNum").append(QString::number(whichoption)).append(QString::number(k))).toString();
            screwid = config->value(QString("carinfo").append(QString::number(whichar)).append("/OPLuoSuanNum").append(QString::number(whichoption)).append(QString::number(k))).toString();

        }
        if(lsNum.toInt())
        {
            QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
            for(int i = 0;i<lsNum.toInt();i++)
            {
                enablenumberLeft = 0;
                //Curve_Is_Null = true;
                data_model[0] = time.toString("yyyy-MM-dd");
                data_model[1] = time.toString("hh:mm:ss");
                data_model[2] = "NOK";
                data_model[3] = "-1";
                data_model[4] = "-1";
                data_model[5] = screwid;
                data_model[6] = vin_pin_sql;
                data_model[7] = "-1";
                data_model[8] = "Curve is null";
                data_model[9] = protmp;
                data_model[10] = "-1";
                sqlinsert(data_model);
            }

        }
        else
        {
            continue;
        }
    }
    delete config;

}

void SqlThread::configOne(QString screwid, QString vin_pin_sql, QString protmp)
{
    QString data_model[11];
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    data_model[0] = time.toString("yyyy-MM-dd");
    data_model[1] = time.toString("hh:mm:ss");
    data_model[2] = "NOK";
    data_model[3] = "-1";
    data_model[4] = "-1";
    data_model[5] = screwid;
    data_model[6] = vin_pin_sql;
    data_model[7] = "-1";
    data_model[8] = "Curve is null";
    data_model[9] = protmp;
    data_model[10] = "-1";
    sqlinsert(data_model);
}


