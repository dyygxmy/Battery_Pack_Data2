#ifndef PARSEJSON_H
#define PARSEJSON_H

#include <QObject>
#include <QVariant>
#include <QFile>
#include <QDebug>
#include "GlobalVarible.h"
#include "./json/parser.h"

class ParseJson : public QObject
{
    Q_OBJECT
public:
    explicit ParseJson(QObject *parent = 0);
    QVariantMap parseJson();
    QVariantMap parseJsonbev();
    QVariantMap parseJsonEncoder();

    int stepNameToTypeCode(QString);

signals:

public slots:

};

#endif // PARSEJSON_H
