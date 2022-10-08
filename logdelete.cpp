#include "logdelete.h"
#include "unistd.h"

LogDelete::LogDelete(QObject *parent) :
    QObject(parent)
{
    this->moveToThread(&m_thread);
    m_thread.start();
}

void LogDelete::init()
{
    qDebug() <<"LogDelete thread start!!";
    connect(&LogTimer, SIGNAL(timeout()), this, SLOT(logDeleteSlot()));
    LogTimer.start(3600000);
}

void LogDelete::logDeleteSlot()
{
    QDir dir("/log");
    if (!dir.exists())
       return ;
    QFileInfoList list = dir.entryInfoList();
    int i=0;

    do{
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.fileName()=="."||fileInfo.fileName()=="..")
        {
            i++;
            continue;
        }
//        qDebug()<<"find "<<fileInfo.filePath();

        QDate date = QDate::currentDate();

        if(QDate::fromString(fileInfo.baseName(),"yyyy-MM-dd") < date.addDays(-30))
        {
            qDebug() << "delete file "+fileInfo.fileName();
            unlink(fileInfo.filePath().toLocal8Bit().data());
        }
        i++;
    }while(i<list.size());
}
