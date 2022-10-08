#ifndef RFIDCONTROLER_H
#define RFIDCONTROLER_H

#include <QObject>
#include <QThread>
#include <QtNetwork>
#include "GlobalVarible.h"
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDomDocument>

class RfidControler : public QObject
{
    Q_OBJECT
public:
    explicit RfidControler(QObject *parent = 0);
    QString rfidip;
    int rfidport;

signals:
    void sendPinToMasterTcp(bool,QByteArray); //wz add
    void sendPinToUi(QString,bool,QString); //发送条码给UI
    void rfidConnected(bool);
    void RFIDSuccess(QString,QByteArray);
    void RFIDDataToPLC(QString,QByteArray);
    void sendRequest();

public slots:
    void rfidInit();
    void pingTimers();
    void receivemss();
    void newConnect();
    void displayError(QAbstractSocket::SocketError);
    void connectedDo();
    void disconnectedDo();
    void rfidDisconnectDo(bool);
    void write_data(QByteArray,QByteArray);
    void read_data(QByteArray,QByteArray);
    void sendReset();

private:
    QThread m_thread;
    QTcpSocket *m_pTcpSocket;

    QTimer pingTimer;
    QString tempPin;
    QString tempG9;
    QByteArray dataTemp;
    bool RFIDIsConnect;

    bool readRFIDFlag;
    int count1;

    QXmlStreamReader reader;
    void parseReplyElement(const QDomElement &element);
    void parseReadElement(const QDomElement &element);
};

#endif // RFIDCONTROLER_H
