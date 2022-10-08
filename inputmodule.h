#ifndef INPUTMODULE_H
#define INPUTMODULE_H

#include <QObject>
#include <QDebug>
#include <QtNetwork>

class inputModule : public QObject
{
    Q_OBJECT
public:
    explicit inputModule(QObject *parent = 0);
    QString IO_Module_IP;

private:
    QThread IO_thread;
    QTcpSocket *IO_Module_Socket;
    bool IOModuleConnectFlag;
    int timerCount;
    int IO_Value;

signals:
    void sendIOValueToStep(int);
public slots:
    void IO_Module_Init();
    void IO_Module_Connect();
    void revIOModuleDataFunc();
    void IOModuleDisConnect();
    void IOModuleTimerFunc();
    void readIOModuleStatus();
//    void revReadIOCMD(QString,bool);

};

#endif // INPUTMODULE_H
