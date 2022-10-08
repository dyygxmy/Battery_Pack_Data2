#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "GlobalVarible.h"
//#include <QtGui>

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit PaintArea(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void setQuery(QSqlQuery,QSqlQuery,QSqlQuery);
//    void mysqlopen();

signals:

public slots:

private:
//    QString condition;
    QSqlDatabase db;
    QSqlQuery query_number;
    QSqlQuery query_datas;
    QSqlQuery query_bound;
    double datas[10000];
    double max;
    double min;
    double ave;
    double _ma;//数组里的最大值

};

#endif // PAINTAREA_H
