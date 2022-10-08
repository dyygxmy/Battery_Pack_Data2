#ifndef UPDATETIME_H
#define UPDATETIME_H

#include <QThread>

class updateTime : public QThread
{
    Q_OBJECT
public:
    explicit updateTime(QObject *parent = 0);

signals:

public slots:

private slots:
    void updateTimeFunc();
protected:
    void run();

};

#endif // UPDATETIME_H
