#include "mainwindow.h"
#include "worker.h"
#include "GlobalVarible.h"
#include "serialComs.h"
#include "./TestInput_Cursor/inputmethod.h"
#include "fisupdate.h"
#include "fisupdatequeue.h"
#include "communication.h"
#include "tightenthread.h"
#include "xmlcreate.h"
#include "inputevents.h"
#include "wificonnect.h"
#include "reinsertserverdb.h"
#include "reinsertserverdbningbo.h"
#include "taotong.h"
#include "sqlthread.h"
#include "split.h"
#include "rfidcontroler.h"

bool isJS = true;                //是否是技术员
int isRFID = 0;                  //条码模式RS232或RFID或队列 0条码枪  1 RFID  2 队列模式
bool isNormal = false;           //状态是否正常
bool workmode = false;           //工作模式与否
bool CsIsConnect = false;        //CS351拧紧枪是否连接
bool RFIDISConnect = false;      //RFID是否连接
bool RDYY  = false;              //CS351是否准备
bool ISmaintenance = false;      //是否在点击配置页面中
bool ISRESET  = false;           //是否复位
bool ISWARNING = false;          //警告
bool Curve_Is_Null = false;      //曲线是否为空
bool TaoTongState = false;       //是否为套筒模式
bool WIFI_STATE = false;         //wifi状态
bool SerialGunMode = true;       //条码枪模式
bool DebugMode = false;          //调试模式
bool battery = true;             //电池与否
bool bound_enabled = false;      //是否启用扭矩角度 控制
bool QueueIsNull = true;         //队列为空标识
bool rfidNextCom = false;        //rfid下辆车已进站
int Line_ID = 1;                 //工线
int SerialMode = 0;
QString SYSS = "";               //data2运行状态  Ready  ING  OK NOK
QString SaveWhat="";             //保存参考变量
QString VIN_PIN_SQL= "";         //条码
QString VIN_PIN_SQL_RFID = "";   //RFID条码
QString SCREWID_SQL= "";         //螺栓编号
QByteArray time_info ="";
QString proNum="";               //当前程序号
QString lsNum="";                //额定螺栓数量
QString CURVE_STR="";
QString SqlUserName = "";
QString SqlPassword ="";
QString WirelessIp ="";
QString CS351Ip ="";
int QualifiedNum=0;              //合格数量
int TimeLeft = 0;                //进度条剩余时间
int enablenumberLeft = 0;        //剩余螺栓数量
int BackTimeLeft = 0;            //归位剩余时间
int TAOTONG = -1;
QString Operator = "";
QString info[7] ={};
QString status[20][5]={};
QString Version="";
QReadWriteLock lock;
QReadWriteLock RFIDlock;
QQueue<QVector<QString> > queue;

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
    QApplication a(argc, argv);
    QApplication::setOverrideCursor(Qt::BlankCursor);                   //去掉光标
//    qInstallMsgHandler(outputMessage);                                  //重定向日志到文件
    /**
   *Data2程序main函数开始初始化一些参数变量，无线网卡ip
   *
   *
   **/
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    configIniRead->value("baseinfo/serialMode").toInt()?SerialGunMode=true:SerialGunMode=false;
    isRFID = configIniRead->value("baseinfo/isRFID").toInt();
    Operator = configIniRead->value("baseinfo/Operator").toString();
    Line_ID = configIniRead->value("baseinfo/Line_ID").toInt();
    configIniRead->value("baseinfo/bound").toInt()?bound_enabled=true:bound_enabled=false;
    SqlUserName = configIniRead->value("baseinfo/SQLUserName").toString();
    SqlPassword = configIniRead->value("baseinfo/SQLPassword").toString();
    WirelessIp = configIniRead->value("baseinfo/WirelessIp").toString();
    CS351Ip = configIniRead->value("baseinfo/cs351Ip").toString();
    Version = "N2.004";
    qDebug()<< "Version:"<<Version;
    system((QString("ifconfig wlan0 ")+configIniRead->value("baseinfo/WirelessIp").toString()+" netmask "+configIniRead->value("baseinfo/netmask").toString()+" &").toLatin1().data());
    system((QString("ifconfig eth0 ")+configIniRead->value("baseinfo/LocalIp").toString()+" &").toLatin1().data());
    QString freetds = "/usr/local/arm/freetds/etc/freetds.conf";
    QFile file1(freetds);
    if(!file1.open(QIODevice::ReadOnly| QIODevice::Text))
    {
        qDebug()   << "Cannot open testdsn file1 for Reading";
    }
    QString str1 (file1.readAll());
    if(str1.contains("[testdsn1]", Qt::CaseInsensitive)&&str1.contains("[testdsn2]", Qt::CaseInsensitive))
    {
        str1.replace(QRegExp("\\[testdsn1\\]\\s*host = \\S*"),QString("[testdsn1]\n\thost = ")+configIniRead->value("baseinfo/CurveServerIp").toString());
        str1.replace(QRegExp("\\[testdsn2\\]\\s*host = \\S*"),QString("[testdsn2]\n\thost = ")+configIniRead->value("baseinfo/DataServerIp").toString());
    }
    file1.close();
    if(!file1.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        qDebug()   << "Cannot open testdsn file1 for Writing";
    }
    file1.write(str1.toUtf8());
    file1.close();

    QString wpa_conf = "/etc/wpa_supplicant.conf";
    QFile file2(wpa_conf);
    if(!file2.open(QIODevice::ReadOnly| QIODevice::Text)){
        qDebug()   << "Cannot open wifi file2 for Reading";
    }
    QString str2 (file2.readAll());
    if(str2.contains("ssid=", Qt::CaseInsensitive)&&str2.contains("psk=", Qt::CaseInsensitive)){
        str2.replace(QRegExp("\nssid=\\S*"),QString("\nssid=\"")+configIniRead->value("baseinfo/SSID").toString()+QString("\""));
        str2.replace(QRegExp("psk=\\S*"),QString("psk=\"")+configIniRead->value("baseinfo/psk").toString()+QString("\""));
    }
    file2.close();
    if(!file2.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
        qDebug()   << "Cannot open wifi file2 for Writing";
    }
    file2.write(str2.toUtf8());
    file2.close();

    system((QString("wpa_cli set_network 0 ssid '\"")+configIniRead->value("baseinfo/SSID").toString()+QString("\"'")+" &").toLatin1().data());
    system((QString("wpa_cli set_network 0 psk  '\"")+configIniRead->value("baseinfo/psk").toString()+QString("\"'")+" &").toLatin1().data());
    system("wpa_cli disable_network 0 &");
    system("wpa_cli enable_network 0 &");
    delete configIniRead;
    time_info.resize(7);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QString k_power;
    QFile power("/var/power");
    power.open(QIODevice::ReadOnly);
    QTextStream in_power(&power);
    in_power >> k_power;
    if(k_power=="11")
        battery=false;
    power.close();
    /*
  * Worker:工作线程
  * 用来接收CS351 B端口发来心跳曲线
*/
    Worker work;
    QTimer::singleShot(0,&work,SLOT(fun1()));

    /*
      * MainWindow:主界面UI
      * 用来显示状态 不可有阻塞操作
    */
    MainWindow w ;
    w.show();

    /*
      * TightenThread:控制CS351线程
      * 发送复位 心跳 使能命令同时与UI交互
    */
    TightenThread  tighten;
    QTimer::singleShot(0,&tighten,SLOT(tightenStart()));

    /*
      * XMLCreate:xml配置生成
      * 主要用来跟上位机通讯
    */
    XMLCreate XML_Create ;
    QTimer::singleShot(0,&XML_Create,SLOT(XML_CreateStart()));

    /*
      * Wificonnect:wifi网络状态判断
      * 包括服务器 rfid连接状态5秒检测一下
    */
    Wificonnect Wifi_Connect ;
    QTimer::singleShot(0,&Wifi_Connect,SLOT(Wifi_ConnectStart()));

    /*
      * FisUpdate:FIS更新线程
      * 与服务器FIS交互定期从server拉取fis数据
    */
    FisUpdate fisupdate;
    QTimer::singleShot(0,&fisupdate,SLOT(myfistimer()));

//    FisUpdateQueue fisupdatequeue;
//    QTimer::singleShot(0,&fisupdatequeue,SLOT(myfistimer()));

    DeleteCarSQL deletecarsql;
    QTimer::singleShot(0,&deletecarsql,SLOT(SQL_Delete_Car()));

    /*
      * SqlThread:本地存储数据线程
      * 与Worker线程交互拿到数据也存储到本地mysql
    */
    SqlThread sqlthread;
    QTimer::singleShot(0,&sqlthread,SLOT(sqlinit()));

    /*
      * Communication:与上位机通讯线程
      * 可配置线程
    */
    Communication communication ;
    QTimer::singleShot(0,&communication,SLOT(mycommunicationtimer()));

    /*
      * ReInsertServerDb 与ReInsertServerDbNingBo:上传数据线程
      * 此处可配置写入不同数据库，后期版本统一方便
      * ReInsertServerDb适用于大众三厂标准数据库
      * ReInsertServerDbNingBo 适用于宁波大众 数据库
    */
    ReInsertServerDb *reinsert;
    ReInsertServerDbNingBo *reinsertningbo;
    if(isRFID!=1)
    {
        //reinsert = new ReInsertServerDb;
        //QTimer::singleShot(0,reinsert,SLOT(reinsert_start()));
        reinsertningbo = new ReInsertServerDbNingBo;
        QTimer::singleShot(0,reinsertningbo,SLOT(reinsert_start()));
    }
    else
    {
        reinsertningbo = new ReInsertServerDbNingBo;
        QTimer::singleShot(0,reinsertningbo,SLOT(reinsert_start()));
    }

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
    QTimer::singleShot(0,&taotong,SLOT(T_start()));

    /*
      * Split:多屏幕显示线程
      * 接收从机数据分屏显示
    */
    Split split;
    QTimer::singleShot(0,&split,SLOT(mysplit_timer()));

    /*
      * SerialComs:RS232串口线程
      * 接收条码枪扫描条码
    */
    SerialComs    *serial;

    /*
      * RfidControler:RFID控制线程
      * 接收RFID条码
    */
    RfidControler *rfid;
    if(isRFID!=1)
    {
        serial = new SerialComs;
        QTimer::singleShot(0,serial,SLOT(comInit()));
        QObject::connect(serial,SIGNAL(serialCom(QString,bool,QString)),&w,SLOT(getSerialNum(QString,bool,QString)),Qt::AutoConnection);
    }
    else
    {
        rfid = new RfidControler;
        QTimer::singleShot(0,rfid,SLOT(rfidInit()));
        QObject::connect(rfid,SIGNAL(sendPinToUi(QString,bool,QString)),&w,SLOT(getSerialNum(QString,bool,QString)),Qt::AutoConnection);
        QObject::connect(rfid,SIGNAL(rfidConnected(bool)),&w,SLOT(setRfidState(bool)),Qt::AutoConnection);
        QObject::connect(rfid,SIGNAL(sendGetCar()),&w,SLOT(receiveGetCar()),Qt::AutoConnection);
        QObject::connect(&Wifi_Connect,SIGNAL(rfidconnect(bool)),rfid,SLOT(rfidDisconnectDo(bool)),Qt::AutoConnection);
    }


    /*
      * 下面都是信号槽的连接 具体看代码
      *
    */
    QObject::connect(&work,SIGNAL(sendfromworkthread(QString *)),&sqlthread,SLOT(sqlinsert(QString *)),Qt::AutoConnection);
    QObject::connect(&work,SIGNAL(send_mainwindow(QString,QString,QString)),&w,SLOT(fromsecondthreaddata(QString,QString,QString)),Qt::AutoConnection);
    QObject::connect(&w,SIGNAL(sendfromsecondthread(QString ,QString,QString)),&sqlthread,SLOT(configOne(QString,QString,QString)),Qt::AutoConnection);
    QObject::connect(&sqlthread,SIGNAL(send_mysqlerror()),&w,SLOT(signal_mysqlerror_do()),Qt::AutoConnection);
    QObject::connect(&w,SIGNAL(sendConfigureAll(int,int,int,int)),&sqlthread,SLOT(receiveConfigureAll(int,int,int,int)),Qt::AutoConnection);

    //QObject::connect(&change,SIGNAL(sendSerial(QString)),w.newconfiginfo,SLOT(receiveDebug(QString)),Qt::AutoConnection);
    QObject::connect(&work,SIGNAL(havedconnect(bool)),&tighten,SLOT(receivehavedconnect(bool)),Qt::AutoConnection);
    QObject::connect(&w,SIGNAL(clo()),&work,SLOT(fromNew1thread()),Qt::AutoConnection);
    QObject::connect(&tighten,SIGNAL(IsTigntenReady(bool)),&w,SLOT(TightenIsReady(bool)),Qt::AutoConnection);
    QObject::connect(&tighten,SIGNAL(GunNotReady()),&w,SLOT(ReceGunNotReady()),Qt::AutoConnection);
    QObject::connect(&tighten,SIGNAL(ReconnectSendOperate()),&w,SLOT(receiveOperate()),Qt::AutoConnection);
    QObject::connect(&w,SIGNAL(sendoperate()),&tighten,SLOT(sendReadOperate()),Qt::AutoConnection);
    QObject::connect(&communication,SIGNAL(update()),&w,SLOT(init()),Qt::AutoConnection);

    QObject::connect(&communication,SIGNAL(xmlcreate()),&XML_Create,SLOT(updateXML()),Qt::AutoConnection);
    QObject::connect(w.newconfiginfo,SIGNAL(xmlcreate()),&XML_Create,SLOT(updateXML()),Qt::AutoConnection);
    QObject::connect(&Wifi_Connect,SIGNAL(wifi_connect(bool)),&w,SLOT(wifishow(bool)),Qt::AutoConnection);
    QObject::connect(&Wifi_Connect,SIGNAL(data_connect(bool)),&w,SLOT(datashow(bool)),Qt::AutoConnection);

    QObject::connect(&inputevents,SIGNAL(sendconfigwarning(bool)),&w,SLOT(configwarning(bool)),Qt::AutoConnection);
    QObject::connect(&inputevents,SIGNAL(sendbatterysta(bool)),&w,SLOT(batteryshow2(bool)),Qt::AutoConnection);
    QObject::connect(&taotong,SIGNAL(sendbattery(QString)),&w,SLOT(batteryshow1(QString)),Qt::AutoConnection);
    QObject::connect(&taotong,SIGNAL(taotong_num(int)),&w,SLOT(taotong_main(int)),Qt::AutoConnection);
    QObject::connect(w.newconfiginfo,SIGNAL(sendGetTime()),&fisupdate,SLOT(QueryTime()),Qt::AutoConnection);
    QObject::connect(&fisupdate,SIGNAL(sendTime(QString)),w.newconfiginfo,SLOT(receivetime(QString)),Qt::AutoConnection);
    QObject::connect(&fisupdate,SIGNAL(time_error(bool)),&w,SLOT(time_warning(bool)),Qt::AutoConnection);
    QObject::connect(w.newconfiginfo,SIGNAL(column_update(QString)),&fisupdate,SLOT(update_column(QString)),Qt::AutoConnection);
//    QObject::connect(w.newconfiginfo,SIGNAL(sendGetTime()),&fisupdatequeue,SLOT(QueryTime()),Qt::AutoConnection);
//    QObject::connect(&fisupdatequeue,SIGNAL(sendTime(QString)),w.newconfiginfo,SLOT(receivetime(QString)),Qt::AutoConnection);
//    QObject::connect(&fisupdatequeue,SIGNAL(time_error(bool)),&w,SLOT(time_warning(bool)),Qt::AutoConnection);
//    QObject::connect(w.newconfiginfo,SIGNAL(column_update(QString)),&fisupdatequeue,SLOT(update_column(QString)),Qt::AutoConnection);
//    QObject::connect(&fisupdatequeue,SIGNAL(FisSendSerial(QString)),&w,SLOT(ReceFisSerial(QString)),Qt::AutoConnection);

//    QObject::connect(&tighten,SIGNAL(IsTigntenReady(bool)),w.vinreverse,SLOT(haveconnect(bool)),Qt::AutoConnection);


    /*
      * QInputContext 屏幕键盘类
      * 修改第三方文件
    */
    QInputContext *im = new InputMethod();
    a.setInputContext( im );


    return a.exec();
}
