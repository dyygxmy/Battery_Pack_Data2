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
//#include <QSharedMemory>
//#include <QBuffer>

#include "./json/parser.h"
#include "inoutput.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString vari1;
    QString vari2;
    QString stationName;
    QString stationWork;
    int vinAttributeBit;
    int Tacktime;
    bool isGostRun;

public:
    void searchVinNum();                          //匹配vin
    void searchG9Num();                           //匹配G9
    int VIN_VerifyFunc(char *buf);                //vin码校验
    bool pinCodeVerify(QByteArray ,int);          //pin码校验
    bool pinCodeRightVerify(QString ,int);
    void VinPinCodeView(bool flags,int whichcars);//vin or pin匹配函数
    void progressBarStart(int interval, int maxValue);
    void progressBarStop();
    void mysqlopen();
    void mysqlclose();
    void showhome();

    void newconfigInit(Newconfiginfo *newconfiginfo);
    void searchJob(QString);

public slots:
    void fromsecondthreaddataAirTest(QString,QString,QString,int);
    void fromsecondthreaddataSB356(QString,QString,QString,int,int);
    void fromsecondthreaddata(QString,QString,QString,int,int);
    void getSerialNum(QString,bool,QString);
    void connectMysql();
    void mainWindowInit();
    void initBack();
    void wifishow(bool);
    void datashow(bool);
    void batteryshow1(QString);
    void closeSave();
    void VinPinCodeViewSB356(QString,int,int);
    void EKSStausChange(int);
    void onShowState(QString);
    void onGetSuspend();
    void onLEDCtrl();
    void onLEDFlash_r();//红闪
    void onLEDFlash_g();//绿闪
    void onLEDFlash_y();//黄闪
    void onLEDFlashAllOFF(int ,QString,bool);
    void revM020FromStep();

//    void tighteningResultDisplay();
signals:
    void closeThread();
//    void sendoperate();
    void sendOperate1(bool,int);
    void sendOperate2(bool,int);
    void sendOperate3(bool,int);
    void sendOperate4(bool,int);
    void sendToPlcReadyClicked();
    void sendOneGroupNok(QString,QString *,QString *);
    void sendfromsecondthread(QString,QString,QString,int);
    void sendConfigureAll(int isoption,int whicharis,int whichpro,int whichoptionis);
    void sendNokAll(int);
    void sendnexo(QString);

    void sendInfo(bool,bool,int,int);
	void killProcess();

    void sendCmdToStep(int ,QString,int);
    void sendM020ToPost(QString);

    void send_TT_IO_value(int,int,int);
    void send_IO_Reset();
    void sendChangeEmergencyStop();
//    void requestJob(QString);

    void sendToInter(QString);
    void sendMoveButtonToStep();
    void sendCS351StatusToStep(bool);
    void sendStepResetEmergencyStop(bool);

    void sendErrorGo(int);
    void sendErrorContinue();
    void sendEKSStatusEvent(int);
    void sendRFIDRead();
    void sendDrectGo();
    void sendRollLable(QString);

public slots:
    void TightenIsReady(bool);
    void recEncoderFromTight(qint64,qint64);
    void PLCIsReady(bool);
    void receiveOperate();
    void timerDelay();
    void revm020FromInterface(QString,QString,QString,QString);
    void ShowTime();
    void UpdateSlot();
    void PdmFlicker();//pdm flicker
//    void on_pushButton_18_clicked();
    void on_pushButton_reset_clicked();
    void receiveCloseInput(bool);
    void configwarning(bool);
    void taotong_main(int);
//    void shutdown(int);
    void setRfidState(bool);
    void showEmergencyStop(bool);
    void resetEmergencyStop(bool);
    void directShowHome();



private slots:
    void signal_mysqlerror_do();
//    void on_pushButton_shutdown_clicked();

    void resetUiDo();//RFID模式 ING 状态确认


    void send_Info();


    void on_pushButton_18_clicked();

    void gunPower(bool);   //条码枪通电断电

    void displayScanBarcode(QString,QString);


    void on_pushButton_16_clicked();


    void on_pushButton_errorGo_clicked();

    void on_pushButton_errorContinue_clicked();

    void on_pushButton_errorContinue_2_clicked();

    void on_pushButton_deep1_clicked();

    void on_pushButton_deep2_clicked();

    void on_pushButton_deepUp_clicked();

    void on_pushButton_deepDown_clicked();

    void on_pushButton_readRFID_clicked();

    void on_pushButton_Go_clicked();

    void on_pushButton_M020SendPost_clicked();

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
    int whichoption;
    QSqlDatabase db;
    QSqlQuery query;
    QSqlQuery query1;
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
    QTimer LEDTime;
    QTimer FlashTime_r;//红闪
    QTimer FlashTime_g;//绿闪
    QTimer FlashTime_y;//黄闪
    //QTimer timerdelay;
//    QTimer timer_showdown;
    QTimer FisTimer;
    QTimer timer_Info;
    QPushButton *butt[50];
    QLabel *label1[50];
    QLabel *label2[50];
    int huCh1,huCh2,huCh3,OKCount,recCount;
    bool isTestOverFlag;
    enum keys{noKey,redKey,greenKey,blackKey};


    int tempnumdpm; //螺丝个数
    bool pdmflicker; //闪烁状态
    int whichpdmnumnow; //当前第几个闪烁
    int ScrewWhichExit;
    int battry_num;
    int bolt_index;//一组螺栓的最后一个
    QTimer queue_timer;
    bool controlMode;   //false 自动  true 手动

    QString VINhead;


    int PDMBoltNum;
    int currentBoltSum;
    int currentBoltNum;
    int currentFirstBolt;

    int groupAllBoltNumCh;
    int systemStatus;
    int preSystemStatus;

    QString vinAttributeCode;
    int linkCount;
    bool isRFIDConnected;
	int pdmflickerNum;
    bool nokBoltFlag;
    bool ttChangeFlag;
    bool isFirst;   //是否第一次连上
//    QSharedMemory sharedMemory;

    bool isSaveShow;
    bool enTaotongFlag;
    bool enBarcodeFlag;
    bool enIOCtlFlag;

    int  taotong_Value;
    int  IO_value;
    bool barcode_Value;
    int  taotongEnValue;

    int airTestStatus;
    int airButtonNum;
    QString packSN;
    bool isEmergencyStop;
    bool LEDIsON_r;
    bool LEDIsON_g;
    bool LEDIsON_y;

public:
    Newconfiginfo * newconfiginfo;
    VinReverse * vinreverse;
    QGraphicsOpacityEffect *e3;
    Save * save;
    InOutput *In_Output;
    bool ConfigOneOrAll;

};

#endif // MAINWINDOW_H
