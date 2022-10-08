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
    void sendCodeToTightening(QString,bool,QString);
    void sendTypeToPLC(int,int);
    void sendReqModuleTray(QString,QString,QString,QString,QString);
    void sendReqModuleTray(QString,QString,QString,QString,QString,QString,QString,QString,QString);
    void sendReqPackStatus();
    void sendReqTightStatus(QString,QString,QString);
    void sendReqStationState(QString,QString,QString,QString,QString);
    void sendBarcodeDisplay(QString,QString);
    void sendReqKeybarcode(QString,QString,QString,int);
    void sendCodeToSerialcom(QString,QString,bool);
    void sendReadIOModuleCmd(QString,bool);
    void sendReqEmergencyStop(bool);
    void sendUIEmergencyStop(bool);
    void sendReqModulePack(QString,QString);
    void sendModuleClean();
    void sendRequestAgvLeave();
    void sendSuspend();
    void sendLEDFlashAllOFF(int , QString,bool);
public slots:
    void stepSelect();
    void stepStart();
    void timerflick();
    void revCMDFunc(int,QString,int);
    bool jumpStepFunc(int,int,int);
    void revPackStatus(QVariant);
    void revModuleTray(bool);
    void revKeyStatus(bool);
    void revMoveButton();
    void revCS351Status(bool);
    void revIOModuleValue(int);
    void revStopButtonSignal(bool);
    void revEmergencyStop(bool);
    void informEmergencyStop(bool);
    void revStationState(bool);
    void revAgvLeaveState(bool);//wz
    void readStepParaInit();
    void getErrorGo();
    void revRFIDStepInit();
    void getErrorContinue();
    void recEKSatatueEvent(int);
    void sendDrectGoSlot();
    //void revAgvLeaveState(bool);
private:
    QThread step_Thread;
    InOutput *IOput;
    ParseJson * parsejson;
    QTimer timeFlicker;

    bool colorTrue;
    int allStep; //all step
    int stepNum; //步骤号
    int stepType;//步骤类型
    int packNum; //任务个数
    int codeNum; //code number
    int timerCount;
    int timerCount1;
    int timerCount2;
    bool startYellowFlash;
    bool startGreenFlash;
    QString recCMDBuf;
    QString codeBuf[10];
    QString tightenResult;
    QString vinCode;
    bool trayLineFlag;
    bool trayReadyFlag;

    bool tempVariant;
    bool temp111;
    bool temp222;
    bool temp333;
    bool tightStartFlag;
    bool stepType1Flag;//receive code Flag
    bool stepType2Flag;//receive tightening finished Flag
    bool stepType3Flag;//read IO input status
    bool getCodeFlag;//get code Flag;
    bool isFirstPackflag;
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
    QString stepPack1Status;
    QString stepPack2Status;
    QString oldPackSN;
    bool stepGetProductTypeFlag;
    bool getTrayPlaceReadyFlag;

    bool reqScanBarcodeFlag;
    bool keyStatusFlag;
    bool moveButtonFlag;
    bool stationFinishFlag;
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
    int jumpPack1();
};

#endif // STEPCONTROL_H
