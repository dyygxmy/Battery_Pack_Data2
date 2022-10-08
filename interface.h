#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMap>
#include <QDebug>
#include <QTime>
#include <QApplication>
#include <QThread>
#include "GlobalVarible.h"

#include "./json/serializer.h"
#include "./json/parser.h"


class RESULT
{
public:
    bool isSuccess;
    QVariantMap Result;
};

class Interface : public QObject
{
    Q_OBJECT
public:
    explicit Interface(QObject *parent = 0);
    RESULT HTTP_Req(QString,QVariantMap);
    RESULT HTTP_Req1(QString,QVariantMap);

signals:
    void sendPackStatus(QVariant);
    void senddutouTightStatus2(QString);
    void sendModuleTray(bool);
    void sendStationState(bool);
    void sendAgvLeaveState(bool);

    void sendreSendReqMoudlePack();
    void sendEmergencyStopCode(bool);
    //void sendStates(QString);
    void sendPack1And2Type(QString,QString);
    //void sendModuleTrayClean();
    void sendGetStationTimerResult(QString,QString);

public slots:
    void init();
    void reqPackStatus();
    void reqTightStatus(QString,QString,QString);
    void moduleTray(QString, QString, QString, QString, QString);    //一个托盘码四个模组码上传
    void moduleTray(QString, QString, QString, QString, QString, QString, QString, QString, QString);    //一个托盘码8个模组码上传
    void modulePack(QString, QString);
    void Keybarcode(QString, QString, QString, int);
    void stationState(QString, QString, QString, QString, QString);
    void reqEmergencyStop(bool);
    void requestAgvLeave();
    void reStationStatus();
    void getStationTimer(QString, QString, QString, QString);
private:
    QThread m_thread;
    QString Server;
    QString AGVServer;
    QNetworkAccessManager *manager;
    //QNetworkReply *reply;
    QNetworkRequest req;

    int TIMEOUT;
    int packTypeTemp;
    bool isEmergencyStop;
    QString urlTemp;
    QVariantMap dataMapTemp;
    bool oneTimesRePost;

};

#endif // INTERFACE_H
