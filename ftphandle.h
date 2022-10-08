#ifndef FTPHANDLE_H
#define FTPHANDLE_H

#include <QObject>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QTimer>
#include "GlobalVarible.h"
#include <QSettings>
#include <QDebug>

class FtpHandle : public QObject
{
    Q_OBJECT
public:
    explicit FtpHandle(QObject *parent = 0);

signals:

public slots:
    void ftpStart();
    void mysqlopen();
    void sqlclose();
    void FindFile(QString);
    void parseFile(QString ,QString);
    void ftptimeouts();

private:
    QThread m_thread;
    QSqlDatabase db2;
    QSqlQuery query2;
    QTimer  ftpupload;

};

#endif // FTPHANDLE_H
