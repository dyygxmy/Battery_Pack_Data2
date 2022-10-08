#ifndef FTPJSONPARSE_H
#define FTPJSONPARSE_H

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QThread>
#include <QDebug>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "GlobalVarible.h"

#include "./json/parser.h"
#include "./json/serializer.h"

struct Return{
    QString screw;
    int groupnum;
};

class FtpJsonParse : public QObject
{
    Q_OBJECT
public:
    explicit FtpJsonParse(QObject *parent = 0);

signals:
    void send_mainwindow(QString,QString,QString,QString,QString,QString);
    void VIN_Match_Wrong(QString,QString);
    void sendfromjsonthread(QVariant);

private:
    QThread jsonParse_thread;
    QSqlDatabase db2;
    QSqlQuery query2;
    QTimer  ftpupload;
    int whichar;  //匹配出来的是哪个车型
    bool pro_is_true;
//    QString data_model[11];
    QString screwid[3];
    int screwid_enable[20];
    QMap<QString,int> wrongMap;
    int groupNum;

public slots:
    void ftpStart();
    void mysqlopen();
    void sqlclose();
    void FindFile(QString);
    void parseFile(QString ,QString);
    void ftptimeouts();
    struct Return configParse(QString,QString,QString);
};

#endif // FTPJSONPARSE_H
