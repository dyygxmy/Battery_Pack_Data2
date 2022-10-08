#ifndef REINSERTSERVERDBNINGBO_H
#define REINSERTSERVERDBNINGBO_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include "GlobalVarible.h"

class ReInsertServerDbNingBo : public QObject
{
    Q_OBJECT
public:
    explicit ReInsertServerDbNingBo(QObject *parent = 0);
//    void datasqlopen();
//    void curvesqlopen();
//    void mysqlopen();
//    void sqlclose();

signals:

public slots:
    void reinsert_start();
    void timer_insert();
//    void printDead();
private:
    QThread m_thread;
   // QTimer  m_insert;
//    QSqlDatabase db1;
//    QSqlDatabase db2;
//    QSqlDatabase db3;
//    QSqlQuery query1;
//    QSqlQuery query2;
//    QSqlQuery query3;
//    QSqlQuery query4;

};

#endif // REINSERTSERVERDBNINGBO_H
