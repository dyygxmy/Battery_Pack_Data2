#ifndef FISUPDATEQUEUE_H
#define FISUPDATEQUEUE_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
//#include <QStringList>
//#include <QVariantList>
class FisUpdateQueue : public QObject
{
    Q_OBJECT
public:
    explicit FisUpdateQueue(QObject *parent = 0);
    void fis_sql_open();
    void mysql_open();
    void sqlclose();

signals:
    void time_error(bool);
    void sendTime(QString);
    void FisSendSerial(QString); //

public slots:
    void myfistimer();
    void fisupdateFunc();
    void update_column(QString);
    void QueryTime();
private:
    QTimer *fisupdataTimer;
    QSqlDatabase db1;
    QSqlDatabase db2;
    QSqlQuery query1;
    QSqlQuery query2;
    QSqlQuery query3;
    QThread fis_thread;
};

#endif // FISUPDATEQUEUE_H
