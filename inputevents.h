#ifndef INPUTEVENTS_H
#define INPUTEVENTS_H

#include <QObject>
#include <QThread>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <QTimer>
#include <stdio.h>
class InputEvents : public QObject
{
    Q_OBJECT
public:
    explicit InputEvents(QObject *parent = 0);
    
signals:
    void sendconfigwarning(bool);
    void sendbatterysta(bool);
public slots:
    void InputEventStart();
    void mtimerarrve3();

private:
    QThread m_thread;
    QTimer m_timer;
    bool issingle;
    struct input_event t;
    int keys_fd;
    int numcount;
    int keyvalue;
    
};

#endif // INPUTEVENTS_H
