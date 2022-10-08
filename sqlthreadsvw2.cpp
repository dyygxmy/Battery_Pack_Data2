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
}
/*
     数据库关闭
*/
void SqlThreadSVW2::sqlclose()
{
    if(db2.isOpen())
        db2.close();
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
            query2.prepare("UPDATE "+Localtable+" SET RecordID =?, IDCode =?, ScrewID =?, Torque =?, Angle =?, Curve =?, TighteningStatus =?, TighteningTime =?, LocalDateTime =now(), Operator =?, UploadMark =?, UploadTime =?, Cycle =?, Program =?, Channel =?, Type =?, Order_id =?, MaxValue =?, MinValue =?, JobStatus =? ,Angle_Max = ?,Angle_Min =? ,Torque_Max = ?,Torque_Min = ? WHERE RecordID = (select Min(t.RecordID) from (select RecordID from "+Localtable+")as t)");
        }
        query2.addBindValue(RecordIDMax+1);//RecordID
        query2.addBindValue(data_model[6]);//IDCode
        query2.addBindValue(screwid_sql);//ScrewID
        query2.addBindValue(data_model[3]);//Torque
        query2.addBindValue(data_model[4]);//Angle
        query2.addBindValue(data_model[8]);//Curve
        query2.addBindValue(data_model[2]);//TighteningStatus
        if(data_model[0]=="" || data_model[1]=="")//TighteningTime+LocalDateTime
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
