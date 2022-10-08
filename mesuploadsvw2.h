#ifndef MESUPLOADSVW2_H
#define MESUPLOADSVW2_H

#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSettings>
#include <QMap>
#include <QTimer>
#include <QTime>
#include <QApplication>
#include "GlobalVarible.h"

#include "./json/serializer.h"
#include "./json/parser.h"

class MESUploadSVW2 : public QObject
{
    Q_OBJECT
public:
    explicit MESUploadSVW2(QObject *parent = 0);
    void mysql_open();
    QString vari2_XL;
public slots:
    void init();

private:
    QThread m_thread;
    QNetworkAccessManager *manager;
    QNetworkAccessManager *manager3;
    QNetworkReply *reply;
    QNetworkRequest req;
    QNetworkRequest req3;
//    QTimer InsertTimer;
    QSqlDatabase db2;
    QSqlQuery query2;
    int GetTimes;
    int GetTimesTemp;
    bool isFirst;
    int wrongRecordID;
    int wrongTimes;
    int TIMEOUT;
    QString strTarget;

private slots:
//    void finishedSlot(QNetworkReply *);
    void dataInsert();
    void keyCodeInsert();
};

#endif // MESUPLOADSVW2_H
