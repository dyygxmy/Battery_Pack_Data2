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
    bool changeOKPressureFlag;//????????????OK ?????????
    bool changeNOKPressureFlag;//????????????NOK ?????????
    bool receiveCountTime;//????????????????????????
    bool getStationTimerFlag;//??????????????????
    countTime cT;
    bool EOT;//????????????????????????
    bool isStepTest;//?????????????????????
    bool forcedRelease;//?????????EKS?????????????????????
    bool countIsStart;//??????????????????
    bool timeFull;//????????????????????????
    bool colorTrue;
    int allStep; //all step
    int stepNum; //?????????
    int stepType;//????????????
    int packNum; //????????????
    int codeNum; //?????????????????????????????????
    int timerCount;
    int timerCount1;
    int timerCount2;
    bool m020ShowMaindownFlag;
    bool AddStationTimerFlag;   //???????????????????????????
    bool startYellowFlash;
    bool startGreenFlash;
    bool holdOnFlag;
    bool needReScaning;
    int holdOnCount;
    QString GPack1SNTemp;
    QString GPack2SNTemp;
    QStringList scanList;//??????????????????????????????????????????
    QStringList scanList1;//??????????????? ??????????????????????????????
    QStringList scanList2;//??????????????? ??????????????????????????????
    QString BarcodePdm;
    bool PDMShowfalg;
//    bool scanIsSamed;
    QString recCMDBuf;
    QString codeBuf[20];//??????????????????????????????????????????????????????
    QString tightenResult;
    QString vinCode;
    QString vinCodeTemp;
    QString vinCodeTempVM;
    bool allowPackType;
    bool trayLineFlag;
    bool trayReadyFlag;

    //QDateTime timeStart;
    QDateTime timeStartPack1;//??????PACK1??????
    QDateTime timeStartPack2;//??????PACK2??????
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
    bool isFirstPackflag;//???????????????PACK1
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
    bool moveButtonFlag;//?????????????????????????????????
    bool stationFinishFlag;//???????????????????????????
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
    QString RFIDCode;//??????PLC??????????????????
};

#endif // STEPCONTROL_H
