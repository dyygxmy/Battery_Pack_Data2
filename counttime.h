#ifndef COUNTTIME_H
#define COUNTTIME_H

#include <QObject>
#include <QThread>

class countTime : public QObject
{
    Q_OBJECT
public:
    explicit countTime(QObject *parent = 0);

signals:

public slots:
    void countStart();
    void countStop();
private:
    QThread thread;
    bool stopFlag;
private slots:
    void countManage();
};

#endif // COUNTTIME_H
