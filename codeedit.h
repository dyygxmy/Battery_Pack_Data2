#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QDialog>
#include <qextserial/qextserialport.h>
#include <stdio.h>
#include <QDateTime>
#include <QTimer>
#include "messagebox.h"
#include <QDebug>
#include <QVariantMap>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFile>
#include <QTextStream>
#include <QGraphicsOpacityEffect>
#include <QLabel>

#include "GlobalVarible.h"
#include "parsejson.h"
#include "./json/serializer.h"
#include "./json/parser.h"


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

    QGraphicsOpacityEffect *shageGOE;

    QTimer readTimer;
    QTimer RFIDTimer;
    void clearUi();
    void initTableWidget( );
    void initListWidget( ) ;
    void saveToJson(QVariantMap) ;

    QVariantMap map;
    QVariantMap setMap;


public slots:
    void init();
    void readKey();
    void slot_read_com();
    void comSendFunc(QByteArray,int);
    void receiveSuccess(QString,QByteArray);
    void RFIDTimeout();
    void setRFIDUI(bool);
    void saveDataToTable( QVariantMap );
//    void slotrecEncoderFromTight(qint64,qint64,qint64);
    void on_radioBtnIO_clicked();
    void on_radioBtnEn_clicked();
    void on_btnAddList_clicked();
    void on_btnDeleteList_clicked();

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

    int indexRow;
    QString indexItem;

//    QString hexConvertToStr(QByteArray);

public:
    void judgeKey(keys);
    DTMessageBox *mesBox;

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
    void on_btnAdd_clicked();
    void on_btnEncoder_clicked();
    void on_btnInsert_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();
    void on_btnSave_clicked();
    void slotListChange(QString);
    void on_pushButton_rfid_clicked();
    void showShageWidget();

    void slotCloseMesBox();
};

#endif // CODEEDIT_H
