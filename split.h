#ifndef SPLIT_H
#define SPLIT_H

#include <QObject>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

//#include <QSettings>
class Split : public QObject
{
    Q_OBJECT
public:
    explicit Split(QObject *parent = 0);
    
    QString &add(int m, const QString &s);
//    QString addzero(int m, QString s);
signals:
    
public slots:
    void mysplit_timer();
    void connection();
    void sendMessage();
    void datas();
    void disConnectDo();
    void displayError(QAbstractSocket::SocketError);

private:
    QThread split_thread;
    QTcpServer *tcpServer;
    QTcpSocket *clientConnection;
    QString data[109];
    QByteArray block;
    QString s;
    QString t;
    QString info_test[7];
    QString status_test[20][5];
    QDataStream *out;
};

#endif // SPLIT_H
