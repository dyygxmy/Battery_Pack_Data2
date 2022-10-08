#ifndef SQLTHREADSVW2_H
#define SQLTHREADSVW2_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include "GlobalVarible.h"
#include <QThread>
class SqlThreadSVW2 : public QObject
{
    Q_OBJECT
public:
    explicit SqlThreadSVW2(QObject *parent = 0);

public slots:
    void sqlinit();
    void mysqlopen();
    void sqlclose();
    void sqlinsert(QVariant);
    void insertKeyCodeInfo(int,int,QString,QString,QString,QString);
    void receiveNokAll(int);
    void configOne(QString,QString,QString,int);
public:
    QThread m_thread;
    QSqlDatabase db2;
    //    QSqlDatabase db3;
    //    QSqlQuery query1;
    QSqlQuery query2;
    bool isFirst;
    //QString controlType;
signals:
    void send_mysqlerror();


};

#endif // SQLTHREADSVW2_H
