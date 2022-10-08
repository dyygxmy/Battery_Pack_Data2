#ifndef STEPCONTROL_H
#define STEPCONTROL_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "inoutput.h"
#include <QSettings>
#include <QDateTime>
#include "parsejson.h"
#include "inputmodule.h"
#include "GlobalVarible.h"
#include "counttime.h"

//Q_DECLARE_METATYPE(PACK_STATUS_STEP)

class StepControl : public QObject
{
    Q_OBJECT
public:
    explicit StepControl(QObject *parent = 0);

    bool CS351StatusFlag;
    bool gostRunFlag;
    QString vari2;
    QString vari1;
    QString IOModuleIP;
    QString stepPack1SNTemp;
    QString stepPack2SNTemp;
    QString GPackTypeTemp;

signals:
    void sendGetStationTimer(QString,int,QString);
    void sendNewCoordinates(int);
    void sendCodeToTightening(QString,bool,QString);
    void sendTypeToPLC(int,int);
    void sendAddTrayPackType(QString);
    void sendgetTrayPackType(QString);
    void sendAddStationTimer(QString,int);
    void sendReqModuleTray(QString,QString,QString,QString,QString);
    void sendReqModuleTray(QString,QString,QString,QString,QString,QString,QString,QString,QString);
    void sendReqModuleTray(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
    void sendReqPackStatus();
    void sendgetStationkeyCode(QString,QString);
    void sendIsMatchModuleKeyCode(QString,QString);
    void sendInitEncoder();
    void sendReqTightStatus(QString,QString,QString);
    void sendReqStationState(QString,QString,QString,QString,QString);
    void sendBarcodeDisplay(QString,QString);
    void sendReqKeybarcode(int,int,QString,QString,QString,QString);
    void sendCodeToSerialcom(QString,QString,bool);
    void sendReadIOModuleCmd(QString,bool);
    void sendUpInAndOutStation(int , QString , QString , QString , QString);
    void sendReqEmergencyStop(bool);
    void sendUIEmergencyStop(bool);
    void sendReqModulePack(QString,QString);
    void sendModuleClean();
    void sendRequestAgvLeave();
    void sendSuspend();
    void sendLEDFlashAllOFF(int , QString,bool);
    void sendPackStartSignal();
    void sendPackEndSingal(QString,QString,QString,QString,QString,QDateTime,QDateTime,QDateTime);
    void sendM020ShowToMainwindow();
    void sendRepairBatterypack(QString);
    void sendGetTrayAndModuleStatus(QString,int);
    void sendBMCETrayClean(QString,QString);

public slots:
    void stepSelect();
    void stepStart();
    void timerflick();
    void revCMDFunc(int,QString,int);
    bool jumpStepFunc(int,int,int);
    void revPackStatus(QVariant);
    void revGetStationkeyCodeResult(bool);
    void revreturngetTrayPackType(QString);
//    void revModuleTray(bool);
    void revKeyStatus(bool);
    void recBMCEFirstStatus(QVariant);
    void revMoveButton();
    void revCS351Status(bool);
    void revIOModuleValue(int);
    void revStopButtonSignal(bool);
//    void revEmergencyStop(bool);
    void informEmergencyStop(bool);
    void revStationState(bool);
    void revAgvLeaveState(bool);//wz
    void readStepParaInit();
    void getErrorGo(int);
    void revRFIDStepInit();
    void getErrorContinue();
    void recEKSatatueEvent(int);
    void sendDrectGoSlot();
    void revTrayAndModuleStatusResoult(int);
    void receiveStationTimer(QString,QString);
    void receiveTestResult(QString);
    //void revAgvLeaveState(bool);
private:
    QThread step_Thread;
    InOutput *IOput;
    ParseJson * parsejson;
    QTimer timeFlicker;
    void stationFinishPass();
    bool changeOKPressureFlag;//气压状态OK 有变化
    bool changeNOKPressureFlag;//气压状态NOK 有变化
    bool receiveCountTime;//收到涂胶记录时间
    bool getStationTimerFlag;//获取涂胶时间
    countTime cT;
    bool EOT;//泄漏完成专用标志
    bool isStepTest;//有到过泄漏步骤
    bool forcedRelease;//是否插EKS钥匙强制放行的
    bool countIsStart;//涂胶计时开始
    bool timeFull;//涂胶满足冷却时间
    bool colorTrue;
    int allStep; //all step
    int stepNum; //步骤号
    int stepType;//步骤类型
    int packNum; //任务个数
    int codeNum; //当前扫码规则的扫码次数
    int timerCount;
    int timerCount1;
    int timerCount2;
    bool m020ShowMaindownFlag;
    bool AddStationTimerFlag;   //添加电池包工位计时
    bool startYellowFlash;
    bool startGreenFlash;
    bool holdOnFlag;
    bool needReScaning;
    int holdOnCount;
    QString GPack1SNTemp;
    QString GPack2SNTemp;
    QStringList scanList;//当前扫码规则下所有的扫码数据
    QStringList scanList1;//存储激光码 只是为了判断条码个数
    QStringList scanList2;//存储印刷码 只是为了判断条码个数
    QString BarcodePdm;
    bool PDMShowfalg;
//    bool scanIsSamed;
    QString recCMDBuf;
    QString codeBuf[20];//存储多条条码，用来存储本地或上传接口
    QString tightenResult;
    QString vinCode;
    QString vinCodeTemp;
    QString vinCodeTempVM;
    bool allowPackType;
    bool trayLineFlag;
    bool trayReadyFlag;

    //QDateTime timeStart;
    QDateTime timeStartPack1;//操作PACK1时间
    QDateTime timeStartPack2;//操作PACK2时间
    QDateTime timeEnd;

    bool tempVariant;
    bool temp111;
    bool temp222;
    bool temp333;
    bool tightStartFlag;
    bool stepType1Flag;//receive code Flag
    bool stepType2Flag;//receive tightening finished Flag
    bool stepType3Flag;//read IO input status
    bool getCodeFlag;//get code Flag;
    bool isFirstPackflag;//是否在操作PACK1
    bool tighteningFinishFlag;
    bool PLCConnectFlag;//plc connect flag
    bool PLC_Handle_Flag;
    bool displayPictureFlag;
    int  displayCount;
    int  airProgram;
    int IO_Module_Value;

    QString stepPackType;//product type
    QString stepPack1SN;
    QString stepPack2SN;
    QString bmceFrontStatus;
    QString stepPack1Status;
    QString stepPack2Status;
    QString oldPackSN;
    bool stepGetProductTypeFlag;
    bool getTrayPlaceReadyFlag;

    bool reqScanBarcodeFlag;
    bool keyStatusFlag;
    bool moveButtonFlag;//屏幕上的放行按钮有点击
    bool stationFinishFlag;//控制显示站完成界面
    bool airTestFlag;
    bool airTestFinish;
    bool waitIOInputFlag;
    bool stopValueFlag;
    bool reqAckFlag;
    bool isPopUpFlag;
    bool sendStationFinishFlag;
    int count1;
    QVariantList stepList;
    void stepParaBufInit(QVariantMap Step);
//    int stepNameToTypeCode(QString stepName);
    int conditionToCode(QString condition);
    bool packStatusOk(QString,QString,QString,QString);
    bool handleJumpStation(QString);
    int jumpPack1();
    QString RFIDCode;//记录PLC发来的托盘号
};

#endif // STEPCONTROL_H
