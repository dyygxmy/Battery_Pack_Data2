#ifndef TIGHTENTHREAD_H
#define TIGHTENTHREAD_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include "GlobalVarible.h"
#include <QtNetwork>
#include <QTime> //10:23 179m
class TightenThread : public QObject
{
    Q_OBJECT
public:
    explicit TightenThread(QObject *parent = 0);
    QString ctrlType;
    QString ctrlIP;
    QString testPro;

signals:
    void IsTigntenReady(bool);
    void ReconnectSendOperate();
    void disconnectTellPortB();
    //void sendStates(QString);
public slots:
    void tightenStart();
    void newConnect(); //连接服务器
    void displayError(QAbstractSocket::SocketError);
    void recsocket();
    void receivehavedconnect(bool);
    void receivedisconnect();
    void sendReadOperate(bool,int);
    QString getHeaderFunc(int,int,int);

    void sendCmdMessage(int);  //发送数据
    void sendCmdToCs351(int);
    void portBSendPortA();
    QString sendGroupEnable(QString);
private:
    QThread m_thread;
    QTcpSocket *tcpSocket;
    QTimer *sendheartt;
    QTimer *timer351;
    QMutex mutex;
    QString xmlhead;
    QString enableTemp;
    QString aliveTemp;
    QString resetTemp;
    QString resetTemp_timewrong;
    QString disableTemp;
    int sendCysleID;
    int cycleId;  //循环号
    int config351count;
    bool port4700LinkFlag; //cs351 4700 port connect flag
    bool port4700DisconnectFlag;
    bool cs351InitFlag;
    int  timerCount;
    int  timerCount1;
    bool sendEnableFlag;
    bool enableFlag;

    QString xml_Grs;
    QString xml_Grs_KNR;
    QString xml_TOL;
    QString xml_STR;
    QString KNR_Temp;
    QString strChannel;
    QString funcSwitchChannel(QString);
    bool handleRecvStatusMess(QByteArray,QString,int);

};

#endif // TIGHTENTHREAD_H
