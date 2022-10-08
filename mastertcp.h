#ifndef MASTERTCP_H
#define MASTERTCP_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QtNetwork>
#include <QDebug>

class MasterTCP : public QObject
{
    Q_OBJECT
public:
    explicit MasterTCP(QObject *parent = 0);

signals:
    void sendReadRfidToConnectplc();

public slots:
    void masterServerStart();
    void DataReceived();
    void heartTimerFunc();
    void closeTcpServer();
    void recMessage();
    void revRFID_PIN(bool,QByteArray);
    void reRfidData();
    void reRequest();
//    void sendCmdToSalve(QString,QString,int);
private:
    QTcpServer *tcpServer1;
    QTcpSocket *tcpSocket1;
    QThread tcp_thread;

    QByteArray revBuf;
    QByteArray sendBuf;
    QByteArray getFromRfid_toMastertcp;
    QString pin_Code,G9_Code;
    bool socketConnectFlag;
    bool connectFlag;
    int timerCount;
    int timerCount1;
    bool sendPinFlag;
    bool sendFlag;
    bool sendToFlag;
    bool readFlagTemp;
    bool systemStatueFlag;
    bool controlStatueFlag;
    QTimer requestTime;
    bool isReady;

};

#endif // MASTERTCP_H
