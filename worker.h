#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QtNetwork>
#include <QDomDocument>
#include <QtCore/QCoreApplication>
#include <QSqlDatabase>
#include <QDebug>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include "GlobalVarible.h"
#include <QSettings>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    QString ctrl_Type;


signals:
    void haverec(int);
    void clo2();
    void sendfromworkthread(QVariant);
    void havedconnect(bool);
    void disconnectTellPortA();
    void send_mainwindowSB356(QString,QString,QString,int,int);
    void send_mainwindow(QString,QString,QString,int,int);
    void sendTighteningResult1(int,int);
    void sendTighteningResult2(int,int);

public slots:
    void recMessage();
    void DataReceived();
    void fun1();
    void closePortBTcpServer();
    void xml_parse(QByteArray);
    void heartTimerFunc();
    void portAsendPortB();
    void revChannleEnFunc(int,int,QString);
    void revbuttonFunc();
private:
    QTcpServer *tcpServer;
    QTcpSocket *m_pTcpSocket;
    QTimer *heartTimer;
    QThread m_thread;
    QByteArray receiveResult;
    QByteArray receiveResultDo;
    QDomNode node;
//    QString data_model[11];
    QString CurveStr;
    int timerCount;
    QString CyclePrevious;
    QString channelPrevious;
    bool socketFlag4710;
    int channelData;
    bool dutouFlag;
    bool moduleFlag;
    int  tightenNum;
    QString pack_SN;
//    QString data_model[15];

};

#endif // WORKER_H
