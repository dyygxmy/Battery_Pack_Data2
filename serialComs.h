#ifndef SERIALCOMS_H
#define SERIALCOMS_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <string.h>
#include <termios.h>
#include <qstringlist.h>
#include "GlobalVarible.h"
class SerialComs : public QObject
{
    Q_OBJECT
public:
    explicit SerialComs(QObject *parent = 0);


signals:
    void sendPartCode(QString);
    void sendCmdToStep(int ,QString,int);
    void sendDebug(QString);
    void sendStationFinishToMaindow(QString,QString);

public slots:
    void comInit();                      //初始化条码枪串口
private:
    QThread m_thread;
    QList<QString> seriallist;//条码链表 存储10个条码以内不能重复
    char buff[20];
    int dev_fd;               //串口文件描述符
    bool isEqual;            //条码是否重复

    int vinLen;
    QByteArray vinHead;
};

#endif // SERIALCOMS_H
