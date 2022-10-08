#ifndef INOUTPUT_H
#define INOUTPUT_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtNetwork>

class InOutput : public QObject
{
    Q_OBJECT
public:
    explicit InOutput(QObject *parent = 0);

    bool readIOInput(QString);
    void writeIOOutput(QString,bool);

signals:

public slots:
};

#endif // INOUTPUT_H
