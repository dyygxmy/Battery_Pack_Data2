#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMap>
#include <QDebug>
#include <QTimer>
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
    RESULT HTTP_Req2(QString, QVariantMap);

signals:
    void sendPackStatus(QVariant);
    void sendGetStationkeyCodeResult(bool);
    void sendReturngetTrayPackType(QString);
    void sendBMCEFirstStatus(QVariant);
    void senddutouTightStatus1(QString);
    void sendGluePackStatus(QString);
    void senddutouTightStatus2(QString);
    void sendModuleTray(bool);
    void sendModulePack(bool);
    void sendKeybarcode(bool);
    void sendGetStationTimer(QDateTime);
    void sendTrayCleanResoult();
    void sendModule(bool, QString);
    void sendStationState(bool);
    void sendAgvLeaveState(bool);

    void sendreSendReqMoudlePack();
    void sendModuleCount(bool);
    void sendEmergencyStopCode(bool);
    void sendm020ToMainWindow(QString,QString,QString,QString);
    void sendStates(QString);
    void sendPack1And2Type(QString,QString);
    void sendKeyCodeInfo(int,int,QString,QString,QString,QString);
    //void sendModuleTrayClean();

public slots:
    void init();
    void reqPackStatus();
    void revAddTrayPackType(QString);
    void revgetTrayPackType(QString);
    void revAddStationTimer(QString,int);
    void revgetStationkeyCode(QString,QString);
    void recIsMatchModuleKeyCode(QString,QString);
    void reqdutouTightStatus(QString,QString);
    void reqGlueWorkFlag(QString);
    void revgetStationTimer(QString,int);
    void reqTightStatus(QString,QString,QString);
    void boundPack();
    void reqModule();
    void reqBMCEStatus(QString);
    void moduleTray(QString, QString, QString, QString, QString);    //一个托盘码四个模组码上传
    void moduleTray(QString, QString, QString, QString, QString, QString, QString, QString, QString);    //一个托盘码8个模组码上传
    void moduleTray(QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString);    //一个托盘码8个模组码上传
    void modulePack(QString, QString);
    void Keybarcode(QString, QString, QString, int);
    void stationState(QString, QString, QString, QString, QString);
    void moduleCount();
    void reqEmergencyStop(bool);
    void requestAgvLeave();
    void revM020fromMainwindow(QString);
    void reqRepairBatterypack(QString);
    void modulePackClean(QString);
    void BMCETrayClean(QString,QString);
    void onUpInAndOutStation(int , QString , QString , QString , QString);
    void uplodeStationStatus();
private:
    QThread m_thread;
    QString Server;
    QString AGVServer;
    QString statusServer;
    QString StatusTemp;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QNetworkRequest req;
    void reboundPack();
    void reRevgetStationkeyCode(QString,QString);
    void rerecIsMatchModuleKeyCode(QString,QString);
    void remodulePack(QString, QString);
    void rereqPackStatus();
    void remoduleTray4(QString, QString, QString, QString, QString);    //一个托盘码四个模组码上传
    void remoduleTray(QString, QString, QString, QString, QString, QString, QString, QString, QString);    //一个托盘码8个模组码上传
    void remoduleTray16(QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString);    //一个托盘码16个模组码上传
    void resendPackstatus(QString, QString, QString, QString, QString);

    int TIMEOUT;
    int packTypeTemp;
    bool isEmergencyStop;
    QString urlTemp;
    QVariantMap dataMapTemp;
    bool oneTimesRePost;

};

#endif // INTERFACE_H
