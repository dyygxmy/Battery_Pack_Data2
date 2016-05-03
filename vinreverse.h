#ifndef VINREVERSE_H
#define VINREVERSE_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QTimer>
#include "GlobalVarible.h"
class VinReverse : public QObject
{
    Q_OBJECT
public:
    explicit VinReverse(QObject *parent = 0);
    
signals:
    
public slots:
    void newconnects();
    void revNexoData();
//    void haveconnect(bool);
    void nexohearts();
    void receiveVin(QString);
    void newConnect();
    void connectTimer();
    void connectDo();
    void disconnectdo();

private:
    QTcpSocket *m_pTcpSocket;
    QThread m_thread;
    QTimer  nexoheart;
    QByteArray sendBuf;
    bool isNexoConnect;
    QString nexo_Vin;
    QTimer connect_Timer;
    int CS351_4545count;
    
};

#endif // VINREVERSE_H
