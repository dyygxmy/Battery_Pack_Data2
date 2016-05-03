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
    
signals:
    void IsTigntenReady(bool);
    void ReconnectSendOperate();
    void GunNotReady();
public slots:
    void tightenStart();
    void newConnect(); //连接服务器
    void sendMessage();  //发送数据
    void sendheart();
    void displayError(QAbstractSocket::SocketError);
    void recsocket();
    void getfile(unsigned short type);//打开xml文件
    void receivehavedconnect(bool);
    void receivedisconnect();
    void cs351_header_func(unsigned short mid,unsigned short size,unsigned  short type, char *buf);
    void number_change_ascii(unsigned int  num,unsigned char count,unsigned int div, char *ascii_buf);
    void sendReadOperate();
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
    QByteArray Data_Xml_Tx;
    int flag;  //复位
    int cycleId;  //循环号
    int isreset;
    int isSendHeart;
    int config351count;
    bool log351In;
    bool log351out;
    bool isconnected;
    bool isdisconnect;

};

#endif // TIGHTENTHREAD_H
