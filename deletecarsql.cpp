#include "deletecarsql.h"

DeleteCarSQL::DeleteCarSQL(QObject *parent) :
    QObject(parent)
{
    m_thread.start();
    this->moveToThread(&m_thread);
}

//插入要删除车条码
void DeleteCarSQL::SQL_Delete_Car()
{
    while(1)
    {
        RFIDlock.lockForRead();
        if(WIFI_STATE && !queue.isEmpty())
        {
            RFIDlock.unlock();
            if(!db1.isOpen()||!QSqlDatabase::contains("Delete_connection"))
                sql_open();
            if(db1.isOpen()||QSqlDatabase::contains("Delete_connection"))
            {
                query1.prepare("INSERT INTO dbo.TaskDeleted(AutoNO, VIN)"
                               "VALUES (?, ?)");
                QQueue<QVector<QString> >::iterator iter= queue.begin();
                query1.addBindValue(iter[0][0]);
                query1.addBindValue(iter[0][1]);
                if(query1.exec())
                {
                    qDebug()<<"Delete_Car Upload success :"<<iter[0][0]<<iter[0][1];
                    queue.erase(iter);
                }
                else
                {
                    qDebug()<<query1.lastError();
                    qDebug()<<"Delete_Car Upload fail: "<<iter[0][0]<<iter[0][1];
                }
                if(db1.isOpen())
                    db1.close();
            }
        }
        else
        {
            RFIDlock.unlock();
        }
        sleep(2);
    }
}

void DeleteCarSQL::sql_open()
{
    if(QSqlDatabase::contains("Delete_connection")){
        db1 = QSqlDatabase::database("Delete_connection");
    }else{
        db1=QSqlDatabase::addDatabase("QODBC","Delete_connection");
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
            qDebug()<<"deletecar sql open error "<<db1.lastError().text();
        }
        else
        {
            qDebug()<< "deletecar sql open ok 2";
        }
    }else
    {
        qDebug()<< "deletecar sql open ok 1";
    }
}

