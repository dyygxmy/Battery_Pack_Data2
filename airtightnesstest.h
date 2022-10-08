#ifndef AIRTIGHTNESSTEST_H
#define AIRTIGHTNESSTEST_H

#include <QObject>
#include <QDebug>
//#include <unistd.h>
#include <fcntl.h>
//#include <getopt.h>
//#include <string.h>
#include <termios.h>

class airTightnessTest : public QObject
{
    Q_OBJECT
public:
    explicit airTightnessTest(QObject *parent = 0);


private:
    void comInit();
    int dev_fd;               //串口文件描述符
    int resultLen;
    bool isEqual;            //条码是否重复

    QString strKanNum;
    QString strProgramnum;
    QString strVL;
    QString strPM;
    QString strResult;
signals:

public slots:

};

#endif // AIRTIGHTNESSTEST_H
