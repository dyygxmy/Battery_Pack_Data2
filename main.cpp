#include "mainwindow.h"
#include "rootdialog.h"
#include "worker.h"
#include "GlobalVarible.h"
#include "serialComs.h"
#include "./TestInput_Cursor/inputmethod.h"

#include "communication.h"
#include "tightenthread.h"

#include "inputevents.h"
#include "wificonnect.h"

#include "taotong.h"

#include "rfidcontroler.h"

#include "logdelete.h"
#include "sqlthreadsvw2.h"
#include "mesuploadsvw2.h"

#include "stepcontrol.h"
#include "interface.h"
#include "serialComsRead.h"

#include <QWSServer>
#include "mastertcp.h"
bool onSendStatus = true;//开机上传状态
bool GIsTestStartFlag = false;//启用测试标志位
bool GIsInterfaceUpSuccess = false;//接口上传是否成功

bool isNormal = false;           //状态是否正常
bool workmode = false;           //工作模式与否
bool CsIsConnect = false;        //CS351拧紧枪是否连接
bool RFIDIsConnect = false;      //RFID是否连接
bool PLCIsConnect = false;       //PLC connect
bool RDYY  = false;              //CS351是否准备
bool ISmaintenance = false;      //是否在点击配置页面中
bool ISRESET  = false;           //是否复位
bool ISWARNING = false;          //警告

bool TaoTongState = false;       //是否为套筒模式
bool WIFI_STATE = false;         //wifi状态
bool SerialGunMode = true;       //条码枪模式 wired barcode and wireless barcode
bool DebugMode = false;          //调试模式
bool bound_enabled = false;      //是否启用扭矩角度 控制
bool GWorkStatus = false;
bool GgetRobotTightenReadyFlag = false;
bool GErrorTight = false;
bool GrecErrorResult = true;
int GsendEnableCount = 5;
int GrecEnableCount = 0;
int GtightCount = 0;
int GScanTimes = 0;
int GisTightenGoOn = 0;
//bool isParse = true;
int Line_ID = 1;                 //工线
//int SerialMode = 0;

QString GAirPackStatus = "3";
QString SYSS = "Ready";               //data2运行状态  Ready  ING  OK NOK
QString SaveWhat="";             //保存参考变量
QString VIN_PIN_SQL= "";         //条码
QString VIN_PIN_SQL_RFID = "";   //RFID条码
QString GTrayStatus = "1";

QString Type = "";

QString SCREWID_SQL= "";         //螺栓编号
//QByteArray time_info ="";
QString lsNum="";                //额定螺栓数量
QString SqlUserName = "";
QString SqlPassword ="";
QString WirelessIp ="";
QString ControllerIp_1 ="";
QString ControllerIp_2 ="";
QString strProNum = "";
int GWhichPackError = 0;
int GwhichStepNum = 0;
int BoltOrder[2];
int QualifiedNum=0;              //合格数量
int TimeLeft = 0;                //进度条剩余时间
int enablenumberLeft = 0;        //剩余螺栓数量
//int BackTimeLeft = 0;            //归位剩余时间
int TAOTONG = -1;
int StationStatus= 0;
int GrfidStatusTemp = 0;
QString Operator = "";
QString Version="";
QString Station="";
QString GPackType = "PHEV";
int GStepListSize = 0;
QString GBevPass = "";
QString GgetBoltIsOk = "";
QString GRFIDMessage = "";
QString GscanRegExp = "";
QString GscanRegExpCode = "";
QReadWriteLock lock;
QString Gvariable2 = "";
QByteArray GrecvStatusMsee = "";

QReadWriteLock WIFIlock;
QString GPack1SN = "";
QString GPack2SN = "";
QString ControlType_1= "";
QString ControlType_2= "";
QString tablePreview="";
QString Localtable="";
QByteArray sendToRfid = "";
int BoltTotalNum = 0;   //螺栓总数
int GProgramIndex = 0;
int GBoltOkNum = 0;
bool NOKflag = false;          //是否有NOK
int GBoltAllNum = 0;
bool GNokFlag = false;
bool GNokFlagTemp = false;
bool GPareXMLFlag = false;
bool GEKSReRFID = false;
QString GNokNumber = "";
QString GErrorChannel = "";
CAR_INFOR carInfor;
int Gcarcode = 0;
int preChannel=0;
int groupNumCh = 0;
QString channelNum = "1";
QString GchannelNum = "1";
bool isRFID = false;      //RFID模式
bool isBarCode = false;   //条码模式RS232
bool isOutFlag = false;   //output

bool GUpdateTimeFlag = false;//时间同步标志
QString GUpdateTimeVal = "";//记录需要同步的时间值

#define OP_ONLY_SEND_ENABLE
/******************日志打印级别函数****************************/
void outputMessage(QtMsgType type, const char *msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
        abort();
    }
    if(text != QString("Warning:"))
    {
        QString message = QString("[%1] %2 %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz ddd")).arg(text).arg(msg);
        QDateTime time = QDateTime::currentDateTime();
        QString date = time.toString("yyyy-MM-dd");

        QFile file(QString("/log/")+date+QString(".log"));
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream text_stream(&file);
        text_stream << message << endl;
        file.flush();
        file.close();
    }
    mutex.unlock();
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv, QApplication::GuiClient);
    QWSServer::setCursorVisible(false);//在电脑上运行要注释
    qInstallMsgHandler(outputMessage);                                  //重定向日志到文件
    QApplication::setOverrideCursor(Qt::BlankCursor);                   //去掉光标--在电脑上运行要注释
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utf8);
    QTextCodec::setCodecForLocale(utf8);
    QTextCodec::setCodecForCStrings(utf8);


    qRegisterMetaType<QVariant>("QVariant");
    system("dos2unix /config.ini /config_bound.ini");
    /**
   *Data2程序main函数开始初始化一些参数变量，无线网卡ip
   *
   *
   **/
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    configIniRead->value("baseinfo/serialMode").toInt()?SerialGunMode=true:SerialGunMode=false;
    configIniRead->value("baseinfo/isRFID").toInt()?isRFID=true:isRFID=false;
    configIniRead->value("baseinfo/isBarCode").toInt()?isBarCode=true:isBarCode=false;

    Station = configIniRead->value("baseinfo/StationId").toString();

    Line_ID = configIniRead->value("baseinfo/Line_ID").toInt();
    configIniRead->value("baseinfo/bound").toInt()?bound_enabled=true:bound_enabled=false;
    SqlUserName = configIniRead->value("baseinfo/SQLUserName").toString();
    SqlPassword = configIniRead->value("baseinfo/SQLPassword").toString();
    WirelessIp = configIniRead->value("baseinfo/WirelessIp").toString();
    ControllerIp_1 = configIniRead->value("baseinfo/ControllerIp_1").toString();
    ControllerIp_2 = configIniRead->value("baseinfo/ControllerIp_2").toString();
    strProNum = configIniRead->value("baseinfo/APSSID").toString();
    channelNum = configIniRead->value("baseinfo/channelNO").toString();
    GchannelNum = configIniRead->value("baseinfo/channelNO").toString();
    QString variable1 = configIniRead->value("baseinfo/APSSID").toString();
    QString variable2 = configIniRead->value("baseinfo/APpsk").toString();
    GBevPass = configIniRead->value("baseinfo/BevPass").toString();
    GgetBoltIsOk = configIniRead->value("baseinfo/SetBlotOk").toString();//螺栓定位方式取值
    Gvariable2 = variable2;
    QString strRfidip = configIniRead->value("baseinfo/RfidIp").toString();
    int     intRfidport  = configIniRead->value("baseinfo/RfidPort").toInt();
    QString station_Name = configIniRead->value("baseinfo/StationName").toString();
    QString station_Work = configIniRead->value("baseinfo/StationId").toString();
    int vin_AttributeBit = configIniRead->value("baseinfo/vinAttributeBit").toInt();
    int tack_Time = configIniRead->value("baseinfo/TackTime").toInt();
    ControlType_1 = configIniRead->value("baseinfo/ControlType_1").toString();
    ControlType_2 = configIniRead->value("baseinfo/ControlType_2").toString();
    if(ControlType_1=="")
    {
        ControlType_1="CS351";
        configIniRead->setValue("baseinfo/ControlType_1","CS351");
    }
    if(ControlType_2=="")
    {
        ControlType_2="OFF";
        configIniRead->setValue("baseinfo/ControlType_2","OFF");
    }


    Version = "V2.191010_1";    //date
    if(GIsTestStartFlag){
        ControlType_1 = "OFF";
    }
    qDebug()<< "ControlType_1:" << ControlType_1 <<"ControlType_2:"<<ControlType_2<< "Version:"<<Version;

//    int connectNet = configIniRead->value("baseinfo/connectNet").toInt();
//    if(connectNet == 0 )//wlan0
//        system((QString("ifconfig wlan0 ")+configIniRead->value("baseinfo/WirelessIp").toString()+" netmask "+configIniRead->value("baseinfo/netmask").toString()+" &").toLatin1().data());
//    else
//        system((QString("ifconfig wlan0 ")+configIniRead->value("baseinfo/WirelessIp").toString()+" netmask 255.255.255.0 &").toLatin1().data());
//    if(connectNet == 1)//eth0
//        system((QString("ifconfig eth0 ")+configIniRead->value("baseinfo/LocalIp").toString()+" netmask "+configIniRead->value("baseinfo/netmask").toString()+"  &").toLatin1().data());
//    else
//        system((QString("ifconfig eth0 ")+configIniRead->value("baseinfo/LocalIp").toString()+" netmask 255.255.255.0 &").toLatin1().data());
//    if(connectNet == 2 )//eth1
//        system((QString("ifconfig eth1 ")+configIniRead->value("baseinfo/LocalIp2").toString()+" netmask "+configIniRead->value("baseinfo/netmask").toString()+"  &").toLatin1().data());
//    else
//        system((QString("ifconfig eth1 ")+configIniRead->value("baseinfo/LocalIp2").toString()+" netmask 255.255.0.0 &").toLatin1().data());
    QString freetds = "/usr/local/arm/freetds/etc/freetds.conf";
    QFile file1(freetds);
    if(!file1.open(QIODevice::ReadOnly| QIODevice::Text))
    {
        qDebug()<< "Cannot open testdsn file1 for Reading";
    }
    else
    {
        QString str1 (file1.readAll());
        if(str1.contains("[testdsn1]", Qt::CaseInsensitive)&&str1.contains("[testdsn2]", Qt::CaseInsensitive))
        {
            str1.replace(QRegExp("\\[testdsn1\\]\\s*host = \\S*"),QString("[testdsn1]\n\thost = ")+configIniRead->value("baseinfo/CurveServerIp").toString());
            str1.replace(QRegExp("\\[testdsn2\\]\\s*host = \\S*"),QString("[testdsn2]\n\thost = ")+configIniRead->value("baseinfo/DataServerIp").toString());
            str1.replace(QRegExp("\\[testdsn3\\]\\s*host = \\S*"),QString("[testdsn3]\n\thost = ")+configIniRead->value("baseinfo/AndonServerIp").toString());
        }
        file1.close();
        if(!file1.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            qDebug()   << "Cannot open testdsn file1 for Writing";
        }
        else
        {
            file1.write(str1.toUtf8());
            file1.close();
        }
    }

//    QString wpa_conf = "/etc/wpa_supplicant.conf";
//    QFile file2(wpa_conf);
//    if(!file2.open(QIODevice::ReadOnly| QIODevice::Text)){
//        qDebug()   << "Cannot open wifi file2 for Reading";
//    }
//    else
//    {
//        QString str2 (file2.readAll());
//        if(str2.contains("ssid=", Qt::CaseInsensitive)&&str2.contains("psk=", Qt::CaseInsensitive)){
//            str2.replace(QRegExp("\nssid=\\S*"),QString("\nssid=\"")+configIniRead->value("baseinfo/SSID").toString()+QString("\""));
//            str2.replace(QRegExp("psk=\\S*"),QString("psk=\"")+configIniRead->value("baseinfo/psk").toString()+QString("\""));
//        }
//        file2.close();
//        if(!file2.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
//            qDebug()   << "Cannot open wifi file2 for Writing";
//        }
//        else
//        {
//            file2.write(str2.toUtf8());
//            file2.close();
//        }
//    }

//    system((QString("wpa_cli set_network 0 ssid '\"")+configIniRead->value("baseinfo/SSID").toString()+QString("\"'")+" &").toLatin1().data());
//    system((QString("wpa_cli set_network 0 psk  '\"")+configIniRead->value("baseinfo/psk").toString()+QString("\"'")+" &").toLatin1().data());
//    system("wpa_cli disable_network 0 &");
//    system("wpa_cli enable_network 0 &");
//    if( connectNet ==0 )
//        system((QString("route add default gw ")+configIniRead->value("baseinfo/gateway").toString()+" wlan0 &").toLatin1().data());
//    else if( connectNet == 1)
//        system((QString("route add default gw ")+configIniRead->value("baseinfo/gateway").toString()+" eth0 &").toLatin1().data());
//    else if( connectNet == 2)
//        system((QString("route add default gw ")+configIniRead->value("baseinfo/gateway").toString()+" eth1 &").toLatin1().data());
    delete configIniRead;
    //    time_info.resize(7);


    system("mv -f /curveFile/* /curveFileDel/"); //move error curve


    tablePreview="SVW2FisPreview";
    Localtable="SVW2TighteningDatas";


    /*
      * MainWindow:主界面UI
      * 用来显示状态 不可有阻塞操作
    */
    Newconfiginfo *newconfiginfo = new Newconfiginfo;
    newconfiginfo->vari2_XL = variable2;
    MainWindow *w = new MainWindow ;
    w->vari1 = variable1;
    w->vari2 = variable2;
    w->stationName = station_Name;
    w->stationWork = station_Work;
    w->vinAttributeBit = vin_AttributeBit;
    w->Tacktime = tack_Time;
    QTimer::singleShot(0,w,SLOT(mainWindowInit()));

    RootDialog m ;
    m.vari1_r = variable1;
    m.show();
    m.init(w,newconfiginfo);

    QObject::connect(newconfiginfo,SIGNAL(closeconfig()),w,SLOT(initBack()),Qt::AutoConnection);
    /*
      * Wificonnect:wifi网络状态判断
      * 包括服务器 rfid连接状态5秒检测一下
    */
    Wificonnect Wifi_Connect ;
    QTimer::singleShot(0,&Wifi_Connect,SLOT(Wifi_ConnectStart()));

    LogDelete logDelete ;
    QTimer::singleShot(0,&logDelete,SLOT(init()));

    TightenThread       *tighten_xml = NULL;
    Worker    *work = NULL;

    SqlThreadSVW2 * sqlthreadsvw2 = NULL;
    MESUploadSVW2 *mesUploadSVW2 = new MESUploadSVW2;              //data upload
    mesUploadSVW2->vari2_XL = variable2;
    QTimer::singleShot(0,mesUploadSVW2,SLOT(init()));

    /*
              * Worker:工作线程
              * 用来接收CS351 B端口发来心跳曲线
            */
    if(ControlType_1 != "OFF")
    {
        work = new Worker;
        work->ctrl_Type = ControlType_1;
        QTimer::singleShot(0,work,SLOT(fun1()));               //cs351 port 4710
    /*
              * TightenThread:控制CS351线程
              * 发送复位 心跳 使能命令同时与UI交互
            */
        tighten_xml = new TightenThread;
        tighten_xml->ctrlType = ControlType_1;
        tighten_xml->ctrlIP = ControllerIp_1;
        tighten_xml->testPro = strProNum;
        QTimer::singleShot(0,tighten_xml,SLOT(tightenStart()));//cs351 port 4700
    }
    /*
          * SqlThread:本地存储数据线程
          * 与Worker线程交互拿到数据也存储到本地mysql
        */

    sqlthreadsvw2 = new SqlThreadSVW2;
    QTimer::singleShot(0,sqlthreadsvw2,SLOT(sqlinit()));       //Local Mysql

    QObject::connect(w,SIGNAL(sendfromsecondthread(QString ,QString,QString,int)),sqlthreadsvw2,SLOT(configOne(QString,QString,QString,int)),Qt::AutoConnection);

    QObject::connect(sqlthreadsvw2,SIGNAL(send_mysqlerror()),w,SLOT(signal_mysqlerror_do()),Qt::AutoConnection);
    QObject::connect(w,SIGNAL(sendNokAll(int)),sqlthreadsvw2,SLOT(receiveNokAll(int)),Qt::AutoConnection);
    /*
      * Communication:与上位机通讯线程
      * 可配置线程
    */
    Communication communication ;
    QTimer::singleShot(0,&communication,SLOT(mycommunicationtimer()));

    /*
      * InputEvents:中断线程
      * 主要响应钥匙的操作，后期添加中断可在此线程操作
    */
    InputEvents inputevents ;
    QTimer::singleShot(0,&inputevents,SLOT(InputEventStart()));

    /*
      * TaoTong:套筒操作线程
      * 套筒操作
    */
    TaoTong taotong;
    if(variable1.mid(0,1) == "M")
    {
        taotong.vari1_type = "M";
    }
    QTimer::singleShot(0,&taotong,SLOT(T_start()));
    /*
      * StepControl:步骤线程
      *
    */
    StepControl *stepCtrl = NULL;
    if(variable1.mid(0,1) == "M")
    {
        stepCtrl = new StepControl;
        stepCtrl->vari1 = variable1;
        stepCtrl->vari2 = variable2;
        stepCtrl->IOModuleIP = strRfidip;
        if(ControlType_1 == "OFF")
        {
            stepCtrl->CS351StatusFlag = true;//CS351
        }
        QTimer::singleShot(0,stepCtrl,SLOT(stepStart()));
    }
    /*
      * RfidControler:RFID控制线程
      * 接收RFID条码
    */
    RfidControler *rfid = NULL;
    SerialComsRead *serial = NULL;

    serial = new SerialComsRead;
    serial->vari2_XL = variable2;
    serial->vari1 = variable1;
    QTimer::singleShot(0,serial,SLOT(serialComStart()));

    rfid = new RfidControler;
    rfid->rfidip = strRfidip;
    rfid->rfidport = intRfidport;
    QTimer::singleShot(0,rfid,SLOT(rfidInit()));

    MasterTCP *master = NULL;
    master = new MasterTCP;
    QTimer::singleShot(0,master,SLOT(masterServerStart()));
    QObject::connect(rfid,SIGNAL(sendPinToMasterTcp(bool,QByteArray)),master,SLOT(revRFID_PIN(bool,QByteArray)),Qt::AutoConnection);
    QObject::connect(rfid,SIGNAL(sendRequest()),master,SLOT(reRfidData()),Qt::AutoConnection);

    Interface *interface = new Interface;
    QTimer::singleShot(0,interface,SLOT(init()));
    QObject::connect(rfid,SIGNAL(sendPinToUi(QString,bool,QString)),w,SLOT(getSerialNum(QString,bool,QString)),Qt::AutoConnection);
    QObject::connect(rfid,SIGNAL(rfidConnected(bool)),w,SLOT(setRfidState(bool)),Qt::AutoConnection);
    QObject::connect(&Wifi_Connect,SIGNAL(rfidconnect(bool)),rfid,SLOT(rfidDisconnectDo(bool)),Qt::AutoConnection);
    QObject::connect(m.codeEdit,SIGNAL(sendRFIDWrite(QByteArray,QByteArray)),rfid,SLOT(write_data(QByteArray,QByteArray)),Qt::AutoConnection);
    QObject::connect(m.codeEdit,SIGNAL(sendKey(int)),w,SLOT(EKSStausChange(int)),Qt::AutoConnection);

    QObject::connect(m.codeEdit,SIGNAL(sendRFIDRead(QByteArray,QByteArray)),rfid,SLOT(read_data(QByteArray,QByteArray)),Qt::AutoConnection);
    QObject::connect(m.codeEdit,SIGNAL(sendRFIDReset()),rfid,SLOT(sendReset()),Qt::AutoConnection);
    QObject::connect(rfid,SIGNAL(RFIDSuccess(QString,QByteArray)),m.codeEdit,SLOT(receiveSuccess(QString,QByteArray)),Qt::AutoConnection);
    QObject::connect(rfid,SIGNAL(rfidConnected(bool)),m.codeEdit,SLOT(setRFIDUI(bool)),Qt::AutoConnection);
    QObject::connect(serial,SIGNAL(sendCodeToMainwindow(QString,QString)),w,SLOT(displayScanBarcode(QString,QString)),Qt::AutoConnection);

    if(ControlType_1 != "OFF")
    {
        QObject::connect(work,SIGNAL(send_mainwindow(QString,QString,QString,int,int)),w,SLOT(fromsecondthreaddata(QString,QString,QString,int,int)),Qt::AutoConnection);
        QObject::connect(work,SIGNAL(send_mainwindowSB356(QString,QString,QString,int,int)),w,SLOT(fromsecondthreaddataSB356(QString,QString,QString,int,int)),Qt::AutoConnection);
        QObject::connect(tighten_xml,SIGNAL(IsTigntenReady(bool)),w,SLOT(TightenIsReady(bool)),Qt::AutoConnection);
        QObject::connect(tighten_xml,SIGNAL(ReconnectSendOperate()),w,SLOT(receiveOperate()),Qt::AutoConnection);
        QObject::connect(w,SIGNAL(sendOperate1(bool,int)),tighten_xml,SLOT(sendReadOperate(bool,int)),Qt::AutoConnection);
        QObject::connect(work,SIGNAL(havedconnect(bool)),tighten_xml,SLOT(receivehavedconnect(bool)),Qt::AutoConnection);
        QObject::connect(work,SIGNAL(disconnectTellPortA()),tighten_xml,SLOT(portBSendPortA()),Qt::AutoConnection);
        QObject::connect(tighten_xml,SIGNAL(disconnectTellPortB()),work,SLOT(portAsendPortB()),Qt::AutoConnection);
        QObject::connect(work,SIGNAL(sendfromworkthread(QVariant)),sqlthreadsvw2,SLOT(sqlinsert(QVariant)),Qt::AutoConnection);

    }
    if(variable1.mid(0,1) == "M")
    {
        QObject::connect(serial,SIGNAL(sendCmdToStep(int,QString,int)),stepCtrl,SLOT(revCMDFunc(int,QString,int)),Qt::AutoConnection);
        QObject::connect(w,SIGNAL(sendCmdToStep(int,QString,int)),stepCtrl,SLOT(revCMDFunc(int,QString,int)),Qt::AutoConnection);
        QObject::connect(stepCtrl,SIGNAL(sendCodeToTightening(QString,bool,QString)),w,SLOT(getSerialNum(QString,bool,QString)),Qt::AutoConnection);
        QObject::connect(stepCtrl,SIGNAL(sendLEDFlashAllOFF(int,QString,bool)),w,SLOT(onLEDFlashAllOFF(int,QString,bool)),Qt::AutoConnection);
        QObject::connect(stepCtrl,SIGNAL(sendBarcodeDisplay(QString,QString)),w,SLOT(displayScanBarcode(QString,QString)),Qt::AutoConnection);
        QObject::connect(stepCtrl,SIGNAL(sendSuspend()),w,SLOT(onGetSuspend()),Qt::AutoConnection);
        QObject::connect(&m,SIGNAL(sendKeyToStep(bool)),stepCtrl,SLOT(revKeyStatus(bool)),Qt::AutoConnection);
        QObject::connect(w,SIGNAL(sendMoveButtonToStep()),stepCtrl,SLOT(revMoveButton()),Qt::AutoConnection);
        QObject::connect(w,SIGNAL(sendCS351StatusToStep(bool)),stepCtrl,SLOT(revCS351Status(bool)),Qt::AutoConnection);
        QObject::connect(w,SIGNAL(sendErrorGo()),stepCtrl,SLOT(getErrorGo()),Qt::AutoConnection);
        QObject::connect(w,SIGNAL(sendRFIDRead()),stepCtrl,SLOT(revRFIDStepInit()),Qt::AutoConnection);
        QObject::connect(w,SIGNAL(sendErrorContinue()),stepCtrl,SLOT(getErrorContinue()),Qt::AutoConnection);
        QObject::connect(w,SIGNAL(sendEKSStatusEvent(int)),stepCtrl,SLOT(recEKSatatueEvent(int)),Qt::AutoConnection);

        QObject::connect(w,SIGNAL(sendDrectGo()),stepCtrl,SLOT(sendDrectGoSlot()),Qt::AutoConnection);
        QObject::connect(stepCtrl,SIGNAL(sendRequestAgvLeave()),interface,SLOT(requestAgvLeave()),Qt::AutoConnection);//request product type
        QObject::connect(stepCtrl,SIGNAL(sendGetStationTimer(QString,QString,QString,QString)),interface,SLOT(getStationTimer(QString,QString,QString,QString)),Qt::AutoConnection);//request product type
        QObject::connect(mesUploadSVW2,SIGNAL(UploadStationStatus()),interface,SLOT(reStationStatus()),Qt::AutoConnection);//request product type

        QObject::connect(stepCtrl,SIGNAL(sendReqModulePack(QString,QString)),interface,SLOT(modulePack(QString,QString)),Qt::AutoConnection);//module put in box and bound pack
        QObject::connect(stepCtrl,SIGNAL(sendReqModuleTray(QString, QString, QString, QString, QString)),interface,SLOT(moduleTray(QString, QString, QString, QString, QString)),Qt::AutoConnection);//vm040 upload tray number and module SN
        QObject::connect(stepCtrl,SIGNAL(sendReqModuleTray(QString, QString, QString, QString, QString, QString, QString, QString, QString)),interface,SLOT(moduleTray(QString, QString, QString, QString, QString, QString, QString, QString, QString)),Qt::AutoConnection);//vm040 upload tray number and module SN
        QObject::connect(interface,SIGNAL(sendModuleTray(bool)),stepCtrl,SLOT(revModuleTray(bool)),Qt::AutoConnection);//receive ACK
        QObject::connect(interface,SIGNAL(sendGetStationTimerResult(QString,QString)),stepCtrl,SLOT(receiveStationTimer(QString,QString)),Qt::AutoConnection);//receive ACK
        QObject::connect(stepCtrl,SIGNAL(sendReqPackStatus()),interface,SLOT(reqPackStatus()),Qt::AutoConnection);//request product type
        QObject::connect(stepCtrl,SIGNAL(sendReqTightStatus(QString,QString,QString)),interface,SLOT(reqTightStatus(QString,QString,QString)),Qt::AutoConnection);//request tight status
        QObject::connect(interface,SIGNAL(sendPackStatus(QVariant)),stepCtrl,SLOT(revPackStatus(QVariant)),Qt::AutoConnection);//receive product type
        QObject::connect(stepCtrl,SIGNAL(sendReqStationState(QString, QString, QString, QString, QString)),interface,SLOT(stationState(QString, QString, QString, QString, QString)),Qt::AutoConnection);//upload station finish signal
        QObject::connect(interface,SIGNAL(sendStationState(bool)),stepCtrl,SLOT(revStationState(bool)),Qt::AutoConnection);//receive product type
        QObject::connect(stepCtrl,SIGNAL(sendReqKeybarcode(QString, QString, QString, int)),interface,SLOT(Keybarcode(QString, QString ,QString,int)),Qt::AutoConnection);//key barcode upload
        QObject::connect(stepCtrl,SIGNAL(sendCodeToSerialcom(QString,QString,bool)),serial,SLOT(revManuulCmd(QString,QString,bool)),Qt::AutoConnection);
        QObject::connect(serial,SIGNAL(sendResultToSQL(QVariant)),sqlthreadsvw2,SLOT(sqlinsert(QVariant)),Qt::AutoConnection);
        QObject::connect(serial,SIGNAL(sendResultTomainwindow(QString,QString,QString,int)),w,SLOT(fromsecondthreaddataAirTest(QString,QString,QString,int)),Qt::AutoConnection);
        QObject::connect(interface,SIGNAL(sendAgvLeaveState(bool)),stepCtrl,SLOT(revAgvLeaveState(bool)),Qt::AutoConnection);//receive product type


        QObject::connect(stepCtrl,SIGNAL(sendUIEmergencyStop(bool)),w,SLOT(showEmergencyStop(bool)),Qt::AutoConnection);
        QObject::connect(stepCtrl,SIGNAL(sendReqEmergencyStop(bool)),interface,SLOT(reqEmergencyStop(bool)),Qt::AutoConnection);
        QObject::connect(w,SIGNAL(sendStepResetEmergencyStop(bool)),stepCtrl,SLOT(informEmergencyStop(bool)),Qt::AutoConnection);
        QObject::connect(interface,SIGNAL(sendEmergencyStopCode(bool)),stepCtrl,SLOT(revEmergencyStop(bool)),Qt::AutoConnection);
        QObject::connect(m.stepEdit,SIGNAL(stepParaInit()),stepCtrl,SLOT(readStepParaInit()),Qt::AutoConnection);
    }
    QObject::connect(serial,SIGNAL(sendStationFinishToMaindow(QString,QString)),w,SLOT(displayScanBarcode(QString,QString)),Qt::AutoConnection);
    /*
      * 下面都是信号槽的连接 具体看代码
      *
    */

    QObject::connect(&Wifi_Connect,SIGNAL(wifi_connect(bool)),w,SLOT(wifishow(bool)),Qt::AutoConnection);
    QObject::connect(&Wifi_Connect,SIGNAL(wifi_connect(bool)),&m,SLOT(wifishow(bool)),Qt::AutoConnection);
    QObject::connect(&Wifi_Connect,SIGNAL(data_connect(bool)),w,SLOT(datashow(bool)),Qt::AutoConnection);

    QObject::connect(&inputevents,SIGNAL(sendconfigwarning(bool)),w,SLOT(configwarning(bool)),Qt::AutoConnection);

    QObject::connect(w,SIGNAL(send_IO_Reset()),&taotong,SLOT(rev_IO_Reset()),Qt::AutoConnection);
    QObject::connect(w,SIGNAL(send_TT_IO_value(int,int,int)),&taotong,SLOT(rev_TT_IO_Func(int,int,int)),Qt::AutoConnection);
    QObject::connect(&taotong,SIGNAL(sendbattery(QString)),w,SLOT(batteryshow1(QString)),Qt::AutoConnection);
    QObject::connect(&taotong,SIGNAL(sendbattery(QString)),&m,SLOT(batteryshow1(QString)),Qt::AutoConnection);
    QObject::connect(&taotong,SIGNAL(taotong_num(int)),w,SLOT(taotong_main(int)),Qt::AutoConnection);
    QObject::connect(w,SIGNAL(sendInfo(bool,bool,int,int)),&communication,SLOT(receiveInfo(bool,bool,int,int)),Qt::AutoConnection);

    /*
      * QInputContext 屏幕键盘类
      * 修改第三方文件
    */
    QInputContext *im = new InputMethod();
    a.setInputContext( im );



    return a.exec();
}
