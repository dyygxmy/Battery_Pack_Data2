#include "ftphandle.h"
#include <unistd.h>
FtpHandle::FtpHandle(QObject *parent) :
    QObject(parent)
{    
    m_thread.start();
    this->moveToThread(&m_thread);
}

void FtpHandle::ftpStart()
{
    qDebug() << "ftpstart thread!!!!!!!!!!!!" ;
    connect(&ftpupload,SIGNAL(timeout()),this,SLOT(ftptimeouts()));
    ftpupload.start(5000);
}

void FtpHandle::mysqlopen()
{
    if(QSqlDatabase::contains("ftpmysqlconnection")){
        db2 = QSqlDatabase::database("ftpmysqlconnection");
    }else{
        db2=QSqlDatabase::addDatabase("QMYSQL","ftpmysqlconnection");
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
            qDebug()<< "ftp localmysql "<< db2.lastError().text();
        }else
        {
            qDebug()<< "ftp localmysql ok2 ";
        }
    }else
    {
        qDebug()<< "ftp localmysql ok1 ";
    }
}

/*
*     数据库关闭
*/
void FtpHandle::sqlclose()
{
    if(db2.isOpen())
        db2.close();
}

void FtpHandle::ftptimeouts()
{
    FindFile("/curveFile");
}

void FtpHandle::FindFile(QString path)
{
    //qDebug() << "find file1";
    QDir dir(path);
    if (!dir.exists())
       return ;
    dir.setFilter(QDir::Dirs|QDir::Files);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    int i=0;
    do{
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.fileName()=="."||fileInfo.fileName()=="..")
        {
            i++;
            continue;
        }
        bool bisDir=fileInfo.isDir();
        if(bisDir)
        {
            //nFiles++;
            // qDebug() << QString("%1 %2 %3").arg(fileInfo.size(), 10)
           // qDebug() << "find file2";
            FindFile(fileInfo.filePath());
            qDebug() << "find fil3";
        }
        else
        {
            //nFiles++;
            // qDebug() << QString("%1 %2 %3").arg(fileInfo.size(), 10)
            // .arg(fileInfo.fileName(),10).arg(fileInfo.path())<<endl;
            qDebug() << "find file parsing!!" << fileInfo.baseName();
            parseFile(fileInfo.absoluteFilePath(),fileInfo.baseName().right(10));

        }
        i++;
    }while(i<list.size());
}

void FtpHandle::parseFile(QString filepath,QString cycle)
{
    qDebug() << "find file4" << cycle;
    cycle = QString::number(cycle.toInt());
    QFile txt1(filepath);
    if(!txt1.open(QIODevice::ReadOnly))
        qDebug()<<txt1.errorString();
    QByteArray txt2=txt1.readAll();
    if(txt2.right(1) == "}")
    {
        qDebug() << "file open ok" << cycle;
        if(!db2.isOpen())
            mysqlopen();
        if(db2.isOpen())
        {
            bool inserttmp = query2.exec("update "+Localtable+" set Curve =\'"+txt2+"\' where Cycle="+cycle+" and Curve='null' order by RecordID desc limit 1");
            int numRows=query2.numRowsAffected();
            if(inserttmp && numRows>0)
            {
                query2.numRowsAffected();
                qDebug() << "curve insert success cycle = "+cycle;
                unlink(filepath.toLocal8Bit().data());
                qDebug() << "delete file "+filepath.right(21);
            }
            else if(!inserttmp)
            {
                qDebug() << query2.lastError().text() <<" || curve insert fail cycle = "+cycle;
            }
            else
            {
                qDebug()<< "numRows = 0";
            }
        }
        else
        {
            qDebug() << "ftp mysql not open "+filepath+" insert fail";
        }
        sqlclose();
    }
    else
    {
        qDebug()<<"file wrong not json";
    }
}

