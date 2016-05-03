#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GlobalVarible.h"
#include <QSettings>
#include "newconfiginfo.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include "save.h"
#include "vinreverse.h"
#include <QTextCodec>
//#include "datamodel.h"
#include "deletecarsql.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
public:
    void searchVinNum();                          //匹配vin
    void searchG9Num();                           //匹配G9
    void judge();
    int VIN_VerifyFunc(char *buf);                //vin码校验
    bool pinCodeVerify(QByteArray ,int);          //pin码校验
    bool pinCodeRightVerify(QString ,int);
    void VinPinCodeView(bool flags,int whichcars);//vin or pin匹配函数
    void Start(int interval, int maxValue);
    void Stop();
    void red_led(int);
    void green_led(int);
    void yellow_led(int);
    void white_led(int);
    void nok_led(int);
    void mysqlopen();
    void mysqlclose();
    void sendWebValue(int states,QString namepdm);

public slots:

    void fromsecondthreaddata(QString,QString,QString);
    void getSerialNum(QString,bool,QString);
    void connectMysql();
    void init();
    void wifishow(bool);
    void datashow(bool);
    void batteryshow1(QString);
    void batteryshow2(bool);
    void time_warning(bool);
    void ReceGunNotReady();
    void battery15();

public :
signals:
    void clo();
    void clo1();
    void sendoperate();
    void sendfromsecondthread(QString ,QString,QString);
    void sendConfigureAll(int isoption,int whicharis,int whichpro,int whichoptionis);
    void sendnexo(QString);
    void sendDeleteCar_VIN(QString);
    void SQL_deleteCar(QString,QString);

public slots:
    void TightenIsReady(bool);
    void receiveOperate();
    void timerDelay();
    //void on_pushButton_11_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_delete_clicked();
    void on_pushButton_13_clicked();
    void on_pushButton_14_clicked();
    void on_pushButton_15_clicked();
    void on_pushButton_0_clicked();
    void ShowTime();
    void UpdateSlot();
    void PdmFlicker();//pdm flicker
    void on_pushButton_18_clicked();
    void on_pushButton_reset_clicked();
    void receiveCloseInput(bool);
    void configwarning(bool);
    void taotong_main(int);
    void shutdown(int);
    void clocked();
    void setRfidState(bool);
    void delete_car(bool);

private slots:
    void on_pushButton_17_clicked();
    void signal_mysqlerror_do();
    void on_pushButton_shutdown_clicked();
    void ReceFisSerial(QString);
    void FisTimerDo(); //当前条码打完 延时
    void UpdateSqlFlag();
    void receiveGetCar();
    void resetUiDo();//RFID模式 ING 状态确认

    void on_pushButton_16_clicked();

private:
    Ui::MainWindow *ui;
    QString message;
    QString serialNums;
    //QString tempPin;
    QString tempG9;
    QByteArray Data_Xml_Txheart;
    int isFull;
    QString temp;
    QString PDM_PATH;
    int person;
    int optionOrNot;  //是否选配
    int whichar;  //匹配出来的是哪个车型
    int enableLsnumber;
    int whichpronumis;//当前程序号
    int whichoption;
    QSqlDatabase db;
    QSqlQuery query;
    QSqlRecord record;
    QSqlField field;
    int  equeloptionbnum;
    int  equeloptionknum;
    QString selectVin;//sql
    int bxuannumtmp;
    QString PDMCurrentState;
    bool pdmnowromisOk;
    int m_CurrentValue;         //当前值
    int m_UpdateInterval;       //更新间隔
    int m_MaxValue;             //最大值
    QTimer m_Timer;
    QTimer timerpdm;
    //QTimer timerdelay;
    QTimer shutdown_timer;
    QTimer timer_showdown;
    QTimer FisTimer;
    int numpdm;
    QPushButton *butt[50];
    QLabel *label1[50];
    QLabel *label2[50];
    int tempnumdpm; //螺丝个数
    bool pdmflicker; //闪烁状态
    int whichpdmnumnow; //当前第几个闪烁
    int Tacktime;
    int ScrewWhichExit;
    int battry_num;


public:
    Newconfiginfo * newconfiginfo;
    VinReverse * vinreverse;
    DeleteCarSQL * deletecarsql;
    QGraphicsOpacityEffect *e3;
    Save * save;
    bool ConfigOneOrAll;



};

#endif // MAINWINDOW_H
