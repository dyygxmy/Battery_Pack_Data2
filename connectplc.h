#ifndef CONNECTPLC_H
#define CONNECTPLC_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QtNetwork>
#include <QDateTime>
#include "readw500.h"
#include "parsejson.h"


class ConnectPLC : public QObject
{
    Q_OBJECT
public:
    explicit ConnectPLC(QObject *parent = 0);
    int channel_Num;

signals:
    void sendPLCConnect(bool);
    void startW500CMD(int,QString);
    void sendCmdToMain(QString,int,int);
    void sendOperate(bool,QString);
    void sendAddStationTimer(QString,int);
    void sendChToWork(int,int,QString);
    void sendCmdToStep(int,QString,int);
    void sendReadCmdToRFID(QByteArray,QByteArray);
    void sendStationFinishToMaindow(QString,QString);
    void sendReqDutouTightStatus(QString,QString);
    void sendGlueWorkFlag(QString);
    void sendgetStationTimer(QString,int,QString);
    void sendReqPackStatus();
    void sendBoundPack();
    void sendReqModule();
    void sendReqStationState(QString,QString,QString,QString,QString);
    void sendReqModuleTray(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
    void sendReqModuleTray(QString,QString,QString,QString,QString,QString,QString,QString,QString);
    void sendReqModuleTray(QString,QString,QString,QString,QString);
    void sendReqKeybarcode(int,int,QString,QString,QString,QString);
    void sendReqModulePack(QString,QString);
    void sendReqModuleCount();
    void sendStartBarcode();
    void sendShowPush();
    void sendStates(QString);
    void sendBMCEFirstStation(QString);
    //void sendSuspend();
    void sendModuleTrayClean(QString);
//    void sendBMCETrayClean(QString,QString);
    void sendUpInAndOutStation(int , QString , QString , QString , QString);
private:
    QString readMesTemp;//记录本次读到PLC的数据
    QThread plc_thread;
    QTcpSocket *tcpSocket;
    ParseJson * parsejson;

    int DBWRLen;//读取多少(B)长度的DBW
//    QPair<int,QByteArray>writeDataPair;
    QList<QPair<int,QPair<int,QByteArray> > >taskList;

    QByteArray startCmdBuf1;
    QByteArray startCmdBuf2;
    QByteArray readCmdBuf;
    QByteArray writeCmdBuf;

    QString PLC_IP;
    bool isRepeatWritePLC;//是重复写入PLC
    int countRepeatWrite;//重复写入的次数
    bool sendNOStationFlag;
    bool packUplodeStatus1;
    bool packUplodeStatus2;
    bool plcRecvFlag;
    bool boxErrorFlag;
    bool nextTrayflag;
    int trayfinishBoundCount;
    bool plcAckFlag;
    QString packUplodeTemp1;
    QString packUplodeTemp2;
    QString dotouPositionNum1;
    QString dotouPositionNum2;

    bool packStatusTrueFlag;

    QString codeBuf[20];
    int codeNum;
    int scanCount;
    int length;
    int tmpPackNum;
    int stationAddr;
    int setUpCommFlag;
    int sendCmd;
    int aliveCount;
//    int isOneCount;
    bool plcLinkFlag;
    bool clearPLCFlag;
    bool glueTightClearFlag;
    bool trayProtectFlag;
    int sendCount;
    int readAddr;
    int readLen;
    int trayIsAction;
    int trayBindCount;
    int writeLen;//写入多少(B)长度的DBW
    int writeAddr;
    int reqProductTypeAddr;
    int reqBoundAddr;
    int conditionType;
    char strWriteBuf[50];
    char strWriteBufTemp[50];
    int tightenResult;
    bool cleanAllDWFlag;
//    bool readPLCFlag;
    bool statusValidFlag;
    bool isReadRfidFlag;
    bool statusValidFlagTemp;
    bool writePLCFlag;
    bool writeProductTypeFlag;
    bool writeProductFinishFlag;

    void errorHandle();
    bool reqProductTypeFlag; //request product type
    bool getProductTypeFlag;
    bool getGlueProductTypeFlag;
    bool getDistroyStatus;
//    bool onlyRunFirst;
    bool ackProductTypeFlag;

    bool reqTightenPositionFlag;
    bool getTightenPositionFlag;
    bool getGluePositionFlag;
    bool getGlueBindSuccess;
    bool reqRobotTightenReadyFlag;
    bool getRobotTightenReadyFlag;
    bool reqStationFinishFlag;
    bool getStationFinishFlag;
    bool getTightenFinishFlag; //station finish
    bool getTightErrorFlag;

    bool dutouStatusPosition;
    bool reqBoxPositionFlag;
    bool getBoxPositionFlag;
    bool reqBoxFinishFlag;
    bool getBoxFinishFlag;
    bool reqBoundInforFlag;
    bool getBoundInforFlag;
    bool fixtureHoldFlag;

    bool reqDutouPositionFlag;
    bool getDutoupositionFlag;
    bool reqDutouRecordDataFlag;
    bool getDutouRecordDataFlag;
    bool getDutouRecordFinishFlag;
    bool getStationErrorFinishFlag;


    bool tellTrayReadyFlag;
    bool getTrayReadyFlag;
    bool reqModuleBoxNumFlag;
    bool getModuleBoxNumFlag;
    bool getRfidUpDowmFlag;
    bool reqModuleRecordDataFlag;
    bool getModuleRecordDataFlag;
    bool getModuleRecordFinishFlag;
    bool tellRollerBedReadyFlag;
    bool getRollerBedReadyFlag;

    bool reqBindingSNFlag;   //request binding
    bool getBindingSNFlag;
    bool ackBoundPackFlag;

    bool reqUpdownBoxPositionFLag;
    bool getUpdownBoxPositionFlag;
    bool reqScanBarcodeFlag;
    bool getScanBarcodeFlag;
    bool getScanBarcodeVM010Flag;
    bool getRFIDCodeVM010Flag;
    bool reqUpdownBoxRecordDataFlag;
    bool getUpdownBoxRecordDataFlag;
    bool getTrayType;
    bool reqModuleTypeFlag;
    bool getModuleTypeFlag;
    bool tellModuleMoveFinishFlag;
    bool getModuleMoveFinishFlag;

    bool reqPLCMoveTrayFlag;
    bool getPLCMoveTrayFlag;

    bool mannulModeFlag;
    bool startWorkFlag;
    bool startWorkWriteFlag;
    bool pack1StatusFlag;
    bool pack1StatusWriteFlag;

    int  tightenCount;
    int  boxCount;
    int  dutouCount;
    int  moduleCount;
    int  moduleBindCount;
    bool plcStopFlag;
//    int  PLC_StepNum;
    int  tighten_Num;       //pack count all
    int  box_Num;
    int  dutou_Num;
    int  module_Num;
    QString moduleMessage;
    int  module_typeNum;
    int  scanResult;
    int  readRFIDResult;
    int  readRFIDCount;
    int  moduleBoundResult;
    int  pack12Num;
    int  pack1Result;
    int  pack2Result;
    int  countInitValue;
    int  countValue;
    bool  workSecondePackFlag;
    bool  tightenTestFlag;
    bool reqProductTrayFlag;
    QString vinCode;

    QString updataTraySN;
    QString updataTrayTemp;
    QString RFIDCode;
    QString RFIDCodeTemp;
    QString RFIDCodeTemp1;
    QString PLCPackType;//product type
    QString PLCPackAGVSN;
    QString PLCPack1SN;
    QString PLCPack2SN;
    QString PLCPack1Status;
    QString PLCPack2Status;
    QList<QString> RFIDlist;

    int PLC_AllStep;
    int PLCWorkAllStep;
    int mannulNum;
    int pack1StatusNum;
    int startWorkNum;
    int glueStartWorkNum;
    int W500EnableFlag;
    int moveTrayStep;
    int stationFinishAddr;
    int glueStartWorkAddr;
    int glueTightPackStatusAddr;
    int glueTightProductTypeAddr;
    int writeToPlcPackSNAddr;
    int writeToPlcPack1SNAddr;
    int writeToPlcPack2SNAddr;
    QString regExpVm1;
    QString regExpVm2;
    QString lingjianTemp;
    QString VariantDiff;
    QString dataDiff;
    QString dataDiff1;
    bool glueStartWorkFlag;
    bool glueTightPackStatusFlag;
    bool glueTightProductTypeFlag;
    int trayFinishAddr;
    QString configName;

    int curStepNum;
    QString strBoxCode;
    QString strBoxCodeTemp;
    bool strBoxCodeFlag;
    bool strBoxCoderegExpFlag;
    bool stationSkip;
    int tightenTimes;
    QString w500_IP;
    void handleBandEvent();
    void cleanBandWvent();
    void cleanAllDW();
    void writePackSNToPlc(QString,QString);
    void changePackResult();
    void stepParaPLCBufInit(QVariantMap);
    bool handScanMessageLong(QString, QString);
    bool handScanMessageLongA010(QString);
    bool handScanMessageShort(QString);
    QStringList scanListShort;
    QStringList scanListLong;
    QStringList scanList;

    bool longFlag;
    bool shortFlag;
    QVariantList PLCstepList;
    QVariantMap PLCStepMap;
    QByteArray moduleBuf;
    int ackCode;
public slots:
    void disConnectDO();
    void plcConnects();
    void recReadyClicked();
    void recvTrayCleanResult();
    void recvMasterReadRfid();
    void ctlPLCHandle(QByteArray,bool);
//    void revPLCData();
    void readPLCData(QString);
    void timerFuncXS();
    void reSendReqModulePack();
    //void reModuleClean( );
    void writePLCFunc(int,int,int,char *);
    void readPLCFunc(int,int,int);
    void PLCStart();
    void revCmdFunc(int,int);//receive tightening result
    void writeDataToPLCFunc();
    bool handlePLCStepTypeFunc(int,int);

    void revPackStatus(QVariant);
    void recgetStationTimer(int);
    void recDutouTightStatus1(QString);
    void recDutouTightStatus2(QString);
    void revGluePackStatus(QString);
    void revModulePack(bool);
    void revFixtureStatus(bool);
//    void revKeybarcode(bool);
//    void revModule(bool,QString);
    void revStationState(bool);
//    void revModuleCount(bool);

    void revBoxCodeFunc(QString);
    void ackCodeHandle(QString);
    void revRFIDAck(QString,QByteArray);
    void revCS351Status(bool);
    void revRFIDRead();
    int readPackStatus(int,int);
    void stepInit();
    void slot_trayReleaseFunc();
};

#endif // CONNECTPLC_H
