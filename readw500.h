#ifndef READW500_H
#define READW500_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QtNetwork>

class READW500 : public QObject
{
    Q_OBJECT
public:
    explicit READW500(QObject *parent = 0);
    QString W500IP;

signals:
    void sendTighteningResult1(int,int);
    void sendTighteningResult2(int,int);
public slots:
    void W500Connect();
    void W500DisConnect();
    void W500ReceiveDataFunc();
    void W500TimerFunc();
    void W500Start();
    void revW500StartCmd(int,QString);
    void readW500Cmd();
    void writeW500Cmd(int,QByteArray);
private:
    QTcpSocket *W500_Socket;
    QThread W500_Thread;

    bool W500ConnectFlag;
    bool reqW500ResultFlag;
    int  timerCount;
    int  IP_Right;
    int  channelNum;
    bool writeSNFlag;
    QString snBuf;
};

#endif // READW500_H
