#ifndef DELETECARSQL_H
#define DELETECARSQL_H

#include <QObject>
#include "GlobalVarible.h"
#include <QThread>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

class DeleteCarSQL : public QObject
{
    Q_OBJECT
public:
    explicit DeleteCarSQL(QObject *parent = 0);

signals:

public slots:
    void sql_open();
    void SQL_Delete_Car();

private:
    QThread m_thread;
    QSqlDatabase db1;
    QSqlQuery query1;
};

#endif // DELETECARSQL_H
