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
    void senddutouTightStatus1(QString);
    void senddutouTightStatus2(QString);
    void sendModuleTray(bool);
    void sendModulePack(bool);
    void sendKeybarcode(bool);
    void sendModule(bool, QString);
    void sendStationState(bool);
    void sendAgvLeaveState(bool);

    void sendreSendReqMoudlePack();
    void sendModuleCount(bool);
    void sendEmergencyStopCode(bool);
    void sendStates(QString);
    void sendPack1And2Type(QString,QString);
    void sendKeyCodeInfo(int,int,QString,QString,QString,QString);
    //void sendModuleTrayClean();

public slots:
    void init();
    void reqPackStatus();
    void reqdutouTightStatus(QString,QString);
    void reqTightStatus(QString,QString,QString);
    void boundPack();
    void reqModule();
    void moduleTray(QString, QString, QString, QString, QString);    //一个托盘码四个模组码上传
    void moduleTray(QString, QString, QString, QString, QString, QString, QString, QString, QString);    //一个托盘码8个模组码上传
    void modulePack(QString, QString);
    void Keybarcode(QString, QString, QString, int);
    void stationState(QString, QString, QString, QString, QString);
    void moduleCount();
    void reqEmergencyStop(bool);
    void requestAgvLeave();
    void modulePackClean(QString);
    void onUpInAndOutStation(int , QString , QString , QString , QString);
private:
    QThread m_thread;
    QString Server;
    QString AGVServer;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QNetworkRequest req;

    void reReqPackStatus();
    int TIMEOUT;
    int packTypeTemp;
    bool isEmergencyStop;
    QString urlTemp;
    QVariantMap dataMapTemp;
    bool oneTimesRePost;

};

#endif // INTERFACE_H
