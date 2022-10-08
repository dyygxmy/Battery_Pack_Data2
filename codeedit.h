#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QDialog>
#include <qextserial/qextserialport.h>
#include <stdio.h>
#include <QDateTime>
#include <QTimer>
#include <QDebug>

namespace Ui {
class CodeEdit;
}

class CodeEdit : public QDialog
{
    Q_OBJECT

public:
    explicit CodeEdit(QWidget *parent = 0);
    ~CodeEdit();
    void readCmdFunc(int,unsigned char);
    void writeCmdFunc(int,unsigned char, QByteArray);
    QTimer readTimer;
    QTimer RFIDTimer;
    void clearUi();

public slots:
    void init();
    void readKey();
    void slot_read_com();
    void comSendFunc(QByteArray,int);
    void receiveSuccess(QString,QByteArray);
    void RFIDTimeout();
    void setRFIDUI(bool);

private:
    Ui::CodeEdit *ui;
    QextSerialPort *myCom;
    QDateTime cur_time;
    QString strTime;
    QByteArray revRsDataBuf,writeBuf;

    int  revRsDataLen;
    int  revDataAckFlag;
    int  revDataAliveFlag;
    int  midCount;
    int  sendDataLen;

    bool firstFlag;
    bool readFlag;
    bool writeFlag;
    bool writeAckFlag;
    enum keys{noKey,redKey,greenKey,blackKey};
    int tmpKey;
    bool isWrite;
    bool isRFIDConnected;

//    QString hexConvertToStr(QByteArray);

public:
    void judgeKey(keys);

signals:
    void sendRFIDReset();
    void sendRFIDWrite(QByteArray,QByteArray);
    void sendRFIDRead(QByteArray,QByteArray);
    void sendLogin(bool);
    void sendKey(int);

private slots:
    void on_pushButton_black_clicked();
    void on_pushButton_red_clicked();
    void on_pushButton_green_clicked();
    void on_pushButton_read_clicked();
    void on_pushButton_clicked();
    void on_pushButton_RFIDWrite_clicked();
    void on_pushButton_RFIDRead_clicked();
    void on_pushButton_RFIDClear_clicked();
};

#endif // CODEEDIT_H
