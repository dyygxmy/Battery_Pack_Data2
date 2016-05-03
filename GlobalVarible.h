#ifndef GLOBALVARIBLE_H
#define GLOBALVARIBLE_H

#endif // GLOBALVARIBLE_H

#include <QReadWriteLock>
#include <QQueue>
#include <QVector>
extern bool isJS;
extern int isRFID;  //是否是RFID
extern bool isNormal;//是否正常
extern bool workmode;//工作模式
extern bool CsIsConnect; //CS351状态
extern bool RFIDISConnect;
extern bool RDYY;        //是否准备
extern bool ISmaintenance;
extern bool ISRESET;
extern bool ISWARNING;
extern bool Curve_Is_Null;
extern bool TaoTongState;
extern bool WIFI_STATE;
extern bool SerialGunMode;
extern bool DebugMode;
extern bool QueueIsNull;
extern bool rfidNextCom;   //rfid下辆车已进站标识
extern int Line_ID;
extern int SerialMode;// 0条码枪  1 RFID  2 队列模式
extern bool bound_enabled;
extern QString SYSS;     //状态
extern QString SaveWhat; //要保存什么
extern QString VIN_PIN_SQL;//VIN 码
extern QString VIN_PIN_SQL_RFID;//PIN 码
extern QString SCREWID_SQL;//螺栓编号
extern QByteArray time_info;//实时信息包
extern QString proNum;      //当前程序号
extern QString lsNum;       //额定螺栓数量
extern QString CURVE_STR;
extern QString SqlUserName;
extern QString SqlPassword;
extern QString WirelessIp;
extern QString CS351Ip;
extern int QualifiedNum;  //合格数量
extern int TimeLeft;      //剩余时间
extern int BackTimeLeft;
extern int enablenumberLeft; //剩余螺栓数量
extern int TAOTONG;
extern QString Operator;
extern QString info[7];
extern QString status[20][5];
extern QString Version;
extern bool battery;
extern QReadWriteLock lock;
extern QReadWriteLock RFIDlock;
extern QQueue<QVector<QString> > queue;






