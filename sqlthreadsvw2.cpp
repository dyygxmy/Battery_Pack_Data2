#include "sqlthreadsvw2.h"


SqlThreadSVW2::SqlThreadSVW2(QObject *parent) :
    QObject(parent)
{
    m_thread.start();
    this->moveToThread(&m_thread);
}

void SqlThreadSVW2::sqlinit()
{
    //QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    db2=QSqlDatabase::addDatabase("QMYSQL","mysqlconnections");
    db2.setHostName("localhost");
    db2.setDatabaseName("Tighten");
    db2.setUserName("root");
    db2.setPassword("123456");
    query2 = QSqlQuery(db2);
    //delete configIniRead;
    isFirst = true;

    FirstRevGPackNum();
//    if(ControlType_1 == "OFF")
//    {
//        DATA_STRUCT demo;
//        demo.data_model[0] = "";
//        demo.data_model[1] = "";
//        demo.data_model[2] = "";
//        demo.data_model[3] = "1";
//        demo.data_model[4] = "2";
//        demo.data_model[5] = "12345";
//        demo.data_model[6] = "lsn";
//        demo.data_model[7] = "-1";
//        demo.data_model[8] = "Curve is null";;
//        demo.data_model[9] = "1";
//        demo.data_model[10] = "";
//        demo.data_model[11] = "0";
//        demo.data_model[12] = "0";
//        demo.data_model[13] = "0";
//        demo.data_model[14] = "0";
//        demo.data_model[15] = "0";
//        QVariant DataVar;
//        DataVar.setValue(demo);
//        sqlinsert(DataVar);
//    }
}
/*
     数据库关闭
*/
void SqlThreadSVW2::sqlclose()
{
    //     if(db1.isOpen())
    //       db1.close();
    if(db2.isOpen())
        db2.close();
    //    if(db3.isOpen())
    //        db3.close();
}
//本地mysql open
void SqlThreadSVW2::mysqlopen()
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
            qDebug()<< "SqlThreadSVW2 localmysql "<< db2.lastError().text();
            //**************打不开重启mysql********************
            emit send_mysqlerror();

        }else
        {
            qDebug()<< "SqlThreadSVW2 localmysql ok 2";
        }
    }else
    {
        qDebug()<< "SqlThreadSVW2 localmysql ok 1";
    }
}

//接收存储拧紧结果或泄漏结果
void SqlThreadSVW2::sqlinsert(QVariant DataVar)
{
    //QString *data_model = data_models;
    DATA_STRUCT dataStruct;
    dataStruct = DataVar.value<DATA_STRUCT>();
    QString data_model[25];
    for(int i = 0; i<21; i++)
    {
        data_model[i] = dataStruct.data_model[i];
    }
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
    data_model[11] = Type;
    data_model[12] = Order;
    data_model[13] = MaxValue;
    data_model[14] = MinValue;
    data_model[15] = JobStatus;
    data_model[16] = WI_blc_max;//最大角度
    data_model[17] = WI_blc_min;//最小角度
    data_model[18] = MI_blc_max;//最大扭矩
    data_model[19] = MI_blc_min;//最小扭矩
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
    int Order = data_model[12].toInt();
    int JobStatus = data_model[15].toInt();

    
    if(Order == BoltTotalNum && data_model[2] == "OK")
    {
        if(NOKflag)
            JobStatus = 2;
        else
            JobStatus = 1;
    }
    double MaxValue = data_model[13].toDouble();
    double MinValue = data_model[14].toDouble();

    if(screwid_sql==0)
    {
        msg= QString("Screwid=0 discard the data : IDCode:")+data_model[6]+QString("||ScrewID:")+QString::number(screwid_sql)+QString("||Torque:")+data_model[3]+
                QString("||Angle:")+data_model[4]+QString("||TighteningStatus:")+data_model[2]+QString("||TighteningTime:")+data_model[0]+QString("||")+data_model[1]+QString
                ("||LocalDateTime:")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd")+QString("||Operator:")+Operator+QString("||UploadMark:")+QString::
                number(uploadmark)+QString("||Cycle:")+data_model[7]+QString("||Program:")+data_model[9]+QString("||Channel:")+data_model[10];
        qDebug()<<msg;
        return;
    }

    qDebug() << "Program:" << Program;

    //    if(curvesql && datasql)
    //        uploadmark = 1;//all successful
    //    else if(curvesql && !datasql)
    //        uploadmark = 2;//data not successful
    //    else if(!curvesql && datasql)
    //        uploadmark = 3;
    //    else if(!curvesql && !datasql)
    if(Program==99 || screwid_sql==100000000 || screwid_sql==200000000 || screwid_sql==300000000 || screwid_sql==400000000)
        uploadmark = 1;
    else
    {
            uploadmark = 4;
    }

    //**************************本地数据库*******************************************
    if(db2.isOpen() && QSqlDatabase::contains("mysqlconnections"))
    {
        if(isFirst)
        {
            if(!query2.exec("CREATE TABLE IF NOT EXISTS "+Localtable+" (RecordID int not null primary key auto_increment, IDCode varchar(128) not null,ScrewID varchar(28) null,Torque real not null,Angle real not null,Curve mediumtext not null,TighteningStatus char(3) not null,TighteningTime datetime not null,LocalDateTime datetime not null,Operator char(15) null,UploadMark tinyint not null,UploadTime datetime null,Cycle int null,Program int null, Channel int null, Type char(20) not null, Order_id int null, MaxValue float null, MinValue float null, JobStatus int null)"))
            {
                qDebug()<<"create table "+Localtable+" fail"<<query2.lastError();
                emit send_mysqlerror();
            }
            else
                isFirst = false;
        }

        query2.exec("SELECT MAX(RecordID) FROM "+Localtable);
        query2.next();
        int RecordIDMax = query2.value(0).toInt();

        query2.exec("SELECT COUNT(*) FROM "+Localtable);
        query2.next();
        int numRows = query2.value(0).toInt();
        if (numRows < 1000)
        {
            query2.prepare("INSERT INTO "+Localtable+"(RecordID, IDCode, ScrewID, Torque, Angle, Curve, TighteningStatus, TighteningTime, LocalDateTime, Operator, UploadMark, UploadTime, Cycle, Program, Channel, Type, Order_id, MaxValue, MinValue, JobStatus ,Angle_Max ,Angle_Min,Torque_Max,Torque_Min)"
                           "VALUES (?, ?, ?, ?, ?, ?, ?, ?, now(), ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,?,?,?,?)");
        }
        else
        {
            query2.prepare("UPDATE "+Localtable+" SET RecordID =?, IDCode =?, ScrewID =?, Torque =?, Angle =?, Curve =?, TighteningStatus =?, TighteningTime =?, LocalDateTime =now(), Operator =?, UploadMark =?, UploadTime =?, Cycle =?, Program =?, Channel =?, Type =?, Order_id =?, MaxValue =?, MinValue =?, JobStatus =? , Angle_Max = ?,Angle_Min =? ,Torque_Max = ?,Torque_Min = ? WHERE RecordID = (select Min(t.RecordID) from (select RecordID from "+Localtable+")as t)");
        }
        query2.addBindValue(RecordIDMax+1);//RecordID
        query2.addBindValue(data_model[6]);//IDCode
        query2.addBindValue(screwid_sql);//ScrewID
        query2.addBindValue(data_model[3]);//Torque
        query2.addBindValue(data_model[4]);//Angle
        query2.addBindValue(data_model[8]);//Curve
        query2.addBindValue(data_model[2]);//TighteningStatus
        if(data_model[0]!="" && data_model[1]!="")//TighteningTime+LocalDateTime
            query2.addBindValue(data_model[0]+" "+data_model[1]);
        else
            query2.addBindValue(QDateTime::currentDateTime());
        //query2.addBindValue(now());
        query2.addBindValue(Operator);//Operator
        query2.addBindValue(uploadmark);//UploadMark
        query2.addBindValue("");//UploadTime
        query2.addBindValue(Cycle);//Cycle
        query2.addBindValue(Program);//Program
        query2.addBindValue(Channel);//Channel
        query2.addBindValue(data_model[11]);//Type
        query2.addBindValue(Order);//Order_id
        query2.addBindValue(MaxValue);//MaxValue
        query2.addBindValue(MinValue);//MinValue
        query2.addBindValue(JobStatus);//JobStatus

        query2.addBindValue(data_model[16]);//Angle_Max
        query2.addBindValue(data_model[17]);//Angle_Min
        query2.addBindValue(data_model[18]);//Torque_Max
        query2.addBindValue(data_model[19]);//Torque_Min

        bool inserttmp = query2.exec();
        int numRowsAffected=query2.numRowsAffected();

        if(!inserttmp)
        {
            qDebug()<<"insert error" << query2.lastError();
            if(!db2.isOpen())
                mysqlopen();
            if(!query2.exec())
            {
                if(!query2.exec())
                {
                    //插入3次不成功存文件
                    msg= QString("SqlThreadSVW2 mysql insert false: IDCode:")+data_model[6]+QString("||ScrewID:")+QString::number(screwid_sql)+QString("||Torque:")+data_model[3]+QString("||Angle:")+data_model[4]+QString("||TighteningStatus:")+data_model[2]+QString("||TighteningTime:")+data_model[0]+QString("||")+data_model[1]+QString("||LocalDateTime:")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd")+QString("||Operator:")+Operator+QString("||UploadMark:")+QString::number(uploadmark)+QString("||Cycle:")+data_model[7]+QString("||Program:")+data_model[9]+QString("||Channel:")+data_model[10];
                    qDebug()<<msg;
                    qDebug()<<"need to repair table "+Localtable;
                    //重启mysql 服务记录 缓存文件
                    emit send_mysqlerror();
                }
                else
                {
                    msg= QString("SqlThreadSVW2 mysql insert success3: IDCode:")+data_model[6]+QString("||ScrewID:")+QString::number(screwid_sql)+QString("||Torque:")+data_model[3]+QString("||Angle:")+data_model[4]+QString("||TighteningStatus:")+data_model[2]+QString("||TighteningTime:")+data_model[0]+QString("||")+data_model[1]+QString("||LocalDateTime:")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd")+QString("||Operator:")+Operator+QString("||UploadMark:")+QString::number(uploadmark)+QString("||Cycle:")+data_model[7]+QString("||Program:")+data_model[9]+QString("||Channel:")+data_model[10];
                    qDebug()<<msg;
                }
            }
            else
            {
                msg= QString("SqlThreadSVW2 mysql insert success2: IDCode:")+data_model[6]+QString("||ScrewID:")+QString::number(screwid_sql)+QString("||Torque:")+data_model[3]+QString("||Angle:")+data_model[4]+QString("||TighteningStatus:")+data_model[2]+QString("||TighteningTime:")+data_model[0]+QString("||")+data_model[1]+QString("||LocalDateTime:")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd")+QString("||Operator:")+Operator+QString("||UploadMark:")+QString::number(uploadmark)+QString("||Cycle:")+data_model[7]+QString("||Program:")+data_model[9]+QString("||Channel:")+data_model[10];
                qDebug()<<msg;
            }
        }
        else if(inserttmp && numRowsAffected ==0)
        {
            msg= QString("SqlThreadSVW2 mysql insert false: IDCode:")+data_model[6]+QString("||ScrewID:")+QString::number(screwid_sql)+QString("||Torque:")+data_model[3]+QString("||Angle:")+data_model[4]+QString("||TighteningStatus:")+data_model[2]+QString("||TighteningTime:")+data_model[0]+QString("||")+data_model[1]+QString("||LocalDateTime:")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd")+QString("||Operator:")+Operator+QString("||UploadMark:")+QString::number(uploadmark)+QString("||Cycle:")+data_model[7]+QString("||Program:")+data_model[9]+QString("||Channel:")+data_model[10];
            qDebug()<<msg;
            qDebug()<<"need to optimize table "+Localtable;
            emit send_mysqlerror();

        }
        else if(inserttmp && numRowsAffected >0)
        {
            msg= QString("SqlThreadSVW2 mysql insert success1: IDCode:")+data_model[6]+QString("||ScrewID:")+QString::number(screwid_sql)+QString("||Torque:")+data_model[3]+QString("||Angle:")+data_model[4]+QString("||TighteningStatus:")+data_model[2]+QString("||TighteningTime:")+data_model[0]+QString("||")+data_model[1]+QString("||LocalDateTime:")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd")+QString("||Operator:")+Operator+QString("||UploadMark:")+QString::number(uploadmark)+QString("||Cycle:")+data_model[7]+QString("||Program:")+data_model[9]+QString("||Channel:")+data_model[10];
            qDebug()<<msg;
//            qDebug()<<"??????????????????????"<<data_model[8];
        }
        sqlclose();
    }
    else
    {
        qDebug()<<"db2 not open or unconnected";
    }
}

/***********************************************/
//multi channel NOK all
/***********************************************/
void SqlThreadSVW2::receiveNokAll(int ch)
{
    Q_UNUSED(ch);
    //    QString data_model[16];
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间

//    for(int i=0;i<ch;i++)
//    {
        for(int j=0;j<20;j++)
        {
            for(int k=0;k<carInfor.boltNum[j];k++)
            {
                DATA_STRUCT demo;
                demo.data_model[0] = time.addSecs(k*(j+1)).toString("yyyy-MM-dd");
                demo.data_model[1] = time.addSecs(k*(j+1)).toString("hh:mm:ss");
                demo.data_model[2] = "NOK";
                demo.data_model[3] = "-1";
                demo.data_model[4] = "-1";
                demo.data_model[5] = carInfor.boltSN[j];
                demo.data_model[6] = VIN_PIN_SQL;
                demo.data_model[7] = "-1";
                demo.data_model[8] = "Curve is null";
                demo.data_model[9] = carInfor.proNo[j];
                demo.data_model[10] = "0";

                demo.data_model[11] = Type;
                demo.data_model[12] = QString::number(BoltOrder[0]++);
                demo.data_model[13] = "0";
                demo.data_model[14] = "0";
                demo.data_model[15] = "0";

                if((BoltOrder[0]-1) == BoltTotalNum)
                    demo.data_model[15] = "2";
                QVariant DataVar;
                DataVar.setValue(demo);
                sqlinsert(DataVar);
            }
        }
//    }
}

void SqlThreadSVW2::configOne(QString screwid, QString vin_pin_sql, QString protmp,int Channel)
{
    Q_UNUSED(Channel);
    //    QString data_model[11];
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    DATA_STRUCT demo;
    demo.data_model[0] = time.toString("yyyy-MM-dd");
    demo.data_model[1] = time.toString("hh:mm:ss");
    demo.data_model[2] = "NOK";
    demo.data_model[3] = "-1";
    demo.data_model[4] = "-1";
    demo.data_model[5] = screwid;
    demo.data_model[6] = vin_pin_sql;
    demo.data_model[7] = "-1";
    demo.data_model[8] = "Curve is null";
    demo.data_model[9] = protmp;
    demo.data_model[10] = "0";

    demo.data_model[11] = Type;
    demo.data_model[12] = QString::number(BoltOrder[0]++);
    demo.data_model[13] = "0";
    demo.data_model[14] = "0";
    demo.data_model[15] = "0";

    if((BoltOrder[0]-1) == BoltTotalNum)
        demo.data_model[15] = "2";
    QVariant DataVar;
    DataVar.setValue(demo);
    sqlinsert(DataVar);
}

void SqlThreadSVW2::insertKeyCodeInfo(int LineID,int OrderSN,QString PartsCode,QString KeyCode,QString PackSN,QString StationID)
{
    QString data_model[13];
//    DATA_STRUCT dataStruct;
//    dataStruct = DataVar.value<DATA_STRUCT>();
//    for(int i = 0; i<8; i++)
//    {
//        data_model[i] = dataStruct.data_model[i];
//    }
    /*******************************
     * 数据模型
     * table:KeyCodeInfo
    data_model[0] = ID;int(main)
    data_model[1] = LineID;int
    data_model[2] = Flag;int
    data_model[3] = PartsCode;nvarchar(100)
    data_model[4] = KeyCode;varchar(200)
    data_model[5] = PackSN;varchar(50)
    data_model[6] = StationID;varchar(50)
    data_model[7] = UpdateTime;datetime
    data_model[8] = CLocalTime;datetime
    data_model[9] = OrderSN;
    *******************************/
    if(!db2.isOpen()||!QSqlDatabase::contains("mysqlconnections")){
        mysqlopen();
    }
    int Flag = 0;
    data_model[3] = PartsCode;
    data_model[4] = KeyCode;
    data_model[5] = PackSN;
    data_model[6] = StationID;
    data_model[7] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    data_model[8] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    //**************************本地数据库*******************************************
    if(db2.isOpen() && QSqlDatabase::contains("mysqlconnections"))
    {

        {
            if(!query2.exec("CREATE TABLE IF NOT EXISTS  KeyCodeInfo("
                            "ID int not null primary key auto_increment,"
                            "LineID int,"
                            "Flag int,"
                            "PartsCode nvarchar(100),"
                            "KeyCode varchar(200),"
                            "PackSN varchar(50),"
                            "StationID varchar(50),"
                            "UpdateTime datetime,"
                            "CLocalTime datetime,"
                            "OrderSN int)"))
            {
                qDebug()<<"create table KeyCodeInfo fail"<<query2.lastError();
                emit send_mysqlerror();
            }
        }
        QString aaa = QString("SELECT KeyCode FROM KeyCodeInfo WHERE (KeyCode='%1')").arg( KeyCode);
        qDebug()<<"selectKeyCodeSql:"<<aaa;
        query2.exec(aaa);
        int bbb = query2.numRowsAffected();
        qDebug()<<"rowsCount:"<<bbb;
//        if(bbb > 0)
        if(0)
        {
            qDebug()<<"issameKeyCode";
        }
        else
        {
        query2.exec("SELECT MAX(ID) FROM KeyCodeInfo");
        query2.next();
        int IDMax = query2.value(0).toInt();

        query2.exec("SELECT COUNT(*) FROM KeyCodeInfo");
        query2.next();
        int numRows = query2.value(0).toInt();
        if(numRows<3000)
        {
            query2.prepare("INSERT INTO KeyCodeInfo(ID, LineID,Flag, PartsCode, KeyCode, PackSN, StationID, UpdateTime, CLocalTime,OrderSN)"
                           "VALUES (:ID, :LineID,:Flag, :PartsCode, :KeyCode, :PackSN, :StationID, :UpdateTime, :CLocalTime,:OrderSN)");
        }
        else
        {
            query2.prepare("UPDATE KeyCodeInfo SET ID = :ID, LineID = :LineID,Flag = :Flag, PartsCode = :PartsCode, KeyCode = :KeyCode, PackSN = :PackSN, StationID = :StationID, UpdateTime = :UpdateTime, CLocalTime = :CLocalTime,OrderSN = :OrderSN WHERE ID = (select Min(t.ID) from (select ID from KeyCodeInfo)as t)");
        }
//        {
//            query2.prepare("INSERT INTO KeyCodeInfo(ID, LineID,Flag, PartsCode, KeyCode, PackSN, StationID, UpdateTime, CLocalTime,OrderSN)"
//                           "VALUES (:ID, :LineID,:Flag, :PartsCode, :KeyCode, :PackSN, :StationID, :UpdateTime, :CLocalTime,:OrderSN)");
//        }
        query2.bindValue(":ID",IDMax+1);
        query2.bindValue(":LineID",LineID);
        query2.bindValue(":Flag",Flag);
        query2.bindValue("PartsCode",data_model[3]);
        query2.bindValue("KeyCode",data_model[4]);
        query2.bindValue("PackSN",data_model[5]);
        query2.bindValue("StationID",data_model[6]);
        query2.bindValue("UpdateTime",data_model[7]);
        query2.bindValue("CLocalTime",data_model[8]);
        query2.bindValue(":OrderSN",OrderSN);
        bool inserttmp = query2.exec();
        int numRowsAffected=query2.numRowsAffected();

        if(!inserttmp)
        {
            qDebug()<<"insert error keycode" << query2.lastError();
            qDebug()<<"data_model keycode[1]:"<<data_model[4];
            if(!db2.isOpen())
                mysqlopen();
            if(!query2.exec())
            {
                //重启mysql 服务记录 缓存文件
                emit send_mysqlerror();//send to win重启系统
            }
            else
            {
                qDebug()<<data_model;
            }
        }
        else if(inserttmp && numRowsAffected ==0)
        {
            qDebug()<<"data_model keycode[2]:"<<data_model[4];
            emit send_mysqlerror();//send to win重启系统

        }
        else if(inserttmp && numRowsAffected >0)
        {
            qDebug()<<"data_model keycode[3]:"<<data_model;
        }
        }
        sqlclose();
    }
    else
    {
        qDebug()<<"db2 not open or unconnected";
    }
}

void SqlThreadSVW2::moduleTray(QString TraySN,QString module1 ,QString module2, QString module3, QString module4,QString module5 ,QString module6, QString module7, QString module8)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    int packTypeTemp;
    QString stationTemp;
    QString partsCodeTemp;
    partsCodeTemp = GscanRegExpCode;
    QString timeTemp;
    timeTemp = time.toString("yyyy-MM-dd hh:mm:ss");
    if(TraySN.mid(0,1) == "P")
    {
        packTypeTemp = 1;
    }
    else
    {
        packTypeTemp = 2;
    }
    if(Station == "M100B")
    {
        stationTemp = "VM040B";
    }
    else
    {
        stationTemp = Station;
    }
//***************************************************
    if(!db2.isOpen()||!QSqlDatabase::contains("mysqlconnections")){
        mysqlopen();
    }
    //**************************本地数据库*******************************************
    if(db2.isOpen() && QSqlDatabase::contains("mysqlconnections"))
    {
        if(!query2.exec("CREATE TABLE IF NOT EXISTS  TrayCodeInfo("
                        "ID int not null primary key auto_increment,"
                        "stationTemp varchar(50),"
                        "Line_ID varchar(50),"
                        "TraySN varchar(50),"
                        "packTypeTemp varchar(50),"
                        "partsCodeTemp varchar(50),"
                        "timeTemp datetime,"
                        "module1 varchar(200),"
                        "module2 varchar(200),"
                        "module3 varchar(200),"
                        "module4 varchar(200),"
                        "module5 varchar(200),"
                        "module6 varchar(200),"
                        "module7 varchar(200),"
                        "module8 varchar(200),"
                        "Flag int)"))
        {
            qDebug()<<"create table trayCodeInfo fail"<<query2.lastError();
            emit send_mysqlerror();
        }
        QString aaa = QString("UPDATE TrayCodeInfo SET Flag = 1 WHERE (TraySN='%1')").arg(TraySN);
        qDebug()<<"aaa"<<aaa;
        query2.exec(aaa);
        int bbb = query2.numRowsAffected();
        qDebug()<<"bbb"<<bbb;
        if(0)
        {
            qDebug()<<"issameKeyCode";
        }
        else
        {
        query2.exec("SELECT MAX(ID) FROM TrayCodeInfo");
        query2.next();
        int IDMax = query2.value(0).toInt();

        query2.exec("SELECT COUNT(*) FROM TrayCodeInfo");
        query2.next();
        int numRows = query2.value(0).toInt();
        if(numRows<3000)
        {
            query2.prepare("INSERT INTO TrayCodeInfo(ID, stationTemp,Line_ID, TraySN, packTypeTemp, partsCodeTemp, timeTemp, module1, module2, module3, module4, module5, module6, module7, module8 ,Flag)"
                           "VALUES (:ID, :stationTemp,:Line_ID, :TraySN, :packTypeTemp, :partsCodeTemp, :timeTemp, :module1, :module2,:module3,:module4,:module5,:module6,:module7,:module8,:Flag)");
        }
        else
        {
            query2.prepare("UPDATE TrayCodeInfo SET ID = :ID, stationTemp = :stationTemp,Line_ID = :Line_ID, TraySN = :TraySN, packTypeTemp = :packTypeTemp, partsCodeTemp = :partsCodeTemp, timeTemp = :timeTemp, module1 = :module1, module2 = :module2,module3 = :module3,module4 = :module4,module5 = :module5,module6 = :module6,module7 = :module7,module8 = :module8,Flag = :Flag WHERE ID = (select Min(t.ID) from (select ID from TrayCodeInfo)as t)");
        }
        query2.bindValue(":ID",IDMax+1);
        query2.bindValue(":stationTemp",stationTemp);
        query2.bindValue(":Line_ID",Line_ID);
        query2.bindValue(":TraySN",TraySN);
        query2.bindValue(":packTypeTemp",packTypeTemp);
        query2.bindValue(":partsCodeTemp",partsCodeTemp);
        query2.bindValue(":timeTemp",timeTemp);
        query2.bindValue(":module1",module1);
        query2.bindValue(":module2",module2);
        query2.bindValue(":module3",module3);
        query2.bindValue(":module4",module4);
        query2.bindValue(":module5",module5);
        query2.bindValue(":module6",module6);
        query2.bindValue(":module7",module7);
        query2.bindValue(":module8",module8);
        query2.bindValue(":Flag",0);

        bool inserttmp = query2.exec();
        int numRowsAffected=query2.numRowsAffected();

        if(!inserttmp)
        {
            qDebug()<<"insert error traykeycode" << query2.lastError();
            if(!db2.isOpen())
                mysqlopen();
            if(!query2.exec())
            {
                //重启mysql 服务记录 缓存文件
                emit send_mysqlerror();//send to win重启系统
            }
            else
            {
            }
        }
        else if(inserttmp && numRowsAffected ==0)
        {
            qDebug()<<"data_model traykeycode[2]:";
            emit send_mysqlerror();//send to win重启系统

        }
        else if(inserttmp && numRowsAffected >0)
        {
            qDebug()<<"data_model traykeycode[3]:";
        }
        }
        sqlclose();
    }
    else
    {
        qDebug()<<"db2 not open or unconnected";
    }

}

//存储PackOutputStatus数据
void SqlThreadSVW2::revPackMesFromStep(QString packSN,QString packType,QDateTime timeStart,QDateTime timeEnd,QString packStatus,double diffTime)
{
    QString stepPackTypeTemp;
    if(packType == "1")
    {
        stepPackTypeTemp = "PHEV";
    }
    else
    {
        stepPackTypeTemp = "BEV";
    }
    //int aaa = timeEnd.toTime_t() - timeStart.toTime_t();
    //packCount = QString::number(aaa);
    //***************************************************
        if(!db2.isOpen()||!QSqlDatabase::contains("mysqlconnections")){
            mysqlopen();
        }
        //**************************本地数据库*******************************************
        if(db2.isOpen() && QSqlDatabase::contains("mysqlconnections"))
        {
            QVariantList columnNameList;
            if(query2.exec("select COLUMN_NAME from information_schema.COLUMNS where table_name = 'PackOutputStatus'")){
                while(query2.next()){
                    columnNameList<<query2.value(0);
                }
            }

            if(!query2.exec("CREATE TABLE IF NOT EXISTS  PackOutputStatus("
                            "ID int not null primary key auto_increment,"
                            "packSN varchar(50),"
                            "packType varchar(50),"
                            "timeStart datetime,"
                            "timeEnd datetime,"
                            "diffTime double,"
                            "employeeID varchar(20),"
                            "packStatus varchar(50))"))
            {
                qDebug()<<"create table PackOutputStatus fail"<<query2.lastError();
                emit send_mysqlerror();
            }
            query2.exec("SELECT MAX(ID) FROM PackOutputStatus");
            query2.next();
            int IDMax = query2.value(0).toInt();

            query2.exec("SELECT COUNT(*) FROM PackOutputStatus");
            query2.next();
            int numRows = query2.value(0).toInt();
            if(numRows<3000)
            {
                query2.prepare("INSERT INTO PackOutputStatus(ID,packSN,packType,timeStart,timeEnd,diffTime,employeeID,packStatus)"
                               "VALUES (:ID,:packSN,:packType,:timeStart,:timeEnd,:diffTime,:employeeID,:packStatus)");
            }
            else
            {
                query2.prepare("UPDATE PackOutputStatus SET ID = :ID,packSN=:packSN,packType=:packType,timeStart=:timeStart,"
                               "timeEnd=:timeEnd,diffTime=:diffTime,employeeID=:employeeID,packStatus=:packStatus WHERE ID="
                               "(select Min(t.ID) from (select ID from PackOutputStatus)as t)");
            }
            query2.bindValue(":ID",IDMax+1);
            query2.bindValue(":packSN",packSN);
            query2.bindValue(":packType",packType);
            query2.bindValue(":timeStart",timeStart);
            query2.bindValue(":timeEnd",timeEnd);
            query2.bindValue(":diffTime",diffTime);
            query2.bindValue(":employeeID",EKSBuff);
            query2.bindValue(":packStatus",packStatus);
            //query2.bindValue(":packCount",packCount);
            //query2.bindValue(":packNumber",packNumber);

            bool inserttmp = query2.exec();
            int numRowsAffected=query2.numRowsAffected();

            if(!inserttmp)
            {
                qDebug()<<"insert error PackOutputStatus" << query2.lastError();
                if(!db2.isOpen())
                    mysqlopen();
                if(!query2.exec())
                {
                    //重启mysql 服务记录 缓存文件
                    emit send_mysqlerror();//send to win重启系统
                }
                else
                {
                }
            }
            else if(inserttmp && numRowsAffected ==0)
            {
                qDebug()<<"data_model PackOutputStatus[2]:";
                emit send_mysqlerror();//send to win重启系统

            }
            else if(inserttmp && numRowsAffected >0)
            {
                qDebug()<<"data_model PackOutputStatus[3]:";
            }
//            }
            sqlclose();
        }
        else
        {
            qDebug()<<"db2 not open or unconnected";
        }
}

//刚开机查询产量数据
void SqlThreadSVW2::FirstRevGPackNum()
{
    //***************************************************
    if(!db2.isOpen()||!QSqlDatabase::contains("mysqlconnections")){
        mysqlopen();
    }
    //**************************本地数据库*******************************************
    if(db2.isOpen() && QSqlDatabase::contains("mysqlconnections"))
    {
        if(!query2.exec("CREATE TABLE IF NOT EXISTS  PackOutputStatus("
                        "ID int not null primary key auto_increment,"
                        "packSN varchar(50),"
                        "packType varchar(50),"
                        "timeStart datetime,"
                        "timeEnd datetime,"
                        "diffTime double,"
                        "employeeID varchar(20),"
                        "packStatus varchar(50))"))
        {
            qDebug()<<"create table PackOutputStatus fail"<<query2.lastError();
            emit send_mysqlerror();
        }
        QString aaa = QString("SELECT * FROM PackOutputStatus WHERE TO_DAYS(timeEnd) = TO_DAYS(now())");
        query2.exec(aaa);
//        int numRowsAffected=query2.numRowsAffected();
//        GpackNumber = numRowsAffected;//以前是记录数据条数
        int outputTemp = 0;
        while(query2.next()){
            int packStatus = query2.value(7).toInt();
            if(1 == packStatus){//合格才算产量
                outputTemp++;
            }
        }
        GOutputToday = outputTemp;
        qDebug()<<"aaa"<<aaa<<"GOutputToday:"<<GOutputToday;
        emit sendDisplayDailyProduction();
        sqlclose();
    }
    else
    {
        qDebug()<<"db2 not open or unconnected";
    }
    QTimer::singleShot(1*60*60*1000,this,SLOT(FirstRevGPackNum()));
}

//通过屏幕控件手动查询设定时间范围产量状态 计算节拍 (功能还不成熟，后期再做)
//void SqlThreadSVW2::revTimeFromNewConfig(QString timeStart, QString timeEnd)
//{
//    //***************************************************
//    if(!db2.isOpen()||!QSqlDatabase::contains("mysqlconnections")){
//        mysqlopen();
//    }
//    //**************************本地数据库*******************************************
//    if(db2.isOpen() && QSqlDatabase::contains("mysqlconnections"))
//    {
//        if(!query2.exec("CREATE TABLE IF NOT EXISTS  PackOutputStatus("
//                        "ID int not null primary key auto_increment,"
//                        "packSN varchar(50),"
//                        "packType varchar(50),"
//                        "timeStart datetime,"
//                        "timeEnd datetime,"
//                        "diffTime int,"
//                        "employeeID varchar(20),"
//                        "packStatus varchar(50))"))
//        {
//            qDebug()<<"create table PackOutputStatus fail"<<query2.lastError();
//            emit send_mysqlerror();
//        }
////        QString aaa = QString("SELECT packCount FROM PackOutputStatus WHERE TO_DAYS(timeStart) >= TO_DAYS('%1') and  TO_DAYS(timeStart) < TO_DAYS('%2')").arg(timeStart).arg(timeEnd);
//        QString aaa = QString("SELECT * FROM PackOutputStatus WHERE timeStart >= '%1' and timeStart < '%2'").arg(timeStart).arg(timeEnd);
//        query2.exec(aaa);
//        int numRowsAffected=query2.numRowsAffected();
//        int sum = 0;
//        qDebug()<<"numRowsAffected"<<numRowsAffected<<aaa;
////        for(int i = 0;i<numRowsAffected;i++)
////        {
////            query2.next();
////            sum += query2.value(0).toInt();
////        }
//        while (query2.next())
//        {
//            if(query2.value(5).toInt() == 1){
//                sum++;
//            }
//        }
//        int avg = 0;
//        if(numRowsAffected < 1)
//        {
//            qDebug()<<"error";
//            avg = 1;
//        }
//        else
//        {
//            avg = sum/numRowsAffected;
//        }
//        qDebug()<<"aaa"<<aaa<<"GpackNumber"<<numRowsAffected;
//        emit sendResultToNewconfig(avg);
//        sqlclose();
//    }
//    else
//    {
//        qDebug()<<"db2 not open or unconnected";
//    }
//}


void SqlThreadSVW2::insertEKSInOutData()
{
    if(!db2.isOpen()||!QSqlDatabase::contains("mysqlconnections")){
        mysqlopen();
    }
    //**************************本地数据库*******************************************
    if(db2.isOpen() && QSqlDatabase::contains("mysqlconnections"))
    {
        if(!query2.exec("CREATE TABLE IF NOT EXISTS  EKSInPutInfo("
                        "ID int not null primary key auto_increment,"
                        "Station varchar(50),"
                        "InEKSTime datetime,"
                        "OutEKSTime datetime,"
                        "uploadStatus int,"
                        "EmployeeNo varchar(50))"))
        {
            qDebug()<<"create table EKSInPutInfo fail"<<query2.lastError();
            emit send_mysqlerror();
        }
        query2.exec("SELECT MAX(ID) FROM EKSInPutInfo");
        query2.next();
        int IDMax = query2.value(0).toInt();

        query2.exec("SELECT COUNT(*) FROM EKSInPutInfo");
        query2.next();
        int numRows = query2.value(0).toInt();

        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        if(EKSBuff.size() > 0){//存储插钥匙时间
            if(numRows<3000)
            {
                query2.prepare("INSERT INTO EKSInPutInfo(ID,Station,InEKSTime,OutEKSTime,uploadStatus,EmployeeNo)"
                               "VALUES (:ID,:Station,:InEKSTime,:OutEKSTime,:uploadStatus,:EmployeeNo)");
            }
            else
            {
                query2.prepare("UPDATE EKSInPutInfo SET ID = :ID,Station=:Station,InEKSTime=:InEKSTime,OutEKSTime=:OutEKSTime,"
                               "uploadStatus=:uploadStatus,EmployeeNo=:EmployeeNo WHERE ID="
                               "(select Min(t.ID) from (select ID from EKSInPutInfo)as t)");
            }
            query2.bindValue(":InEKSTime",time);
            query2.bindValue(":ID",IDMax+1);
            query2.bindValue(":Station",Station);
            query2.bindValue(":uploadStatus",0);
            query2.bindValue(":EmployeeNo",EKSBuff);
        }else{//存储拔钥匙时间并上传
            QString InEKSTime;
            QString employeeNo;
            query2.exec("SELECT ID,InEKSTime,EmployeeNo FROM EKSInPutInfo WHERE OutEKSTime IS NULL ORDER BY ID DESC LIMIT 1");
            while(query2.next()){
                EKS_ID = query2.value(0).toString();
                InEKSTime = QDateTime::fromString(query2.value(1).toString(),"yyyy-MM-ddThh:mm:ss").toString("yyyy-MM-dd hh:mm:ss");
                employeeNo = query2.value(2).toString();
            }

            query2.prepare("UPDATE EKSInPutInfo SET OutEKSTime = :OutEKSTime WHERE OutEKSTime IS NULL ORDER BY ID DESC LIMIT 1");
            query2.bindValue(":OutEKSTime",time);
            emit sendUploadEKSInfo(InEKSTime,time,employeeNo);
        }
        bool inserttmp = query2.exec();
        int numRowsAffected=query2.numRowsAffected();

        if(!inserttmp)
        {
            qDebug()<<"insert error EKSInPutInfo" << query2.lastError();
            if(!db2.isOpen())
                mysqlopen();
            if(!query2.exec())
            {
                //重启mysql 服务记录 缓存文件
                emit send_mysqlerror();//send to win重启系统
            }
            else
            {
                qDebug()<<"insert EKSInPutInfo success222:"<<numRowsAffected;
            }
        }else{
            qDebug()<<"insert EKSInPutInfo success:"<<numRowsAffected;
        }
        sqlclose();
    }
    else
    {
        qDebug()<<"db2 not open for EKSInPutInfo";
    }
}


void SqlThreadSVW2::receiveEKSUploadResult(bool result)
{
    if(result){
        if(!db2.isOpen()||!QSqlDatabase::contains("mysqlconnections")){
            mysqlopen();
        }
        //**************************本地数据库*******************************************
        if(db2.isOpen() && QSqlDatabase::contains("mysqlconnections"))
        {
            if(!query2.exec("CREATE TABLE IF NOT EXISTS  EKSInPutInfo("
                            "ID int not null primary key auto_increment,"
                            "Station varchar(50),"
                            "InEKSTime datetime,"
                            "OutEKSTime datetime,"
                            "uploadStatus int,"
                            "EmployeeNo varchar(50))"))
            {
                qDebug()<<"create table EKSInPutInfo fail"<<query2.lastError();
                emit send_mysqlerror();
            }
            bool updateResult = query2.exec("UPDATE EKSInPutInfo SET uploadStatus = 1 WHERE ID = " + EKS_ID);
            if(!updateResult){
                qDebug()<<"UPDATE EKSInPutInfo SET uploadStatus error" << query2.lastError();
            }else{
                EKS_ID = "";
                qDebug()<<"UPDATE EKSInPutInfo SET uploadStatus success";
            }
            sqlclose();
        }else{
            qDebug()<<"db2 not open for EKSInPutInfo";
        }
    }
}
