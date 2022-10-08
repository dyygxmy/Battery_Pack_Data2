#ifndef SERIALCOMSREAD_H
#define SERIALCOMSREAD_H

#include <QObject>
#include <QThread>
#include <qextserial/qextserialport.h>
#include <QTimer>
#include <QDebug>
#include <QTime>
#include "GlobalVarible.h"

class SerialComsRead : public QObject
{
    Q_OBJECT
public:
    explicit SerialComsRead(QObject *parent = 0);
    QString vari2_XL;
    QString vari1;
signals:
    void sendCmdToStep(int ,QString,int);
    void sendCodeToPLC(QString);
    void sendCodeToMainwindow(QString,QString);   
    void sendResultTomainwindow(QString,QString,QString,int);
    void sendResultToSQL(QVariant);
private:
    QThread m_thread;
    QextSerialPort *myCom;
    QTime t_time;

    int timerCount;
    int scanCodeCount;
    QByteArray revComBuf;
    bool startScanCodeFlag;
    bool revStartFlag;
    int  timeoutCount;
    QString boltSN;
    QString packSN;
    bool airResultValidFlag;
private slots:
    void sendSerialCMD();
    void slot_read_com3();
    void timerReadComFunc();
    bool comInit();
    void serialComStart();
    void revAutoBarcodeCmd();
    void revManuulCmd(QString,QString,bool);
    void airTestResult(QString);
};

#endif // TIGHTENTHREAD_H

