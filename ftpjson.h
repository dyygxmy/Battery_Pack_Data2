#ifndef FTPJSON_H
#define FTPJSON_H

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QThread>
#include <QDebug>
#include <QSettings>
#include <QMap>

#include "./json/parser.h"

class FtpJson : public QObject
{
    Q_OBJECT
public:
    explicit FtpJson(QObject *parent = 0);
    bool matchVin();
    
signals:
    void send_mainwindow(QString,QString,QString,int,int,int);
    void sendfromjsonthread(QVariant);
    void coming_VIN(QString);
    void VIN_Match_Wrong(QString,QString);
    
public slots:
    void myjson();
    void jsontimeout();
    void FindFile(QString);
    void parseFile(QString ,QString);
    QString configParse(QString,QString,QString);

private:
    QThread json_thread;
    QTimer jsonParse;
//    QString data_model[11];
    int whichar;  //匹配出来的是哪个车型
    int whichpronumis;//当前程序号
    QString screwid[3];
//    bool channel_is_true;
    bool pro_is_true;
    int screwid_enable[20];
    QMap<QString,int> wrongMap;

};

#endif // FTPJSON_H
