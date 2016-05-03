#ifndef RFIDCONTROLER_H
#define RFIDCONTROLER_H

#include <QObject>
#include <QThread>
#include <QtNetwork>
#include "GlobalVarible.h"
#include <QSettings>
class RfidControler : public QObject
{
    Q_OBJECT
public:
    explicit RfidControler(QObject *parent = 0);

signals:
    void sendPinToUi(QString,bool,QString); //发送条码给UI
    void sendGetCar();
    void rfidConnected(bool);
public slots:
    void rfidInit();
    void pingTimers();
    void receivemss();
    void newConnect();
    void displayError(QAbstractSocket::SocketError);
    void connectedDo();
    void disconnectedDo();
    void rfidDisconnectDo(bool);
    void pinIsEqual();
private:
    QThread m_thread;
    QTcpSocket *m_pTcpSocket;
    QList<QString> seriallist;//条码链表 存储10个条码以内不能重复
    QList<QString> pinqueuelist;
    int whichindex;
    bool isequeal;
    QTimer pingTimer;
    QString rfidip;
    QString tempPin;
    QString tempG9;
    int rfidport;
    bool RFIDISConnect;

};

#endif // RFIDCONTROLER_H
