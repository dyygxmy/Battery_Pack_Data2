#ifndef NEWCONFIGINFO_H
#define NEWCONFIGINFO_H

#include <QDialog>
#include <QFileDialog>
#include<QMouseEvent>
#include <QPushButton>
#include <QListWidgetItem>
#include <QDir>
#include <QStringList>
#include "optiondialog.h"
#include <QGraphicsDropShadowEffect>
#include "save.h"
#include "basicset.h"
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QTableWidget>
#include <QHeaderView>
#include <QList>
#include "paintarea.h"
#include "inoutput.h"

//#include <QSqlQuery>

namespace Ui {
class Newconfiginfo;
}

class Newconfiginfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit Newconfiginfo(QWidget *parent = 0);
    ~Newconfiginfo();
    void initui();
    void setinitUi(int);
    void updownReadOperate(int);  // 1 up   0 down
    void updownWriteOperate(int); // 1 up   0 down
    void moveDo();  //pushbutton_right do something
    void warnings();
    void buttclicked();
    void xuanchoux(int);
    void judge();
    void baseInfoIsChange();
    void advancedIsChange();
    void masterslaveIsChange();
    void wifi_connect();
    void backShow(); //取消之后的效果
    void pagechange();//历史查询翻页
    void connect_localMySQL();
    void mysqlopen();
    void historyclear();
    void savePDM();
    void clearCache();
    void bound_show();
    void bound_save();
    void boundIsChange();
    void show_bound();
    void bound_init();
    void bound_update();
    void history();
    void PDMEdit();
    void configList();
    void systemConfigure();
    void restartShow(bool);
    void queryResult(QString);
    QString vari2_XL;
signals:
    void closeconfig();
    void sendGetTime();
    void xmlcreate();
    void sendTrayCntinue(bool);
    void column_update(QString);
    void sendTruncateResult(bool);

public slots:
    void receivetime(QString);
    void receiveoptioninfo(QString,QString,bool);
    void on_pushButton_clicked();
    void receiveSaveState(bool);
    void on_listWidget_currentRowChanged(int currentRow);
    void receivecancelpdm();
    void receivebaseinfocancel();
    void receiveBaseinfo(QString,QString,QString,QString);
    void receiveBaseinfoSave(bool);
    void pdmSelect(QListWidgetItem*);
    void pdminit();
    void receiveMasterSlaveState(bool);
    void receiveDebug(QString);
    void onShowPushButon();//显示当前模组绑定bypass图标

    void on_pushButton_59_clicked();

    void on_pushButton_63_clicked();

    void on_pushButton_64_clicked();

    void on_pushButton_65_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_butt1_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_xuan1_clicked();

    void on_pushButton_xuan2_clicked();

    void on_pushButton_xuan3_clicked();

    void on_pushButton_xuan4_clicked();

    void on_pushButton_xuan5_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    bool eventFilter(QObject *, QEvent *);

    void on_pushButton_28_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_32_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_33_clicked();


    void on_comboBox_2_currentIndexChanged(const QString &arg1);

    void on_pushButton_24_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_52_clicked();

    void on_pushButton_17_clicked();



    void on_pushButton_butt2_clicked();

    void on_pushButton_butt3_clicked();

    void on_pushButton_butt4_clicked();

    void on_pushButton_butt5_clicked();

    void on_pushButton_62_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_34_clicked();


    //    void on_pushButton_38_clicked();

    //    void on_pushButton_51_clicked();

    //    void on_pushButton_37_clicked();

    //    void on_pushButton_50_clicked();

    //    void on_pushButton_87_clicked();

    //    void on_pushButton_88_clicked();

    void on_pushButton_61_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();



    void ShowTime();

    void on_pushButton_68_clicked();

    void on_pushButton_69_clicked();

    void on_pushButton_80_clicked();

    void on_pushButton_85_clicked();

    void on_pushButton_81_clicked();

    void on_pushButton_92_clicked();

    void on_pushButton_82_clicked();

    void on_pushButton_93_clicked();

    void on_pushButton_83_clicked();

    void on_pushButton_94_clicked();

    void on_pushButton_84_clicked();

    void on_pushButton_95_clicked();

    void on_pushButton_58_clicked();

    void on_pushButton_66_clicked();

    void on_pushButton_num1_clicked();

    void on_pushButton_num2_clicked();

    void on_pushButton_num3_clicked();

    void on_pushButton_num4_clicked();

    void on_pushButton_num5_clicked();

    void on_pushButton_num6_clicked();

    void on_pushButton_num7_clicked();

    void on_pushButton_num8_clicked();

    void on_pushButton_num9_clicked();

    void on_pushButton_num0_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_98_clicked();

    void on_pushButton_97_clicked();

    void on_pushButton_100_clicked();




    void on_pushButton_bxuan1_clicked();

    void on_pushButton_bxuan2_clicked();

    void on_pushButton_bxuan3_clicked();

    void on_pushButton_bxuan4_clicked();

    void on_pushButton_bxuan5_clicked();

    void on_pushButton_kxuan1_clicked();

    void on_pushButton_kxuan2_clicked();

    void on_pushButton_kxuan3_clicked();

    void on_pushButton_kxuan4_clicked();

    void on_pushButton_kxuan5_clicked();

    void on_pushButton_89_clicked();

    void on_pushButton_90_clicked();

    void receiveDesignle(bool);



    void on_pushButton_search_clicked();

    void on_pushButton_first_clicked();

    void on_pushButton_last_clicked();

    void on_pushButton_previous_clicked();

    void on_pushButton_next_clicked();

    void on_pushButton_pronum_add_clicked();

    void on_pushButton_pronum_minus_clicked();

    void on_pushButton_number_add_clicked();

    void on_pushButton_number_minus_clicked();

    void on_pushButton_xuanpronum_add_clicked();

    void on_pushButton_xuanpronum_minus_clicked();

    void on_pushButton_xuannumber_add_clicked();

    void on_pushButton_xuannumber_minus_clicked();


    void on_pushButton_36_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_86_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_1_clicked();

    void on_pushButton_37_clicked();

    void receiveBound(bool);

    void mysqlTruncate();

private slots:
    void on_pushButton_49_clicked();

    void on_pushButton_taotong_add_clicked();

    void on_pushButton_taotong_minus_clicked();

    void on_pushButton_50_clicked();

    void on_Line_radioButton_1_clicked();

    void on_Line_radioButton_2_clicked();

    void on_pushButton_104_clicked();

    void on_pushButton_55_clicked();

    void on_pushButton_tens_add_clicked();

    void on_pushButton_tens_minus_clicked();

    void on_pushButton_ones_add_clicked();

    void on_pushButton_ones_minus_clicked();

    void on_pushButton_88_clicked();

    void on_pushButton_87_clicked();

    void on_pushButton_bound_prev_clicked();

    void on_pushButton_bound_next_clicked();

    void on_pushButton_bound_clicked();

    void on_pushButton_56_clicked();

    void on_Led_red_clicked();

    void on_Led_green_clicked();

    void on_Led_yellow_clicked();

    void on_Led_white_clicked();


    void on_IO_Reset_clicked();

    void on_Led_nok_clicked();
	

    void on_pushButton_channel_add_clicked();

    void on_pushButton_channel_minus_clicked();

    void on_pushButton_22_clicked();


    void on_pushButton_channel_add_2_clicked();

    void on_pushButton_channel_minus_2_clicked();

    void on_pushButton_taotong_add_2_clicked();

    void on_pushButton_taotong_minus_2_clicked();

    void on_pushButton_pronum_add_history_clicked();

    void on_pushButton_pronum_minus_history_clicked();

    void on_pushButton_down_clicked();

    void on_pushButton_up_clicked();

    void on_pushButton_threeDays_clicked();

    void on_pushButton_oneWeek_clicked();

    void on_pushButton_oneMonth_clicked();

    void on_tabWidget_currentChanged(int index);


    void on_pushButton_taryContinue_clicked();

private:
    Ui::Newconfiginfo *ui;
    InOutput *IOput;
    // Ui::OptionDialog * uioption;
    bool isbarcode;
    bool isrfid;
    bool isqueue;
    int line_ID;
    int pagenum;
    int whichcar; //哪个车型被选中
    int isedit;
    int currentpages;
    bool optionIscheck;
    int whichoption;
    QPushButton *butt1[100];
    QPushButton * label[100];
    int numpdm;
    int temppdm ;
    QString pathpdm;

    QString  filename;
    int ispdminit;
    bool  ifcansave;
    int nowpdmnum;
    int nowcomboxnum;
    QString whichpdmyinyong;
    QString bxuanname;
    QString bxuancode;
    QString kxuanname;
    QString kxuancode;
    int bxuanpeinum;
    int kxuanpeinum;
    bool isxuanlook;
    bool isSavedpdm;
    int previouswhichar;
    OptionDialog *od;
    QGraphicsOpacityEffect *e3;
    int bxuanwhich; //哪个必选装件被选中
    bool bxuanstate; //比选还是可选
    QString bxuannamelist[20][20];
    QString bxuancodelist[20][20];
    QString kxuannamelist[20][20];
    QString kxuancodelist[20][20];
    QString lsnumersxuanlist[20][20];//数量
    QString luoxuanlist[20][20];//暂存螺栓编号
    QString proxuanlist[20][20];//程序号
    QString channelxuanlist[20][20];
    QString taotongxuanlist[20][20];

    QString bxuannamelist2[20][20];
    QString bxuancodelist2[20][20];
    QString kxuannamelist2[20][20];
    QString kxuancodelist2[20][20];
    QString lsnumersxuanlist2[20][20];//数量
    QString luoxuanlist2[20][20];//暂存螺栓编号
    QString proxuanlist2[20][20];
    QString channelxuanlist2[20][20];
    QString taotongxuanlist2[20][20];
    QString PDMxuan_Name[20];
    QString PDMxuan_Name2[20];
    QPushButton  *buttonbox[5];

    QString carStyle;
    QString G9tmp;
    QString Vintmp;
//    bool Out[4];
    QString PDM_Name;
    QString PDM_Name2;
    QString luo[20];//暂存螺栓编号
    QString pro[20];//程序号
    QString lsnumers[20];//数量
    bool ch[20][5];   //channel
    QString luo2[20];//暂存螺栓编号
    QString pro2[20];//程序号
    QString lsnumers2[20];//数量
    bool ch2[20][5];   //channel
    QString taotong[20];
    QString taotong2[20];
    QString channel[20];
    QString channel2[20];
//    QString pdmxuanlist[20];
//    QString pdmxuanlist2[20];
    QString pdmxuan;

    int isFull;
    QString temp;
    int person;
    int change;
    QString newpassword;
    Save * save;
    BasicSet *basicset;

    int year;
    int month;
    int date;
    int hour;
    int minute;
    int second;

    QString controllerIp_01;
    QString controllerIp_02;
    QString PortA;
    QString PortB;
    QString RfidIp;
    QString RfidPort;
    QString DataServerIp;
    QString CurveServerIp;
    QString AndonServerIp;
    QString ServerPort;
    QString BarcodeGun;
    bool isbaseinfochange;
    bool isadvancedchange;
    bool ismasterslavechange;
    QString  whichButtonClick;

    int pages;
    int thepage;
    QString thepages;
    QString affall;
    QString aff;
    QSqlQueryModel *model;
    QString MysqlMin;
    QSqlDatabase db;
    QSqlQuery *query;
    QSqlQuery *query1;
    QString VIN;
    QString ScrewID;
    QString condition;
    int new_pronum;
    int new_number;
    bool isoptionsaved;
    bool isoption;

    QString bound[100][4];
    int PronumNow;
    bool isboundchange;
    QString bound_temp[100][4];
    int bound_pages;
    int bound_current_page;
    QList<int> list;
    QList<int>::iterator iter;
    int current;
    QStringList header_vertical;
    QTableWidgetItem *tableWidgetItem[10][5];
    bool bound_enabled_temp;
    bool restart_enabled_temp;

    PaintArea *area;
    QSqlQuery query_number;
    QSqlQuery query_datas;
    QSqlQuery query_bound;
};

#endif // NEWCONFIGINFO_H
