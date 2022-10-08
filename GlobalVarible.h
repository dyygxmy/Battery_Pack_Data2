#ifndef GLOBALVARIBLE_H
#define GLOBALVARIBLE_H

#include <QReadWriteLock>
#include <QQueue>
#include <QVector>
#include <QMetaType>
#include <QVariant>
#include "unistd.h"
#define pointDebug() qDebug()<<"point["+QString::number(__LINE__)+"]"


extern bool GUpdateTimeFlag;//时间同步标志
extern QString GUpdateTimeVal;//记录需要同步的时间值
extern bool GIsTestStartFlag;//启用测试标志位
extern bool GIsInterfaceUpSuccess;//接口上传是否成功
extern bool isRFID;   //是否是RFID
extern bool isBarCode;//
extern bool isGostRun;   //wz

extern bool isNormal;//是否正常
extern bool workmode;//工作模式
extern bool CsIsConnect; //CS351状态
extern bool RFIDIsConnect;//RFID State
extern bool PLCIsConnect;//PLC State
extern bool RDYY;        //是否准备
extern bool ISmaintenance;//IDLE state
extern bool ISRESET;
extern bool ISWARNING;
extern bool GgetRobotTightenReadyFlag;
extern int  GtightCount;
extern int  GScanTimes;
extern int  GisTightenGoOn;
extern bool GWorkStatus;
extern bool GErrorTight;
extern bool GrecErrorResult;

extern bool TaoTongState;
extern bool WIFI_STATE;
extern bool SerialGunMode;
extern bool DebugMode;
extern bool GisTightFlag;

extern int Line_ID;
extern int GBlotErrorCount;
extern int GiswhichChannel;
extern bool GisBoltErrorCountFlag;
extern int GencoderFlag;
//extern int SerialMode;// 0条码枪  1 RFID  2 队列模式
extern bool bound_enabled;

extern QString SYSS;     //状态
extern QString SaveWhat; //要保存什么
extern QString VIN_PIN_SQL;//VIN 码
extern QString VIN_PIN_SQL_RFID;//PIN 码
extern QString Type;
extern QString GTrayStatus;
extern QString GPackType;
extern QString GPackSN;     //BMCE模组线零件号

extern QString SCREWID_SQL;//螺栓编号
//extern QByteArray time_info;//实时信息包
extern QString lsNum;       //额定螺栓数量
extern QString SqlUserName;
extern QString SqlPassword;
extern QString WirelessIp;
extern QString EKSBuff;//员工号
extern QString ControllerIp_1;
extern QString ControllerIp_2;
extern QString ControlType_1;
extern QString ControlType_2;
extern QString Station;  //工位
extern QString GRFIDMessage;
extern int GStepListSize;
extern QString GBevPass;
extern int GtightGlueresult;
extern QString GgetBoltIsOk;
extern QString GProDuctType;
extern QString GTightResetOnline;
extern QString GscanRegExp;
extern QString GscanRegExp2;
extern QString GdateDiff;
extern QString GdateDiff2;
extern QString GVRegexp;
extern int GTightStatus;
//extern int GpackCount;
//extern int GpackNumber;
extern int GOutputToday;//统计当天产量
extern QString GscanRegExpCode;//条码匹配规则
extern QByteArray sendToRfid;//wz add
extern QString Gvariable2;
extern QByteArray GrecvStatusMsee;
extern int GWhichPackError;
extern int GwhichStepNum;
//extern qint64 GYENCODERTemp;
//extern qint64 GAENCODERRemp;

extern int BoltOrder[2]; //第几个螺栓
extern int QualifiedNum;  //合格数量
extern int TimeLeft;      //剩余时间
//extern int BackTimeLeft;
extern int enablenumberLeft; //剩余螺栓数量
extern int TAOTONG;
extern int StationStatus;
extern int GrfidStatusTemp;  //下线托盘型号
extern QString Operator;
extern QString Version;
extern QString tablePreview;
extern QString Localtable;
extern int GProgramIndex;
extern int BoltTotalNum;   //螺栓总数
extern int GBoltOkNum;       //合格数量
extern int GBoltAllNum;     //接收螺栓总数
extern int GsendEnableCount;
extern int GrecEnableCount;
extern bool NOKflag;          //是否有NOK
extern bool GNokFlag;       //有不合格螺栓
extern bool GNokFlagTemp;
extern bool GPareXMLFlag;
extern bool GEKSReRFID;
extern int Gcarcode;
extern int GGlueTime;       //涂胶等待超时时间
extern int GMI;
extern QString GNokNumber;  //不合格螺栓序号
extern QReadWriteLock lock;
extern QString GErrorChannel;
extern QReadWriteLock WIFIlock;
extern QString GPack1SN;
extern QString GPack2SN;
enum keys{noKey,redKey,greenKey,blackKey};
extern bool allowNoKey;//允许无钥匙操作
extern int GRemaningCountSpearhead;//枪头剩余使用次数
extern int GWarningCountSpearhead;//枪头报警次数
extern QString GCurrentStep;//记录当前步骤
extern QVariantList datas;
typedef  struct
{
    QString pdmName;
    QString boltSN[20];
    QString proNo[20];
    int boltNum[20];
    int ttNum[20];
    int IONum[20];
    bool barcodeEnable[20];
}CAR_INFOR;

extern CAR_INFOR carInfor;
extern int preChannel;
extern int groupNumCh;
extern QString channelNum;
extern QString GchannelNum;

typedef struct insert_data
{
    QString data_model[25];
}DATA_STRUCT;

Q_DECLARE_METATYPE(DATA_STRUCT)

typedef struct
{
//    bool Result;
//    QString Errorcode;
//    QString PackType;
//    QString Pack1SN;
//    QString Pack2SN;
//    QString Pack1Status;
//    QString Pack2Status;
    bool Result;
    QString AGVSN;
    QString Errorcode;
    QString PackType;
    QString Pack1SN;
    QString Pack2SN;
    QString Pack1Status;
    QString Pack2Status;
}PACK_STATUS;

typedef struct
{
    QString Result; //结果 （0：绑定不成功，1：绑定成功；2：托盘与零件类型不匹配）
    QString AGVSN;  //托盘编号
    QString Type;   //电池包类型 (1:PHEV/2:BEV)
    QString Pack1Type;  //零件编号类型 (A/B/C/D/E/F)
    QString Pack1SN;    //零件编号
    QString Pack1Status;    //1：合格,2：不合格，3：未操作
}BMCE_STATUS;

Q_DECLARE_METATYPE(PACK_STATUS)
Q_DECLARE_METATYPE(BMCE_STATUS)

#endif  // GLOBALVARIBLE_H


