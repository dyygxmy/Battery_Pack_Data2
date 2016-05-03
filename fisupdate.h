#ifndef FISUPDATE_H
#define FISUPDATE_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
//#include <QStringList>
//#include <QVariantList>
class FisUpdate : public QObject
{
    Q_OBJECT
public:
    explicit FisUpdate(QObject *parent = 0);
    void fis_sql_open();
    void mysql_open();
    void sqlclose();

signals:
    void time_error(bool);
    void sendTime(QString);
    
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

#endif // FISUPDATE_H
