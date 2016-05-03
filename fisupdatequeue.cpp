#include "fisupdatequeue.h"
#include <QSqlRecord>
#include <QSettings>
#include "GlobalVarible.h"
FisUpdateQueue::FisUpdateQueue(QObject *parent) :
    QObject(parent)
{
    fis_thread.start();
    this->moveToThread(&fis_thread);
}

void FisUpdateQueue::myfistimer()
{
    qDebug()<< "fisupdate thread start";
    db1=QSqlDatabase::addDatabase("QODBC","SQLServerconnection");
    db1.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=1;SQL_ATTR_CONNECTION_TIMEOUT=1");
    db1.setDatabaseName("sqltighten");
    db1.setPort(1433);
    db1.setUserName(SqlUserName);
    db1.setPassword(SqlPassword);
    query1 = QSqlQuery(db1);

    db2=QSqlDatabase::addDatabase("QMYSQL","mysqlconnection");
    db2.setHostName("localhost");
    db2.setDatabaseName("Tighten");
    db2.setUserName("root");
    db2.setPassword("123456");
    query2 = QSqlQuery(db2);
    query3 = QSqlQuery(db2);

    //    if(!db1.isOpen())
    //        fis_sql_open();
    //    if(!db2.isOpen())
    //        mysql_open();

    fisupdataTimer = new QTimer(this);
    connect(fisupdataTimer,SIGNAL(timeout()),this,SLOT(fisupdateFunc()));
    fisupdataTimer->start(50000);
}

//50秒更新Fis
void FisUpdateQueue::fisupdateFunc()
{
    if(WIFI_STATE)
    {
        if(!db1.isOpen()||!QSqlDatabase::contains("SQLServerconnection"))
            fis_sql_open();
        if(!db2.isOpen()||!QSqlDatabase::contains("mysqlconnection"))
            mysql_open();

        if(db1.isOpen() && db2.isOpen() && QSqlDatabase::contains("SQLServerconnection") && QSqlDatabase::contains("mysqlconnection"))
        {
            //更新时间
            if(!query1.exec("select CONVERT(VARCHAR(20),getdate(),120)"))
                qDebug()<<query1.lastError();
            if (query1.next())
            {
                QString datetime = query1.value(0).toString();
                system((QString("date -s \"") +datetime+QString("\" &")).toLatin1().data());

                //将系统时间写入RTC
                system("hwclock -w &");
                if(datetime.mid(0,4).toInt()<2015)
                    emit time_error(true);
                else
                    emit time_error(false);
            }

            if(!query2.exec("select count(*) from information_schema.columns where table_name='TaskPreview'"))
                qDebug()<<query2.lastError();
            int column = 0;
            while(query2.next())
                column = query2.value(0).toInt();
            if(!query2.exec("select column_name from information_schema.columns where table_name='TaskPreview'"))
                qDebug()<<query2.lastError();
            for(int i=0;i<8;i++)    //gai!!!!!!!!!!!!!!!!!!!!!!!!
                query2.next();
            QString condition = "RecordID, AutoNO, VIN, Type, Style, Seat ";
            QString column_name = "RecordID, AutoNO, VIN, Type, Style, Seat ,LocalDateTime, UseFlag";
            QString question_mark ="?, ?, ?, ?, ?, ?, now(), ?";
            QString update_name = "RecordID =?, AutoNO =?, VIN =?, Type =?, Style =?, Seat =?, LocalDateTime=now(),UseFlag=?";
            while(query2.next())
            {
                condition = condition + ", "+query2.value(0).toString();
                column_name = column_name + ", "+query2.value(0).toString();
                question_mark = question_mark+", ?";
                update_name = update_name+ ", "+query2.value(0).toString()+"=? ";
            }
           // update_name = update_name+"=?";

//            QString question_mark ="?";
//            QString update_name = query2.value(0).toString();
//            while(query2.next())
//            {
//                column_name = column_name + ", "+query2.value(0).toString();
//                question_mark = question_mark+", ?";
//                update_name = update_name+ "=?, "+query2.value(0).toString();
//            }
//            update_name = update_name+"=?";
            //qDebug()<<"condition"<<condition;
            //qDebug()<<"column_name"<<column_name;
            //qDebug()<<"question_mark"<<question_mark;
            //qDebug()<<"update_name"<<update_name;

            query2.exec("SELECT MAX(RecordID) FROM TaskPreview");
            query2.next();
            QString MysqlMax = query2.value(0).toString();
//            qDebug()<<"MysqlMax:"<<MysqlMax;
            QString aff= "SELECT "+condition+" FROM dbo.TaskPreview WHERE RecordId >"+MysqlMax;   //+" and Line_ID ="+QString::number(Line_ID);
            if(!query1.exec(aff))
                qDebug() << "110" << query1.lastError();
            while(query1.next())
            {
                query2.exec("SELECT COUNT(*) FROM TaskPreview");
                query2.next();
                int numRows = query2.value(0).toInt();
                if (numRows < 7000)
                {
                    query2.prepare("insert into TaskPreview ("+column_name+") values ("+question_mark+")");
                }
                else
                {
                    query2.prepare("UPDATE TaskPreview SET "+update_name+"  WHERE RecordID = (select Min(t.RecordID) from (select RecordID from TaskPreview)as t)");
                }
                query2.addBindValue(query1.value(0).toInt());
                query2.addBindValue(query1.value(1).toString());
                query2.addBindValue(query1.value(2).toString());
                query2.addBindValue(query1.value(3).toString());
                query2.addBindValue(query1.value(4).toString());
                query2.addBindValue(query1.value(5).toString());
                query2.addBindValue(0);
                for(int i=8;i<column;i++)  //gai!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    query2.addBindValue(query1.value(i-2).toString());
                if(!query2.exec())
                    qDebug() << query2.lastError();
            }
            
            //更新删除车辆表
            query2.exec("SELECT MAX(RecordID) FROM TaskDeleted");
            query2.next();
            MysqlMax = query2.value(0).toString();
//            qDebug()<<"MysqlMax"<<MysqlMax;
            aff = "SELECT RecordID, AutoNO, VIN FROM dbo.TaskDeleted WHERE RecordID > "+MysqlMax;
            if(!query1.exec(aff))
                qDebug() << "144" << query1.lastError();
            int Flag =0;
            while(query1.next())
            {
                if(query2.exec("UPDATE TaskPreview SET UseFlag = 2 where AutoNO ='"+query1.value(1).toString()+"' or VIN = '"+query1.value(2).toString()+"'"))
                {
                    if(query2.numRowsAffected() == 1)
                        qDebug()<<"Update success AutoNO:"+query1.value(1).toString()+" VIN:"+query1.value(2).toString();
                    else if(query2.numRowsAffected() == 0)
                        qDebug()<<"There is no AutoNO:"+query1.value(1).toString()+" VIN:"+query1.value(2).toString();
                    Flag = 1;
                }
                else
                {
                    Flag = 0;
                    qDebug()<<"Update fail AutoNO:"+query1.value(1).toString()+" VIN:"+query1.value(2).toString()<<query2.lastError();
                }
                query2.prepare("insert into TaskDeleted (RecordID, AutoNO, VIN, LocalDateTime, Flag) values (?, ?, ?, now(), ?)");
                query2.addBindValue(query1.value(0).toInt());
                query2.addBindValue(query1.value(1).toString());
                query2.addBindValue(query1.value(2).toString());
                query2.addBindValue(Flag);
                if(!query2.exec())
                    qDebug()<< "Insert TaskDeleted fail AutoNO:"+query1.value(1).toString()+" VIN:"+query1.value(2).toString()<<query2.lastError();
                else
                    qDebug()<< "Insert TaskDeleted success ";
            }
        }
        sqlclose();
    }
    if((isRFID == 2) && !ISmaintenance && CsIsConnect)
    {
        if(QueueIsNull)
        {
            if(!db2.isOpen()||!QSqlDatabase::contains("mysqlconnection"))
                mysql_open();
            if(db2.isOpen()||QSqlDatabase::contains("mysqlconnection"))
            {
                if(!query2.exec("select VIN from TaskPreview where RecordId in(select min(RecordId) from TaskPreview where UseFlag = 0)"))
                    qDebug() <<"fis 148 row "  << query2.lastError();
                else
                {
                    if(query2.next())
                    {
                        emit FisSendSerial(query2.value(0).toString());
                    }
                    else
                        qDebug() << "fis is null";
                }
            }
            else
                qDebug() << "fis open fail";
        }
    }
}
//更新列
void FisUpdateQueue::update_column(QString column)
{
    if(WIFI_STATE)
    {
        if(!db1.isOpen()||!QSqlDatabase::contains("SQLServerconnection"))
            fis_sql_open();
        if(!db2.isOpen()||!QSqlDatabase::contains("mysqlconnection"))
            mysql_open();

        if(db1.isOpen() && db2.isOpen() && QSqlDatabase::contains("SQLServerconnection") && QSqlDatabase::contains("mysqlconnection"))
        {
            bool isColumnName = false;
            query1.exec("select column_name from information_schema.columns where table_name='TaskPreview'");
            while(query1.next())
            {
                if(column == query1.value(0).toString())
                    isColumnName = true;
            }
            if(isColumnName)
            {
                //qDebug()<<"column"<<column;
                if(query2.exec("select RecordID from TaskPreview order by RecordID DESC LIMIT 1000"))
                {
                    while(query2.next())
                    {
                        //qDebug()<<"query2"<<query2.value(0).toString();
                        query1.exec("select "+column+" from TaskPreview where RecordID="+query2.value(0).toString());
                        query1.next();
                        //qDebug()<<"query1"<<query1.value(0).toString();
                        //qDebug()<<"UPDATE FisPreview SET "+column+" = \'"+query1.value(0).toString()+"\' WHERE RecordID = "+query2.value(0).toString();
                        if(!query3.exec("UPDATE TaskPreview SET "+column+" = \'"+query1.value(0).toString()+"\' WHERE RecordID = "+query2.value(0).toString()))
                            qDebug()<<query3.lastError();
                    }
                }
                else
                    qDebug()<<query2.lastError();
            }
        }
        sqlclose();
    }
    else
        qDebug()<<"wifi unconnect update_column old datas fail";
}

// 数据服务器open FisPreview表
void FisUpdateQueue::fis_sql_open()
{
    if(QSqlDatabase::contains("SQLServerconnection")){
        db1 = QSqlDatabase::database("SQLServerconnection");
    }else{
        db1=QSqlDatabase::addDatabase("QODBC","SQLServerconnection");
        db1.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=1;SQL_ATTR_CONNECTION_TIMEOUT=1");
        db1.setDatabaseName("sqltighten");
        db1.setPort(1433);
        db1.setUserName(SqlUserName);
        db1.setPassword(SqlPassword);
        query1 = QSqlQuery(db1);
    }

    if(!db1.open())
    {
        if(!db1.open())
        {
            qDebug()<<"sqlserver fisupdate "<<db1.lastError().text();
        }
        else
        {
            qDebug()<< "sqlserver fisupdate open ok 2";
        }
    }else
    {
        qDebug()<< "sqlserver fisupdate open ok 1";
    }
}

// 本地mysql open
void FisUpdateQueue::mysql_open()
{
    if(QSqlDatabase::contains("mysqlconnection")){
        db2 = QSqlDatabase::database("mysqlconnection");
    }else{
        db2=QSqlDatabase::addDatabase("QMYSQL","mysqlconnection");
        db2.setHostName("localhost");
        db2.setDatabaseName("Tighten");
        db2.setUserName("root");
        db2.setPassword("123456");
        query2 = QSqlQuery(db2);
        query3 = QSqlQuery(db2);
    }

    if(!db2.open())
    {
        if(!db2.open())
        {
            qDebug()<< "fisupdate localmysql "<< db2.lastError().text();
        }else
        {
            qDebug()<< "fisupdate localmysql open ok 2";
        }
    }else
    {
        qDebug()<< "fisupdate localmysql open ok 1";
    }
}

// 数据库关闭
void FisUpdateQueue::sqlclose()
{
    if(db1.isOpen())
        db1.close();
    if(db2.isOpen())
        db2.close();
}

//查询时间
void FisUpdateQueue::QueryTime()
{
    if(WIFI_STATE)
    {
        if(!db1.isOpen()||!QSqlDatabase::contains("SQLServerconnection"))
            fis_sql_open();
        query1.exec("select CONVERT(VARCHAR(20),getdate(),120)");
        query1.next();
        QString datetime = query1.value(0).toString();
        emit sendTime(datetime);

        if(db1.isOpen())
            db1.close();
    }
}

