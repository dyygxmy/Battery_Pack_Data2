#ifndef SQLTHREADSVW2_H
#define SQLTHREADSVW2_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include "GlobalVarible.h"
#include <QThread>
#include <qtimer.h>
class SqlThreadSVW2 : public QObject
{
    Q_OBJECT
public:
    explicit SqlThreadSVW2(QObject *parent = 0);

public slots:
    void sqlinit();
    void mysqlopen();
    void sqlclose();
    void sqlinsert(QVariant);
    void insertKeyCodeInfo(int,int,QString,QString,QString,QString);
    void receiveNokAll(int);
    void FirstRevGPackNum();
//    void revTimeFromNewConfig(QString,QString);
    void configOne(QString,QString,QString,int);
    void revPackMesFromStep(QString,QString,QDateTime,QDateTime,QString,double);
    void moduleTray(QString, QString, QString, QString, QString, QString, QString, QString, QString);    //一个托盘码8个模组码上传
    void insertEKSInOutData();//存储EKS钥匙插拔时间记录
    void receiveEKSUploadResult(bool);

public:
    QThread m_thread;
    QSqlDatabase db2;
    //    QSqlDatabase db3;
    //    QSqlQuery query1;
    QSqlQuery query2;
    bool isFirst;
    //QString controlType;
signals:
    void send_mysqlerror();
    void sendResultToNewconfig(int);
    void sendDisplayDailyProduction();
    void sendUploadEKSInfo(QString,QString,QString);
private:
    QString EKS_ID;
//    QString InEKSTime;
};

#endif // SQLTHREADSVW2_H
