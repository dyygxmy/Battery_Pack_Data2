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
class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    void dbconnect();


signals:
    void haverec(int);
    void clo2();
    void sendfromworkthread(QString *);
    void havedconnect(bool);
    void havedconnect(int);
    void send_mainwindow(QString,QString,QString);
public slots:
    void recMessage();
    void DataReceived();
    void fun1();
    void fromNew1thread();
    void xml_parse(QByteArray);
private:
    QTcpServer *tcpServer;
    QTcpSocket *m_pTcpSocket;
    QThread m_thread;
    QByteArray receiveResult;
    QByteArray receiveResultDo;
    QDomNode node;
    QString timeValue;
    QString torqueValue;
    QString angleValue;
    QString gwkStatus;
    QString data_model[11];



};

#endif // WORKER_H
