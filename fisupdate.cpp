#include "fisupdate.h"
#include <QSqlRecord>
#include <QSettings>
#include "GlobalVarible.h"
FisUpdate::FisUpdate(QObject *parent) :
    QObject(parent)
{
    fis_thread.start();
    this->moveToThread(&fis_thread);
}

void FisUpdate::myfistimer()
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
void FisUpdate::fisupdateFunc()
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
                qDebug() << query1.lastError();
            if (query1.next())
            {
                QString datetime = query1.value(0).toString();
                system((QString("date -s \"") +datetime+QString("\"")).toLatin1().data());

                //将系统时间写入RTC
                system("hwclock -w");
                if(datetime.mid(0,4).toInt()<2015)
                    emit time_error(true);
                else
                    emit time_error(false);

            }

            if(!query2.exec("select count(*) from information_schema.columns where table_name='FisPreview'"))
                qDebug()<<query2.lastError();
            int column = 0;
            while(query2.next())
                column = query2.value(0).toInt();
//            qDebug()<< "column" << column;
            //    query2.exec("SELECT * FROM FisPreview");
            //    query2.next();
            //    int column = query2.record().count();
            if(!query2.exec("select column_name from information_schema.columns where table_name='FisPreview'"))
                qDebug()<<query2.lastError();
            for(int i=0;i<6;i++)
                query2.next();
            QString condition = "ID, werk, spj, Knr, VIN";
            QString column_name = "RecordID, Werk, SPJ, KNR, VIN ,LocalDateTime";
            QString question_mark ="?, ?, ?, ?, ?, now()";
            QString update_name = "RecordID =?, Werk =?, SPJ =?, KNR =?, VIN =?, LocalDateTime=now()";
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
//            qDebug()<<"condition"<<condition;
//            qDebug()<<"column_name"<<column_name;
//            qDebug()<<"question_mark"<<question_mark;
//            qDebug()<<"update_name"<<update_name;

            query2.exec("SELECT MAX(RecordID) FROM FisPreview");
            query2.next();
            QString MysqlMax = query2.value(0).toString();
//            qDebug() << MysqlMax;
//            QString aff= "SELECT "+condition+" FROM dbo.pnr WHERE RecordId >"+MysqlMax+" and Line_ID ="+QString::number(Line_ID);
            QString aff= "SELECT "+condition+" FROM dbo.pnr WHERE ID >"+MysqlMax;
            if(!query1.exec(aff))
                qDebug() <<"query1.lastError()"<<query1.lastError();
            while(query1.next())
            {
                //qDebug()<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
                query2.exec("SELECT COUNT(*) FROM FisPreview");
                query2.next();
                int numRows = query2.value(0).toInt();
                if (numRows < 7000)
                {
                    query2.prepare("insert into FisPreview ("+column_name+") values ("+question_mark+")");
                }
                else
                {
                    query2.prepare("UPDATE FisPreview SET "+update_name+"  WHERE RecordID = (select Min(t.RecordID) from (select RecordID from FisPreview)as t)");
                }
                query2.addBindValue(query1.value(0).toInt());
                query2.addBindValue(query1.value(1).toString());
                query2.addBindValue(query1.value(2).toString());
                query2.addBindValue(query1.value(3).toString());
                query2.addBindValue(query1.value(4).toString());
                for(int i=6;i<column;i++)
                    query2.addBindValue(query1.value(i-1).toString());
                if(!query2.exec())
                    qDebug() <<"query2.lastError()"<<query2.lastError();
            }



            //    int m;
            //    aff= "SELECT count(*) FROM Data.FisPreview WHERE RecordId >"+MysqlMax;
            //    query1.exec(aff);
            //    query1.next();
            //    if(query1.isValid())
            //    {
            //        m=query1.value(0).toInt();

            //        aff= "SELECT * FROM Data.FisPreview WHERE RecordId >";
            //        aff.append(MysqlMax);

            //        query1.exec(aff);
            //        query1.next();
            //    if(query1.isValid())
            //    {
            //        QVariantList Columns[column];
            //        Columns[0] << query1.value(0).toInt();
            //        int i;
            //        for(i=1;i<column;i++)
            //            Columns[i] << query1.value(i).toString();
            //        while(query1.next())
            //        {
            //            Columns[0] << query1.value(0).toInt();
            //            for(i=1;i<column;i++)
            //                Columns[i] << query1.value(i).toString();
            //        }

            //        query2.exec("select column_name from information_schema.columns where table_name='FisPreview'");
            //        query2.next();
            //        QString column_name = query2.value(0).toString();
            //        QString question_mark ="?";
            //        while(query2.next())
            //        {
            //            column_name = column_name + ", "+query2.value(0).toString();
            //            question_mark = question_mark+", ?";
            //        }
            //        query2.prepare("insert into FisPreview ("+column_name+") values ("+question_mark+")");
            //        for(i=0;i<column;i++)
            //            query2.addBindValue(Columns[i]);
            //        if(!query2.execBatch())
            //            qDebug() << query2.lastError();


            //        QString Values[m][column];
            //        int i;
            //        m=1;
            //        for(i=0;i<column;i++)
            //            Values[0][i]=query1.value(i).toString();
            //        while(query1.next())
            //        {
            //            for(i=0;i<column;i++)
            //                Values[m][i]=query1.value(i).toString();
            //            m++;
            //        }
            //        query2.exec("select column_name from information_schema.columns where table_name='FisPreview'");
            //        query2.next();
            //        QString column_name = query2.value(0).toString();
            //        QString question_mark ="?";
            //        QString update_name = query2.value(0).toString();
            //        while(query2.next())
            //        {
            //            column_name = column_name + ", "+query2.value(0).toString();
            //            question_mark = question_mark+", ?";
            //            update_name = update_name+ "=?, "+query2.value(0).toString();
            //        }
            //        update_name = update_name+"=?";

            //        for(i=0;i<m;i++)
            //        {
            //            query2.exec("SELECT COUNT(*) FROM FisPreview");
            //            query2.next();
            //            int numRows = query2.value(0).toInt();
            //            if (numRows < 7000)
            //            {
            //                query2.prepare("insert into FisPreview ("+column_name+") values ("+question_mark+")");
            //            }
            //            else
            //            {
            //                query2.prepare("UPDATE FisPreview SET "+update_name+"  WHERE RecordID = (select Min(t.RecordID) from (select RecordID from FisPreview)as t)");
            //            }
            //            query2.addBindValue(Values[i][0].toInt());
            //            for(j=1;j<column;j++)
            //                query2.addBindValue(Values[i][j]);
            //            if(!query2.exec())
            //                qDebug() << query2.lastError();
            //        }
            //! [超过3000条后覆盖最早的]
            //        //        query2.exec("SELECT * FROM FisPreview");
            //        //        query2.last();
            //        //        int numRows = query2.at() + 1;
            //        query2.exec("SELECT count(*) FROM FisPreview");
            //        query2.first();
            //        int numRows = query2.value(0).toInt();
            //        if (numRows > 3100)
            //        {
            //            query2.exec("delete from FisPreview order by RecordID limit "+ QString::number(numRows-3000));
            //            //query2.exec("optimize table FisPreview");
            //        }
            //! [超过1100条后覆盖最早的]

            //    }

            //    query2.exec("select column_name from information_schema.columns where table_name='FisPreview'");
            //    query2.next();
            //    QString columns = query2.value(0).toString();
            //    while(query.next())
            //    {
            //        columns = columns + ", "+query->value(0).toString();
            //        qDebug()<<query->value(0).toString();
            //    }
            //    qDebug()<< "columns" << columns;
            //    if (columns != "RecordID, Werk, SPJ, KNR, VIN, GSP, SAB, RAD, REI, BRS, LocalDateTime, HIS")
            //    {
            //        query2.exec("select column_name from information_schema.columns where table_name='FisPreview'");
            //        query2.last();
            //        QString Add = query->value(0).toString();
            //        query1.exec(aff);
            //        query1.next();
            //        if(query1.isValid())
            //        {
            //            while(query1.next())
            //            {
            //                query2.exec("UPDATE FisPreview SET "+Add+" = "+query1.value(12).toString()+" WHERE Recordid = "+query1.value(0).toString());
            //            }
            //        }
            //    }
        }
        sqlclose();
    }
}

void FisUpdate::update_column(QString column)
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
            query1.exec("select column_name from information_schema.columns where table_name='FisPreview'");
            while(query1.next())
            {
                if(column == query1.value(0).toString())
                    isColumnName = true;
            }
            if(isColumnName)
            {
                //qDebug()<<"column"<<column;
                if(query2.exec("select RecordID from FisPreview order by RecordID DESC LIMIT 1000"))
                {
                    while(query2.next())
                    {
                        //qDebug()<<"query2"<<query2.value(0).toString();
                        query1.exec("select "+column+" from FisPreview where RecordID="+query2.value(0).toString());
                        query1.next();
                        //qDebug()<<"query1"<<query1.value(0).toString();
                        //qDebug()<<"UPDATE FisPreview SET "+column+" = \'"+query1.value(0).toString()+"\' WHERE RecordID = "+query2.value(0).toString();
                        if(!query3.exec("UPDATE FisPreview SET "+column+" = \'"+query1.value(0).toString()+"\' WHERE RecordID = "+query2.value(0).toString()))
                            qDebug()<<"update column query3"<<query3.lastError();
                    }
                }
                else
                    qDebug()<<"update column query2"<<query2.lastError();
            }
        }
        sqlclose();
    }
    else
        qDebug()<<"wifi unconnect update_column old datas fail";
}

// 数据服务器open FisPreview表
void FisUpdate::fis_sql_open()
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
void FisUpdate::mysql_open()
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
void FisUpdate::sqlclose()
{
    if(db1.isOpen())
        db1.close();
    if(db2.isOpen())
        db2.close();
}

//查询时间
void FisUpdate::QueryTime()
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
