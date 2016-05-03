#include "newconfiginfo.h"
#include "ui_newconfiginfo.h"
#include "GlobalVarible.h"
#include <QSettings>
#include <QDebug>
#include <QTime>
#include <QDateTime>
#include <QSqlError>
#include <QTableView>
#include <QSqlQuery>
#include <QProcess>
#include <QScrollBar>

Newconfiginfo::Newconfiginfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Newconfiginfo)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    // this->setCursor(Qt::BlankCursor); //去掉光标
    this->setGeometry(QRect(0, 0, 1366, 768));
    pagenum = 1;
    optionIscheck = false;
    numpdm = 0;
    temppdm = 0;
    isrfid = isRFID;
    line_ID = Line_ID;
    ui->label_100->hide();
    whichcar = 0;
    previouswhichar = whichcar;
    isedit = -1;
    pages = 0;
    thepage = 0;
    currentpages = 0;
    ifcansave = false;
    ispdminit = 0;
    nowpdmnum = 0;
    nowcomboxnum = 0;
    whichoption = 0;
    isxuanlook = true;
    isbaseinfochange = false;
    isadvancedchange = false;
    ismasterslavechange = false;
    isoptionsaved = true; // option is saved or not
    isSavedpdm = true;//是否 pdm正常保存
    bxuanwhich = 0;
    isoption  = false;
    PronumNow = 0;
    isboundchange = false;
    buttonbox[0] = ui->pushButton_butt1;
    buttonbox[1] = ui->pushButton_butt2;
    buttonbox[2] = ui->pushButton_butt3;
    buttonbox[3] = ui->pushButton_butt4;
    buttonbox[4] = ui->pushButton_butt5;
    //    ui->label_taotongg->hide();
    //    ui->pushButton_taotong_add->hide();
    //    ui->pushButton_taotong_minus->hide();
    //    ui->lineEdit_taotong->hide();
    ui->lineEdit_taotong->setText(tr("无"));
    ui->pushButton_10->setEnabled(false);
    ui->pushButton_11->setEnabled(false);
    ui->label_version->setText(Version);
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);

    cs351Ip = configIniRead->value("baseinfo/cs351Ip").toString();
    PortA = configIniRead->value("baseinfo/PortA").toString();
    PortB = configIniRead->value("baseinfo/PortB").toString();
    RfidIp = configIniRead->value("baseinfo/RfidIp").toString();
    RfidPort = configIniRead->value("baseinfo/RfidPort").toString();
    DataServerIp = configIniRead->value("baseinfo/DataServerIp").toString();
    CurveServerIp = configIniRead->value("baseinfo/CurveServerIp").toString();
    AndonServerIp = configIniRead->value("baseinfo/AndonServerIp").toString();
    BarcodeGun = configIniRead->value("baseinfo/BarcodeGun").toString();
    delete configIniRead;

    QRegExp rx("[0-9.]{7,15}");
    QValidator *validator = new QRegExpValidator(rx, this );
    ui->lineEdit_localip->setValidator(validator);
    ui->lineEdit_wirelessip->setValidator(validator);
    ui->lineEdit_netmask->setValidator(validator);
    ui->lineEdit_gateway->setValidator(validator);
    ui->lineEdit_tacktime->setValidator(validator);
    ui->lineEdit_slave_1->setValidator(validator);
    ui->lineEdit_slave_2->setValidator(validator);
    ui->lineEdit_slave_3->setValidator(validator);
    ui->lineEdit_torque_max->setValidator(validator);
    ui->lineEdit_torque_min->setValidator(validator);
    ui->lineEdit_angle_max->setValidator(validator);
    ui->lineEdit_angle_min->setValidator(validator);
    if(isRFID!=1)
        ui->lineEdit_G9->setEnabled(false);
    QRegExp rx1("[0-9]{1,9}");
    QValidator *validator1 = new QRegExpValidator(rx1, this );
    ui->year->setValidator(validator1);
    ui->month->setValidator(validator1);
    ui->date->setValidator(validator1);
    ui->hour->setValidator(validator1);
    ui->minute->setValidator(validator1);
    ui->second->setValidator(validator1);
    ui->lineEdit_pronum->setValidator(validator1);
    ui->lineEdit_number->setValidator(validator1);
    ui->lineEdit_xuanpronum->setValidator(validator1);
    ui->lineEdit_xuannumber->setValidator(validator1);
    ui->lineEdit_Lsnumber->setValidator(validator1);
    ui->lineEdit_xuanLsnum->setValidator(validator1);

    ui->lineEdit_psk->setEchoMode(QLineEdit::Password);
    ui->label_bx1name->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_bx1code->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_bx2name->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_bx2code->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_bx3name->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_bx3code->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_bx4name->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_bx4code->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_bx5name->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_bx5code->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_kx1name->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_kx1code->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_kx2name->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_kx2code->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_kx3name->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_kx3code->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_kx4name->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_kx4code->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_kx5name->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_kx5code->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->pushButton_70->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->pushButton_71->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->pushButton_72->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->pushButton_73->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->pushButton_74->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->pushButton_75->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->pushButton_76->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->pushButton_77->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->pushButton_78->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->pushButton_79->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->stackedWidget_6->setCurrentIndex(1);

    connect_localMySQL();
    bound_init();
    // pdminit();
}

Newconfiginfo::~Newconfiginfo()
{
    delete ui;
}

void Newconfiginfo::clearCache()
{
    ui->label_carname->clear();
    ui->label_G9->clear();
    ui->label_VIN->clear();
    ui->label_isxuan->clear();
    ui->label_Lsnum->clear();
    ui->label_pronum->clear();
    ui->label_number->clear();
    // ui->label_pageum->setText("1");
    pagenum = 2;
    on_pushButton_8_clicked();

}
void Newconfiginfo::initui()
{
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_3->setCurrentIndex(0);
    ui->pushButton_12->hide();
    ui->pushButton_15->hide();
    ui->pushButton_16->hide();
    ui->pushButton_14->hide();
    ui->label_83->show();
    ui->label_84->hide();
    ui->label_85->hide();
    ui->label_86->hide();
    //管理员   技术员
    //isJS = false;

    //读取车型配置信息

    for(int i = 1;i < 6;i++ )
    {
        if(configIniRead->value(QString("carinfo").append(QString::number(i)).append("/IsOptionalStation")).toInt())
        {
            buttonbox[i-1]->setText(configIniRead->value(QString("carinfo").append(QString::number(i)).append("/carcx")).toString());
            buttonbox[i-1]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/67.bmp);font:14pt;color: rgb(248, 248, 255);");
        }
        else
        {
            for(int k = 1;k < 6;k++)
            {
                if(configIniRead->value(QString("carinfo").append(QString::number(i)).append("/LSNumber").append(QString::number(k))).toInt())
                {
                    buttonbox[i-1]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/67.bmp);font:14pt;color: rgb(248, 248, 255);");
                    buttonbox[i-1]->setText(configIniRead->value(QString("carinfo").append(QString::number(i)).append("/carcx")).toString());
                    break;
                }
            }
        }

    }

    if(isJS)
    {
        //技术
        ui->label_86->hide();
        ui->pushButton_28->hide();
        ui->stackedWidget_2->setCurrentIndex(0);
        ui->label_stationname->setText(configIniRead->value("baseinfo/StationName").toString());
        ui->label_stationid->setText(configIniRead->value("baseinfo/StationId").toString());
        ui->label_localip->setText(configIniRead->value("baseinfo/LocalIp").toString());
        ui->label_4->setText(configIniRead->value("baseinfo/Operator").toString());
        ui->label_6->setText(configIniRead->value("baseinfo/WirelessIp").toString());
        ui->label_csip->setText(configIniRead->value("baseinfo/cs351Ip").toString());
        ui->label_csporta->setText(configIniRead->value("baseinfo/PortA").toString());
        ui->label_csportb->setText(configIniRead->value("baseinfo/PortB").toString());
        ui->label_rfip->setText(configIniRead->value("baseinfo/RfidIp").toString());
        ui->label_rfport->setText(configIniRead->value("baseinfo/RfidPort").toString());
        ui->label_dataserverip->setText(configIniRead->value("baseinfo/DataServerIp").toString());
        ui->label_curveserverip->setText(configIniRead->value("baseinfo/CurveServerIp").toString());
        ui->label_BarcodeGun_2->setText(configIniRead->value("baseinfo/BarcodeGun").toString());
        ui->label_14->setText(configIniRead->value("baseinfo/SSID").toString());
        ui->label_5->setText(configIniRead->value("baseinfo/TackTime").toString());   //节拍时间


        if(isRFID == 1)   //RFID 条码抢模式的 界面初始化
        {
            ui->stackedWidget_7->setCurrentIndex(0);
        }
        else
        {
            ui->stackedWidget_7->setCurrentIndex(1);
        }
    }
    else
    {
        //管理
        if(isRFID == 1)   //RFID 条码抢模式的 界面初始化
        {
            ui->stackedWidget_4->setCurrentIndex(0);
        }
        else
        {
            ui->stackedWidget_4->setCurrentIndex(1);
        }
        ui->pushButton_28->show();
        ui->stackedWidget_2->setCurrentIndex(2);

        // ui->stackedWidget_4->setCurrentIndex(0);
        ui->stackedWidget_5->setCurrentIndex(0);

        ui->lineEdit_staname->setText(configIniRead->value("baseinfo/StationName").toString());
        ui->lineEdit_Operator->setText(configIniRead->value("baseinfo/Operator").toString());
        ui->lineEdit_localip->setText(configIniRead->value("baseinfo/LocalIp").toString());
        ui->lineEdit_wirelessip->setText(configIniRead->value("baseinfo/WirelessIp").toString());
        ui->lineEdit_staid->setText(configIniRead->value("baseinfo/StationId").toString());
        ui->label_97->setText(configIniRead->value("baseinfo/cs351Ip").toString());
        ui->label_155->setText(configIniRead->value("baseinfo/RfidIp").toString());
        ui->label_154->setText(configIniRead->value("baseinfo/DataServerIp").toString());
        ui->label_153->setText(configIniRead->value("baseinfo/BarcodeGun").toString());
        //        ui->lineEdit_csip->setText(configIniRead->value("baseinfo/cs351Ip").toString());
        //        ui->lineEdit_csporta->setText(configIniRead->value("baseinfo/PortA").toString());
        //        ui->lineEdit_csportb->setText(configIniRead->value("baseinfo/PortB").toString());
        //        ui->lineEdit_serip->setText(configIniRead->value("baseinfo/ServerIp").toString());
        //        ui->lineEdit_serport->setText(configIniRead->value("baseinfo/ServerPort").toString());


    }

    delete configIniRead;
}
void Newconfiginfo::on_pushButton_clicked()
{
    //QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
    ui->pushButton_butt1->setEnabled(true);
    ui->pushButton_butt2->setEnabled(true);
    ui->pushButton_butt3->setEnabled(true);
    ui->pushButton_butt4->setEnabled(true);
    ui->pushButton_butt5->setEnabled(true);
    if(SaveWhat == "pdm")
    {
        ui->label_69->show();
        ui->comboBox->show();
        e3 = new QGraphicsOpacityEffect(this);
        e3->setOpacity(0.5);
        ui->label_100->setGraphicsEffect(e3);
        ui->label_100->show();
        ui->label_100->setGeometry(0,0,1366,768);
        SaveWhat = "pdmoutmain";
        save = new Save(this);
        connect(save,SIGNAL(sendDeSingle(bool)),this,SLOT(receiveDesignle(bool)));
        save->show();
    }
    else
    {
        if(isJS)
        {
            //QSqlDatabase::removeDatabase("QMYSQL");
            //QSqlDatabase::removeDatabase("qt_sql_default_connection");
            historyclear();
            //workmode = true;
            //config->setValue("baseinfo/workmode","0");
            workmode = false;
            ISmaintenance = false;
            int tempdata = numpdm;
            for(int i = 0;i<tempdata;i++)
            {
                delete butt1[i];
                numpdm--;
            }
            ui->listWidget->setCurrentRow(1);
            this->close();
            emit closeconfig();
            emit xmlcreate();
            //delete this;
        }
        else
        {
            whichButtonClick = "baseback";
            if(!isbaseinfochange)
            {
                baseInfoIsChange();
            }
            else
            {
                //QSqlDatabase::removeDatabase("QMYSQL");
                //QSqlDatabase::removeDatabase("qt_sql_default_connection");
                historyclear();
                //workmode = true;
                //config->setValue("baseinfo/workmode","0");
                workmode = false;
                ISmaintenance = false;
                int tempdata = numpdm;
                for(int i = 0;i<tempdata;i++)
                {
                    delete butt1[i];
                    numpdm--;
                }
                ui->listWidget->setCurrentRow(1);
                DebugMode = false;
                this->close();
                emit closeconfig();
                emit xmlcreate();
                system("cp /config.ini /config1.ini &");
            }
        }

    }

    // on_pushButton_15_clicked();
    //delete config;
}

void Newconfiginfo::baseInfoIsChange()
{
    // 监听
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    if (cs351Ip != configIniRead->value("baseinfo/cs351Ip").toString()||
            PortA != configIniRead->value("baseinfo/PortA").toString()||
            PortB != configIniRead->value("baseinfo/PortB").toString()||
            RfidIp != configIniRead->value("baseinfo/RfidIp").toString()||
            RfidPort != configIniRead->value("baseinfo/RfidPort").toString()||
            DataServerIp != configIniRead->value("baseinfo/DataServerIp").toString()||
            CurveServerIp != configIniRead->value("baseinfo/CurveServerIp").toString()||
            AndonServerIp != configIniRead->value("baseinfo/AndonServerIp").toString()||
            BarcodeGun != configIniRead->value("baseinfo/BarcodeGun").toString()||
            ui->lineEdit_staname->text() != configIniRead->value("baseinfo/StationName").toString()||
            ui->lineEdit_Operator->text() != configIniRead->value("baseinfo/Operator").toString()||
            ui->lineEdit_localip->text() != configIniRead->value("baseinfo/LocalIp").toString()||
            ui->lineEdit_wirelessip->text() != configIniRead->value("baseinfo/WirelessIp").toString()||
            ui->lineEdit_staid->text() != configIniRead->value("baseinfo/StationId").toString() )
    {
        e3 = new QGraphicsOpacityEffect(this);
        e3->setOpacity(0.5);
        ui->label_100->setGraphicsEffect(e3);
        ui->label_100->show();
        ui->label_100->setGeometry(0,0,1366,768);
        SaveWhat = "baseinfo";
        save = new Save(this);
        connect(save,SIGNAL(sendSaveBaseinfo(bool)),this,SLOT(receiveBaseinfoSave(bool)));
        save->show();
        isbaseinfochange = true;
        delete configIniRead;
    }
    else
    {
        delete configIniRead;
        isbaseinfochange = true;
        if (whichButtonClick == "baseback")
        {
            on_pushButton_clicked();
            isbaseinfochange = false;
        }
        else if (whichButtonClick == "about")
        {
            on_pushButton_13_clicked();
            isbaseinfochange = false;
        }
        else if (whichButtonClick == "PDMEdit")
        {
            on_pushButton_28_clicked();
            isbaseinfochange = false;
        }
        else if (whichButtonClick == "config")
        {
            ui->stackedWidget_2->setCurrentIndex(4);
            ui->label_83->hide();
            ui->label_84->show();
            ui->label_85->hide();
            ui->label_86->hide();
            //            ui->pushButton_13->hide();
            //            ui->label_52->setFocus();
            isbaseinfochange = false;
        }
        else if (whichButtonClick == "history")
        {
            ui->stackedWidget_2->setCurrentIndex(3);
            ui->label_83->hide();
            ui->label_84->hide();
            ui->label_85->show();
            ui->label_86->hide();
            //            ui->pushButton_13->hide();
            //            ui->label_52->setFocus();
            isbaseinfochange = false;
        }
        else if (whichButtonClick == "advanced")
        {
            on_pushButton_62_clicked();
            isbaseinfochange = false;
        }
        //        else if (whichButtonClick == "base")
        //        {
        //            isbaseinfochange = false;
        //        }

    }
}

void Newconfiginfo::on_listWidget_currentRowChanged(int currentRow)
{
    //ui->label_22->setText(QString::number(currentRow));

    if(SaveWhat == "pdm")
    {
        nowcomboxnum =  currentRow;
        e3 = new QGraphicsOpacityEffect(this);
        e3->setOpacity(0.5);
        ui->label_100->setGraphicsEffect(e3);
        ui->label_100->show();
        ui->label_100->setGeometry(0,0,1366,768);
        SaveWhat = "pdmout";
        save = new Save(this);
        connect(save,SIGNAL(sendDeSingle(bool)),this,SLOT(receiveDesignle(bool)));
        save->show();
    }
    else
    {
        int tempdata = numpdm;
        for(int i = 0;i<tempdata;i++)
        {
            delete butt1[i];
            numpdm--;
        }
        ui->listWidget->setStyleSheet("QListWidget::item:seleted{ border-color: rgb(51, 153, 255);} QListWidget{font: 18pt ;color: rgb(248, 248, 255);background:transparent;}");
        SaveWhat = "";
        if(isJS)
        {
            if(currentRow == 0)
            {
                historyclear();
                ui->stackedWidget_2->setCurrentIndex(0);
                ui->label_83->show();
                ui->label_84->hide();
                ui->label_85->hide();
                //            ui->pushButton_13->show();
            }
            else if(currentRow == 3)
            {
                historyclear();
                ui->stackedWidget_2->setCurrentIndex(4);
                ui->label_83->hide();
                ui->label_84->show();
                ui->label_85->hide();
                //            ui->pushButton_13->hide();
                //            ui->label_52->setFocus();

            }else if(currentRow == 6)
            {
                ui->stackedWidget_2->setCurrentIndex(3);
                ui->label_83->hide();
                ui->label_84->hide();
                ui->label_85->show();
                //            ui->pushButton_13->hide();
                //            ui->label_52->setFocus();

            }

        }else
        {
            if(currentRow == 0)
            {
                historyclear();
                ui->stackedWidget_2->setCurrentIndex(2);
                ui->label_83->show();
                ui->label_84->hide();
                ui->label_85->hide();
                ui->label_86->hide();
                //            ui->pushButton_13->show();
                //            ui->label_52->setFocus();

            }
            else if(currentRow == 3)
            {
                historyclear();
                if (ui->stackedWidget_2->currentIndex()==2)
                {
                    whichButtonClick = "config";
                    if(!isbaseinfochange)
                        baseInfoIsChange();
                }
                else
                {
                    ui->stackedWidget_2->setCurrentIndex(4);
                    ui->label_83->hide();
                    ui->label_84->show();
                    ui->label_85->hide();
                    ui->label_86->hide();
                }
            }
            else if(currentRow == 6)
            {
                if (ui->stackedWidget_2->currentIndex()==2)
                {
                    whichButtonClick = "history";
                    if(!isbaseinfochange)
                        baseInfoIsChange();
                }
                else
                {
                    ui->stackedWidget_2->setCurrentIndex(3);
                    ui->label_83->hide();
                    ui->label_84->hide();
                    ui->label_85->show();
                    ui->label_86->hide();
                }
            }
        }
    }
}
// cancel
void Newconfiginfo::receivebaseinfocancel()
{

    delete e3;
    ui->label_100->hide();
    delete basicset;
}
void Newconfiginfo::receiveBaseinfo(QString a , QString b, QString c)
{
    if (SaveWhat == "cs351")
    {
        cs351Ip = a;
        PortA = b;
        PortB = c;
        ui->label_97->setText(cs351Ip);
    }
    else if(SaveWhat=="RFID")
    {
        RfidIp = a;
        RfidPort = b;
        ui->label_155->setText(RfidIp);

    }
    else if(SaveWhat=="server")
    {
        DataServerIp = a;
        CurveServerIp = b;
        AndonServerIp = c;
        ui->label_154->setText(DataServerIp);
    }
    else if(SaveWhat=="Gun")
    {
        BarcodeGun = a;
        ui->label_153->setText(BarcodeGun);
    }
    delete e3;
    ui->label_100->hide();
    delete basicset;

}
void Newconfiginfo::on_pushButton_59_clicked()
{
    //cs351设置 按钮
    e3 = new QGraphicsOpacityEffect(this);
    e3->setOpacity(0.5);
    ui->label_100->setGraphicsEffect(e3);
    ui->label_100->show();
    ui->label_100->setGeometry(0,0,1366,768);
    SaveWhat = "cs351";
    basicset = new BasicSet(this);
    basicset->setConfigValue351(cs351Ip,PortA,PortB);
    connect(basicset,SIGNAL(sendBaseinfo(QString,QString,QString)),this,SLOT(receiveBaseinfo(QString,QString,QString)));
    connect(basicset,SIGNAL(sendbaseinfocancel()),this,SLOT(receivebaseinfocancel()));
    basicset->show();

}

void Newconfiginfo::on_pushButton_60_clicked()
{
    //RFID 设置按钮

    e3 = new QGraphicsOpacityEffect(this);
    e3->setOpacity(0.5);
    ui->label_100->setGraphicsEffect(e3);
    ui->label_100->show();
    ui->label_100->setGeometry(0,0,1366,768);
    SaveWhat = "RFID";
    basicset = new BasicSet(this);
    basicset->setSerialOrRfidMode(RfidIp,RfidPort);
    connect(basicset,SIGNAL(sendBaseinfo(QString,QString,QString)),this,SLOT(receiveBaseinfo(QString,QString,QString)));
    connect(basicset,SIGNAL(sendbaseinfocancel()),this,SLOT(receivebaseinfocancel()));
    basicset->show();

}
void Newconfiginfo::on_pushButton_61_clicked()
{
    //server
    e3 = new QGraphicsOpacityEffect(this);
    e3->setOpacity(0.5);
    ui->label_100->setGraphicsEffect(e3);
    ui->label_100->show();
    ui->label_100->setGeometry(0,0,1366,768);
    SaveWhat = "server";
    basicset = new BasicSet(this);
    basicset->setServerValue(DataServerIp,CurveServerIp,AndonServerIp);
    connect(basicset,SIGNAL(sendBaseinfo(QString,QString,QString)),this,SLOT(receiveBaseinfo(QString,QString,QString)));
    connect(basicset,SIGNAL(sendbaseinfocancel()),this,SLOT(receivebaseinfocancel()));
    basicset->show();
}

void Newconfiginfo::on_pushButton_65_clicked()
{
    //条码枪

    //ui->stackedWidget_4->setCurrentIndex(1);
    e3 = new QGraphicsOpacityEffect(this);
    e3->setOpacity(0.5);
    ui->label_100->setGraphicsEffect(e3);
    ui->label_100->show();
    ui->label_100->setGeometry(0,0,1366,768);
    SaveWhat = "Gun";
    basicset = new BasicSet(this);
    basicset->setSerialOrRfidMode(BarcodeGun,"");
    basicset->setchecking();
    connect(basicset,SIGNAL(sendBaseinfo(QString,QString,QString)),this,SLOT(receiveBaseinfo(QString,QString,QString)));
    connect(basicset,SIGNAL(sendbaseinfocancel()),this,SLOT(receivebaseinfocancel()));
    basicset->show();
}


void Newconfiginfo::on_pushButton_63_clicked()
{
    //取消
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    ui->lineEdit_staname->setText(configIniRead->value("baseinfo/StationName").toString());
    ui->lineEdit_Operator->setText(configIniRead->value("baseinfo/Operator").toString());
    ui->lineEdit_localip->setText(configIniRead->value("baseinfo/LocalIp").toString());
    ui->lineEdit_wirelessip->setText(configIniRead->value("baseinfo/WirelessIp").toString());
    ui->lineEdit_staid->setText(configIniRead->value("baseinfo/StationId").toString());

    cs351Ip = configIniRead->value("baseinfo/cs351Ip").toString();
    PortA = configIniRead->value("baseinfo/PortA").toString();
    PortB = configIniRead->value("baseinfo/PortB").toString();
    RfidIp = configIniRead->value("baseinfo/RfidIp").toString();
    RfidPort = configIniRead->value("baseinfo/RfidPort").toString();
    DataServerIp = configIniRead->value("baseinfo/DataServerIp").toString();
    CurveServerIp= configIniRead->value("baseinfo/CurveServerIp").toString();
    AndonServerIp= configIniRead->value("baseinfo/AndonServerIp").toString();
    BarcodeGun = configIniRead->value("baseinfo/BarcodeGun").toString();

    ui->label_97->setText(cs351Ip);
    ui->label_155->setText(RfidIp);
    ui->label_154->setText(DataServerIp);
    ui->label_153->setText(BarcodeGun);

    ui->label_100->hide();

    delete configIniRead;
}

void Newconfiginfo::receiveBaseinfoSave(bool statetmp)
{
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    if(statetmp)
    {
        //true
        configIniRead->setValue("baseinfo/StationName",ui->lineEdit_staname->text());
        configIniRead->setValue("baseinfo/Operator",ui->lineEdit_Operator->text());
        Operator=ui->lineEdit_Operator->text();
        configIniRead->setValue("baseinfo/StationId",ui->lineEdit_staid->text());
        configIniRead->setValue("baseinfo/cs351Ip",cs351Ip);
        configIniRead->setValue("baseinfo/PortA",PortA);
        configIniRead->setValue("baseinfo/PortB",PortB);
        configIniRead->setValue("baseinfo/RfidIp",RfidIp);
        configIniRead->setValue("baseinfo/RfidPort",RfidPort);
        configIniRead->setValue("baseinfo/AndonServerIp",AndonServerIp);
        configIniRead->setValue("baseinfo/BarcodeGun",BarcodeGun);


        if(CurveServerIp != configIniRead->value("baseinfo/CurveServerIp").toString()||
                DataServerIp != configIniRead->value("baseinfo/DataServerIp").toString()  )
        {
            configIniRead->setValue("baseinfo/CurveServerIp",CurveServerIp);
            configIniRead->setValue("baseinfo/DataServerIp",DataServerIp);

            QString fileName = "/usr/local/arm/freetds/etc/freetds.conf";
            QFile file(fileName);
            if(!file.open(QIODevice::ReadOnly| QIODevice::Text)){
                qDebug()   << "Cannot open testdsn file for Reading";
            }
            QString str (file.readAll());
            if(str.contains("[testdsn1]", Qt::CaseInsensitive)&&str.contains("[testdsn2]", Qt::CaseInsensitive)){
                str.replace(QRegExp("\\[testdsn1\\]\\s*host = \\S*"),QString("[testdsn1]\n\thost = ")+CurveServerIp);
                str.replace(QRegExp("\\[testdsn2\\]\\s*host = \\S*"),QString("[testdsn2]\n\thost = ")+DataServerIp);
            }
            file.close();
            if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
                qDebug()   << "Cannot open testdsn file for Writing";
            }
            file.write(str.toUtf8());
            file.close();
        }

        configIniRead->setValue("baseinfo/DataServerIp",DataServerIp);

        if(ui->lineEdit_localip->text() != configIniRead->value("baseinfo/LocalIp").toString()||
                ui->lineEdit_wirelessip->text() != configIniRead->value("baseinfo/WirelessIp").toString())
        {
            QString fileName = "/etc/profile";
            QFile file(fileName);
            if(!file.open(QIODevice::ReadOnly| QIODevice::Text)){
                qDebug()   << "Cannot open profile file for Reading";
            }
            QString str (file.readAll());
            if(ui->lineEdit_localip->text() != configIniRead->value("baseinfo/LocalIp").toString())
            {
                configIniRead->setValue("baseinfo/LocalIp",ui->lineEdit_localip->text());
                system(QString("ifconfig eth0 ").append(ui->lineEdit_localip->text()+" &").toLatin1().data());
                if(str.contains("ifconfig eth0", Qt::CaseInsensitive)){
                    str.replace(QRegExp("ifconfig eth0 \\S*"),QString("ifconfig eth0 ")+ui->lineEdit_localip->text());
                }
            }
            if(ui->lineEdit_wirelessip->text() != configIniRead->value("baseinfo/WirelessIp").toString())
            {
                configIniRead->setValue("baseinfo/WirelessIp",ui->lineEdit_wirelessip->text());
                system((QString("ifconfig wlan0 ")+ui->lineEdit_wirelessip->text()+QString(" netmask ")+configIniRead->value("baseinfo/netmask").toString()+" &").toLatin1().data());
                if(str.contains("ifconfig wlan0", Qt::CaseInsensitive)){
                    str.replace(QRegExp("ifconfig wlan0 \\S*"),QString("ifconfig wlan0 ")+ui->lineEdit_wirelessip->text());
                }
            }

            file.close();
            if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
                qDebug()   << "Cannot open profile file for Writing";
            }
            file.write(str.toUtf8());
            file.close();
        }

        ui->label_100->hide();
        delete e3;
        delete save;

    }else
    {
        //false
        ui->lineEdit_staname->setText(configIniRead->value("baseinfo/StationName").toString());
        ui->lineEdit_Operator->setText(configIniRead->value("baseinfo/Operator").toString());
        ui->lineEdit_localip->setText(configIniRead->value("baseinfo/LocalIp").toString());
        ui->lineEdit_wirelessip->setText(configIniRead->value("baseinfo/WirelessIp").toString());
        ui->lineEdit_staid->setText(configIniRead->value("baseinfo/StationId").toString());

        cs351Ip = configIniRead->value("baseinfo/cs351Ip").toString();
        PortA = configIniRead->value("baseinfo/PortA").toString();
        PortB = configIniRead->value("baseinfo/PortB").toString();
        RfidIp = configIniRead->value("baseinfo/RfidIp").toString();
        RfidPort = configIniRead->value("baseinfo/RfidPort").toString();
        DataServerIp = configIniRead->value("baseinfo/DataServerIp").toString();
        CurveServerIp = configIniRead->value("baseinfo/CurveServerIp").toString();
        AndonServerIp = configIniRead->value("baseinfo/AndonServerIp").toString();
        BarcodeGun = configIniRead->value("baseinfo/BarcodeGun").toString();

        ui->label_97->setText(cs351Ip);
        ui->label_155->setText(RfidIp);
        ui->label_154->setText(DataServerIp);
        ui->label_153->setText(BarcodeGun);
        ui->label_100->hide();
        delete e3;
        delete save;
    }
    delete configIniRead;
    isbaseinfochange = true;

    if(whichButtonClick == "baseback")
    {
        on_pushButton_clicked();
        isbaseinfochange = false;
    }
    else if (whichButtonClick == "about")
    {
        on_pushButton_13_clicked();
        isbaseinfochange = false;
    }
    else if (whichButtonClick == "PDMEdit")
    {
        on_pushButton_28_clicked();
        isbaseinfochange = false;
    }
    else if (whichButtonClick == "config")
    {
        ui->stackedWidget_2->setCurrentIndex(4);
        ui->label_83->hide();
        ui->label_84->show();
        ui->label_85->hide();
        ui->label_86->hide();
        //        ui->pushButton_13->hide();
        ui->label_52->setFocus();
        isbaseinfochange = false;
    }
    else if (whichButtonClick == "history")
    {
        ui->stackedWidget_2->setCurrentIndex(3);
        ui->label_83->hide();
        ui->label_84->hide();
        ui->label_85->show();
        ui->label_86->hide();
        //        ui->pushButton_13->hide();
        ui->label_52->setFocus();
        isbaseinfochange = false;
    }
    else if (whichButtonClick == "advanced")
    {
        on_pushButton_62_clicked();
        isbaseinfochange = false;
    }
    else if (whichButtonClick == "save")
    {
        isbaseinfochange = false;
    }
    //    else if (whichButtonClick == "base")
    //    {
    //        isbaseinfochange = false;
    //    }

}

void Newconfiginfo::on_pushButton_64_clicked()
{
    //保存 参数按钮
    //保存
    whichButtonClick = "save";
    e3 = new QGraphicsOpacityEffect(this);
    e3->setOpacity(0.5);
    ui->label_100->setGraphicsEffect(e3);
    ui->label_100->show();
    ui->label_100->setGeometry(0,0,1366,768);

    SaveWhat = "baseinfo";
    save = new Save(this);
    connect(save,SIGNAL(sendSaveBaseinfo(bool)),this,SLOT(receiveBaseinfoSave(bool)));
    save->show();
}


void Newconfiginfo::moveDo()
{
    //左右 滑动 逻辑

    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    //qDebug() << "pagenum is is is " << pagenum;
    ui->label_carname->clear();
    ui->label_G9->clear();
    ui->label_VIN->clear();
    ui->label_isxuan->clear();
    ui->label_Lsnum->clear();
    ui->label_pronum->clear();
    ui->label_number->clear();
    previouswhichar = 0;
    int buttnumber = 0;
    buttnumber = (pagenum -1)*5+1;
    int t = 0;
    for(int j = buttnumber;j < (buttnumber+5);j++)
    {
        if(configIniRead->value(QString("carinfo").append(QString::number(j)).append("/IsOptionalStation")).toInt()) //有选配
        {
            buttonbox[t]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/67.bmp);font:14pt;color: rgb(248, 248, 255);");
            buttonbox[t]->setText(configIniRead->value(QString("carinfo").append(QString::number(j)).append("/carcx")).toString());
        }
        else //非选配
        {
            int i = 1;
            for(i = 1;i < 6;i++)
            {
                if(configIniRead->value(QString("carinfo").append(QString::number(j)).append("/LSNumber").append(QString::number(i))).toInt())
                {
                    buttonbox[t]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/67.bmp);font:14pt;color: rgb(248, 248, 255);");
                    buttonbox[t]->setText(configIniRead->value(QString("carinfo").append(QString::number(j)).append("/carcx")).toString());
                    break;
                }
            }
            if(i == 6)
            {
                buttonbox[t]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                buttonbox[t]->setText("");
            }
        }
        t++;
    }

    delete configIniRead;
}
void Newconfiginfo::on_pushButton_8_clicked()
{
    //left
    if(pagenum == 1)
        pagenum = 2;
    else
    {
        ui->pushButton_12->hide();
    }
    pagenum--;
    ui->label_pageum->setText(QString::number(pagenum));
    moveDo();

}

void Newconfiginfo::on_pushButton_9_clicked()
{
    //right
    if(pagenum == 4)
        pagenum = 3;
    else
    {
        ui->pushButton_12->hide();
    }
    pagenum++;
    ui->label_pageum->setText(QString::number(pagenum));
    moveDo();
}
void Newconfiginfo::buttclicked()
{
    //点击清空
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_2->addItem("");
    ui->pushButton_10->setEnabled(true);
    ui->pushButton_11->setEnabled(true);
    for(int i= 0;i < 250 ;i++)
    {

        if(configIniRead->value(QString("pdminfo").append(QString::number(i+1)).append("/pdmname")).toString()!= "")
        {
            ui->comboBox_2->addItem(configIniRead->value(QString("pdminfo").append(QString::number(i+1)).append("/pdmname")).toString());
        }
    }
    ui->comboBox_3->addItem("");
    for(int i= 0;i < 250 ;i++)
    {

        if(configIniRead->value(QString("pdminfo").append(QString::number(i+1)).append("/pdmname")).toString()!= "")
        {
            ui->comboBox_3->addItem(configIniRead->value(QString("pdminfo").append(QString::number(i+1)).append("/pdmname")).toString());
        }
    }
    ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
    ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
    ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
    ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
    ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
    ui->pushButton_xuan1->setEnabled(false);
    ui->pushButton_xuan2->setEnabled(false);
    ui->pushButton_xuan3->setEnabled(false);
    ui->pushButton_xuan4->setEnabled(false);
    ui->pushButton_xuan5->setEnabled(false);
    ui->pushButton_70->setText("");
    ui->pushButton_71->setText("");
    ui->pushButton_72->setText("");
    ui->pushButton_73->setText("");
    ui->pushButton_74->setText("");
    ui->pushButton_75->setText("");
    ui->pushButton_76->setText("");
    ui->pushButton_77->setText("");
    ui->pushButton_78->setText("");
    ui->pushButton_79->setText("");
    ui->lineEdit_carname->clear();
    ui->lineEdit_G9->clear();
    ui->lineEdit_VIN->clear();
    ui->lineEdit_Lsnumber->clear();
    ui->lineEdit_pronum->setText("0");
    ui->lineEdit_number->setText("0");
    ui->lineEdit_xuanLsnum->clear();
    ui->lineEdit_xuanpronum->setText("0");
    ui->lineEdit_xuannumber->setText("0");
    ui->pushButton_22->hide();
    ui->comboBox_2->hide();
    isedit = 0;
    carStyle = "";
    G9tmp = "";
    Vintmp = "";
    pdmxuan = "";
    for(int m = 0;m<20;m++)
    {
        luo[m] = "0";
        pro[m] = "0";
        lsnumers[m] = "0";
        taotong[m] = "0";

        luo2[m] = "0";
        pro2[m] = "0";
        lsnumers2[m] = "0";
        taotong2[m] = "0";
    }
    for(int i = 0;i<5;i++)
    {
        for(int k =0;k < 20;k++)
        {
            luoxuanlist[i][k]="0";
            proxuanlist[i][k]="0";
            lsnumersxuanlist[i][k]="0";
            bxuannamelist[i][k]="";
            bxuancodelist[i][k]="";
            kxuannamelist[i][k]="";
            kxuancodelist[i][k]="";
            luoxuanlist2[i][k]="0";
            proxuanlist2[i][k]="0";
            lsnumersxuanlist2[i][k]="0";
            bxuannamelist2[i][k]="";
            bxuancodelist2[i][k]="";
            kxuannamelist2[i][k]="";
            kxuancodelist2[i][k]="";

        }
        pdmxuanlist[i]="";
        pdmxuanlist2[i]="";
    }

    ui->label_currentindex->setText("1");
    ui->label_119->setText("1");
    ui->label_51->setText("1");
    // qDebug() <<  "priouswhich car is   sjkdjf ji " << previouswhichar;

    // currentpages = ui->label_51->text().toInt();
    int tmpisselect = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/IsOptionalStation")).toInt();
    int tmpisselect1 = configIniRead->value(QString("carinfo").append(QString::number(previouswhichar)).append("/IsOptionalStation")).toInt();
    if(previouswhichar <= 5*pagenum && previouswhichar >0) //点击别的 回复之前button效果
    {
        if(previouswhichar == 4)
        {
            //qDebug() << "aaa";
        }
        if(previouswhichar != whichcar)
        {
            int buttnumber = 0;
            buttnumber = (pagenum -1)*5+1;
            if(tmpisselect1) //有选配
            {
                buttonbox[previouswhichar-buttnumber]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/67.bmp);font:14pt;color: rgb(248, 248, 255);");
                buttonbox[previouswhichar-buttnumber]->setText(configIniRead->value(QString("carinfo").append(QString::number(previouswhichar)).append("/carcx")).toString());
            }
            else //非选配
            {
                int i = 1;
                for(i = 1;i < 6;i++)
                {
                    if(configIniRead->value(QString("carinfo").append(QString::number(previouswhichar)).append("/LSNumber").append(QString::number(i))).toInt())
                    {
                        buttonbox[previouswhichar-buttnumber]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/67.bmp);font:14pt;color: rgb(248, 248, 255);");
                        buttonbox[previouswhichar-buttnumber]->setText(configIniRead->value(QString("carinfo").append(QString::number(previouswhichar)).append("/carcx")).toString());
                        break;
                    }
                }
                if(i == 6)
                {
                    buttonbox[previouswhichar-buttnumber]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                    buttonbox[previouswhichar-buttnumber]->setText("");
                }
            }
        }

    }
    if(tmpisselect)  ////第一组有选配车型信息
    {
        //有选配
        carStyle = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/carcx")).toString();
        G9tmp = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/G9")).toString();
        Vintmp = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/VIN")).toString();
        ui->pushButton_15->hide();
        //for()
        ui->pushButton_70->show();
        ui->pushButton_71->show();
        ui->pushButton_72->show();
        ui->pushButton_73->show();
        ui->pushButton_74->show();
        ui->pushButton_75->show();
        ui->pushButton_76->show();
        ui->pushButton_77->show();
        ui->pushButton_78->show();
        ui->pushButton_79->show();

        isedit = 1;
        isoption = true;
        //ui->label_isxuan->setText("yes");
        ui->stackedWidget_5->setCurrentIndex(0);
        ui->label_carname->setText(carStyle);
        ui->label_G9->setText(G9tmp);
        ui->label_VIN->setText(Vintmp);
        ui->label_isxuan->setText("Yes");
        ui->label_Lsnum->hide();
        ui->label_pronum->hide();
        ui->label_number->hide();
        ui->label_108->hide();
        ui->label_109->hide();
        ui->label_110->hide();
        ui->pushButton_10->hide();
        ui->pushButton_11->hide();
        ui->label_currentindex->hide();
        ui->label_52->hide();
        ui->pushButton_12->show();
        ui->pushButton_xuan1->setEnabled(false);
        ui->pushButton_xuan2->setEnabled(false);
        ui->pushButton_xuan3->setEnabled(false);
        ui->pushButton_xuan4->setEnabled(false);
        ui->pushButton_xuan5->setEnabled(false);
        //chushihua
        for(int j= 1;j< 6;j++)
        {
            for(int i = 0;i<20;i++)
            {
                luoxuanlist[j-1][i]  = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/OPLuoSuanNum").append(QString::number(j)).append(QString::number(i+1))).toString();
                proxuanlist[j-1][i]  =  configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/OPProNum").append(QString::number(j)).append(QString::number(i+1))).toString();
                lsnumersxuanlist[j-1][i]=  configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/OPLSNumber").append(QString::number(j)).append(QString::number(i+1))).toString();

                bxuannamelist[j-1][i] = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/bxuanname").append(QString::number(j)).append(QString::number(i+1))).toString();
                bxuancodelist[j-1][i] = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/bxuancode").append(QString::number(j)).append(QString::number(i+1))).toString();

                kxuannamelist[j-1][i] = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/kxuanname").append(QString::number(j)).append(QString::number(i+1))).toString();
                kxuancodelist[j-1][i] = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/kxuancode").append(QString::number(j)).append(QString::number(i+1))).toString();
                luoxuanlist2[j-1][i] = luoxuanlist[j-1][i];
                proxuanlist2[j-1][i]  = proxuanlist[j-1][i] ;
                lsnumersxuanlist2[j-1][i] = lsnumersxuanlist[j-1][i];
                bxuannamelist2[j-1][i] = bxuannamelist[j-1][i];
                bxuancodelist2[j-1][i] = bxuancodelist[j-1][i];
                kxuannamelist2[j-1][i] =  kxuannamelist[j-1][i];
                kxuancodelist2[j-1][i] = kxuancodelist[j-1][i];

            }
            pdmxuanlist2[j-1]=  configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/pdmyinyong").append(QString::number(j))).toString();
            pdmxuanlist[j-1] = pdmxuanlist2[j-1];
            if(lsnumersxuanlist[j-1][0].toInt())
            {
                if(j == 1)
                {

                    ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                    ui->pushButton_70->setText(proxuanlist[j-1][0]);
                    ui->pushButton_71->setText(lsnumersxuanlist[j-1][0]);
                    if(isJS)
                    {
                        ui->pushButton_xuan1->setEnabled(true);
                    }


                }
                else if(j == 2)
                {
                    ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                    ui->pushButton_72->setText(proxuanlist[j-1][0]);
                    ui->pushButton_73->setText(lsnumersxuanlist[j-1][0]);
                    if(isJS)
                    {
                        ui->pushButton_xuan2->setEnabled(true);
                    }
                }
                else if(j == 3)
                {
                    ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                    ui->pushButton_74->setText(proxuanlist[j-1][0]);
                    ui->pushButton_75->setText(lsnumersxuanlist[j-1][0]);
                    if(isJS)
                    {
                        ui->pushButton_xuan3->setEnabled(true);
                    }
                } else if(j == 4)
                {
                    ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                    ui->pushButton_76->setText(proxuanlist[j-1][0]);
                    ui->pushButton_77->setText(lsnumersxuanlist[j-1][0]);
                    if(isJS)
                    {
                        ui->pushButton_xuan4->setEnabled(true);
                    }
                }
                else if(j == 5)
                {
                    ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                    ui->pushButton_78->setText(proxuanlist[j-1][0]);
                    ui->pushButton_79->setText(lsnumersxuanlist[j-1][0]);
                    if(isJS)
                    {
                        ui->pushButton_xuan5->setEnabled(true);
                    }
                }


            }else
            {
                if(j == 1)
                {
                    // ui->pushButton_xuan1->setEnabled(false);
                    ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");

                }
                else if(j == 2)
                {
                    //ui->pushButton_xuan2->setEnabled(false);
                    ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");

                }
                else if(j == 3)
                {
                    // ui->pushButton_xuan3->setEnabled(false);
                    ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");

                }
                else if(j == 4)
                {
                    // ui->pushButton_xuan4->setEnabled(false);
                    ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");

                }
                else if(j == 5)
                {
                    // ui->pushButton_xuan5->setEnabled(false);
                    ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");

                }

            }
        }
        if(isJS)
        {
            ui->pushButton_12->hide();
            ui->stackedWidget_13->setCurrentIndex(1);
        }

    }else // 非选配  再判断是否有值
    {
        int i = 1;
        for(i = 1;i < 6;i++)
        {
            if(configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/LSNumber").append(QString::number(i))).toInt())
            {
                //有车型  把编辑 使能

                if(isJS)
                    ui->pushButton_12->hide();
                else
                    ui->pushButton_12->show();
                //没选配
                //将5个螺栓编号 全部取出放到数组
                carStyle =  configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/carcx")).toString();
                G9tmp = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/G9")).toString();
                Vintmp = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/VIN")).toString();
                for(int i =0;i<20;i++)
                {
                    luo[i] =  configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/LuoSuanNum").append(QString::number(i+1))).toString();
                    pro[i] =  configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/ProNum").append(QString::number(i+1))).toString();
                    lsnumers[i] = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/LSNumber").append(QString::number(i+1))).toString();
                    taotong[i] =  configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/Taotong").append(QString::number(i+1))).toString();
                    luo2[i] = luo[i];
                    pro2[i] =  pro[i];
                    lsnumers2[i]  =  lsnumers[i] ;
                   // qDebug() << "taotong["+QString::number(i+1)+"]" << taotong[i];
                    if(taotong[i]!="1"&& taotong[i]!="2"&& taotong[i]!="3"&& taotong[i]!="4")
                    {
                        configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/Taotong").append(QString::number(i+1)),"0");
                        taotong[i] = "0"; 
                    }
                    taotong2[i]  = taotong[i];
                    // qDebug() << "taotong2["+QString::number(i+1)+"]" << taotong[i];
                }
                for(int k = 0;k< 250;k++)
                {
                    if(configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/pdmyinyong")).toString() == ui->comboBox_2->itemText(k))
                    {
                        ui->comboBox_2->setCurrentIndex(k);
                        break;
                    }

                }

                isedit = 0;
                isoption = false;
                ui->pushButton_22->show();
                ui->comboBox_2->show();
                ui->pushButton_15->hide();
                ui->stackedWidget_5->setCurrentIndex(0);
                ui->label_isxuan->setText("no");
                ui->pushButton_14->hide();
                ui->pushButton_16->hide();

                ui->label_115->show();
                ui->label_116->show();
                ui->label_117->show();
                ui->label_119->show();
                ui->label_60->show();
                ui->pushButton_6->show();
                ui->pushButton_7->show();
                ui->lineEdit_Lsnumber->show();
                ui->lineEdit_pronum->show();
                ui->lineEdit_number->show();
                ui->pushButton_pronum_add->show();
                ui->pushButton_pronum_minus->show();
                ui->pushButton_number_add->show();
                ui->pushButton_number_minus->show();
                ui->pushButton_52->show();
                ui->pushButton_5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/14.bmp);");
                optionIscheck = false;
                ui->label_Lsnum->show();
                ui->label_pronum->show();
                ui->label_number->show();
                ui->label_108->show();
                ui->label_109->show();
                ui->label_110->show();
                ui->pushButton_10->show();
                ui->pushButton_11->show();
                ui->label_currentindex->show();
                //ui->label_52->hide();

                ui->pushButton_70->hide();
                ui->pushButton_71->hide();
                ui->pushButton_72->hide();
                ui->pushButton_73->hide();
                ui->pushButton_74->hide();
                ui->pushButton_75->hide();
                ui->pushButton_76->hide();
                ui->pushButton_77->hide();
                ui->pushButton_78->hide();
                ui->pushButton_79->hide();

                ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/21.bmp);");
                ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/21.bmp);");
                ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/21.bmp);");
                ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/21.bmp);");
                ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/21.bmp);");
                ui->label_carname->setText(carStyle);
                ui->label_G9->setText(G9tmp);
                ui->label_VIN->setText(Vintmp);
                ui->label_Lsnum->setText(luo[0]);
                ui->label_pronum->setText(pro[0]);
                ui->label_number->setText(lsnumers[0]);
                break;
            }
        }
        if(i == 6)
        {
            if(!isJS)
            {
                isedit = 0;
                isoption = false;
                ui->stackedWidget_3->setCurrentIndex(1);
                ui->pushButton_8->setEnabled(false);
                ui->pushButton_9->setEnabled(false);
                ui->pushButton_12->hide();
                ui->pushButton_16->show();
                ui->pushButton_15->show();
                ui->pushButton_14->show();
                ui->label_115->show();
                ui->label_116->show();
                ui->label_117->show();
                ui->label_119->show();
                ui->label_60->show();
                ui->pushButton_6->show();
                ui->pushButton_7->show();
                ui->lineEdit_Lsnumber->show();
                ui->lineEdit_pronum->show();
                ui->lineEdit_number->show();

                ui->pushButton_pronum_add->show();
                ui->pushButton_pronum_minus->show();
                ui->pushButton_number_add->show();
                ui->pushButton_number_minus->show();
                ui->pushButton_52->show();
                ui->pushButton_22->show();
                ui->comboBox_2->show();
                ui->pushButton_5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/14.bmp);");
                ui->label_carname->clear();
                ui->label_G9->clear();
                ui->label_VIN->clear();
                ui->label_isxuan->clear();
                ui->label_Lsnum->clear();
                ui->label_pronum->clear();
                ui->label_number->clear();
                ui->lineEdit_taotong->setText(tr("无"));
                optionIscheck = false;
                if(whichcar == 1 || whichcar == 6 || whichcar == 11 || whichcar == 16)
                {
                    ui->pushButton_butt2->setEnabled(false);
                    ui->pushButton_butt3->setEnabled(false);
                    ui->pushButton_butt4->setEnabled(false);
                    ui->pushButton_butt5->setEnabled(false);
                    ui->pushButton_butt2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_butt3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_butt4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_butt5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                }else if(whichcar == 2 || whichcar == 7 || whichcar == 12 || whichcar == 17)
                {
                    ui->pushButton_butt1->setEnabled(false);
                    ui->pushButton_butt3->setEnabled(false);
                    ui->pushButton_butt4->setEnabled(false);
                    ui->pushButton_butt5->setEnabled(false);
                    ui->pushButton_butt1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_butt3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_butt4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_butt5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                }
                else if(whichcar == 3 || whichcar == 8 || whichcar == 13 || whichcar == 18)
                {
                    ui->pushButton_butt1->setEnabled(false);
                    ui->pushButton_butt2->setEnabled(false);
                    ui->pushButton_butt4->setEnabled(false);
                    ui->pushButton_butt5->setEnabled(false);
                    ui->pushButton_butt1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_butt2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_butt4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_butt5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                }else if(whichcar == 4 || whichcar == 9 || whichcar == 14 || whichcar == 19)
                {
                    ui->pushButton_butt1->setEnabled(false);
                    ui->pushButton_butt2->setEnabled(false);
                    ui->pushButton_butt3->setEnabled(false);
                    ui->pushButton_butt5->setEnabled(false);
                    ui->pushButton_butt1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_butt2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_butt3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_butt5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                }else if(whichcar == 5 || whichcar == 10 || whichcar == 15 || whichcar == 20)
                {
                    ui->pushButton_butt1->setEnabled(false);
                    ui->pushButton_butt2->setEnabled(false);
                    ui->pushButton_butt3->setEnabled(false);
                    ui->pushButton_butt4->setEnabled(false);
                    ui->pushButton_butt1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_butt2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_butt3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_butt4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                }
                ui->pushButton_5->setEnabled(true);  //选配按钮

                ui->stackedWidget_5->setCurrentIndex(1);
            }
            else
            {
                //技术操作员
                if(whichcar == 1 || whichcar == 6 || whichcar == 11 || whichcar == 16)
                {
                    ui->pushButton_butt1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                }else if(whichcar == 2 || whichcar == 7 || whichcar == 12 || whichcar == 17)
                {
                    ui->pushButton_butt2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                }
                else if(whichcar == 3 || whichcar == 8 || whichcar == 13 || whichcar == 18)
                {
                    ui->pushButton_butt3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");

                }else if(whichcar == 4 || whichcar == 9 || whichcar == 14 || whichcar == 19)
                {
                    ui->pushButton_butt4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");

                }else if(whichcar == 5 || whichcar == 10 || whichcar == 15 || whichcar == 20)
                {
                    ui->pushButton_butt5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                }
                ui->pushButton_12->hide();
            }

        }


    }
    previouswhichar = whichcar;
    delete configIniRead;
}
void Newconfiginfo::on_pushButton_butt1_clicked()
{
    //butt1 即第一个车型选中之后效果
    ui->pushButton_butt1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/9.bmp);font:14pt;color: rgb(248, 248, 255);");
    int toolpagenum = pagenum -1;
    for(int i = 0;i< 4;i++)
    {

        if(toolpagenum == i)
        {
            whichcar = toolpagenum*5 + 1;  //算出当前是哪辆车被选中
            break;
        }

    }
    buttclicked();


}
void Newconfiginfo::on_pushButton_butt2_clicked()
{
    //butt2 即第2个车型选中之后效果
    ui->pushButton_butt2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/9.bmp);font:14pt;color: rgb(248, 248, 255);");
    int toolpagenum = pagenum -1;
    for(int i = 0;i< 4;i++)
    {

        if(toolpagenum == i)
        {
            whichcar = toolpagenum*5 + 2;  //算出当前是哪辆车被选中
            break;
        }

    }
    buttclicked();
}

void Newconfiginfo::on_pushButton_butt3_clicked()
{
    //butt3 即第3个车型选中之后效果
    ui->pushButton_butt3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/9.bmp);font:14pt;color: rgb(248, 248, 255);");
    int toolpagenum = pagenum -1;
    for(int i = 0;i< 4;i++)
    {

        if(toolpagenum == i)
        {
            whichcar = toolpagenum*5 + 3;  //算出当前是哪辆车被选中
            break;
        }

    }
    buttclicked();
}

void Newconfiginfo::on_pushButton_butt4_clicked()
{
    //butt4 即第4个车型选中之后效果
    ui->pushButton_butt4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/9.bmp);font:14pt;color: rgb(248, 248, 255);");
    int toolpagenum = pagenum -1;
    for(int i = 0;i< 4;i++)
    {

        if(toolpagenum == i)
        {
            whichcar = toolpagenum*5 + 4;  //算出当前是哪辆车被选中
            break;
        }

    }
    buttclicked();

}

void Newconfiginfo::on_pushButton_butt5_clicked()
{
    //butt5 即第5个车型选中之后效果
    ui->pushButton_butt5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/9.bmp);font:14pt;color: rgb(248, 248, 255);");
    int toolpagenum = pagenum -1;
    for(int i = 0;i< 4;i++)
    {

        if(toolpagenum == i)
        {
            whichcar = toolpagenum*5 + 5;  //算出当前是哪辆车被选中
            break;
        }

    }
    buttclicked();
}
void Newconfiginfo::updownReadOperate(int judges)
{
    //up  currentindex值改变对应操作
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    //察看  up down
    if(!isedit)
    {
        if(judges)
        {
            currentpages = ui->label_currentindex->text().toInt() + 1;
            if(currentpages == 21)
            {
                currentpages = 1;
            }
        }
        else
        {
            currentpages = ui->label_currentindex->text().toInt() - 1;
            if(currentpages == 0)
            {
                currentpages = 20;
            }
        }
        ui->label_currentindex->setText(QString::number(currentpages));
        ui->label_carname->setText(configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/carcx")).toString());
        ui->label_G9->setText(configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/G9")).toString());
        ui->label_VIN->setText(configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/VIN")).toString());

        ui->label_Lsnum->setText(configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/LuoSuanNum").append(QString::number(currentpages))).toString());
        ui->label_pronum->setText(configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/ProNum").append(QString::number(currentpages))).toString());
        ui->label_number->setText(configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/LSNumber").append(QString::number(currentpages))).toString());


    }else
    {

        if(judges)
        {
            currentpages = ui->label_74->text().toInt() + 1;
            if(currentpages == 21)
            {
                currentpages = 1;
            }
        }
        else
        {
            currentpages = ui->label_74->text().toInt() - 1;
            if(currentpages == 0)
            {
                currentpages = 20;
            }
        }
        ui->label_74->setText(QString::number(currentpages));
        ui->label_75->setText(luoxuanlist2[whichoption-1][currentpages-1]);
        ui->label_76->setText(proxuanlist2[whichoption-1][currentpages-1]);
        ui->label_77->setText(lsnumersxuanlist2[whichoption-1][currentpages-1]);
        ui->label_78->setText(pdmxuanlist2[whichoption-1]);
        //QString pathpdm = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/pdmyinyong").append(QString::number(whichoption)).append(QString::number(currentpages))).toString();
        // qDebug() << "";
        //ui->label_78->setText(configIniRead->value(pathpdm.append("/pdmname")).toString());
    }


    delete   configIniRead;
}
void Newconfiginfo::on_pushButton_10_clicked()
{
    // read up
    updownReadOperate(1);

}

void Newconfiginfo::on_pushButton_11_clicked()
{
    //read down
    updownReadOperate(0);
}

void Newconfiginfo::on_pushButton_5_clicked()
{
    //选配按钮
    if(optionIscheck)
    {
        isedit = 0;
        ui->label_115->show();
        ui->label_116->show();
        ui->label_117->show();
        ui->label_119->show();
        ui->label_60->show();
        ui->pushButton_6->show();
        ui->pushButton_7->show();
        ui->lineEdit_Lsnumber->show();
        ui->lineEdit_pronum->show();
        ui->lineEdit_number->show();
        ui->label_taotongg->show();
        ui->pushButton_taotong_add->show();
        ui->pushButton_taotong_minus->show();
        ui->lineEdit_taotong->show();

        ui->pushButton_pronum_add->show();
        ui->pushButton_pronum_minus->show();
        ui->pushButton_number_add->show();
        ui->pushButton_number_minus->show();
        ui->pushButton_52->show();
        ui->pushButton_22->show();
        ui->comboBox_2->show();
        ui->pushButton_xuan1->setEnabled(false);
        ui->pushButton_xuan2->setEnabled(false);
        ui->pushButton_xuan3->setEnabled(false);
        ui->pushButton_xuan4->setEnabled(false);
        ui->pushButton_xuan5->setEnabled(false);
        ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/14.bmp);");
        ui->pushButton_70->hide();
        ui->pushButton_71->hide();
        ui->pushButton_72->hide();
        ui->pushButton_73->hide();
        ui->pushButton_74->hide();
        ui->pushButton_75->hide();
        ui->pushButton_76->hide();
        ui->pushButton_77->hide();
        ui->pushButton_78->hide();
        ui->pushButton_79->hide();

        optionIscheck = false;

    }
    else
    {
        isedit = 1;
        isxuanlook = 0;
        ui->label_115->hide();
        ui->label_116->hide();
        ui->label_117->hide();
        ui->label_119->hide();
        ui->label_60->hide();
        ui->pushButton_6->hide();
        ui->pushButton_7->hide();
        ui->lineEdit_Lsnumber->hide();
        ui->lineEdit_pronum->hide();
        ui->lineEdit_number->hide();
        ui->label_taotongg->hide();
        ui->pushButton_taotong_add->hide();
        ui->pushButton_taotong_minus->hide();
        ui->lineEdit_taotong->hide();
        ui->pushButton_pronum_add->hide();
        ui->pushButton_pronum_minus->hide();
        ui->pushButton_number_add->hide();
        ui->pushButton_number_minus->hide();
        ui->pushButton_52->hide();
        ui->pushButton_xuan1->setEnabled(true);
        ui->pushButton_xuan2->setEnabled(true);
        ui->pushButton_xuan3->setEnabled(true);
        ui->pushButton_xuan4->setEnabled(true);
        ui->pushButton_xuan5->setEnabled(true);
        ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
        ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
        ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
        ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
        ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
        ui->pushButton_5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/13.bmp);");
        ui->pushButton_70->show();
        ui->pushButton_71->show();
        ui->pushButton_72->show();
        ui->pushButton_73->show();
        ui->pushButton_74->show();
        ui->pushButton_75->show();
        ui->pushButton_76->show();
        ui->pushButton_77->show();
        ui->pushButton_78->show();
        ui->pushButton_79->show();
        ui->pushButton_22->hide();
        ui->comboBox_2->hide();
        QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
        for(int i = 0;i<5;i++)
        {

            if(lsnumersxuanlist[i][0].toInt())
            {
                if(i == 0)
                {
                    //qDebug() << "fsd";
                    ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                }
                else if(i == 1)
                    ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                else if(i == 2)
                    ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                else if(i == 3)
                    ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                else if(i == 4)
                    ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");

            }else
            {
                if(i == 0)
                    ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                else if(i == 1)
                    ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                else if(i == 2)
                    ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                else if(i == 3)
                    ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                else if(i == 4)
                    ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");

            }
        }
        delete  configIniRead;
        optionIscheck = true;
    }
}
void Newconfiginfo::updownWriteOperate(int judges)
{
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    //编辑  up   down
    if(isedit == 1)
    {
        if(judges)
        {
            currentpages = ui->label_51->text().toInt() + 1;
            if(currentpages == 21)
            {
                currentpages = 1;
            }
        }
        else
        {
            currentpages = ui->label_51->text().toInt() - 1;
            if(currentpages == 0)
            {
                currentpages = 20;
            }
        }

        ui->label_51->setText(QString::number(currentpages));
        ui->lineEdit_xuanLsnum->setText(luoxuanlist2[whichoption-1][currentpages-1]);
        if (proxuanlist2[whichoption-1][currentpages-1] == "")
            ui->lineEdit_xuanpronum->setText("0");
        else ui->lineEdit_xuanpronum->setText(proxuanlist2[whichoption-1][currentpages-1]);
        if (lsnumersxuanlist2[whichoption-1][currentpages-1] == "")
            ui->lineEdit_xuannumber->setText("0");
        else ui->lineEdit_xuannumber->setText(lsnumersxuanlist2[whichoption-1][currentpages-1]);
    }
    else
    {
        if(judges)
        {
            currentpages = ui->label_119->text().toInt() + 1;
            if(currentpages == 21)
            {
                currentpages = 1;
            }
        }
        else
        {
            currentpages = ui->label_119->text().toInt() - 1;
            if(currentpages == 0)
            {
                currentpages = 20;
            }
        }
        ui->label_119->setText(QString::number(currentpages));
        ui->lineEdit_carname->setText(carStyle);
        ui->lineEdit_G9->setText(G9tmp);
        ui->lineEdit_VIN->setText(Vintmp);

        ui->lineEdit_Lsnumber->setText(luo2[currentpages-1]);
        if (pro2[currentpages-1] == "")
            ui->lineEdit_pronum->setText("0");
        else
            ui->lineEdit_pronum->setText(pro2[currentpages-1]);
        if (lsnumers2[currentpages-1] == "")
            ui->lineEdit_number->setText("0");
        else
            ui->lineEdit_number->setText(lsnumers2[currentpages-1]);
        if(taotong2[currentpages-1] == "0" || taotong2[currentpages-1] == "")
            ui->lineEdit_taotong->setText(tr("无"));
        else
            ui->lineEdit_taotong->setText(taotong2[currentpages-1]);


    }
    delete configIniRead;
}
void Newconfiginfo::on_pushButton_6_clicked()
{
    // edit up
    // ui->label_119->text().toInt();
    //上下滑动 取值
    carStyle = ui->lineEdit_carname->text();
    G9tmp = ui->lineEdit_G9->text();
    Vintmp = ui->lineEdit_VIN->text();
    luo2[ui->label_119->text().toInt()-1] = ui->lineEdit_Lsnumber->text();
    pro2[ui->label_119->text().toInt()-1] = ui->lineEdit_pronum->text();
    lsnumers2[ui->label_119->text().toInt()-1] = ui->lineEdit_number->text();
    taotong2[ui->label_119->text().toInt()-1] = ui->lineEdit_taotong->text();
    updownWriteOperate(1);

}

void Newconfiginfo::on_pushButton_7_clicked()
{
    //edit down
    carStyle = ui->lineEdit_carname->text();
    G9tmp = ui->lineEdit_G9->text();
    Vintmp = ui->lineEdit_VIN->text();
    luo2[ui->label_119->text().toInt()-1] = ui->lineEdit_Lsnumber->text();
    pro2[ui->label_119->text().toInt()-1] = ui->lineEdit_pronum->text();
    lsnumers2[ui->label_119->text().toInt()-1] = ui->lineEdit_number->text();
    taotong2[ui->label_119->text().toInt()-1] = ui->lineEdit_taotong->text();
    updownWriteOperate(0);
}

void Newconfiginfo::on_pushButton_14_clicked()
{
    //edit 保存
    ui->stackedWidget_3->setCurrentIndex(0);
    ui->pushButton_5->setEnabled(false);


    if(SaveWhat == "" || SaveWhat == "de" || SaveWhat == "deall")
    {

        e3 = new QGraphicsOpacityEffect(this);
        e3->setOpacity(0.5);
        ui->label_100->setGraphicsEffect(e3);
        ui->label_100->show();
        ui->label_100->setGeometry(0,0,1366,768);
        save = new Save(this);
        connect(save,SIGNAL(sendDeSingle(bool)),this,SLOT(receiveDesignle(bool)));
        save->show();

    }
    else if((SaveWhat == "desingle") || (SaveWhat == "saveself"))
    {
        if(isedit == 0)
        {
            //非选配信息  保存
            carStyle = ui->lineEdit_carname->text();
            G9tmp = ui->lineEdit_G9->text();
            Vintmp = ui->lineEdit_VIN->text();
            luo2[ui->label_119->text().toInt()-1] = ui->lineEdit_Lsnumber->text();
            pro2[ui->label_119->text().toInt()-1] = ui->lineEdit_pronum->text();
            lsnumers2[ui->label_119->text().toInt()-1] = ui->lineEdit_number->text();
            if(ui->lineEdit_taotong->text()!="1"&& ui->lineEdit_taotong->text()!="2"&&ui->lineEdit_taotong->text()!="3"&&ui->lineEdit_taotong->text()!="4")
                taotong2[ui->label_119->text().toInt()-1] = "0";
            else
                taotong2[ui->label_119->text().toInt()-1] = ui->lineEdit_taotong->text();

            //qDebug() << ui->label_119->text().toInt()-1;
            QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
            configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/carcx"),ui->lineEdit_carname->text());
            configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/G9"),ui->lineEdit_G9->text());
            configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/VIN"),ui->lineEdit_VIN->text());
            configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/pdmyinyong"),ui->comboBox_2->currentText());
            for(int i = 0;i< 20;i++)
            {
                luo[i] = luo2[i];
                pro[i] =  pro2[i];
                lsnumers[i]  =  lsnumers2[i] ;
                taotong[i] = taotong2[i];
                if(taotong[i]!="1"&& taotong[i]!="2"&& taotong[i]!="3"&& taotong[i]!="4")
                {
                    //configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/Taotong").append(QString::number(i+1)),"0");
                    taotong[i] = "0";
                }
            }
            int counts = 0;
            for(int i = 0;i<20;i++)
            {
                if(lsnumers2[i].toInt())
                {
                    counts++;
                }
                configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/LuoSuanNum").append(QString::number(i+1)),luo[i]);
                configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/ProNum").append(QString::number(i+1)),pro[i]);
                configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/LSNumber").append(QString::number(i+1)),lsnumers[i]);
                configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/Taotong").append(QString::number(i+1)),taotong[i]);
            }

            for(int j = 1;j < 6;j++)
            {
                for(int i = 0;i< 20;i++)
                {


                    configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/OPLuoSuanNum").append(QString::number(j)).append(QString::number(i+1)),"0");
                    configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/OPProNum").append(QString::number(j)).append(QString::number(i+1)),"0");
                    configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/OPLSNumber").append(QString::number(j)).append(QString::number(i+1)),"0");

                    configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/bxuanname").append(QString::number(j)).append(QString::number(i+1)),"");
                    configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/bxuancode").append(QString::number(j)).append(QString::number(i+1)),"");
                    configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/kxuanname").append(QString::number(j)).append(QString::number(i+1)),"");
                    configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/kxuancode").append(QString::number(j)).append(QString::number(i+1)),"");

                }
                configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/pdmyinyong").append(QString::number(j)),"");


            }

            configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/IsOptionalStation"),"0");

            //存储几条 有效配置信息
            if(!counts)
            {
                if(whichcar == 1 || whichcar == 6 || whichcar == 11 || whichcar == 16)
                {
                    ui->pushButton_butt1->setText("");
                }else if(whichcar == 2 || whichcar == 7 || whichcar == 12 || whichcar == 17)
                {
                    ui->pushButton_butt2->setText("");
                }
                else if(whichcar == 3 || whichcar == 8 || whichcar == 13 || whichcar == 18)
                {
                    ui->pushButton_butt3->setText("");
                }else if(whichcar == 4 || whichcar == 9 || whichcar == 14 || whichcar == 19)
                {
                    ui->pushButton_butt4->setText("");
                }else if(whichcar == 5 || whichcar == 10 || whichcar == 15 || whichcar == 20)
                {
                    ui->pushButton_butt5->setText("");
                }
            }
            //            configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/IsSelect"),QString::number(counts));

            // QMessageBox::warning(this,tr("结果"),tr("保存成功!"),QMessageBox::Yes);
            ui->stackedWidget_5->setCurrentIndex(0);
            ui->label_carname->setText(carStyle);
            ui->label_VIN->setText(Vintmp);
            ui->label_G9->setText(G9tmp);
            ui->label_Lsnum->setText(luo[0]);
            ui->label_pronum->setText(pro[0]);
            ui->label_number->setText(lsnumers[0]);
            ui->label_isxuan->setText("no");

            ui->label_Lsnum->show();
            ui->label_Lsnum->show();
            ui->label_pronum->show();
            ui->label_number->show();
            ui->label_108->show();
            ui->label_109->show();
            ui->label_110->show();
            ui->label_52->show();
            ui->pushButton_10->show();
            ui->pushButton_11->show();
            ui->label_currentindex->show();
            delete configIniRead;
            backShow();
            isoption = false;

        }else if(isedit == 1)
        {
            //  qDebug() << "llllllllllllllllllsdf";
            QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
            configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/carcx"),ui->lineEdit_carname->text());
            configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/G9"),ui->lineEdit_G9->text());
            configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/VIN"),ui->lineEdit_VIN->text());
            carStyle = ui->lineEdit_carname->text();
            G9tmp = ui->lineEdit_G9->text();
            Vintmp = ui->lineEdit_VIN->text();
            ui->label_carname->setText(carStyle);
            ui->label_VIN->setText(Vintmp);
            ui->label_G9->setText(G9tmp);
            int counts = 0;
            int countnum = 0;
            for(int j = 1;j < 6;j++)
            {
                for(int i = 0;i< 20;i++)
                {
                    if(lsnumersxuanlist2[j-1][i].toInt())
                    {
                        counts++;
                    }

                    configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/OPLuoSuanNum").append(QString::number(j)).append(QString::number(i+1)),luoxuanlist2[j-1][i]);
                    configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/OPProNum").append(QString::number(j)).append(QString::number(i+1)),proxuanlist2[j-1][i]);
                    configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/OPLSNumber").append(QString::number(j)).append(QString::number(i+1)),lsnumersxuanlist2[j-1][i]);

                    configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/bxuanname").append(QString::number(j)).append(QString::number(i+1)),bxuannamelist2[j-1][i]);
                    configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/bxuancode").append(QString::number(j)).append(QString::number(i+1)),bxuancodelist2[j-1][i]);
                    configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/kxuanname").append(QString::number(j)).append(QString::number(i+1)),kxuannamelist2[j-1][i]);
                    configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/kxuancode").append(QString::number(j)).append(QString::number(i+1)),kxuancodelist2[j-1][i]);

                }
                configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/pdmyinyong").append(QString::number(j)),pdmxuanlist[j-1]);
                //                configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/OptionInfoNum").append(QString::number(j)),QString::number(counts));
                if(!counts)
                    countnum++;
            }
            if(countnum == 20)
            {
                // int whichbuttons = 0;

                // if()
                if(pagenum == 1)
                {
                    buttonbox[whichcar-1]->setText("");
                }
                else if(pagenum == 2)
                {
                    buttonbox[whichcar-6]->setText("");
                }
                else if(pagenum == 3)
                {
                    buttonbox[whichcar-11]->setText("");
                }
                else if(pagenum == 4)
                {
                    buttonbox[whichcar-16]->setText("");
                }
                ui->label_carname->clear();
                ui->label_G9->clear();
                ui->label_VIN->clear();
                ui->label_isxuan->clear();
                configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/IsOptionalStation"),"0");
            }else
            {
                configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/IsOptionalStation"),"1");
            }


            //            configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/IsSelect"),"0");



            for(int i = 0;i<20;i++)
            {

                configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/LuoSuanNum").append(QString::number(i+1)),"0");
                configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/ProNum").append(QString::number(i+1)),"0");
                configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/LSNumber").append(QString::number(i+1)),"0");
                configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/Taotong").append(QString::number(i+1)),"0");
            }
            configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/pdmyinyong"),"");
            SaveWhat = "";
            delete configIniRead;
            ui->pushButton_xuan1->setEnabled(false);
            ui->pushButton_xuan2->setEnabled(false);
            ui->pushButton_xuan3->setEnabled(false);
            ui->pushButton_xuan4->setEnabled(false);
            ui->pushButton_xuan5->setEnabled(false);
            backShow();
            isoptionsaved = true;
            ui->label_Lsnum->hide();
            ui->label_Lsnum->hide();
            ui->label_pronum->hide();
            ui->label_number->hide();
            ui->label_isxuan->setText("yes");
            ui->label_108->hide();
            ui->label_109->hide();
            ui->label_110->hide();
            ui->label_52->hide();
            ui->pushButton_10->hide();
            ui->pushButton_11->hide();
            ui->label_currentindex->hide();
            isoption = true; //选配与否

        }

        SaveWhat = "";

    }



}

void Newconfiginfo::receiveDesignle(bool tmp)
{
    if(SaveWhat == "de")
    {
        if(tmp)
        {
            SaveWhat = "desingle";
            on_pushButton_14_clicked();
        }
        else
        {
            SaveWhat = "";
            QSettings *configIniRead = new QSettings("/config.ini",QSettings::IniFormat);
            carStyle =  configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/carcx")).toString();
            G9tmp = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/G9")).toString();
            Vintmp = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/VIN")).toString();
            for(int i =0;i<20;i++)
            {
                luo[i] =  configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/LuoSuanNum").append(QString::number(i+1))).toString();
                pro[i] =  configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/ProNum").append(QString::number(i+1))).toString();
                lsnumers[i] = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/LSNumber").append(QString::number(i+1))).toString();
            }
            delete configIniRead;
            // on_pushButton_15_clicked();
        }

    }else  if(SaveWhat == "")
    {

        if(tmp)
        {
            SaveWhat = "saveself";
            on_pushButton_14_clicked();

        }
        else
        {
            SaveWhat = "";
            if(!isedit)
            {
                QSettings *configIniRead = new QSettings("/config.ini",QSettings::IniFormat);
                carStyle =  configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/carcx")).toString();
                G9tmp = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/G9")).toString();
                Vintmp = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/VIN")).toString();
                for(int i =0;i<20;i++)
                {
                    luo[i] =  configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/LuoSuanNum").append(QString::number(i+1))).toString();
                    pro[i] =  configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/ProNum").append(QString::number(i+1))).toString();
                    lsnumers[i] = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/LSNumber").append(QString::number(i+1))).toString();
                }
                delete configIniRead;
                backShow();
            }else
            {
              if(whichoption >=1 && whichoption < 6)
              {
                for(int j = 0;j< 20;j++)
                {
                    //luoxuanlist2[whichoption-1][j] = luoxuanlist[whichoption-1][j];
                    //proxuanlist2[whichoption-1][j] = proxuanlist[whichoption-1][j];

                    lsnumersxuanlist2[whichoption-1][j] = lsnumersxuanlist[whichoption-1][j];
                    bxuannamelist2[whichoption-1][j] = bxuannamelist[whichoption-1][j];
                    bxuancodelist2[whichoption-1][j] = bxuancodelist[whichoption-1][j];
                    kxuannamelist2[whichoption-1][j] = kxuannamelist[whichoption-1][j];
                    kxuancodelist2[whichoption-1][j] = kxuancodelist[whichoption-1][j];

                }
                backShow();
                pdmxuanlist2[whichoption-1]=  pdmxuanlist[whichoption-1];
                ui->pushButton_xuan1->setEnabled(false);
                ui->pushButton_xuan2->setEnabled(false);
                ui->pushButton_xuan3->setEnabled(false);
                ui->pushButton_xuan4->setEnabled(false);

                ui->pushButton_xuan5->setEnabled(false);
                isoptionsaved = true;

            }
           }

        }
        ui->stackedWidget_5->setCurrentIndex(0);
        SaveWhat ="";
        if(buttonbox[whichcar-(pagenum-1)*5-1]->text() == "")
        {
            ui->pushButton_12->hide();
        }
    }else if(SaveWhat == "save2")
    {
        if(tmp)
        {
            SaveWhat = "saveself";
            on_pushButton_25_clicked();
        }
        else
        {
            SaveWhat = "";

        }
        SaveWhat ="";
    }
    else if(SaveWhat == "deall")
    {
        if(tmp)
        {
            ui->lineEdit_carname->clear();
            ui->lineEdit_G9->clear();
            ui->lineEdit_VIN->clear();
            ui->lineEdit_Lsnumber->setText("0");
            ui->lineEdit_pronum->setText("0");
            ui->lineEdit_number->setText("0");
            ui->lineEdit_taotong->setText(tr("无"));
            ui->label_carname->clear();
            ui->label_G9->clear();
            ui->label_VIN->clear();
            ui->label_Lsnum->clear();
            ui->label_number->clear();
            ui->label_isxuan->clear();

            ui->comboBox_2->setCurrentIndex(0);
            SaveWhat = "saveself";
            carStyle="";
            G9tmp="";
            Vintmp="";
            for(int k = 1;k < 6;k++) //5个程序号 删除
            {
                luo2[k-1]="0";
                pro2[k-1]="0";
                lsnumers2[k-1]="0";
                taotong2[k-1] = "0";
            }
            on_pushButton_14_clicked();

        }
        SaveWhat ="";
        if(buttonbox[whichcar-(pagenum-1)*5-1]->text() == "")
        {
            ui->pushButton_12->hide();
        }

    }
    else if(SaveWhat == "OPCANCEL")
    {
        if(tmp)
        {
            SaveWhat = "saveself";
            on_pushButton_25_clicked();

        }else
        {
            for(int j = 0;j< 20;j++)
            {

                luoxuanlist2[whichoption-1][j] = luoxuanlist[whichoption-1][j];
                proxuanlist2[whichoption-1][j] = proxuanlist[whichoption-1][j];
                lsnumersxuanlist2[whichoption-1][j] = lsnumersxuanlist[whichoption-1][j];
                bxuannamelist2[whichoption-1][j] = bxuannamelist[whichoption-1][j];
                bxuancodelist2[whichoption-1][j] = bxuancodelist[whichoption-1][j];
                kxuannamelist2[whichoption-1][j] = kxuannamelist[whichoption-1][j];
                kxuancodelist2[whichoption-1][j] = kxuancodelist[whichoption-1][j];

            }

            pdmxuanlist2[whichoption-1]=  pdmxuanlist[whichoption-1];
            // ui->pushButton_12->hide();
            ui->stackedWidget->setCurrentIndex(1);
            ui->stackedWidget_2->setCurrentIndex(4);
            //ui->pushButton_15->show();
            SaveWhat = "";
        }
        SaveWhat = "";
    }
    else if(SaveWhat == "CANCEL")
    {
        ui->label_isxuan->setText("no");
        ui->label_carname->setText(ui->lineEdit_carname->text());
        ui->label_G9->setText(ui->lineEdit_G9->text());
        ui->label_VIN->setText(ui->lineEdit_VIN->text());
        if(tmp)
        {
            for(int i = 0;i< 20;i++)
            {
                luo[i] = luo2[i];
                pro[i] =  pro2[i];
                lsnumers[i]  =  lsnumers2[i] ;
            }
            SaveWhat = "saveself";
            ui->label_Lsnum->setText(luo2[0]);
            ui->label_pronum->setText(pro2[0]);
            ui->label_number->setText(lsnumers2[0]);
            on_pushButton_14_clicked();

        }else
        {
            for(int i = 0;i< 20;i++)
            {
                luo2[i] = luo[i];
                pro2[i] =  pro[i];
                lsnumers2[i]  =  lsnumers[i] ;
            }
            ui->stackedWidget_5->setCurrentIndex(0);

            ui->label_Lsnum->setText(luo[0]);
            ui->label_pronum->setText(pro[0]);
            ui->label_number->setText(lsnumers[0]);

            backShow();
            SaveWhat = "";
        }
    }
    else if(SaveWhat == "OPDELONE")
    {
        if(tmp)
        {// 删除一个选配
            isoptionsaved = false;
            for(int i = 0;i<20;i++)
            {

                luoxuanlist2[whichoption-1][i]  = "0";
                proxuanlist2[whichoption-1][i]  =  "0";
                lsnumersxuanlist2[whichoption-1][i]=  "0";
                bxuannamelist2[whichoption-1][i] = "";
                bxuancodelist2[whichoption-1][i] = "";

                kxuannamelist2[whichoption-1][i] = "";
                kxuancodelist2[whichoption-1][i] = "";

            }
            pdmxuanlist2[whichoption-1]=  "";
            ui->stackedWidget->setCurrentIndex(1);
            if(whichoption == 1)
            {
                ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                ui->pushButton_70->setText("");
                ui->pushButton_71->setText("");
            }else if(whichoption == 2)
            {
                ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                ui->pushButton_72->setText("");
                ui->pushButton_73->setText("");
            }else if(whichoption == 3)
            {
                ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                ui->pushButton_74->setText("");
                ui->pushButton_75->setText("");
            }else if(whichoption == 4)
            {
                ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                ui->pushButton_76->setText("");
                ui->pushButton_77->setText("");
            }else if(whichoption == 5)
            {
                ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                ui->pushButton_78->setText("");
                ui->pushButton_79->setText("");
            }

        }
        SaveWhat ="";
    }else if(SaveWhat == "OPDELALL")
    {
        if(tmp)
        {
            SaveWhat = "saveself";
            ui->lineEdit_carname->clear();
            ui->lineEdit_G9->clear();
            ui->lineEdit_VIN->clear();
            ui->label_carname->clear();
            ui->label_G9->clear();
            ui->label_VIN->clear();
            ui->label_isxuan->clear();

            for(int j= 1;j< 6;j++)
            {
                for(int i = 0;i<20;i++)
                {

                    luoxuanlist[j-1][i]  = "0";
                    proxuanlist[j-1][i]  =  "0";
                    lsnumersxuanlist[j-1][i]=  "0";

                    bxuannamelist[j-1][i] = "";
                    bxuancodelist[j-1][i] = "";

                    kxuannamelist[j-1][i] = "";
                    kxuancodelist[j-1][i] = "";
                    luoxuanlist2[j-1][i]  = "0";
                    proxuanlist2[j-1][i]  =  "0";
                    lsnumersxuanlist2[j-1][i]=  "0";

                    bxuannamelist2[j-1][i] = "";
                    bxuancodelist2[j-1][i] = "";

                    kxuannamelist2[j-1][i] = "";
                    kxuancodelist2[j-1][i] = "";

                }
                pdmxuanlist[j-1]=  "";
                pdmxuanlist2[j-1]=  "";
            }
            on_pushButton_14_clicked();
            QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
            configIniRead->setValue(QString("carinfo").append(QString::number(whichcar)).append("/IsOptionalStation"),"0");
            delete configIniRead;
            ui->stackedWidget_5->setCurrentIndex(0);
            if(whichcar == 1 || whichcar== 6 || whichcar == 11 || whichcar == 16)
            {
                ui->pushButton_butt1->setText("");
                ui->pushButton_butt1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
            }
            else if(whichcar == 2 || whichcar== 7 || whichcar== 12 || whichcar == 17)
            {
                ui->pushButton_butt2->setText("");
                ui->pushButton_butt2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
            }

            else if(whichcar == 3 || whichcar== 8 || whichcar== 13 || whichcar == 18)
            {
                ui->pushButton_butt3->setText("");
                ui->pushButton_butt3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
            }
            else if(whichcar == 4 || whichcar== 9 || whichcar== 14 || whichcar == 19)
            {
                ui->pushButton_butt4->setText("");
                ui->pushButton_butt4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
            }
            else if(whichcar == 5 || whichcar== 10 || whichcar== 15 || whichcar == 20)
            {
                ui->pushButton_butt5->setText("");
                ui->pushButton_butt5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
            }

            ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
            ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
            ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
            ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
            ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
            ui->pushButton_70->setText("");
            ui->pushButton_71->setText("");
            ui->pushButton_72->setText("");
            ui->pushButton_73->setText("");
            ui->pushButton_74->setText("");
            ui->pushButton_75->setText("");
            ui->pushButton_76->setText("");
            ui->pushButton_77->setText("");
            ui->pushButton_78->setText("");
            ui->pushButton_79->setText("");
        }
        SaveWhat ="";
    }
    else if(SaveWhat == "wifiunconnect")
    {


    }
    else if(SaveWhat == "pdmwaring")
    {
        SaveWhat = "pdm";
    }
    else if(SaveWhat == "pdmout")
    {
        if(tmp)
        {
            savePDM();
        }
        on_listWidget_currentRowChanged(nowcomboxnum);
        //        SaveWhat = "";
    }
    else if(SaveWhat == "pdmoutmain")
    {
        if(tmp)
        {
            savePDM();
        }
        //SaveWhat ="";
        //on_pushButton_clicked();

    }
    else if(SaveWhat == "pdmoutabout")
    {
        if(tmp)
        {
            savePDM();
        }
        SaveWhat ="";
        on_pushButton_13_clicked();
    }
    else if(SaveWhat == "pdmdel")
    {

        QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
        int numpdmnow = 0;
        if(tmp)
        {
            for(int i = 0;i < 1000;i++ )
            {
                QString name = configIniRead->value(QString("pdminfo").append(QString::number(i+1)).append("/pdmname")).toString();

                if(name == ui->comboBox->itemText(nowcomboxnum))
                {
                    nowpdmnum = i + 1;
                    numpdmnow = nowpdmnum;
                    //qDebug() << "fdddddddddddddddddddddddd" << nowpdmnum;
                    break;
                }
            }
            configIniRead->remove(QString("pdminfo").append(QString::number(nowpdmnum)));

            ui->comboBox->removeItem(nowcomboxnum);
            int pdmnumber = configIniRead->value("baseinfo/pdmnumber").toInt();
            int pdmmax = configIniRead->value("baseinfo/pdmmax").toInt();
            configIniRead->setValue(QString("baseinfo/pdmnumber"),QString::number(pdmnumber-1));
            //qDebug() << "fdfffffffffffff" << pdmmax << pdmnumber;
            if(0 == (pdmnumber-1))
            {
                pdmmax = 0;
            }
            else
            {
                if(numpdmnow == pdmmax)
                {

                    for(int j= numpdmnow-1;j>0;j--)
                    {
                        if(configIniRead->value(QString("pdminfo").append(QString::number(j)).append("/pdmname")).toString() != "")
                        {
                            pdmmax = j;
                            //qDebug() << "fdfdf" << pdmmax << j;
                            break;
                        }

                    }
                }
            }
            configIniRead->setValue(QString("baseinfo/pdmmax"),QString::number(pdmmax));
        }
        else
        {

        }
        SaveWhat = "";
        delete configIniRead;

    }
    else if(SaveWhat == "pdm")
    {
        if(tmp)
        {
            savePDM();
        }
        else
        {
            //             int tempdata = numpdm;
            //             for(int i = 0;i<tempdata;i++)
            //             {
            //                 delete butt1[i];
            //                 numpdm--;
            //             }
        }

        SaveWhat ="";

    }
    ui->label_100->hide();
    delete e3;
    delete save;
    //qDebug()<<"whywhy  SaveWhat"<<SaveWhat;
    if(SaveWhat == "pdmoutmain")
    {
        on_pushButton_clicked();
    }
}

void Newconfiginfo::savePDM()
{
    QSettings *configIniRead = new QSettings("/config.ini",QSettings::IniFormat);
    if(isSavedpdm)
    {
        // 代表  添加新的pdm图
        int pdmmax = configIniRead->value("baseinfo/pdmmax").toInt();
        configIniRead->setValue(QString("pdminfo").append(QString::number(pdmmax+1)).append(QString("/pdmname")),ui->lineEdit_pdmname->text());
        configIniRead->setValue(QString("pdminfo").append(QString::number(pdmmax+1)).append(QString("/pdmpath")),pathpdm);
        configIniRead->setValue(QString("pdminfo").append(QString::number(pdmmax+1)).append(QString("/num")),QString::number(numpdm));  //螺栓个数
        for(int i = 0;i < numpdm;i++)
        {
            //qDebug() << "The coordinate of the butt[temp]"<< butt1[i]->x()<<","<<butt1[i]->y()<<")";
            double tempx = (((butt1[i]->x() - (50 + 200))* 1000) )/1051;
            double tempy = (((butt1[i]->y() - (20 + 120)) * 1000) )/531;

            configIniRead->setValue(QString("pdminfo").append(QString::number((pdmmax+1))).append(QString("/tempx").append(QString::number((i+1)))),QString::number(tempx));
            configIniRead->setValue(QString("pdminfo").append(QString::number((pdmmax+1))).append(QString("/tempy").append(QString::number((i+1)))),QString::number(tempy));
        }
        int pdmnumber = configIniRead->value("baseinfo/pdmnumber").toInt();
        configIniRead->setValue("baseinfo/pdmnumber",QString::number((pdmnumber+1)));
        configIniRead->setValue("baseinfo/pdmmax",QString::number((pdmmax+1)));
        ui->comboBox->addItem(ui->lineEdit_pdmname->text());

        //qDebug() << "huahua";
        ui->comboBox->setCurrentIndex(ui->comboBox->count()-1);
        ui->label_69->show();
        ui->comboBox->show();

    }else  //修改的pdm保存
    {
        //qDebug() << "没进来kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk";
        configIniRead->setValue(QString("pdminfo").append(QString::number(nowpdmnum)).append(QString("/pdmname")),ui->lineEdit_pdmname->text());
        configIniRead->setValue(QString("pdminfo").append(QString::number(nowpdmnum)).append(QString("/pdmpath")),pathpdm);
        configIniRead->setValue(QString("pdminfo").append(QString::number(nowpdmnum)).append(QString("/num")),QString::number(numpdm));
        for(int i = 0;i < numpdm;i++)
        {
            //qDebug() << "The coordinate of the butt[temp]"<< butt1[i]->x()<<","<<butt1[i]->y()<<")";
            double tempx = (((butt1[i]->x() - (50 + 200))* 1000) )/1051;
            double tempy = (((butt1[i]->y() - (20 + 120)) * 1000) )/531;

            configIniRead->setValue(QString("pdminfo").append(QString::number((nowpdmnum))).append(QString("/tempx").append(QString::number((i+1)))),QString::number(tempx));
            configIniRead->setValue(QString("pdminfo").append(QString::number((nowpdmnum))).append(QString("/tempy").append(QString::number((i+1)))),QString::number(tempy));
        }
        ui->comboBox->setItemText(ui->comboBox->currentIndex(),ui->lineEdit_pdmname->text());
    }
    delete configIniRead;
}


void Newconfiginfo::backShow()
{
    QSettings *configIniRead = new QSettings("/config.ini",QSettings::IniFormat);
    ui->pushButton_14->hide();
    ui->pushButton_15->hide();
    ui->pushButton_16->hide();
    ui->pushButton_12->show();
    ui->pushButton_butt1->setEnabled(true);
    ui->pushButton_butt2->setEnabled(true);
    ui->pushButton_butt3->setEnabled(true);
    ui->pushButton_butt4->setEnabled(true);
    ui->pushButton_butt5->setEnabled(true);

    ui->pushButton_8->setEnabled(true);
    ui->pushButton_9->setEnabled(true);

    int buttnumber = 0;
    buttnumber = (pagenum -1)*5+1;
    int t = 0;
    for(int j = buttnumber;j < (buttnumber+5);j++)
    {

        if(configIniRead->value(QString("carinfo").append(QString::number(j)).append("/IsOptionalStation")).toInt()) //有选配
        {
            buttonbox[t]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/67.bmp);font:14pt;color: rgb(248, 248, 255);");
            buttonbox[t]->setText(configIniRead->value(QString("carinfo").append(QString::number(j)).append("/carcx")).toString());
        }
        else //非选配
        {
            int i = 1;
            for(i = 1;i < 21;i++)
            {
                if(configIniRead->value(QString("carinfo").append(QString::number(j)).append("/LSNumber").append(QString::number(i))).toInt())
                {
                    buttonbox[t]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/67.bmp);font:14pt;color: rgb(248, 248, 255);");
                    buttonbox[t]->setText(configIniRead->value(QString("carinfo").append(QString::number(j)).append("/carcx")).toString());
                    break;
                }
            }
            if(i == 21)
            {
                buttonbox[t]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                buttonbox[t]->setText("");
            }
        }
        t++;
    }



    if(configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/IsOptionalStation")).toInt()) //有选配
    {
        buttonbox[whichcar-buttnumber]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/9.bmp);font:14pt;color: rgb(248, 248, 255);");
        buttonbox[whichcar-buttnumber]->setText(configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/carcx")).toString());
    }
    else //非选配
    {
        int i = 1;
        for(i = 1;i < 21;i++)
        {
            int ttt = configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/LSNumber").append(QString::number(i))).toInt();
            if(ttt)
            {
                buttonbox[whichcar-buttnumber]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/9.bmp);font:14pt;color: rgb(248, 248, 255);");
                buttonbox[whichcar-buttnumber]->setText(configIniRead->value(QString("carinfo").append(QString::number(whichcar)).append("/carcx")).toString());
                break;
            }
        }
        if(i == 21)
        {
            buttonbox[whichcar-buttnumber]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
            buttonbox[whichcar-buttnumber]->setText("");
        }
    }
    if(isedit == 1)
    {
        for(int j =1;j<6;j++)
        {
            if(lsnumersxuanlist2[j-1][0].toInt())
            {
                if(j == 1)
                {

                    ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                    ui->pushButton_70->show();
                    ui->pushButton_71->show();
                    ui->pushButton_70->setText(proxuanlist2[j-1][0]);
                    ui->pushButton_71->setText(lsnumersxuanlist2[j-1][0]);


                }
                else if(j == 2)
                {
                    ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                    ui->pushButton_72->show();
                    ui->pushButton_73->show();
                    ui->pushButton_72->setText(proxuanlist2[j-1][0]);
                    ui->pushButton_73->setText(lsnumersxuanlist2[j-1][0]);

                }
                else if(j == 3)
                {
                    ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                    ui->pushButton_74->show();
                    ui->pushButton_75->show();
                    ui->pushButton_74->setText(proxuanlist2[j-1][0]);
                    ui->pushButton_75->setText(lsnumersxuanlist2[j-1][0]);

                } else if(j == 4)
                {
                    ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                    ui->pushButton_76->show();
                    ui->pushButton_77->show();
                    ui->pushButton_76->setText(proxuanlist2[j-1][0]);
                    ui->pushButton_77->setText(lsnumersxuanlist2[j-1][0]);

                }
                else if(j == 5)
                {
                    ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                    ui->pushButton_78->show();
                    ui->pushButton_79->show();
                    ui->pushButton_78->setText(proxuanlist2[j-1][0]);
                    ui->pushButton_79->setText(lsnumersxuanlist2[j-1][0]);

                }


            }
            else
            {
                if(j == 1)
                {
                    // ui->pushButton_xuan1->setEnabled(false);
                    ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_70->setText("");
                    ui->pushButton_71->setText("");

                }
                else if(j == 2)
                {
                    //ui->pushButton_xuan2->setEnabled(false);
                    ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_72->setText("");
                    ui->pushButton_73->setText("");

                }
                else if(j == 3)
                {
                    // ui->pushButton_xuan3->setEnabled(false);
                    ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_74->setText("");
                    ui->pushButton_75->setText("");

                }
                else if(j == 4)
                {
                    // ui->pushButton_xuan4->setEnabled(false);
                    ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_76->setText("");
                    ui->pushButton_77->setText("");

                }
                else if(j == 5)
                {
                    // ui->pushButton_xuan5->setEnabled(false);
                    ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
                    ui->pushButton_78->setText("");
                    ui->pushButton_79->setText("");

                }

            }

        }
    }

    delete configIniRead;

}


void Newconfiginfo::on_pushButton_15_clicked()
{
    //edit  取消
    ui->label_Lsnum->show();
    ui->label_pronum->show();
    ui->label_number->show();
    ui->label_108->show();
    ui->label_109->show();
    ui->label_110->show();

    if(isedit == isoption)
    {
    }
    else
    {
        if(!isoption)
        {
            ui->pushButton_xuan1->setEnabled(false);
            ui->pushButton_xuan2->setEnabled(false);
            ui->pushButton_xuan3->setEnabled(false);
            ui->pushButton_xuan4->setEnabled(false);
            ui->pushButton_xuan5->setEnabled(false);
            ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/21.bmp);");
            ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/21.bmp);");
            ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/21.bmp);");
            ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/21.bmp);");
            ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/21.bmp);");
        }
        isedit = isoption;
    }
    ui->stackedWidget_3->setCurrentIndex(0);
    if(isedit == 1)
    {
        ui->label_carname->setText(ui->lineEdit_carname->text());
        ui->label_VIN->setText(ui->lineEdit_VIN->text());
        ui->label_G9->setText(ui->lineEdit_G9->text());
        ui->label_isxuan->setText("yes");
        ui->pushButton_22->hide();
        ui->comboBox_2->hide();

        if(!isoptionsaved)
        {
            on_pushButton_14_clicked();
        }else
        {
            backShow();
            ui->pushButton_xuan1->setEnabled(false);
            ui->pushButton_xuan2->setEnabled(false);
            ui->pushButton_xuan3->setEnabled(false);
            ui->pushButton_xuan4->setEnabled(false);
            ui->pushButton_xuan5->setEnabled(false);
            for(int j =1;j<6;j++)
            {
                if(lsnumersxuanlist[j-1][0].toInt())
                {
                    if(j == 1)
                    {

                        ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                        ui->pushButton_70->show();
                        ui->pushButton_71->show();
                        ui->pushButton_70->setText(proxuanlist[j-1][0]);
                        ui->pushButton_71->setText(lsnumersxuanlist[j-1][0]);


                    }
                    else if(j == 2)
                    {
                        ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                        ui->pushButton_72->show();
                        ui->pushButton_73->show();
                        ui->pushButton_72->setText(proxuanlist[j-1][0]);
                        ui->pushButton_73->setText(lsnumersxuanlist[j-1][0]);

                    }
                    else if(j == 3)
                    {
                        ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                        ui->pushButton_74->show();
                        ui->pushButton_75->show();
                        ui->pushButton_74->setText(proxuanlist[j-1][0]);
                        ui->pushButton_75->setText(lsnumersxuanlist[j-1][0]);

                    } else if(j == 4)
                    {
                        ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                        ui->pushButton_76->show();
                        ui->pushButton_77->show();
                        ui->pushButton_76->setText(proxuanlist[j-1][0]);
                        ui->pushButton_77->setText(lsnumersxuanlist[j-1][0]);

                    }
                    else if(j == 5)
                    {
                        ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                        ui->pushButton_78->show();
                        ui->pushButton_79->show();
                        ui->pushButton_78->setText(proxuanlist[j-1][0]);
                        ui->pushButton_79->setText(lsnumersxuanlist[j-1][0]);

                    }


                }
                else
                {
                    if(j == 1)
                    {
                        // ui->pushButton_xuan1->setEnabled(false);
                        ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");

                    }
                    else if(j == 2)
                    {
                        //ui->pushButton_xuan2->setEnabled(false);
                        ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");

                    }
                    else if(j == 3)
                    {
                        // ui->pushButton_xuan3->setEnabled(false);
                        ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");

                    }
                    else if(j == 4)
                    {
                        // ui->pushButton_xuan4->setEnabled(false);
                        ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");

                    }
                    else if(j == 5)
                    {
                        // ui->pushButton_xuan5->setEnabled(false);
                        ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");

                    }

                }

            }
        }
        ui->label_Lsnum->hide();
        ui->label_Lsnum->hide();
        ui->label_pronum->hide();
        ui->label_number->hide();
        ui->label_isxuan->setText("yes");
        ui->label_108->hide();
        ui->label_109->hide();
        ui->label_110->hide();
        ui->label_52->hide();
        ui->pushButton_10->hide();
        ui->pushButton_11->hide();
        ui->label_currentindex->hide();
        ui->stackedWidget_5->setCurrentIndex(0);
    }
    else if(!isedit)
    {
        luo2[ui->label_119->text().toInt()-1] = ui->lineEdit_Lsnumber->text();
        pro2[ui->label_119->text().toInt()-1] = ui->lineEdit_pronum->text();
        lsnumers2[ui->label_119->text().toInt()-1] = ui->lineEdit_number->text();
        //qDebug() << pro2[ui->label_119->text().toInt()-1] << ui->label_119->text();
        int i = 0 ;
        for(i = 0;i<20;i++)
        {
            //qDebug() << carStyle << ui->lineEdit_carname->text() << pro[i] << pro2[i] << i;
            if(carStyle!=ui->lineEdit_carname->text() || Vintmp!=ui->lineEdit_VIN->text() || G9tmp!=ui->lineEdit_G9->text() ||
                    pro[i]!=pro2[i] || lsnumers2[i]!=lsnumers[i])
            {
                SaveWhat = "CANCEL";
                e3 = new QGraphicsOpacityEffect(this);
                e3->setOpacity(0.5);
                ui->label_100->setGraphicsEffect(e3);
                ui->label_100->show();
                ui->label_100->setGeometry(0,0,1366,768);

                save = new Save(this);
                connect(save,SIGNAL(sendDeSingle(bool)),this,SLOT(receiveDesignle(bool)));
                save->show();
                break;
            }
        }
        if(i == 20)
        {
            ui->stackedWidget_5->setCurrentIndex(0);
            backShow();
        }
        if(buttonbox[whichcar-(pagenum-1)*5-1]->text() == "")
        {
            ui->pushButton_12->hide();
        }
    }
}

void Newconfiginfo::on_pushButton_16_clicked()
{
    //删除全部
    if(!isedit)
    { //
        SaveWhat = "deall";

        e3 = new QGraphicsOpacityEffect(this);
        e3->setOpacity(0.5);
        ui->label_100->setGraphicsEffect(e3);
        ui->label_100->show();
        ui->label_100->setGeometry(0,0,1366,768);

        save = new Save(this);
        connect(save,SIGNAL(sendDeSingle(bool)),this,SLOT(receiveDesignle(bool)));
        save->show();

    }
    else
    { // 选配删除
        SaveWhat = "OPDELALL";
        e3 = new QGraphicsOpacityEffect(this);
        e3->setOpacity(0.5);
        ui->label_100->setGraphicsEffect(e3);
        ui->label_100->show();
        ui->label_100->setGeometry(0,0,1366,768);

        save = new Save(this);
        connect(save,SIGNAL(sendDeSingle(bool)),this,SLOT(receiveDesignle(bool)));
        save->show();

    }


}

void Newconfiginfo::on_pushButton_12_clicked()
{
    //编辑 按钮
    isxuanlook = false;
    ui->stackedWidget_3->setCurrentIndex(1);
    ui->stackedWidget_5->setCurrentIndex(1);
    ui->label_119->setText("1");
    ui->pushButton_12->hide();
    ui->pushButton_16->show();
    ui->pushButton_15->show();
    ui->pushButton_14->show();
    currentpages = ui->label_119->text().toInt();
    ui->pushButton_8->setEnabled(false);
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_9->setEnabled(false);


    if(whichcar == 1 || whichcar == 6 || whichcar == 11 || whichcar == 16)
    {
        ui->pushButton_butt1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/9.bmp);font:14pt;color: rgb(248, 248, 255);");
        ui->pushButton_butt2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt2->setEnabled(false);
        ui->pushButton_butt3->setEnabled(false);
        ui->pushButton_butt4->setEnabled(false);
        ui->pushButton_butt5->setEnabled(false);
        ui->pushButton_butt1->setEnabled(false);

    }
    else if(whichcar == 2 || whichcar == 7 || whichcar == 12 || whichcar == 17)
    {
        ui->pushButton_butt1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/9.bmp);font:14pt;color: rgb(248, 248, 255);");
        ui->pushButton_butt3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt1->setEnabled(false);
        ui->pushButton_butt2->setEnabled(false);
        ui->pushButton_butt4->setEnabled(false);
        ui->pushButton_butt3->setEnabled(false);
        ui->pushButton_butt5->setEnabled(false);

    }else if(whichcar == 3 ||whichcar == 8 || whichcar == 13 || whichcar == 18)
    {

        ui->pushButton_butt1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/9.bmp);font:14pt;color: rgb(248, 248, 255);");
        ui->pushButton_butt4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt1->setEnabled(false);
        ui->pushButton_butt2->setEnabled(false);
        ui->pushButton_butt3->setEnabled(false);
        ui->pushButton_butt4->setEnabled(false);
        ui->pushButton_butt5->setEnabled(false);
    }else if(whichcar == 4 || whichcar == 9 || whichcar == 14 || whichcar == 19)
    {

        ui->pushButton_butt1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/9.bmp);font:14pt;color: rgb(248, 248, 255);");
        ui->pushButton_butt5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt1->setEnabled(false);
        ui->pushButton_butt2->setEnabled(false);
        ui->pushButton_butt4->setEnabled(false);
        ui->pushButton_butt3->setEnabled(false);
        ui->pushButton_butt5->setEnabled(false);
    }else if(whichcar == 5 || whichcar == 10 || whichcar == 15 || whichcar == 20)
    {
        ui->pushButton_butt1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/15.bmp);");
        ui->pushButton_butt5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/9.bmp);font:14pt;color: rgb(248, 248, 255);");
        ui->pushButton_butt1->setEnabled(false);
        ui->pushButton_butt2->setEnabled(false);
        ui->pushButton_butt3->setEnabled(false);
        ui->pushButton_butt4->setEnabled(false);
        ui->pushButton_butt5->setEnabled(false);
    }

    ui->lineEdit_carname->setText(carStyle);
    ui->lineEdit_G9->setText(G9tmp);
    ui->lineEdit_VIN->setText(Vintmp);

    if(!isedit)
    {
        ui->lineEdit_Lsnumber->setText(luo[0]);
        ui->lineEdit_pronum->setText(pro[0]);
        ui->lineEdit_number->setText(lsnumers[0]);
        ui->label_taotongg->show();
        ui->pushButton_taotong_add->show();
        ui->pushButton_taotong_minus->show();
        ui->lineEdit_taotong->show();
        if(taotong[0] == "" || taotong[0] == "0")
            ui->lineEdit_taotong->setText(tr("无"));
        else
            ui->lineEdit_taotong->setText(taotong[0]);
    }
    else
    {
        //选配 编辑
        ui->pushButton_6->hide();
        ui->pushButton_7->hide();
        ui->lineEdit_Lsnumber->hide();
        ui->lineEdit_pronum->hide();
        ui->lineEdit_number->hide();
        ui->pushButton_pronum_add->hide();
        ui->pushButton_pronum_minus->hide();
        ui->pushButton_number_add->hide();
        ui->pushButton_number_minus->hide();
        ui->pushButton_12->hide();
        ui->pushButton_52->hide();
        ui->label_60->hide();
        ui->label_115->hide();
        ui->label_116->hide();
        ui->label_117->hide();
        ui->label_119->hide();
        ui->label_taotongg->hide();
        ui->pushButton_taotong_add->hide();
        ui->pushButton_taotong_minus->hide();
        ui->lineEdit_taotong->hide();
        ui->pushButton_xuan1->setEnabled(true);
        ui->pushButton_xuan2->setEnabled(true);
        ui->pushButton_xuan3->setEnabled(true);
        ui->pushButton_xuan4->setEnabled(true);
        ui->pushButton_xuan5->setEnabled(true);
        ui->pushButton_5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/13.bmp);");
        optionIscheck = true;
        for(int i = 0;i<5;i++)
        {
            if(lsnumersxuanlist[i][0].toInt())
            {
                if(i == 0)
                    ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                else if(i == 1)
                    ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                else if(i == 2)
                    ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                else if(i == 3)
                    ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                else if(i == 4)
                    ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");

            }else
            {
                if(i == 0)
                    ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                else if(i == 1)
                    ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                else if(i == 2)
                    ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                else if(i == 3)
                    ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
                else if(i == 4)
                    ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");

            }
        }
    }

}

//鼠标过滤事件  拖动
bool Newconfiginfo::eventFilter(QObject *target, QEvent *evt)
{
    static QPoint lastPnt;
    static bool isHover = false;
    //qDebug()  << "isis";
    for(int i = 0;i < numpdm;i++)
    {
        if(target == butt1[i])
        {
            if(evt->type() == QEvent::MouseButtonPress)
            {
                QMouseEvent* e = static_cast<QMouseEvent*>(evt);
                //QPushButton *child = static_cast<QPushButton *>(childAt(e->pos()));


                // for( i = 0;i < numpdm; i++)
                //{
                if(butt1[i]->rect().contains(e->pos()) && //is the mouse is clicking the key
                        (e->button() == Qt::LeftButton)) //if the mouse click the left key
                {
                    lastPnt = e->pos();
                    //qDebug() << e->pos().x() << e->pos().y();
                    isHover = true;
                }
                // }
            }
            else if(evt->type() == QEvent::MouseMove && isHover)
            {
                QMouseEvent* e = static_cast<QMouseEvent*>(evt);
                int dx = e->pos().x() - lastPnt.x();
                int dy = e->pos().y() - lastPnt.y();
                int x1,y1;
                if (butt1[i]->x()+dx<259)
                    x1=259;
                else if (butt1[i]->x()+dx>1265)
                    x1=1265;
                else
                    x1=butt1[i]->x()+dx;
                if (butt1[i]->y()+dy<130)
                    y1=130;
                else if (butt1[i]->y()+dy>616)
                    y1=616;
                else
                    y1=butt1[i]->y()+dy;
                butt1[i]->move(x1,y1);
            }else if(evt->type() == QEvent::MouseButtonRelease && isHover)
            {
                //qDebug() << "The coordinate of the butt[temp]"<< butt1[i]->x()<<","<<butt1[i]->y()<<")";
                isHover = false;
                SaveWhat = "pdm";
            }
        }
    }

    return false;
}

void Newconfiginfo::xuanchoux(int tmp)
{
    ui->label_bx1name->clear();
    ui->label_bx1code->clear();
    ui->label_bx2name->clear();
    ui->label_bx2code->clear();
    ui->label_bx3name->clear();
    ui->label_bx3code->clear();
    ui->label_bx4name->clear();
    ui->label_bx4code->clear();
    ui->label_bx5name->clear();
    ui->label_bx5code->clear();
    ui->label_kx1name->clear();
    ui->label_kx1code->clear();
    ui->label_kx2name->clear();
    ui->label_kx2code->clear();
    ui->label_kx3name->clear();
    ui->label_kx3code->clear();
    ui->label_kx4name->clear();
    ui->label_kx4code->clear();
    ui->label_kx5name->clear();
    ui->label_kx5code->clear();
    ui->pushButton_bxuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
    ui->pushButton_bxuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
    ui->pushButton_bxuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
    ui->pushButton_bxuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
    ui->pushButton_bxuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");

    ui->pushButton_kxuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
    ui->pushButton_kxuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
    ui->pushButton_kxuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
    ui->pushButton_kxuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
    ui->pushButton_kxuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/16.bmp);");
    ui->comboBox_3->setCurrentIndex(0);
    ui->label_51->setText("1");
    whichoption = tmp;
    for(int k = 0;k< 250;k++)
    {
        if(pdmxuanlist2[whichoption-1] == ui->comboBox_2->itemText(k))
        {
            ui->comboBox_3->setCurrentIndex(k);
            break;
        }

    }
    ui->stackedWidget->setCurrentIndex(2);
    ui->stackedWidget_9->setCurrentIndex(0);
    if(isJS)
    {
        ui->stackedWidget_13->setCurrentIndex(1);
        ui->label_75->setText(luoxuanlist2[whichoption-1][0]);
        ui->label_76->setText(proxuanlist2[whichoption-1][0]);
        ui->label_77->setText(lsnumersxuanlist2[whichoption-1][0]);
        ui->label_78->setText(pdmxuanlist2[whichoption-1]);
        ui->label_75->setText(luoxuanlist2[whichoption-1][currentpages-1]);
        ui->label_76->setText(proxuanlist2[whichoption-1][currentpages-1]);
        ui->label_77->setText(lsnumersxuanlist2[whichoption-1][currentpages-1]);
        ui->pushButton_14->hide();
        ui->pushButton_15->hide();
        ui->pushButton_16->hide();
        ui->pushButton_23->hide();
        ui->pushButton_24->hide();
        ui->pushButton_25->hide();
        ui->label_78->setText(pdmxuanlist2[whichoption-1]);
        ui->pushButton_bxuan1->setEnabled(false);
        ui->pushButton_bxuan2->setEnabled(false);
        ui->pushButton_bxuan3->setEnabled(false);
        ui->pushButton_bxuan4->setEnabled(false);
        ui->pushButton_bxuan5->setEnabled(false);

        ui->pushButton_kxuan1->setEnabled(false);
        ui->pushButton_kxuan2->setEnabled(false);
        ui->pushButton_kxuan3->setEnabled(false);
        ui->pushButton_kxuan4->setEnabled(false);
        ui->pushButton_kxuan5->setEnabled(false);

    }
    else
    {
        ui->stackedWidget_13->setCurrentIndex(0);
    }

    ui->lineEdit_xuanLsnum->setText(luoxuanlist2[whichoption-1][0]);
    if (proxuanlist2[whichoption-1][0]=="")
        ui->lineEdit_xuanpronum->setText("0");
    else ui->lineEdit_xuanpronum->setText(proxuanlist2[whichoption-1][0]);
    if (lsnumersxuanlist2[whichoption-1][0]=="")
        ui->lineEdit_xuannumber->setText("0");
    else ui->lineEdit_xuannumber->setText(lsnumersxuanlist2[whichoption-1][0]);


    if(bxuannamelist2[whichoption-1][0]!="" && bxuancodelist2[whichoption-1][0] !="")
    {
        ui->pushButton_bxuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");
        ui->label_bx1name->setText(bxuannamelist2[whichoption-1][0]);
        ui->label_bx1code->setText(bxuancodelist2[whichoption-1][0]);
        //qDebug() << "nbnb1" << bxuannamelist2[whichoption-1][0] << bxuancodelist[whichoption-1][0];

    }
    if(bxuannamelist2[whichoption-1][1]!="" && bxuancodelist2[whichoption-1][1] !="")
    {
        ui->pushButton_bxuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");

        ui->label_bx2name->setText(bxuannamelist2[whichoption-1][1]);
        ui->label_bx2code->setText(bxuancodelist2[whichoption-1][1]);
        //qDebug() << "nbnb1" << bxuannamelist2[whichoption-1][1] << bxuancodelist2[whichoption-1][1];

    }
    if(bxuannamelist2[whichoption-1][2]!="" && bxuancodelist2[whichoption-1][2] !="")
    {
        ui->pushButton_bxuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");

        ui->label_bx3name->setText(bxuannamelist2[whichoption-1][2]);
        ui->label_bx3code->setText(bxuancodelist2[whichoption-1][2]);
        //qDebug() << "nbnb1" << bxuannamelist2[whichoption-1][2] << bxuancodelist[whichoption-1][2];

    }
    if(bxuannamelist2[whichoption-1][3]!="" && bxuancodelist2[whichoption-1][3] !="")
    {
        ui->pushButton_bxuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");

        ui->label_bx4name->setText(bxuannamelist2[whichoption-1][3]);
        ui->label_bx4code->setText(bxuancodelist2[whichoption-1][3]);
        //qDebug() << "nbnb1" << bxuannamelist2[whichoption-1][3] << bxuancodelist[whichoption-1][3];

    }
    if(bxuannamelist2[whichoption-1][4]!="" && bxuancodelist2[whichoption-1][4] !="")
    {
        ui->pushButton_bxuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");

        ui->label_bx5name->setText(bxuannamelist2[whichoption-1][4]);
        ui->label_bx5code->setText(bxuancodelist2[whichoption-1][4]);
        //qDebug() << "nbnb1" << bxuannamelist[whichoption-1][4] << bxuancodelist[whichoption-1][4];

    }

    if(kxuannamelist2[whichoption-1][0]!="" && kxuancodelist2[whichoption-1][0] !="")
    {
        ui->pushButton_kxuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");

        ui->label_kx1name->setText(kxuannamelist2[whichoption-1][0]);
        ui->label_kx1code->setText(kxuancodelist2[whichoption-1][0]);

    }
    if(kxuannamelist2[whichoption-1][1]!="" && kxuancodelist2[whichoption-1][1] !="")
    {
        ui->pushButton_kxuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");

        ui->label_kx2name->setText(kxuannamelist2[whichoption-1][1]);
        ui->label_kx2code->setText(kxuancodelist2[whichoption-1][1]);

    }
    if(kxuannamelist2[whichoption-1][2]!="" && kxuancodelist2[whichoption-1][2] !="")
    {
        ui->pushButton_kxuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");

        ui->label_kx3name->setText(kxuannamelist2[whichoption-1][2]);
        ui->label_kx3code->setText(kxuancodelist2[whichoption-1][2]);

    }
    if(kxuannamelist2[whichoption-1][3]!="" && kxuancodelist2[whichoption-1][3] !="")
    {
        ui->pushButton_kxuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");

        ui->label_kx4name->setText(kxuannamelist2[whichoption-1][3]);
        ui->label_kx4code->setText(kxuancodelist2[whichoption-1][3]);

    }
    if(kxuannamelist2[whichoption-1][4]!="" && kxuancodelist2[whichoption-1][4] !="")
    {
        ui->pushButton_kxuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");

        ui->label_kx5name->setText(kxuannamelist2[whichoption-1][4]);
        ui->label_kx5code->setText(kxuancodelist2[whichoption-1][4]);

    }

}
//delete config;

void Newconfiginfo::on_pushButton_xuan1_clicked()
{
    //选配按钮  1

    xuanchoux(1);


}

void Newconfiginfo::on_pushButton_xuan2_clicked()
{
    //选配按钮  2
    xuanchoux(2);
}

void Newconfiginfo::on_pushButton_xuan3_clicked()
{
    //选配按钮  3
    xuanchoux(3);
}

void Newconfiginfo::on_pushButton_xuan4_clicked()
{
    //选配按钮  4
    xuanchoux(4);
}

void Newconfiginfo::on_pushButton_xuan5_clicked()
{
    //选配按钮  5
    xuanchoux(5);
}






void Newconfiginfo::on_pushButton_20_clicked()
{
    //选配 关闭
    if(!isJS)
        on_pushButton_24_clicked();
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_2->setCurrentIndex(4);
}

void Newconfiginfo::on_pushButton_18_clicked()
{
    // 选up
    carStyle = ui->lineEdit_carname->text();
    G9tmp = ui->lineEdit_G9->text();
    Vintmp = ui->lineEdit_VIN->text();
    luoxuanlist2[whichoption-1][ui->label_51->text().toInt()-1] = ui->lineEdit_xuanLsnum->text();
    proxuanlist2[whichoption-1][ui->label_51->text().toInt()-1] = ui->lineEdit_xuanpronum->text();
    lsnumersxuanlist2[whichoption-1][ui->label_51->text().toInt()-1] = ui->lineEdit_xuannumber->text();
    updownWriteOperate(1);

}

void Newconfiginfo::on_pushButton_19_clicked()
{
    //   选down
    carStyle = ui->lineEdit_carname->text();
    G9tmp = ui->lineEdit_G9->text();
    Vintmp = ui->lineEdit_VIN->text();
    luoxuanlist2[whichoption-1][ui->label_51->text().toInt()-1] = ui->lineEdit_xuanLsnum->text();
    proxuanlist2[whichoption-1][ui->label_51->text().toInt()-1] = ui->lineEdit_xuanpronum->text();
    lsnumersxuanlist2[whichoption-1][ui->label_51->text().toInt()-1] = ui->lineEdit_xuannumber->text();
    updownWriteOperate(0);
}


void Newconfiginfo::on_pushButton_28_clicked()
{
    if(ui->stackedWidget_2->currentIndex()!=5)
        //pdm编辑
    {
        ispdminit = 0;
        // qDebug()<<"????????????????????????????????????????";
        ui->groupBox_13->setTitle(tr("图片"));
        ui->groupBox_13->setStyleSheet("QGroupBox {font: 14pt;margin-top: 1ex;border-width:0.5px;border-style:solid;border-color: rgb(51, 153, 255);} QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top left;left:15px;margin-left: 5px;margin-right: 5px;padding:1px;}");
        ui->listWidget->setCurrentRow(1);
        historyclear();
        if(isJS)
        {
            //////////////////////////////////////////////////////////////////////////////////////////未写
        }
        else
        {

            whichButtonClick = "PDMEdit";
            ui->comboBox->show();
            ui->label_69->show();
            ui->lineEdit_pdmname->hide();
            ui->pushButton_29->hide();
            ui->pushButton_30->hide();
            ui->pushButton_31->hide();

            if(!isbaseinfochange)
                baseInfoIsChange();
            else
            {
                ui->label_83->hide();
                ui->label_84->hide();
                ui->label_85->hide();
                ui->label_86->show();
                //            ui->pushButton_13->hide();
                ui->stackedWidget_2->setCurrentIndex(5);
                ui->stackedWidget_6->setCurrentIndex(0);
                //ui->stackedWidget_3->setCurrentIndex(4);
                //ui->pushButton_96->setText(tr("PDM编辑"));
                ui->comboBox->clear();
                QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
                ui->comboBox->addItem("");
                //int pdmnumber = config->value("baseinfo/pdmnumber").toInt();

                for(int i= 0;i < 1000 ;i++)
                {

                    if(config->value(QString("pdminfo").append(QString::number(i+1)).append("/pdmname")).toString()!= "")
                    {
                        ui->comboBox->addItem(config->value(QString("pdminfo").append(QString::number(i+1)).append("/pdmname")).toString());
                    }
                }
                SaveWhat = "";
                ispdminit = 1;
                delete config;
                //pdminit();
            }
        }
    }
}

void Newconfiginfo::pdmSelect()
{
    if(numpdm > 0)
    {
        for(int i = 0;i< numpdm;i++)
            butt1[i]->show();
    }
    pathpdm =  ui->listWidget_2->currentItem()->text();
    //qDebug() <<tr( ui->listWidget_2->currentItem()->text().toAscii().data());
    ui->stackedWidget_6->setCurrentIndex(0);
    //qDebug() << tr(pathpdm.toAscii().data());
    ui->groupBox_13->setTitle("");
    ui->groupBox_13->setStyleSheet(QString("border-image: url(/PDM/").append(pathpdm.toAscii()).append(")"));

}


void Newconfiginfo::pdminit()
{//  pdm图 listwidget初始化
    ui->listWidget_2->setViewMode(QListView::IconMode);    //设置QListWidget的显示模式
    ui->listWidget_2->setResizeMode(QListView::Adjust);
    ui->listWidget_2->setMovement(QListView::Static);      //设置QListWidget中的单元项不可被拖动
    ui->listWidget_2->setIconSize(QSize(241, 179));        //设置QListWidget中的单元项的图片大小
    ui->listWidget_2->setTextElideMode(Qt::ElideRight);
    //ui->listWidget_2->setSpacing(10);                       //设置QListWidget中的单元项的间距
    ui->listWidget_2->setStyleSheet("QListWidget{border-width:0.5px;border-style:solid;border-color:rgb(51, 153, 255);} QListWidget::item{border-right-width:0.5px;border-bottom-width:0.5px;border-style:solid;border-color:rgb(51, 153, 255);}QScrollBar:vertical{width:36px;}");
    // ui->listWidget_2->verticalScrollBar()->setStyleSheet("QScrollBar{width:36px;}");

    QDir *dir = new QDir("/PDM");
    ui->listWidget_2->clear();
    QStringList list = dir->entryList();
    QString string;
    for (int index = 0; index < list.size(); index++)
    {
        string = list.at(index);
        if(string == "." || string == "..")
            continue;
        //QListWidgetItem *item = new QListWidgetItem(string);
        //ui->listWidget->addItem(item);
        QListWidgetItem *configButton = new QListWidgetItem(ui->listWidget_2);
        // qDebug() << string;
        QImage image;


        image.load(QString("/PDM/").append(list.at(index)));
        QPixmap objPixmap = QPixmap::fromImage(image.scaled(QSize(241,179)));
        configButton->setIcon(QIcon(objPixmap));
        // configButton->setIcon(QIcon(QString("/PDM/").append(list.at(index))));
        configButton->setText(tr(string.toLatin1().data()));
        configButton->setSizeHint(QSize(253,210));

        configButton->setTextAlignment(Qt::AlignHCenter);
        configButton->setFlags(configButton->flags() & ~Qt::ItemIsSelectable);
    }
    connect(ui->listWidget_2,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(pdmSelect()));
    ui->listWidget_2->setFlow(QListView::LeftToRight);
    ui->listWidget_2->update();
    delete dir;
}
void Newconfiginfo::on_pushButton_29_clicked()
{

    //图片浏览
    SaveWhat = "pdm";
    if(numpdm > 0)
    {
        for(int i = 0;i< numpdm;i++)
            butt1[i]->hide();
    }
    ui->stackedWidget_6->setCurrentIndex(1);

}

//pdm 图片选择取消
void Newconfiginfo::receivecancelpdm()
{
    delete e3;
    ui->label_100->hide();
}
void Newconfiginfo::on_pushButton_30_clicked()
{
    //添加螺栓
    SaveWhat = "pdm";
    numpdm++;
    //       qsrand(time(NULL));

    //       int n = qrand() % 1000;
    //       qsrand(time(NULL));
    //       qsrand(time(NULL));
    //      int n1 = qrand() % 400;
    temppdm = numpdm - 1;
    //qDebug() << "temp is" << temppdm;

    butt1[temppdm] = new QPushButton(this);
    butt1[temppdm]->setFocusPolicy(Qt::NoFocus);
    butt1[temppdm]->setFlat(true);

    butt1[temppdm]->setStyleSheet("font-size: 35px");
    butt1[temppdm]->setText(QString::number(numpdm));
    butt1[temppdm]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/g01.png);");
    butt1[temppdm]->show();

    butt1[temppdm]->setGeometry(400,300,45,45);

    butt1[temppdm]->installEventFilter(this);
}

void Newconfiginfo::on_pushButton_31_clicked()
{
    //删除螺栓
    SaveWhat = "pdm";
    if((temppdm == -1) || (!numpdm))
    {
        temppdm++;
        return;
    }
    //qDebug() << "temp is" << temppdm;
    delete butt1[temppdm];
    temppdm--;
    numpdm--;
}


void Newconfiginfo::on_pushButton_21_clicked()
{
    //pdm取消
    ui->groupBox_13->setTitle(tr("图片"));
    ui->groupBox_13->setStyleSheet("QGroupBox {font: 14pt;margin-top: 1ex;border-width:0.5px;border-style:solid;border-color: rgb(51, 153, 255);} QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top left;left:15px;margin-left: 5px;margin-right: 5px;padding:1px;}");
    ui->label_69->show();
    ui->comboBox->show();
    ui->comboBox->setCurrentIndex(0);
    ui->lineEdit_pdmname->hide();
    ui->pushButton_29->hide();
    ui->pushButton_30->hide();
    ui->pushButton_31->hide();
    int tempdata = numpdm;
    for(int i = 0;i<tempdata;i++)
    {
        delete butt1[i];
        numpdm--;
    }
    ui->pushButton_33->show();
    SaveWhat = "";
}

void Newconfiginfo::on_pushButton_32_clicked()
{
    //pdm编辑保存按钮
    ui->pushButton_33->show();
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    SaveWhat = "pdm";
    int pdmmax = configIniRead->value("baseinfo/pdmmax").toInt();
    int k = 0;
    for( k = 0;k<pdmmax;k++)
    {
        if(!isSavedpdm)
        {
            if(ui->comboBox->currentText() == ui->lineEdit_pdmname->text())
                continue;
        }
        if(configIniRead->value(QString("pdminfo").append(QString::number(k+1)).append(QString("/pdmname"))).toString() == ui->lineEdit_pdmname->text())  // 名字重复报错
        {
            e3 = new QGraphicsOpacityEffect(this);
            e3->setOpacity(0.5);
            ui->label_100->setGraphicsEffect(e3);
            ui->label_100->show();
            ui->label_100->setGeometry(0,0,1366,768);
            SaveWhat = "pdmwaring";
            save = new Save(this);
            connect(save,SIGNAL(sendDeSingle(bool)),this,SLOT(receiveDesignle(bool)));
            save->show();
            break;
        }
    }
    if(k == pdmmax)
    {
        e3 = new QGraphicsOpacityEffect(this);
        e3->setOpacity(0.5);
        ui->label_100->setGraphicsEffect(e3);
        ui->label_100->show();
        ui->label_100->setGeometry(0,0,1366,768);

        save = new Save(this);
        connect(save,SIGNAL(sendDeSingle(bool)),this,SLOT(receiveDesignle(bool)));
        save->show();
    }
    delete configIniRead;
}



void Newconfiginfo::on_comboBox_currentIndexChanged(int index)
{
    //combox 选中
    if (!index)
    {
        ui->groupBox_13->setTitle(tr("图片"));
        ui->groupBox_13->setStyleSheet("QGroupBox {font: 14pt;margin-top: 1ex;border-width:0.5px;border-style:solid;border-color: rgb(51, 153, 255);} QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top left;left:15px;margin-left: 5px;margin-right: 5px;padding:1px;}");
        ui->lineEdit_pdmname->hide();
        ui->pushButton_29->hide();
        ui->pushButton_30->hide();
        ui->pushButton_31->hide();
        int tempdata = numpdm;
        for(int i = 0;i<tempdata;i++)
        {
            delete butt1[i];
            numpdm--;
        }
    }
    else
    {
        QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
        if((ispdminit))
        {
            isSavedpdm = false;
            ui->lineEdit_pdmname->show();
            ui->pushButton_29->show();
            ui->pushButton_30->show();
            ui->pushButton_31->show();
            ui->lineEdit_pdmname->setText(ui->comboBox->currentText());
            int tempdata = numpdm;
            for(int i = 0;i<tempdata;i++)
            {
                delete butt1[i];
                numpdm--;
            }
            nowcomboxnum = index ;
            for(int i = 0;i < 1000;i++ )
            {
                QString name = config->value(QString("pdminfo").append(QString::number(i+1)).append("/pdmname")).toString();
                if(name == ui->comboBox->itemText(index))
                {
                    nowpdmnum = i + 1;
                    break;
                }
            }
            pathpdm = config->value(QString("pdminfo").append(QString::number((nowpdmnum))).append("/pdmpath")).toString();
            ui->groupBox_13->setTitle("");
            ui->groupBox_13->setStyleSheet(QString("border-image: url(/PDM/").append(pathpdm).append(")"));

            int num = config->value(QString("pdminfo").append(QString::number((nowpdmnum))).append("/num")).toInt();
            for(int i = 0;i < num;i++)
            {
                butt1[i] = new QPushButton(this);
                butt1[i]->setFocusPolicy(Qt::NoFocus);
                numpdm++;
                temppdm = numpdm -1;
                butt1[i]->setFlat(true);
                butt1[i]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/g01.png);");
                butt1[i]->setText(QString::number((i+1)));

                int tempx = config->value(QString("pdminfo").append(QString::number((nowpdmnum))).append("/tempx").append(QString::number((i+1)))).toInt();
                int tempy =  config->value(QString("pdminfo").append(QString::number((nowpdmnum))).append("/tempy").append(QString::number((i+1)))).toInt();
                double x = (double)tempx/1000 * 1051 +  250;
                double y = (double)tempy/1000 * 531+ 140;
                butt1[i]->setGeometry(x,y,45,45);
                butt1[i]->show();
                butt1[i]->installEventFilter(this);
            }
        }
        delete config;
    }
    //qDebug()<<"SaveWhat@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<SaveWhat;
}

void Newconfiginfo::on_pushButton_33_clicked()
{
    //删除
    if(ui->comboBox->currentText() != "")
    {
        if(!isSavedpdm)
        {
            SaveWhat = "pdmdel";
            e3 = new QGraphicsOpacityEffect(this);
            e3->setOpacity(0.5);
            ui->label_100->setGraphicsEffect(e3);
            ui->label_100->show();
            ui->label_100->setGeometry(0,0,1366,768);
            save = new Save(this);
            connect(save,SIGNAL(sendDeSingle(bool)),this,SLOT(receiveDesignle(bool)));
            save->show();
        }
    }

}


void Newconfiginfo::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    // 选择pdm图

    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    for(int i = 0;i < 1000;i++ )
    {
        QString name = configIniRead->value(QString("pdminfo").append(QString::number(i+1)).append("/pdmname")).toString();
        if(name == arg1)
        {
            whichpdmyinyong = QString("pdminfo").append(QString::number(i+1));
            break;
        }
    }


    delete configIniRead;

}

void Newconfiginfo::on_pushButton_24_clicked()
{
    //选配取消
    isxuanlook = false;
    int i = 0;
    luoxuanlist2[whichoption-1][ui->label_51->text().toInt()-1] = ui->lineEdit_xuanLsnum->text();
    proxuanlist2[whichoption-1][ui->label_51->text().toInt()-1] = ui->lineEdit_xuanpronum->text();
    lsnumersxuanlist2[whichoption-1][ui->label_51->text().toInt()-1] = ui->lineEdit_xuannumber->text();
    pdmxuanlist2[whichoption-1] = ui->comboBox_3->currentText();
    for(i = 0;i < 20;i++)
    {
        if(luoxuanlist2[whichoption-1][i]!=luoxuanlist[whichoption-1][i] || proxuanlist2[whichoption-1][i]!=proxuanlist[whichoption-1][i] ||
                lsnumersxuanlist2[whichoption-1][i]!=lsnumersxuanlist[whichoption-1][i] || bxuannamelist2[whichoption-1][i]!=bxuannamelist[whichoption-1][i] ||
                bxuancodelist2[whichoption-1][i]!=bxuancodelist[whichoption-1][i] || kxuannamelist2[whichoption-1][i]!=kxuannamelist[whichoption-1][i] ||
                kxuancodelist2[whichoption-1][i]!=kxuancodelist[whichoption-1][i] || pdmxuanlist2[whichoption-1]!= pdmxuanlist[whichoption-1])
        {
            SaveWhat = "OPCANCEL";
            e3 = new QGraphicsOpacityEffect(this);
            e3->setOpacity(0.5);
            ui->label_100->setGraphicsEffect(e3);
            ui->label_100->show();
            ui->label_100->setGeometry(0,0,1366,768);
            save = new Save(this);
            connect(save,SIGNAL(sendDeSingle(bool)),this,SLOT(receiveDesignle(bool)));
            save->show();
            break;
        }

    }
    if(i == 20)
    {
        for(int j = 0;j< 20;j++)
        {
            luoxuanlist2[whichoption-1][j] = luoxuanlist[whichoption-1][j];
            proxuanlist2[whichoption-1][j] = proxuanlist[whichoption-1][j];
            lsnumersxuanlist2[whichoption-1][j] = lsnumersxuanlist[whichoption-1][j];
            bxuannamelist2[whichoption-1][j] = bxuannamelist[whichoption-1][j];
            bxuancodelist2[whichoption-1][j] = bxuancodelist[whichoption-1][j];
            kxuannamelist2[whichoption-1][j] = kxuannamelist[whichoption-1][j];
            kxuancodelist2[whichoption-1][j] = kxuancodelist[whichoption-1][j];

        }

        pdmxuanlist2[whichoption-1]=  pdmxuanlist[whichoption-1];
        ui->pushButton_12->hide();
        ui->stackedWidget->setCurrentIndex(1);
        ui->stackedWidget_2->setCurrentIndex(4);
        ui->pushButton_15->show();
    }

}

void Newconfiginfo::on_pushButton_23_clicked()
{
    //选配删除
    SaveWhat = "OPDELONE";
    e3 = new QGraphicsOpacityEffect(this);
    e3->setOpacity(0.5);
    ui->label_100->setGraphicsEffect(e3);
    ui->label_100->show();
    ui->label_100->setGeometry(0,0,1366,768);

    save = new Save(this);
    connect(save,SIGNAL(sendDeSingle(bool)),this,SLOT(receiveDesignle(bool)));
    save->show();
    // saved = false;

}

void Newconfiginfo::on_pushButton_25_clicked()
{
    //选配保存
    if(SaveWhat == "save2" || SaveWhat == "")
    {
        SaveWhat = "save2";
        e3 = new QGraphicsOpacityEffect(this);
        e3->setOpacity(0.5);
        ui->label_100->setGraphicsEffect(e3);
        ui->label_100->show();
        ui->label_100->setGeometry(0,0,1366,768);
        save = new Save(this);
        connect(save,SIGNAL(sendDeSingle(bool)),this,SLOT(receiveDesignle(bool)));
        save->show();
    }
    else if(SaveWhat == "saveself")
    {
        //qDebug() << "jiushiainininini";
        isoptionsaved = false;
        carStyle = ui->lineEdit_carname->text();
        G9tmp = ui->lineEdit_G9->text();
        Vintmp = ui->lineEdit_VIN->text();
        luoxuanlist2[whichoption-1][ui->label_51->text().toInt()-1] = ui->lineEdit_xuanLsnum->text();
        proxuanlist2[whichoption-1][ui->label_51->text().toInt()-1] = ui->lineEdit_xuanpronum->text();
        lsnumersxuanlist2[whichoption-1][ui->label_51->text().toInt()-1] = ui->lineEdit_xuannumber->text();
        pdmxuanlist2[whichoption-1] = ui->comboBox_3->currentText();
        for(int j = 0;j< 20;j++)
        {
            luoxuanlist[whichoption-1][j] = luoxuanlist2[whichoption-1][j];
            proxuanlist[whichoption-1][j] = proxuanlist2[whichoption-1][j];
            lsnumersxuanlist[whichoption-1][j] = lsnumersxuanlist2[whichoption-1][j];
            bxuannamelist[whichoption-1][j] = bxuannamelist2[whichoption-1][j];
            bxuancodelist[whichoption-1][j] = bxuancodelist2[whichoption-1][j];
            kxuannamelist[whichoption-1][j] = kxuannamelist2[whichoption-1][j];
            kxuancodelist[whichoption-1][j] = kxuancodelist2[whichoption-1][j];

        }
        pdmxuanlist[whichoption-1] = pdmxuanlist2[whichoption-1];
        //qDebug() << luoxuanlist[whichoption-1][ui->label_51->text().toInt()-1];
        //qDebug() <<  proxuanlist[whichoption-1][ui->label_51->text().toInt()-1];
        //qDebug() <<  lsnumersxuanlist[whichoption-1][ui->label_51->text().toInt()-1];
        //qDebug() << whichoption << ui->label_51->text().toInt();
        ui->stackedWidget->setCurrentIndex(1);
        for(int i = 1;i< 6;i++)
        {
            for(int k = 1;k < 21;k++)
            { //判断有几个选配
                if(lsnumersxuanlist[i-1][k-1].toInt() != 0)
                {

                    if(i == 1)
                    {
                        ui->pushButton_xuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                        ui->pushButton_70->show();
                        ui->pushButton_71->show();
                        ui->pushButton_70->setText(proxuanlist[i-1][0]);
                        ui->pushButton_71->setText(lsnumersxuanlist[i-1][0]);
                        break;
                    }
                    else if(i == 2)
                    {
                        ui->pushButton_xuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                        //qDebug() << proxuanlist[i-1][0] << lsnumersxuanlist[i-1][0] << "DFFD";
                        ui->pushButton_72->show();
                        ui->pushButton_73->show();
                        ui->pushButton_72->setText(proxuanlist[i-1][0]);
                        ui->pushButton_73->setText(lsnumersxuanlist[i-1][0]);
                        break;
                    }
                    else if(i == 3)
                    {
                        ui->pushButton_xuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                        ui->pushButton_74->show();
                        ui->pushButton_75->show();
                        ui->pushButton_74->setText(proxuanlist[i-1][0]);
                        ui->pushButton_75->setText(lsnumersxuanlist[i-1][0]);
                        break;
                    } else if(i == 4)
                    {
                        ui->pushButton_xuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                        ui->pushButton_76->setText(proxuanlist[i-1][0]);
                        ui->pushButton_77->setText(lsnumersxuanlist[i-1][0]);
                        break;
                    }
                    else if(i == 5)
                    {
                        ui->pushButton_xuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuan.bmp);");
                        ui->pushButton_78->setText(proxuanlist[i-1][0]);
                        ui->pushButton_79->setText(lsnumersxuanlist[i-1][0]);
                        break;
                    }
                }
            }
        }
        ui->pushButton_14->show();
        ui->pushButton_15->show();
        ui->pushButton_16->show();
        SaveWhat = "";
    }

}


void Newconfiginfo::on_pushButton_52_clicked()
{
    //删除单个
    SaveWhat = "de";
    ui->lineEdit_Lsnumber->setText("");
    ui->lineEdit_pronum->setText("0");
    ui->lineEdit_number->setText("0");
    // ui->comboBox_2->setCurrentIndex(0);
    currentpages = ui->label_119->text().toInt();
    luo[currentpages-1] = "0";
    pro[currentpages-1] = "0";
    lsnumers[currentpages-1] = "0";
    // pdmxuan = "";

}

void Newconfiginfo::on_pushButton_17_clicked()
{
    //删除 单个选配
    ui->lineEdit_xuanLsnum->clear();
    ui->lineEdit_xuanpronum->setText("0");
    ui->lineEdit_xuannumber->setText("0");

    int indextmp = ui->label_51->text().toInt() -1;
    luoxuanlist2[whichoption-1][indextmp] = "0";
    proxuanlist2[whichoption-1][indextmp]= "0";
    lsnumersxuanlist2[whichoption-1][indextmp] = "";
    // ui->comboBox_3->setCurrentIndex(0);

}




void Newconfiginfo::on_pushButton_62_clicked()
{
    //高级选项
    whichButtonClick = "advanced";
    if(!isbaseinfochange)
        baseInfoIsChange();
    else
    {
        ui->stackedWidget_2->setCurrentIndex(6);
        ui->stackedWidget_3->setCurrentIndex(2);
        ui->label_87->show();
        ui->label_88->hide();
        ui->label_101->hide();
        ui->label_98->hide();
        ui->label_99->hide();
        ui->label_162->hide();
        QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
        ui->lineEdit_netmask->setText(configIniRead->value("baseinfo/netmask").toString());
        ui->lineEdit_gateway->setText(configIniRead->value("baseinfo/gateway").toString());
        ui->lineEdit_ssid->setText(configIniRead->value("baseinfo/SSID").toString());
        ui->lineEdit_psk->setText(configIniRead->value("baseinfo/psk").toString());
        ui->lineEdit_APssid->setText(configIniRead->value("baseinfo/APSSID").toString());
        ui->lineEdit_APpsk->setText(configIniRead->value("baseinfo/APpsk").toString());
        ui->lineEdit_tacktime->setText(configIniRead->value("baseinfo/TackTime").toString());   //节拍时间


        if(configIniRead->value("baseinfo/Master_slave").toString()=="master")
        {
            ui->radioButton_1->setChecked(true);
            ui->lineEdit_slave_1->setText(configIniRead->value("baseinfo/slave1").toString());
            if(!ui->lineEdit_slave_1->text().isEmpty())
                ui->checkBox_1->setChecked(true);
            else
            {
                ui->lineEdit_slave_1->setText("");
                ui->checkBox_1->setChecked(false);
            }
            ui->lineEdit_slave_2->setText(configIniRead->value("baseinfo/slave2").toString());
            if(!ui->lineEdit_slave_2->text().isEmpty())
                ui->checkBox_2->setChecked(true);
            else
            {
                ui->lineEdit_slave_2->setText("");
                ui->checkBox_2->setChecked(false);
            }
            ui->lineEdit_slave_3->setText(configIniRead->value("baseinfo/slave3").toString());
            if(!ui->lineEdit_slave_3->text().isEmpty())
                ui->checkBox_3->setChecked(true);
            else
            {
                ui->lineEdit_slave_3->setText("");
                ui->checkBox_3->setChecked(false);
            }
        }
        else if(configIniRead->value("baseinfo/Master_slave").toString()=="slave")
        {
            ui->radioButton_2->setChecked(true);
            ui->checkBox_1->hide();
            ui->checkBox_2->hide();
            ui->checkBox_3->hide();
            ui->lineEdit_slave_1->hide();
            ui->lineEdit_slave_2->hide();
            ui->lineEdit_slave_3->hide();
        }
        else
            ui->radioButton_1->setChecked(true);

        if(isRFID == 0)
        {
            ui->pushButton_26->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/52.bmp);");
            ui->pushButton_27->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/RFID_small.bmp);");
            ui->pushButton_queue->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/queue.bmp);");
        }
        else if(isRFID == 1)
        {
            ui->pushButton_26->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/barcode.bmp);");
            ui->pushButton_27->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/53.bmp);");
            ui->pushButton_queue->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/queue.bmp);");
        }
        else if(isRFID == 2)
        {
            ui->pushButton_26->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/barcode.bmp);");
            ui->pushButton_27->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/RFID_small.bmp);");
            ui->pushButton_queue->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/queue1.bmp);");
        }

        if (Line_ID == 1)
        {
            ui->Line_radioButton_1->setChecked(true);
            ui->Line_radioButton_2->setChecked(false);
        }
        else if (Line_ID == 2)
        {
            ui->Line_radioButton_1->setChecked(false);
            ui->Line_radioButton_2->setChecked(true);
        }
        delete configIniRead;
        ShowTime();
    }
}

void Newconfiginfo::on_pushButton_26_clicked()
{
    //条码枪
    ui->pushButton_26->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/52.bmp);");
    ui->pushButton_27->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/RFID_small.bmp);");
    ui->pushButton_queue->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/queue.bmp);");
    isrfid = 0;
}

void Newconfiginfo::on_pushButton_27_clicked()
{
    //RFID
    ui->pushButton_26->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/barcode.bmp);");
    ui->pushButton_27->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/53.bmp);");
    ui->pushButton_queue->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/queue.bmp);");
    isrfid = 1;
}

void Newconfiginfo::on_pushButton_queue_clicked()
{
    //队列
    ui->pushButton_26->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/barcode.bmp);");
    ui->pushButton_27->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/RFID_small.bmp);");
    ui->pushButton_queue->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/queue1.bmp);");
    isrfid = 2;
}

void Newconfiginfo::receiveSaveState(bool statetmp)
{
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    if(statetmp)
    {
        ui->label_100->hide();
        delete e3;
        delete save;
        configIniRead->setValue("baseinfo/isRFID",QString::number(isrfid));
        isRFID = isrfid;
        if(ui->Line_radioButton_1->isChecked())
        {
           configIniRead->setValue("baseinfo/Line_ID","1");
           Line_ID = 1;
           line_ID = 1;
        }
        else if(ui->Line_radioButton_2->isChecked())
        {
           configIniRead->setValue("baseinfo/Line_ID","2");
           Line_ID = 2;
           line_ID = 2;
        }
        configIniRead->setValue("baseinfo/gateway",ui->lineEdit_gateway->text());
        configIniRead->setValue("baseinfo/TackTime",ui->lineEdit_tacktime->text());    //节拍时间

        if (ui->year->text().toInt() != year||
                ui->month->text().toInt() != month||
                ui->date->text().toInt() != date||
                ui->hour->text().toInt() != hour||
                ui->minute->text().toInt() != minute||
                ui->second->text().toInt() != second)
        {
            QString str = "date -s \"" +ui->year->text()+"-"+ui->month->text() + "-" + ui->date->text()+ " "+ui->hour->text() + ":" + ui->minute->text() + ":" + ui->second->text()+"\" &";

            //qDebug()<<"str"<<str;
            system(str.toLatin1().data());

            //将系统时间写入RTC
            system("hwclock -w &");
            year=ui->year->text().toInt();
            month=ui->month->text().toInt();
            date=ui->date->text().toInt();
            hour=ui->hour->text().toInt();
            minute=ui->minute->text().toInt();
            second=ui->second->text().toInt();

        }

        if (ui->lineEdit_gateway->text() != configIniRead->value("baseinfo/netmask"))
        {
            configIniRead->setValue("baseinfo/netmask",ui->lineEdit_netmask->text());
            system((QString("ifconfig wlan0 netmask ")+ui->lineEdit_netmask->text()+" &").toLatin1().data());
            QString fileName = "/etc/profile";
            QFile file(fileName);
            if(!file.open(QIODevice::ReadOnly| QIODevice::Text)){
                qDebug()   << "Cannot open profile file for Reading";
            }
            QString str (file.readAll());
            if(str.contains("netmask", Qt::CaseInsensitive)){
                str.replace(QRegExp("netmask \\S*"),QString("netmask ")+ui->lineEdit_netmask->text());
            }
            file.close();
            if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
                qDebug()   << "Cannot open profile file for Writing";
            }
            file.write(str.toUtf8());
            file.close();
        }


        if (ui->lineEdit_ssid->text() != configIniRead->value("baseinfo/SSID")||
                ui->lineEdit_psk->text() != configIniRead->value("baseinfo/psk"))
        {
            configIniRead->setValue("baseinfo/SSID",ui->lineEdit_ssid->text());
            configIniRead->setValue("baseinfo/psk",ui->lineEdit_psk->text());
            wifi_connect();
        }
        if (ui->lineEdit_APssid->text() != configIniRead->value("baseinfo/APSSID")||
                ui->lineEdit_APpsk->text() != configIniRead->value("baseinfo/APpsk"))
        {
            configIniRead->setValue("baseinfo/APSSID",ui->lineEdit_APssid->text());
            configIniRead->setValue("baseinfo/APpsk",ui->lineEdit_APpsk->text());
            QString fileName;
            fileName = "/etc/Wireless/RT2870AP/RT2870AP.dat";
            QFile file(fileName);
            if(!file.open(QIODevice::ReadOnly| QIODevice::Text)){
                qDebug()   << "Cannot open AP file for Reading";
            }
            QString str (file.readAll());
            if(str.contains("\nSSID=", Qt::CaseInsensitive)&&str.contains("WPAPSK=", Qt::CaseInsensitive)){
                str.replace(QRegExp("\nSSID=\\w*"),QString("\nSSID=")+ui->lineEdit_APssid->text());
                str.replace(QRegExp("WPAPSK=\\w*"),QString("WPAPSK=")+ui->lineEdit_APpsk->text());
                qDebug()<<"AP changed";
            }
            file.close();
            QFile::remove("/etc/Wireless/RT2870AP/RT2870AP.dat");
            if(!file.open(QIODevice::WriteOnly| QIODevice::Text)){
                qDebug()   << "Cannot open AP file for Writing";
            }
            file.write(str.toUtf8());
            file.close();
            system("ifconfig ra0 down &");
            system("ifconfig ra0 up &");

        }
        delete configIniRead;
    }else
    {
        ui->lineEdit_netmask->setText(configIniRead->value("baseinfo/netmask").toString());
        ui->lineEdit_gateway->setText(configIniRead->value("baseinfo/gateway").toString());
        ui->lineEdit_ssid->setText(configIniRead->value("baseinfo/SSID").toString());
        ui->lineEdit_psk->setText(configIniRead->value("baseinfo/psk").toString());
        ui->lineEdit_APssid->setText(configIniRead->value("baseinfo/APSSID").toString());
        ui->lineEdit_APpsk->setText(configIniRead->value("baseinfo/APpsk").toString());
        ui->lineEdit_tacktime->setText(configIniRead->value("baseinfo/TackTime").toString());   //节拍时间
        delete configIniRead;
        isrfid = isRFID;
        if(Line_ID == 1)
        {
            line_ID = 1;
            ui->Line_radioButton_1->setChecked(true);
            ui->Line_radioButton_2->setChecked(false);
        }
        else if(Line_ID == 2)
        {
            line_ID = 2;
            ui->Line_radioButton_1->setChecked(false);
            ui->Line_radioButton_2->setChecked(true);
        }
        if (isRFID == 1)
        {
            ui->pushButton_27->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/53.bmp);");
            ui->pushButton_26->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/barcode.bmp);");
            ui->pushButton_queue->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/queue.bmp);");
        }
        else if(isRFID == 0)
        {
            ui->pushButton_26->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/52.bmp);");
            ui->pushButton_27->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/RFID_small.bmp);");
            ui->pushButton_queue->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/queue.bmp);");
        }
        else if(isRFID == 2)
        {
            ui->pushButton_27->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/RFID_small.bmp);");
            ui->pushButton_26->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/barcode.bmp);");
            ui->pushButton_queue->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/queue1.bmp);");
        }
        ui->label_100->hide();
        delete e3;
        delete save;
    }
    if(whichButtonClick == "advancedback")
    {
        on_pushButton_3_clicked();
        isadvancedchange = false;
    }
    else if(whichButtonClick == "passwordchange")
    {
        on_pushButton_100_clicked();
        isadvancedchange = false;
    }
    else if(whichButtonClick == "testinterface")
    {
        on_pushButton_66_clicked();
        isadvancedchange = false;
    }
    else if(whichButtonClick == "fisupdatecolumn")
    {
        on_pushButton_67_clicked();
        isadvancedchange = false;
    }
    else if(whichButtonClick == "master_slave")
    {
        on_pushButton_86_clicked();
        isadvancedchange = false;
    }
    else if(whichButtonClick == "bound_config")
    {
        on_pushButton_55_clicked();
        isadvancedchange = false;
    }
    else if(whichButtonClick == "saveadvanced")
    {
        isadvancedchange = false;
    }
}


void Newconfiginfo::on_pushButton_34_clicked()
{
    //保存
    whichButtonClick = "saveadvanced";
    e3 = new QGraphicsOpacityEffect(this);
    e3->setOpacity(0.5);
    ui->label_100->setGraphicsEffect(e3);
    ui->label_100->show();
    ui->label_100->setGeometry(0,0,1366,768);

    SaveWhat = "advanced";
    save = new Save(this);
    connect(save,SIGNAL(sendSaveAdvancedState(bool)),this,SLOT(receiveSaveState(bool)));
    save->show();
}


void Newconfiginfo::on_pushButton_13_clicked()
{
    if(SaveWhat == "pdm")
    {
        ui->label_69->show();
        ui->comboBox->show();
        e3 = new QGraphicsOpacityEffect(this);
        e3->setOpacity(0.5);
        ui->label_100->setGraphicsEffect(e3);
        ui->label_100->show();
        ui->label_100->setGeometry(0,0,1366,768);
        SaveWhat = "pdmoutabout";
        save = new Save(this);
        connect(save,SIGNAL(sendDeSingle(bool)),this,SLOT(receiveDesignle(bool)));
        save->show();
    }
    else
    {
        int tempdata = numpdm;
        for(int i = 0;i<tempdata;i++)
        {
            delete butt1[i];
            numpdm--;
        }
        ui->listWidget->setCurrentRow(1);
        historyclear();

        if(isJS)
        {
            ui->stackedWidget_2->setCurrentIndex(8);
            ui->stackedWidget_3->setCurrentIndex(1);
            ui->label_157->hide();
            ui->pushButton_54->hide();
            QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
            ui->label_SN->setText(configIniRead->value("baseinfo/SN").toString());
            delete configIniRead;
        }
        else
        {
            whichButtonClick = "about";
            if(!isbaseinfochange)
                baseInfoIsChange();
            else
            {
                ui->stackedWidget_2->setCurrentIndex(8);
                ui->stackedWidget_3->setCurrentIndex(1);
                ui->label_157->hide();
                ui->pushButton_54->hide();
                QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
                ui->label_SN->setText(configIniRead->value("baseinfo/SN").toString());
                delete configIniRead;
            }
        }
    }
}

void Newconfiginfo::on_pushButton_2_clicked()
{
    if (whichButtonClick == "about")
    {
        if(isJS)
            ui->stackedWidget_2->setCurrentIndex(0);
        else
            ui->stackedWidget_2->setCurrentIndex(2);
        ui->listWidget->setCurrentRow(1);
        ui->stackedWidget_3->setCurrentIndex(0);
        ui->label_83->show();
        ui->label_84->hide();
        ui->label_85->hide();
        ui->label_86->hide();
        ui->label_157->show();
        ui->pushButton_54->show();
    }
    else
    {
        ui->stackedWidget_3->setCurrentIndex(0);
        ui->stackedWidget_5->setCurrentIndex(0);
        if(isJS)
        {
            ui->stackedWidget_2->setCurrentIndex(0);
            ui->label_83->show();
            ui->label_84->hide();
            ui->label_85->hide();
        }
        else
        {
            //ui->stackedWidget_2->setCurrentIndex(2);
            on_pushButton_14_clicked();
        }

    }
}

void Newconfiginfo::on_pushButton_3_clicked()
{
    whichButtonClick = "advancedback";
    if(!isadvancedchange)
        advancedIsChange();
    else if(!ismasterslavechange)
        masterslaveIsChange();
    else if(!isboundchange)
        boundIsChange();
    else
    {
        ui->lineEdit_column->setText("");
        ui->stackedWidget_2->setCurrentIndex(2);
        ui->stackedWidget_3->setCurrentIndex(0);
        ui->label_83->show();
        ui->label_84->hide();
        ui->label_85->hide();
        ui->label_86->hide();
        if(isRFID == 1)   //RFID 条码抢模式的 界面初始化
        {
            ui->stackedWidget_4->setCurrentIndex(0);
        }
        else
        {
            ui->stackedWidget_4->setCurrentIndex(1);
        }
        //        ui->pushButton_13->show();
    }
}



//! [时钟更改]

void Newconfiginfo::ShowTime()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QTime current_time = QTime::currentTime();
    year=dateTime.date().year();
    month=dateTime.date().month();
    date=dateTime.date().day();

    hour = current_time.hour();
    minute = current_time.minute();
    second = current_time.second();

    ui->year->setText(QString::number(year));
    ui->month->setText(QString::number(month));
    ui->date->setText(QString::number(date));
    ui->hour->setText(QString::number(hour));
    ui->minute->setText(QString::number(minute));
    ui->second->setText(QString::number(second));
}

void Newconfiginfo::on_pushButton_68_clicked()
{
    int new_year=(ui->year->text()).toInt()+1;
    ui->year->setText(QString::number(new_year));
}

void Newconfiginfo::on_pushButton_69_clicked()
{
    int new_year=(ui->year->text()).toInt()-1;
    ui->year->setText(QString::number(new_year));
}

void Newconfiginfo::on_pushButton_80_clicked()
{
    if(ui->month->text().toInt()<12)
    {
        int new_month=(ui->month->text()).toInt()+1;
        ui->month->setText(QString::number(new_month));
    }
    else
        ui->month->setText(QString::number(1));
}

void Newconfiginfo::on_pushButton_85_clicked()
{
    if(ui->month->text().toInt()>1)
    {
        int new_month=(ui->month->text()).toInt()-1;
        ui->month->setText(QString::number(new_month));
    }
    else
        ui->month->setText(QString::number(12));
}

void Newconfiginfo::on_pushButton_81_clicked()
{
    if(ui->date->text().toInt()<31)
    {
        int new_date=(ui->date->text()).toInt()+1;
        ui->date->setText(QString::number(new_date));
    }
    else
        ui->date->setText(QString::number(1));
}

void Newconfiginfo::on_pushButton_92_clicked()
{
    if(ui->date->text().toInt()>1)
    {
        int new_date=(ui->date->text()).toInt()-1;
        ui->date->setText(QString::number(new_date));
    }
    else
        ui->date->setText(QString::number(31));
}

void Newconfiginfo::on_pushButton_82_clicked()
{
    if(ui->hour->text().toInt()<23)
    {
        int new_hour=(ui->hour->text()).toInt()+1;
        ui->hour->setText(QString::number(new_hour));
    }
    else
        ui->hour->setText(QString::number(0));
}

void Newconfiginfo::on_pushButton_93_clicked()
{
    if(ui->hour->text().toInt()>0)
    {
        int new_hour=(ui->hour->text()).toInt()-1;
        ui->hour->setText(QString::number(new_hour));
    }
    else
        ui->hour->setText(QString::number(23));
}

void Newconfiginfo::on_pushButton_83_clicked()
{
    if(ui->minute->text().toInt()<59)
    {
        int new_minute=(ui->minute->text()).toInt()+1;
        ui->minute->setText(QString::number(new_minute));
    }
    else
        ui->minute->setText(QString::number(0));
}

void Newconfiginfo::on_pushButton_94_clicked()
{
    if(ui->minute->text().toInt()>0)
    {
        int new_minute=(ui->minute->text()).toInt()-1;
        ui->minute->setText(QString::number(new_minute));
    }
    else
        ui->minute->setText(QString::number(59));
}

void Newconfiginfo::on_pushButton_84_clicked()
{
    if(ui->second->text().toInt()<59)
    {
        int new_second=(ui->second->text()).toInt()+1;
        ui->second->setText(QString::number(new_second));
    }
    else
        ui->second->setText(QString::number(0));
}

void Newconfiginfo::on_pushButton_95_clicked()
{
    if(ui->second->text().toInt()>0)
    {
        int new_second=(ui->second->text()).toInt()-1;
        ui->second->setText(QString::number(new_second));
    }
    else
        ui->second->setText(QString::number(59));
}

//! [时钟更改]

void Newconfiginfo::on_pushButton_58_clicked()
{
    if(ui->stackedWidget_2->currentIndex()!=6)
    {
        whichButtonClick = "advanceset";
        if(!ismasterslavechange)
            masterslaveIsChange();
        else if(!isboundchange)
            boundIsChange();
        else
        {
            ui->lineEdit_column->setText("");
            ShowTime();
            ui->stackedWidget_2->setCurrentIndex(6);
            ui->label_87->show();
            ui->label_88->hide();
            ui->label_101->hide();
            ui->label_98->hide();
            ui->label_99->hide();
            ui->label_162->hide();
            //            if( system("ping -c 3 10.0.0.1 -i 0.1")!= 0)
            //            {
            //                qDebug()<<"网络连接失败";
            //                //ui->label_wifi->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/unconnect.PNG);");
            //            }
            //            else
            //            {
            //                qDebug()<<"网络连接成功";
            //                //ui->label_wifi->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/connect.PNG);");
            //            }
        }
    }
}

void Newconfiginfo::on_pushButton_66_clicked()
{
    DebugMode = true;
    if(ui->stackedWidget_2->currentIndex() != 7)
    {
        whichButtonClick = "testinterface";
        if(!isadvancedchange)
            advancedIsChange();
        else if(!ismasterslavechange)
            masterslaveIsChange();
        else if(!isboundchange)
            boundIsChange();
        else
        {
            ui->lineEdit_column->setText("");
            ui->stackedWidget_2->setCurrentIndex(7);

            ui->label_87->hide();
            ui->label_101->hide();
            ui->label_88->show();
            ui->label_98->hide();
            ui->label_99->hide();
            ui->label_162->hide();
            //        ui->stackedWidget_3->setCurrentIndex(3);
            //        ui->pushButton_96->setText(tr("测试界面"));
        }
    }
}

//! [密码更改]

void Newconfiginfo::on_pushButton_100_clicked()
{
    if(ui->stackedWidget_2->currentIndex() != 9)
    {
        whichButtonClick = "passwordchange";
        if(!isadvancedchange)
            advancedIsChange();
        else if(!ismasterslavechange)
            masterslaveIsChange();
        else if(!isboundchange)
            boundIsChange();
        else
        {
            ui->lineEdit_column->setText("");
            ui->stackedWidget_2->setCurrentIndex(9);
            ui->label_87->hide();
            ui->label_101->show();
            ui->label_88->hide();
            ui->label_98->hide();
            ui->label_99->hide();
            ui->label_162->hide();
            //        ui->stackedWidget_3->setCurrentIndex(3);
            //        ui->pushButton_96->setText(tr("更改密码"));
            isFull = 0;
            person = 0;
            change = 0;
            temp = "";
            newpassword = "";
            ui->pushButton_password->setStyleSheet("border-image : url(:/re/94.bmp)");
            ui->pushButton_error->setStyleSheet("border-image : url(:)");
            ui->label_hint->setStyleSheet("border-image : url(:/re/111.bmp)");
            ui->pushButton_98->setStyleSheet("border-image : url(:/re/93.bmp)");
            ui->pushButton_97->setStyleSheet("border-image : url(:/re/100.png)");
        }
    }
}


void Newconfiginfo::on_pushButton_num1_clicked()
{
    temp.append("1");
    isFull++;
    judge();
}

void Newconfiginfo::on_pushButton_num2_clicked()
{
    temp.append("2");
    isFull++;
    judge();
}

void Newconfiginfo::on_pushButton_num3_clicked()
{
    temp.append("3");
    isFull++;
    judge();
}

void Newconfiginfo::on_pushButton_num4_clicked()
{
    temp.append("4");
    isFull++;
    judge();
}

void Newconfiginfo::on_pushButton_num5_clicked()
{
    temp.append("5");
    isFull++;
    judge();
}

void Newconfiginfo::on_pushButton_num6_clicked()
{
    temp.append("6");
    isFull++;
    judge();
}

void Newconfiginfo::on_pushButton_num7_clicked()
{
    temp.append("7");
    isFull++;
    judge();
}

void Newconfiginfo::on_pushButton_num8_clicked()
{
    temp.append("8");
    isFull++;
    judge();
}

void Newconfiginfo::on_pushButton_num9_clicked()
{
    temp.append("9");
    isFull++;
    judge();
}

void Newconfiginfo::on_pushButton_num0_clicked()
{
    temp.append("0");
    isFull++;
    judge();
}


void Newconfiginfo::on_pushButton_delete_clicked()
{
    if(isFull == 1)
    {
        ui->pushButton_password->setStyleSheet("border-image : url(:/re/94.bmp)");
        isFull--;
        temp.resize(temp.size() - 1);
    }
    else if(isFull == 2)
    {
        ui->pushButton_password->setStyleSheet("border-image : url(:/re/95.bmp)");
        isFull--;
        temp.resize(temp.size() - 1);
    }
    else if(isFull == 3)
    {
        ui->pushButton_password->setStyleSheet("border-image : url(:/re/96.bmp)");
        isFull--;
        temp.resize(temp.size() - 1);
    }
}

void Newconfiginfo::judge()
{
    if(isFull == 1)
    {
        ui->pushButton_password->setStyleSheet("border-image : url(:/re/95.bmp)");
        ui->pushButton_error->setStyleSheet("border-image: url(:)");
    }
    else if(isFull == 2)
        ui->pushButton_password->setStyleSheet("border-image : url(:/re/96.bmp)");
    else if(isFull == 3)
        ui->pushButton_password->setStyleSheet("border-image : url(:/re/97.bmp)");
    else if(isFull == 4)
    {
        QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
        if (person ==0)
        {
            if (change == 0)
            {
                if(temp == configIniRead->value(QString("baseinfo/GCpassword")).toString())
                {
                    ui->label_hint->setStyleSheet("border-image : url(:/re/112.bmp)");
                    ui->pushButton_password->setStyleSheet("border-image : url(:/re/94.bmp)");
                    ui->pushButton_error->setStyleSheet("border-image : url(:)");
                    change = 1;
                    isFull = 0;
                    temp = "";
                }
                else
                {
                    ui->pushButton_error->setStyleSheet("border-image : url(:/re/124.bmp)");
                    ui->pushButton_password->setStyleSheet("border-image : url(:/re/94.bmp)");
                    isFull = 0;
                    temp = "";
                }
            }
            else if (change == 1)
            {
                newpassword=temp;
                isFull = 0;
                temp = "";
                ui->label_hint->setStyleSheet("border-image : url(:/re/113.bmp)");
                ui->pushButton_password->setStyleSheet("border-image : url(:/re/94.bmp)");
                ui->pushButton_error->setStyleSheet("border-image : url(:)");
                change = 2;
            }
            else if (change == 2)
            {
                if(temp == newpassword)
                {
                    ui->pushButton_password->setStyleSheet("border-image : url(:/re/98.bmp)");
                    configIniRead->setValue("baseinfo/GCpassword",newpassword);
                    ui->stackedWidget_2->setCurrentIndex(6);
                    ui->stackedWidget_3->setCurrentIndex(2);
                }
                else
                {
                    ui->label_hint->setStyleSheet("border-image : url(:/re/113.bmp)");
                    ui->pushButton_password->setStyleSheet("border-image : url(:/re/94.bmp)");
                    ui->pushButton_error->setStyleSheet("border-image : url(:/re/125.bmp)");
                    isFull = 0;
                    temp = "";
                }
            }
        }
        else if (person ==1)
        {
            if (change == 0)
            {
                if(temp == configIniRead->value(QString("baseinfo/JSpassword")).toString())
                {
                    ui->label_hint->setStyleSheet("border-image : url(:/re/112.bmp)");
                    ui->pushButton_password->setStyleSheet("border-image : url(:/re/94.bmp)");
                    ui->pushButton_error->setStyleSheet("border-image : url(:)");
                    change = 1;
                    isFull = 0;
                    temp = "";
                }
                else
                {
                    ui->pushButton_error->setStyleSheet("border-image : url(:/re/124.bmp)");
                    ui->pushButton_password->setStyleSheet("border-image : url(:/re/94.bmp)");
                    isFull = 0;
                    temp = "";
                }
            }
            else if (change == 1)
            {
                newpassword=temp;
                isFull = 0;
                temp = "";
                ui->label_hint->setStyleSheet("border-image : url(:/re/113.bmp)");
                ui->pushButton_password->setStyleSheet("border-image : url(:/re/94.bmp)");
                ui->pushButton_error->setStyleSheet("border-image : url(:)");
                change = 2;
            }
            else if (change == 2)
            {
                if(temp == newpassword)
                {
                    ui->pushButton_password->setStyleSheet("border-image : url(:/re/98.bmp)");
                    configIniRead->setValue("baseinfo/JSpassword",newpassword);
                    ui->stackedWidget_2->setCurrentIndex(6);
                    ui->stackedWidget_3->setCurrentIndex(2);
                }
                else
                {
                    ui->label_hint->setStyleSheet("border-image : url(:/re/113.bmp)");
                    ui->pushButton_password->setStyleSheet("border-image : url(:/re/94.bmp)");
                    ui->pushButton_error->setStyleSheet("border-image : url(:/re/125.bmp)");
                    isFull = 0;
                    temp = "";
                }
            }
        }
        delete configIniRead;
    }
}


void Newconfiginfo::on_pushButton_98_clicked()
{
    if(person != 0)
    {
        ui->pushButton_98->setStyleSheet("border-image : url(:/re/93.bmp)");
        ui->pushButton_97->setStyleSheet("border-image: url(:/re/100.png)");
        ui->label_hint->setStyleSheet("border-image : url(:/re/111.bmp)");
        ui->pushButton_error->setStyleSheet("border-image : url(:)");
        ui->pushButton_password->setStyleSheet("border-image : url(:/re/94.bmp)");
        isFull = 0;
        temp = "";
        person = 0;
        change = 0;
        newpassword = "";
    }
}

void Newconfiginfo::on_pushButton_97_clicked()
{
    if(person != 1)
    {
        ui->pushButton_97->setStyleSheet("border-image : url(:/re/92.bmp)");
        ui->pushButton_98->setStyleSheet("border-image: url(:/re/99.png)");
        ui->label_hint->setStyleSheet("border-image : url(:/re/111.bmp)");
        ui->pushButton_error->setStyleSheet("border-image : url(:)");
        ui->pushButton_password->setStyleSheet("border-image : url(:/re/94.bmp)");
        isFull = 0;
        temp = "";
        person = 1;
        change = 0;
        newpassword = "";
    }
}
//! [密码更改]


void Newconfiginfo::receiveoptioninfo(QString a,QString b,bool c)
{
    if(c)
    {
        if(bxuanstate)
        {
            // 赋值
            bxuannamelist2[whichoption-1][bxuanwhich-1] = a;
            bxuancodelist2[whichoption-1][bxuanwhich-1] = b;
        }else
        {
            kxuannamelist2[whichoption-1][bxuanwhich-1] = a;
            kxuancodelist2[whichoption-1][bxuanwhich-1] = b;
        }

        if(bxuanwhich == 1)
        {
            if(bxuanstate)
            {
                ui->pushButton_bxuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");
                ui->label_bx1name->setText(a);
                ui->label_bx1code->setText(b);
            }else
            {
                ui->pushButton_kxuan1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");
                ui->label_kx1name->setText(a);
                ui->label_kx1code->setText(b);
            }
        }else if(bxuanwhich == 2)
        {
            if(bxuanstate)
            {
                ui->pushButton_bxuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");
                ui->label_bx2name->setText(a);
                ui->label_bx2code->setText(b);
            }
            else
            {
                ui->pushButton_kxuan2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");
                ui->label_kx2name->setText(a);
                ui->label_kx2code->setText(b);
            }
        }else if(bxuanwhich == 3)
        {
            if(bxuanstate)
            {
                ui->pushButton_bxuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");
                ui->label_bx3name->setText(a);
                ui->label_bx3code->setText(b);
            }
            else
            {
                ui->pushButton_kxuan3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");
                ui->label_kx3name->setText(a);
                ui->label_kx3code->setText(b);
            }
        }
        else if(bxuanwhich == 4)
        {
            if(bxuanstate)
            {
                ui->pushButton_bxuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");
                ui->label_bx4name->setText(a);
                ui->label_bx4code->setText(b);
            }
            else
            {
                ui->pushButton_kxuan4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");
                ui->label_kx4name->setText(a);
                ui->label_kx4code->setText(b);
            }
        }
        else if(bxuanwhich == 5)
        {
            if(bxuanstate)
            {
                ui->pushButton_bxuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");
                ui->label_bx5name->setText(a);
                ui->label_bx5code->setText(b);
            }else
            {
                ui->pushButton_kxuan5->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/xuanpei.bmp);");
                ui->label_kx5name->setText(a);
                ui->label_kx5code->setText(b);
            }
        }
    }
    delete od;
    ui->label_100->hide();
    delete e3;



}
void Newconfiginfo::on_pushButton_bxuan1_clicked()
{
    //选配代码 弹框
    bxuanwhich = 1;
    bxuanstate = true;
    e3 = new QGraphicsOpacityEffect(this);
    e3->setOpacity(0.5);
    ui->label_100->setGraphicsEffect(e3);
    ui->label_100->show();
    ui->label_100->setGeometry(0,0,1366,768);
    od = new OptionDialog(this);


    QObject::connect(od,SIGNAL(sendoptioninfo(QString,QString,bool)),this,SLOT(receiveoptioninfo(QString,QString,bool)),Qt::AutoConnection);
    //this->setEnabled(false);
    od->show();
    od->setOptext(ui->label_bx1name->text(),ui->label_bx1code->text());

}

void Newconfiginfo::on_pushButton_bxuan2_clicked()
{

    on_pushButton_bxuan1_clicked();
    bxuanwhich = 2;
    bxuanstate = true;
    od->setOptext(ui->label_bx2name->text(),ui->label_bx2code->text());
}

void Newconfiginfo::on_pushButton_bxuan3_clicked()
{
    on_pushButton_bxuan1_clicked();
    bxuanwhich = 3;
    bxuanstate = true;
    od->setOptext(ui->label_bx3name->text(),ui->label_bx3code->text());
}

void Newconfiginfo::on_pushButton_bxuan4_clicked()
{

    on_pushButton_bxuan1_clicked();
    bxuanwhich = 4;
    bxuanstate = true;
    od->setOptext(ui->label_bx4name->text(),ui->label_bx4code->text());
}

void Newconfiginfo::on_pushButton_bxuan5_clicked()
{

    on_pushButton_bxuan1_clicked();
    bxuanwhich = 5;
    bxuanstate = true;
    od->setOptext(ui->label_bx5name->text(),ui->label_bx5code->text());
}

void Newconfiginfo::on_pushButton_kxuan1_clicked()
{
    on_pushButton_bxuan1_clicked();
    bxuanwhich =1 ;
    bxuanstate = false;
    od->setOptext(ui->label_kx1name->text(),ui->label_kx1code->text());
}

void Newconfiginfo::on_pushButton_kxuan2_clicked()
{
    on_pushButton_bxuan1_clicked();
    bxuanwhich =2 ;
    bxuanstate = false;
    od->setOptext(ui->label_kx2name->text(),ui->label_kx2code->text());
}

void Newconfiginfo::on_pushButton_kxuan3_clicked()
{
    on_pushButton_bxuan1_clicked();
    bxuanwhich =3 ;
    bxuanstate = false;
    od->setOptext(ui->label_kx3name->text(),ui->label_kx3code->text());
}

void Newconfiginfo::on_pushButton_kxuan4_clicked()
{
    on_pushButton_bxuan1_clicked();
    bxuanwhich =4 ;
    bxuanstate = false;
    od->setOptext(ui->label_kx4name->text(),ui->label_kx4code->text());
}

void Newconfiginfo::on_pushButton_kxuan5_clicked()
{
    on_pushButton_bxuan1_clicked();
    bxuanwhich =5;
    bxuanstate = false;
    od->setOptext(ui->label_kx5name->text(),ui->label_kx5code->text());
}

void Newconfiginfo::on_pushButton_89_clicked()
{
    //xuan look up
    updownReadOperate(1);

}

void Newconfiginfo::on_pushButton_90_clicked()
{
    //xuan look down
    updownReadOperate(0);

}


void Newconfiginfo::advancedIsChange()
{
    // 监听
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    if (   ui->lineEdit_netmask->text() != configIniRead->value("baseinfo/netmask")||
           ui->lineEdit_gateway->text() != configIniRead->value("baseinfo/gateway")||
           ui->lineEdit_ssid->text() != configIniRead->value("baseinfo/SSID")||
           ui->lineEdit_psk->text() != configIniRead->value("baseinfo/psk")||
           ui->lineEdit_APssid->text() != configIniRead->value("baseinfo/APSSID")||
           ui->lineEdit_APpsk->text() != configIniRead->value("baseinfo/APpsk")||
           ui->lineEdit_tacktime->text() != configIniRead->value("baseinfo/TackTime")||    //节拍时间
           isRFID != isrfid||
           ui->year->text().toInt() != year||
           ui->month->text().toInt() != month||
           ui->date->text().toInt() != date||
           ui->hour->text().toInt() != hour||
           ui->minute->text().toInt() != minute||
           ui->second->text().toInt() != second||
           line_ID != Line_ID)
    {
        qDebug()<<"??????????????????????????????????????????????????????";
        e3 = new QGraphicsOpacityEffect(this);
        e3->setOpacity(0.5);
        ui->label_100->setGraphicsEffect(e3);
        ui->label_100->show();
        ui->label_100->setGeometry(0,0,1366,768);
        delete configIniRead;
        SaveWhat = "advanced";
        save = new Save(this);
        connect(save,SIGNAL(sendSaveAdvancedState(bool)),this,SLOT(receiveSaveState(bool)));
        save->show();
        isadvancedchange = true;
    }
    else
    {
         delete configIniRead;
        isadvancedchange = true;
        if (whichButtonClick == "advancedback")
        {
            on_pushButton_3_clicked();
            isadvancedchange = false;
        }
        else if (whichButtonClick == "passwordchange")
        {
            on_pushButton_100_clicked();
            isadvancedchange = false;
        }
        else if (whichButtonClick == "testinterface")
        {
            on_pushButton_66_clicked();
            isadvancedchange = false;
        }
        else if (whichButtonClick == "fisupdatecolumn")
        {
            on_pushButton_67_clicked();
            isadvancedchange = false;
        }
        else if (whichButtonClick == "master_slave")
        {
            on_pushButton_86_clicked();
            isadvancedchange = false;
        }
        else if (whichButtonClick == "bound_config")
        {
            on_pushButton_55_clicked();
            isadvancedchange = false;
        }
    }

}


void Newconfiginfo::wifi_connect()
{
    //    system("wpa_supplicant -Dwext -wlan0 -c/etc/wpa_supplicant.conf -dd&");
    //    system("udhcpc -i wlan0");
    //    system("ifconfig ra0 192.168.1.252");

    QString wifiname=ui->lineEdit_ssid->text();
    QString password=ui->lineEdit_psk->text();

    QString fileName = "/etc/wpa_supplicant.conf";
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly| QIODevice::Text)){
        qDebug()   << "Cannot open wifi file for Reading";
    }
    QString str (file.readAll());
    if(str.contains("ssid=", Qt::CaseInsensitive)&&str.contains("psk=", Qt::CaseInsensitive)){
        //qDebug()<<"write success!";
        str.replace(QRegExp("\nssid=\\S*"),QString("\nssid=\"")+wifiname+QString("\""));
        str.replace(QRegExp("psk=\\S*"),QString("psk=\"")+password+QString("\""));
    }
    file.close();
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
        qDebug()   << "Cannot open wifi file for Writing";
    }
    file.write(str.toUtf8());
    file.close();

    system((QString("wpa_cli set_network 0 ssid '\"")+wifiname+QString("\"' &")).toLatin1().data());
    system((QString("wpa_cli set_network 0 psk  '\"")+password+QString("\"' &")).toLatin1().data());
    system("wpa_cli disable_network 0 &");
    system("wpa_cli enable_network 0 &");
    //    system("udhcpc -i wlan0");
    //    if( system("ping -c 3 10.0.0.1")!= 0)
    //    {
    //        qDebug()<<"网络连接失败";
    //       // ui->label_wifi->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/unconnect.PNG);");

    //        e3 = new QGraphicsOpacityEffect(this);
    //        e3->setOpacity(0.5);
    //        ui->label_100->setGraphicsEffect(e3);
    //        ui->label_100->show();
    //        ui->label_100->setGeometry(0,0,1366,768);
    //        SaveWhat = "wifiunconnect";
    //        save = new Save(this);
    //        connect(save,SIGNAL(sendSaveBaseinfo(bool)),this,SLOT(receiveBaseinfoSave(bool)));
    //        save->show();
    //    }
    //    else
    //    {
    //        qDebug()<<"网络连接成功";
    //       // ui->label_wifi->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/connect.PNG);");
    //    }
}


//! [历史查询翻页]
void Newconfiginfo::pagechange()
{
    thepages =QString::number((thepage-1)*10);
    aff = "select IDCode,ScrewID,Program,Torque,Angle,DATE_FORMAT(TighteningTime,'%Y-%m-%d %H:%i:%s'),TighteningStatus,UploadMark,Cycle from TighteningDatas WHERE "+condition+" order by RecordID DESC limit "+thepages +", 10";
    //    aff = "select top(10) IDCode,ScrewID,Torque,Angle,DATE_FORMAT(TighteningTime,'%Y-%m-%d %H:%i:%s'),TighteningStatus,UploadMark,Cycle from TighteningDatas WHERE "+condition+" and RecordID not in (select top(("+ thepages +"-1)*10) RecordID from TighteningDatas WHERE "+condition+")";
    //qDebug()<< "aff="<< aff;
    model->setQuery(aff);
    int i;
    for(i=0;i<10;i++)
    {
        ui->tableView->setRowHeight(i,43);
    }
}

void Newconfiginfo::on_pushButton_search_clicked()
{
    if(!db.isOpen())
    {
        if(!db.open())
            qDebug()<< "newconfig reopen" << db.lastError().text();
        else
            qDebug()<< "newconfig database reopen ok";
    }
    query = new QSqlQuery(db);
    //    query->("SELECT * FROM TighteningDatas");
    VIN = ui->lineEdit_VIN_2->text();
    ScrewID = ui->lineEdit_ScrewID->text();

    if(VIN != "" && ScrewID != "")
        condition = "IDCode COLLATE gb2312_bin LIKE \'%"+VIN+"%\' and ScrewID LIKE \'%"+ScrewID+"%\' and RecordID in (SELECT t.RecordID from (SELECT RecordID FROM TighteningDatas ORDER BY RecordID DESC LIMIT 1000)as t)";
    else if(VIN != "" && ScrewID == "")
        condition = "IDCode COLLATE gb2312_bin LIKE \'%"+VIN+"%\' and RecordID in (SELECT t.RecordID from (SELECT RecordID FROM TighteningDatas ORDER BY RecordID DESC LIMIT 1000)as t)";
    else if(VIN == "" && ScrewID != "")
        condition = "ScrewID LIKE \'%"+ScrewID+"%\' and RecordID in (SELECT t.RecordID from (SELECT RecordID FROM TighteningDatas ORDER BY RecordID DESC LIMIT 1000)as t)";
    else if(VIN == "" && ScrewID == "")
        condition = "RecordID in (SELECT t.RecordID from (SELECT RecordID FROM TighteningDatas ORDER BY RecordID DESC LIMIT 1000)as t)";
    //    affall = "SELECT * FROM Data.TighteningDatas";
    affall = "SELECT RecordId FROM TighteningDatas WHERE "+condition;
    //qDebug()<< "affall="<< affall;
    query->exec(affall);

    query->last();
    int numRows = query->at() + 1;
    //qDebug() << "row number: " << numRows;
    ui->label_num->setText(QString::number(numRows));
    if (numRows%10 != 0)
    {
        pages = numRows/10+1;
    }
    else
    {
        pages = numRows/10;
    }
    // qDebug() << "pages: " << pages;
    ui->label_M->setText(QString::number(pages));
    //thepage = 1;
    if (numRows==0)
    {
        ui->label_N->setText("0");
        thepage = 0;
    }
    else
    {
        thepage = 1;
        ui->label_N->setText(QString::number(thepage));
    }
    //thepages =QString::number((thepage-1)*10);
    thepages = "0";
    aff = "select IDCode,ScrewID,Program,Torque,Angle,DATE_FORMAT(TighteningTime,'%Y-%m-%d %H:%i:%s'),TighteningStatus,UploadMark,Cycle from TighteningDatas WHERE "+condition+" order by RecordID DESC limit "+thepages +", 10";
    //    aff = "select top(10) IDCode,ScrewID,Torque,Angle,TighteningTime,TighteningStatus,UploadMark,Cycle from TighteningDatas WHERE "+condition+" and RecordID not in (select top(("+ thepages +"-1)*10) RecordID from TighteningDatas WHERE "+condition+")";
    //qDebug()<< "aff="<< aff;
    model->setQuery(aff);
    // qDebug()<< db.lastError().text();
    int i;
    for(i=0;i<10;i++)
    {
        ui->tableView->setRowHeight(i,43);
    }
}

void Newconfiginfo::on_pushButton_first_clicked()
{
    if(thepage == 0)
        thepage = 0;
    else
        thepage = 1;
    ui->label_N->setText(QString::number(thepage));
    pagechange();
}

void Newconfiginfo::on_pushButton_last_clicked()
{
    thepage = pages;
    ui->label_N->setText(QString::number(thepage));
    pagechange();
}

void Newconfiginfo::on_pushButton_previous_clicked()
{
    //left
    if(thepage == 1)
        thepage = 2;
    if(thepage == 0)
        thepage =1;
    thepage--;
    ui->label_N->setText(QString::number(thepage));
    pagechange();
}

void Newconfiginfo::on_pushButton_next_clicked()
{
    //right
    if(thepage == pages)
        thepage = pages-1;
    thepage++;
    ui->label_N->setText(QString::number(thepage));
    pagechange();
}
//! [历史查询翻页]

void Newconfiginfo::connect_localMySQL()
{
    //打开本地数据库
    db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("Tighten");
    db.setUserName("root");
    db.setPassword("123456");

    if(!db.open())
    {
        qDebug()<< "newconfig" << db.lastError().text();
    }else
    {
        qDebug()<< "newconfig database ok";
    }


    model = new QSqlQueryModel(this);
    model->setQuery("select IDCode,ScrewID,Program,Torque,Angle,TighteningTime,TighteningStatus,UploadMark,Cycle from TighteningDatas WHERE RecordId <0");
    model->setHeaderData(0, Qt::Horizontal, tr("VIN码"));
    model->setHeaderData(1, Qt::Horizontal, tr("螺栓编号"));
    model->setHeaderData(2, Qt::Horizontal, tr("程序号"));
    model->setHeaderData(3, Qt::Horizontal, tr("扭矩"));
    model->setHeaderData(4, Qt::Horizontal, tr("角度"));
    model->setHeaderData(5, Qt::Horizontal, tr("拧紧时间"));
    model->setHeaderData(6, Qt::Horizontal, tr("拧紧状态"));
    model->setHeaderData(7, Qt::Horizontal, tr("上传标志"));
    model->setHeaderData(8, Qt::Horizontal, tr("循环号"));


    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    //    ui->student_tableview->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    //    ui->student_tableview->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    //    ui->student_tableview->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    //    ui->student_tableview->horizontalHeader()->setResizeMode(4, QHeaderView::Fixed);

    ui->tableView->setModel(model);

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑

    ui->tableView->setShowGrid(false);//显示表格线

    ui->tableView->setColumnWidth(0,190);//第一列宽度
    ui->tableView->setColumnWidth(1,110);
    ui->tableView->setColumnWidth(2,70);
    ui->tableView->setColumnWidth(3,90);
    ui->tableView->setColumnWidth(4,100);
    ui->tableView->setColumnWidth(5,220);
    ui->tableView->setColumnWidth(6,101);
    ui->tableView->setColumnWidth(7,100);
    ui->tableView->setColumnWidth(8,80);


    int i;
    for(i=0;i<10;i++)
    {
        ui->tableView->setRowHeight(i,43);//行宽
    }
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color: rgb(51, 153, 255);"
                                                     "color: rgb(248, 248, 255);border: 0px; font:14pt}");
    ui->tableView->horizontalHeader()->setFixedHeight(51);

    ui->tableView->verticalHeader()->setVisible(false);   //隐藏列表头
    ui->tableView->horizontalHeader()->setVisible(true); //隐藏行表头
    ui->label_M->setText("0");
    ui->label_N->setText("0");

    thepages = "";
}

void Newconfiginfo::on_pushButton_pronum_add_clicked()
{
    new_pronum = ui->lineEdit_pronum->text().toInt()+1;
    ui->lineEdit_pronum->setText(QString::number(new_pronum));
}

void Newconfiginfo::on_pushButton_pronum_minus_clicked()
{
    if (ui->lineEdit_pronum->text().toInt()>0)
    {
        new_pronum=ui->lineEdit_pronum->text().toInt()-1;
        ui->lineEdit_pronum->setText(QString::number(new_pronum));
    }
}

void Newconfiginfo::on_pushButton_number_add_clicked()
{
    new_number=ui->lineEdit_number->text().toInt()+1;
    ui->lineEdit_number->setText(QString::number(new_number));
}

void Newconfiginfo::on_pushButton_number_minus_clicked()
{
    if (ui->lineEdit_number->text().toInt()>0)
    {
        new_number=(ui->lineEdit_number->text()).toInt()-1;
        ui->lineEdit_number->setText(QString::number(new_number));
    }
}

void Newconfiginfo::on_pushButton_xuanpronum_add_clicked()
{
    new_pronum = ui->lineEdit_xuanpronum->text().toInt()+1;
    ui->lineEdit_xuanpronum->setText(QString::number(new_pronum));
}

void Newconfiginfo::on_pushButton_xuanpronum_minus_clicked()
{
    if (ui->lineEdit_xuanpronum->text().toInt()>0)
    {
        new_pronum=ui->lineEdit_xuanpronum->text().toInt()-1;
        ui->lineEdit_xuanpronum->setText(QString::number(new_pronum));
    }
}

void Newconfiginfo::on_pushButton_xuannumber_add_clicked()
{
    new_number=ui->lineEdit_xuannumber->text().toInt()+1;
    ui->lineEdit_xuannumber->setText(QString::number(new_number));
}

void Newconfiginfo::on_pushButton_xuannumber_minus_clicked()
{
    if (ui->lineEdit_xuannumber->text().toInt()>0)
    {
        new_number=(ui->lineEdit_xuannumber->text()).toInt()-1;
        ui->lineEdit_xuannumber->setText(QString::number(new_number));
    }
}


void Newconfiginfo::historyclear()
{
    pages = 0;
    thepage = 0;
    ui->label_M->setText("0");
    ui->label_N->setText("0");
    ui->label_num->setText("________");
    ui->lineEdit_VIN_2->setText("");
    ui->lineEdit_ScrewID->setText("");
    model->setQuery("select IDCode,ScrewID,Program,Torque,Angle,TighteningTime,TighteningStatus,UploadMark,Cycle from TighteningDatas WHERE RecordId <0");
}

//Fis更新列
void Newconfiginfo::on_pushButton_67_clicked()
{
    qDebug()<<"isboundchang"<<isboundchange;
    if(ui->stackedWidget_2-> currentIndex() != 10)
    {
        whichButtonClick = "fisupdatecolumn";
        if(!isadvancedchange)
            advancedIsChange();
        else if(!ismasterslavechange)
            masterslaveIsChange();
        else if(!isboundchange)
            boundIsChange();
        else
        {
            ui->stackedWidget_2->setCurrentIndex(10);
            ui->label_87->hide();
            ui->label_101->hide();
            ui->label_88->hide();
            ui->label_98->show();
            ui->label_99->hide();
            ui->label_162->hide();

            query = new QSqlQuery(db);
            query->exec("select column_name from information_schema.columns where table_name='FisPreview'");
            for(int i=0;i<6;i++)
               query->next();
            QString columns = "";
            if(query->next())
               columns = query->value(0).toString();
            while(query->next())
            {
                columns = columns + ", "+query->value(0).toString();
                //qDebug()<<query->value(0).toString();
            }
            //qDebug()<< "columns" << columns;
            ui->textBrowser->setText(columns);
            ui->lineEdit_column->clear();
            ui->lineEdit_column_2->clear();
            //ui->label_39->setText(columns);
        }
    }
}

//添加列
void Newconfiginfo::on_pushButton_4_clicked()
{
    bool isColumnName=false;
    query = new QSqlQuery(db);
    query->exec("select column_name from information_schema.columns where table_name='FisPreview'");
    while(query->next())
    {
        if(ui->lineEdit_column->text()==query->value(0).toString())
            isColumnName=true;
    }
    if(!isColumnName)
    {
        if(!ui->lineEdit_column->text().isEmpty())
        {
            QString column =ui->lineEdit_column->text();
            query->exec("alter table FisPreview add " + column + " char(3) null");
            qDebug()<<"FisPreview add column "<<column;
            query->exec("select column_name from information_schema.columns where table_name='FisPreview'");
            for(int i=0;i<6;i++)
               query->next();
            QString columns = "";
            if(query->next())
               columns = query->value(0).toString();
            while(query->next())
            {
               columns = columns + ", "+query->value(0).toString();
                //qDebug()<<query->value(0).toString();
            }
            //qDebug()<< "columns" << columns;
            //ui->label_39->setText(columns);
            ui->textBrowser->setText(columns);
            emit column_update(column);
            ui->lineEdit_column->clear();
        }
    }
}

//删除列
void Newconfiginfo::on_pushButton_51_clicked()
{
    query = new QSqlQuery(db);
    query1 = new QSqlQuery(db);
    query1->exec("select column_name from information_schema.columns where table_name='FisPreview'");
    for(int i=0;i<6;i++)
        query1->next();
    while(query1->next())
    {
        if(ui->lineEdit_column_2->text() == query1->value(0).toString())
        {
            query->exec("alter table FisPreview drop " + ui->lineEdit_column_2->text());
            qDebug()<<"FisPreview drop column "<<ui->lineEdit_column_2->text();
            query->exec("select column_name from information_schema.columns where table_name='FisPreview'");
            for(int i=0;i<6;i++)
               query->next();
            QString columns = "";
            if(query->next())
               columns = query->value(0).toString();
            while(query->next())
            {
                columns = columns + ", "+query->value(0).toString();
                //qDebug()<<query->value(0).toString();
            }
            //qDebug()<< "columns" << columns;
            //ui->label_39->setText(columns);
            ui->textBrowser->setText(columns);
            ui->lineEdit_column_2->clear();
        }
        //qDebug()<<query->value(0).toString();
    }
}

void Newconfiginfo::on_pushButton_36_clicked()
{
    //添加PDM
    ui->lineEdit_pdmname->show();
    ui->pushButton_29->show();
    ui->pushButton_30->show();
    ui->pushButton_31->show();
    ui->label_69->hide();
    ui->comboBox->hide();
    pathpdm = "";
    ui->lineEdit_pdmname->clear();
    SaveWhat = "pdm";
    isSavedpdm = true;
    int tempdata = numpdm;
    ui->lineEdit_pdmname->setFocus();
    for(int i = 0;i<tempdata;i++)
    {
        delete butt1[i];
        numpdm--;
    }
    ui->groupBox_13->setTitle(tr("图片"));
    ui->groupBox_13->setStyleSheet("QGroupBox {font: 14pt;margin-top: 1ex;border-width:0.5px;border-style:solid;border-color: rgb(51, 153, 255);} QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top left;left:15px;margin-left: 5px;margin-right: 5px;padding:1px;}");
    ui->pushButton_33->hide();
}


void Newconfiginfo::on_pushButton_86_clicked()
{
    if(ui->stackedWidget_2-> currentIndex() != 11)
    {
        whichButtonClick = "master_slave";
        if(!isadvancedchange)
            advancedIsChange();
        else if(!isboundchange)
            boundIsChange();
        else
        {
            ismasterslavechange = false;
            ui->stackedWidget_2->setCurrentIndex(11);
            ui->label_87->hide();
            ui->label_101->hide();
            ui->label_88->hide();
            ui->label_98->hide();
            ui->label_99->show();
            ui->label_162->hide();
        }
    }
}

void Newconfiginfo::on_radioButton_2_clicked()
{
    ui->checkBox_1->hide();
    ui->checkBox_2->hide();
    ui->checkBox_3->hide();
    ui->lineEdit_slave_1->hide();
    ui->lineEdit_slave_2->hide();
    ui->lineEdit_slave_3->hide();

}

void Newconfiginfo::on_radioButton_1_clicked()
{
    ui->checkBox_1->show();
    ui->checkBox_2->show();
    ui->checkBox_3->show();
    ui->lineEdit_slave_1->show();
    ui->lineEdit_slave_2->show();
    ui->lineEdit_slave_3->show();
}

void Newconfiginfo::on_pushButton_37_clicked()
{
    whichButtonClick = "savemasterslave";
    e3 = new QGraphicsOpacityEffect(this);
    e3->setOpacity(0.5);
    ui->label_100->setGraphicsEffect(e3);
    ui->label_100->show();
    ui->label_100->setGeometry(0,0,1366,768);

    SaveWhat = "Master_slave";
    save = new Save(this);
    connect(save,SIGNAL(sendSaveMasterSlaveState(bool)),this,SLOT(receiveMasterSlaveState(bool)));
    save->show();
}

void Newconfiginfo::receiveMasterSlaveState(bool statetmp)
{
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    if(statetmp)
    {
        QButtonGroup getModeSelect;
        getModeSelect.addButton(ui->radioButton_1,1);//将radioButton加进组
        getModeSelect.addButton(ui->radioButton_2,2);

        if(getModeSelect.checkedId()==1)
        {
            //qDebug()<<"master";
            configIniRead->setValue("baseinfo/Master_slave","master");
            if(ui->checkBox_1->isChecked())
            {
                if(!ui->lineEdit_slave_1->text().isEmpty())
                    configIniRead->setValue("baseinfo/slave1",ui->lineEdit_slave_1->text());
                else
                    ui->checkBox_1->setChecked(false);
            }
            else
            {
                ui->lineEdit_slave_1->clear();
                configIniRead->setValue("baseinfo/slave1","");
            }
            if(ui->checkBox_2->isChecked())
            {
                if(!ui->lineEdit_slave_2->text().isEmpty())
                    configIniRead->setValue("baseinfo/slave2",ui->lineEdit_slave_2->text());
                else
                    ui->checkBox_2->setChecked(false);
            }
            else
            {
                ui->lineEdit_slave_2->clear();
                configIniRead->setValue("baseinfo/slave2","");
            }
            if(ui->checkBox_3->isChecked())
            {
                if(!ui->lineEdit_slave_3->text().isEmpty())
                    configIniRead->setValue("baseinfo/slave3",ui->lineEdit_slave_3->text());
                else
                    ui->checkBox_3->setChecked(false);
            }
            else
            {
                ui->lineEdit_slave_3->clear();
                configIniRead->setValue("baseinfo/slave3","");
            }
            system("killall -9 client &");
            system("echo 0 > /sys/class/graphics/fb2/blank &");
            system("/etc/data2/client -qws -display \"LinuxFb:/dev/fb2:mmWidth500:mmHeight300:0\" &");
        }
        else if(getModeSelect.checkedId()==2)
        {
            //qDebug()<<"slave";
            configIniRead->setValue("baseinfo/Master_slave","slave");
            configIniRead->setValue("baseinfo/slave1","");
            configIniRead->setValue("baseinfo/slave2","");
            configIniRead->setValue("baseinfo/slave3","");
            ui->lineEdit_slave_1->setText("");
            ui->lineEdit_slave_2->setText("");
            ui->lineEdit_slave_3->setText("");
            ui->checkBox_1->setChecked(false);
            ui->checkBox_2->setChecked(false);
            ui->checkBox_3->setChecked(false);
        }
    }
    else
    {
        if(configIniRead->value("baseinfo/Master_slave").toString()=="master")
        {
            ui->radioButton_1->setChecked(true);
            ui->lineEdit_slave_1->setText(configIniRead->value("baseinfo/slave1").toString());
            if(!ui->lineEdit_slave_1->text().isEmpty())
                ui->checkBox_1->setChecked(true);
            else
            {
                ui->lineEdit_slave_1->setText("");
                ui->checkBox_1->setChecked(false);
            }
            ui->lineEdit_slave_2->setText(configIniRead->value("baseinfo/slave2").toString());
            if(!ui->lineEdit_slave_2->text().isEmpty())
                ui->checkBox_2->setChecked(true);
            else
            {
                ui->lineEdit_slave_2->setText("");
                ui->checkBox_2->setChecked(false);
            }
            ui->lineEdit_slave_3->setText(configIniRead->value("baseinfo/slave3").toString());
            if(!ui->lineEdit_slave_3->text().isEmpty())
                ui->checkBox_3->setChecked(true);
            else
            {
                ui->lineEdit_slave_3->setText("");
                ui->checkBox_3->setChecked(false);
            }
        }
        else if(configIniRead->value("baseinfo/Master_slave").toString()=="slave")
        {
            ui->radioButton_2->setChecked(true);
            ui->checkBox_1->hide();
            ui->checkBox_2->hide();
            ui->checkBox_3->hide();
            ui->lineEdit_slave_1->hide();
            ui->lineEdit_slave_2->hide();
            ui->lineEdit_slave_3->hide();
        }
        else
            ui->radioButton_1->setChecked(true);
    }
    delete configIniRead;
    ui->label_100->hide();
    delete e3;
    delete save;

    if(whichButtonClick == "advancedback")
    {
        on_pushButton_3_clicked();
        //ismasterslavechange = false;
    }
    else if(whichButtonClick == "passwordchange")
    {
        on_pushButton_100_clicked();
        //ismasterslavechange = false;
    }
    else if(whichButtonClick == "testinterface")
    {
        on_pushButton_66_clicked();
        //ismasterslavechange = false;
    }
    else if(whichButtonClick == "fisupdatecolumn")
    {
        on_pushButton_67_clicked();
        //ismasterslavechange = false;
    }
    else if(whichButtonClick == "advanceset")
    {
        on_pushButton_58_clicked();
        //ismasterslavechange = false;
    }
    else if(whichButtonClick == "savemasterslave")
    {
        //        ismasterslavechange = false;
    }
    else if(whichButtonClick == "bound_config")
    {
        on_pushButton_55_clicked();
        //ismasterslavechange = false;
    }
}

void Newconfiginfo::receiveDebug(QString vinbuf)
{
    ui->lineEdit_staname_5->setText(vinbuf);
}


void Newconfiginfo::masterslaveIsChange()
{
    // Master_slave监听
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    QString temp;
    bool temp1,temp2,temp3;
    QButtonGroup getModeSelect;
    getModeSelect.addButton(ui->radioButton_1,1);//将radioButton加进组
    getModeSelect.addButton(ui->radioButton_2,2);

    if(getModeSelect.checkedId()==1)
    {
        temp="master";
    }
    else if(getModeSelect.checkedId()==2)
    {
        temp="slave";
    }

    if(configIniRead->value("baseinfo/slave1").toString().isEmpty())
        temp1=false;
    else
        temp1=true;
    if(configIniRead->value("baseinfo/slave2").toString().isEmpty())
        temp2=false;
    else
        temp2=true;
    if(configIniRead->value("baseinfo/slave3").toString().isEmpty())
        temp3=false;
    else
        temp3=true;
    if (   temp != configIniRead->value("baseinfo/Master_slave")||
           configIniRead->value("baseinfo/slave1").toString()!=ui->lineEdit_slave_1->text()||
           configIniRead->value("baseinfo/slave2").toString()!=ui->lineEdit_slave_2->text()||
           configIniRead->value("baseinfo/slave3").toString()!=ui->lineEdit_slave_3->text()||
           ui->checkBox_1->isChecked()!=temp1||
           ui->checkBox_2->isChecked()!=temp2||
           ui->checkBox_3->isChecked()!=temp3   )
    {
        e3 = new QGraphicsOpacityEffect(this);
        e3->setOpacity(0.5);
        ui->label_100->setGraphicsEffect(e3);
        ui->label_100->show();
        ui->label_100->setGeometry(0,0,1366,768);

        SaveWhat = "Master_slave";
        save = new Save(this);
        connect(save,SIGNAL(sendSaveMasterSlaveState(bool)),this,SLOT(receiveMasterSlaveState(bool)));
        save->show();
        ismasterslavechange = true;
        delete configIniRead;
    }
    else
    {
        delete configIniRead;
        ismasterslavechange = true;
        if (whichButtonClick == "advancedback")
        {
            on_pushButton_3_clicked();
            //ismasterslavechange = false;
        }
        else if (whichButtonClick == "passwordchange")
        {
            on_pushButton_100_clicked();
            //ismasterslavechange = false;
        }
        else if (whichButtonClick == "testinterface")
        {
            on_pushButton_66_clicked();
            //ismasterslavechange = false;
        }
        else if (whichButtonClick == "fisupdatecolumn")
        {
            on_pushButton_67_clicked();
            //ismasterslavechange = false;
        }
        else if (whichButtonClick == "advanceset")
        {
            on_pushButton_58_clicked();
            //ismasterslavechange = false;
        }
        else if (whichButtonClick == "bound_config")
        {
            on_pushButton_55_clicked();
            //ismasterslavechange = false;
        }
    }
}


void Newconfiginfo::receivetime(QString  datetime)
{
    //qDebug()<<"datetime"<<datetime;
    ui->year->setText(datetime.mid(0,4));
    ui->month->setText(datetime.mid(5,2));
    ui->date->setText(datetime.mid(8,2));
    ui->hour->setText(datetime.mid(11,2));
    ui->minute->setText(datetime.mid(14,2));
    ui->second->setText(datetime.mid(17,2));
}
void Newconfiginfo::on_pushButton_38_clicked()
{
    emit sendGetTime();
}

void Newconfiginfo::on_pushButton_49_clicked()
{
    if(numpdm > 0)
    {
        for(int i = 0;i< numpdm;i++)
            butt1[i]->show();
    }
    ui->stackedWidget_6->setCurrentIndex(0);
}

//***********************套筒按钮加减**********************************
void Newconfiginfo::on_pushButton_taotong_add_clicked()
{
    if(ui->lineEdit_taotong->text().toInt()!=1 && ui->lineEdit_taotong->text().toInt()!=2 && ui->lineEdit_taotong->text().toInt()!=3 && ui->lineEdit_taotong->text().toInt()!=4)
        ui->lineEdit_taotong->setText("0");

    if((ui->lineEdit_taotong->text().toInt()+1) == 5)
        ui->lineEdit_taotong->setText("4");
    else
        ui->lineEdit_taotong->setText(QString::number(ui->lineEdit_taotong->text().toInt()+1));

}

void Newconfiginfo::on_pushButton_taotong_minus_clicked()
{
    if(ui->lineEdit_taotong->text().toInt()!=1&&ui->lineEdit_taotong->text().toInt()!=2&&ui->lineEdit_taotong->text().toInt()!=3&&ui->lineEdit_taotong->text().toInt()!=4)
        ui->lineEdit_taotong->setText("1");

    if((ui->lineEdit_taotong->text().toInt()-1) <= 0)
        ui->lineEdit_taotong->setText(tr("无"));
    else
    {
        ui->lineEdit_taotong->setText(QString::number(ui->lineEdit_taotong->text().toInt()-1));
    }

}

void Newconfiginfo::on_pushButton_50_clicked()
{
    system("rm /etc/pointercal &");
    system("/usr/local/tslib-instal/bin/ts_calibrate &");
}

void Newconfiginfo::on_Line_radioButton_1_clicked()
{
    ui->Line_radioButton_2->setChecked(false);
    line_ID = 1;
}

void Newconfiginfo::on_Line_radioButton_2_clicked()
{
    ui->Line_radioButton_1->setChecked(false);
    line_ID = 2;
}

void Newconfiginfo::on_pushButton_104_clicked()
{
    ui->lineEdit_staname_5->clear();
}

void Newconfiginfo::on_pushButton_55_clicked()
{
    if(ui->stackedWidget_2->currentIndex() != 12)
    {
        whichButtonClick = "bound_config";
        if(!isadvancedchange)
            advancedIsChange();
        else if(!ismasterslavechange)
            masterslaveIsChange();
        else
        {
            ui->stackedWidget_2->setCurrentIndex(12);
            ui->label_87->hide();
            ui->label_101->hide();
            ui->label_88->hide();
            ui->label_98->hide();
            ui->label_99->hide();
            ui->label_162->show();

            isboundchange = false;
            on_pushButton_88_clicked();
        }
    }
}

void Newconfiginfo::bound_show()
{
    ui->lineEdit_ProNumber->setText(QString::number(PronumNow));
    ui->lineEdit_torque_max->setText(bound[PronumNow][0]);
    ui->lineEdit_torque_min->setText(bound[PronumNow][1]);
    ui->lineEdit_angle_max->setText(bound[PronumNow][2]);
    ui->lineEdit_angle_min->setText(bound[PronumNow][3]);
}

void Newconfiginfo::bound_save()
{
    bound[PronumNow][0]=ui->lineEdit_torque_max->text();
    bound[PronumNow][1]=ui->lineEdit_torque_min->text();
    bound[PronumNow][2]=ui->lineEdit_angle_max->text();
    bound[PronumNow][3]=ui->lineEdit_angle_min->text();
}

void Newconfiginfo::on_pushButton_tens_add_clicked()
{
    bool wrong=false;
    if(ui->lineEdit_torque_max->text()!=""&& ui->lineEdit_torque_min->text()!=""&&
            ui->lineEdit_torque_max->text().toDouble()<ui->lineEdit_torque_min->text().toDouble())
    {
        wrong = true;
        ui->lineEdit_torque_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
        ui->lineEdit_torque_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
    }
    else
    {
        ui->lineEdit_torque_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
        ui->lineEdit_torque_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
    }
    if(ui->lineEdit_angle_max->text()!=""&& ui->lineEdit_angle_min->text()!=""&&
            ui->lineEdit_angle_max->text().toDouble()<ui->lineEdit_angle_min->text().toDouble())
    {
        wrong = true;
        ui->lineEdit_angle_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
        ui->lineEdit_angle_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
    }
    else
    {
        ui->lineEdit_angle_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
        ui->lineEdit_angle_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
    }
    if(!wrong)
    {
        bound_save();
        if(PronumNow+10<100)
        {
            PronumNow=PronumNow+10;
            bound_show();
        }
    }
}

void Newconfiginfo::on_pushButton_tens_minus_clicked()
{
    bool wrong=false;
    if(ui->lineEdit_torque_max->text()!=""&& ui->lineEdit_torque_min->text()!=""&&
            ui->lineEdit_torque_max->text().toDouble()<ui->lineEdit_torque_min->text().toDouble())
    {
        wrong = true;
        ui->lineEdit_torque_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
        ui->lineEdit_torque_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
    }
    else
    {
        ui->lineEdit_torque_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
        ui->lineEdit_torque_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
    }
    if(ui->lineEdit_angle_max->text()!=""&& ui->lineEdit_angle_min->text()!=""&&
            ui->lineEdit_angle_max->text().toDouble() < ui->lineEdit_angle_min->text().toDouble())
    {
        wrong = true;
        ui->lineEdit_angle_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
        ui->lineEdit_angle_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
    }
    else
    {
        ui->lineEdit_angle_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
        ui->lineEdit_angle_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
    }
    if(!wrong)
    {
        bound_save();
        if(PronumNow-10>=0)
        {
            PronumNow=PronumNow-10;
            bound_show();
        }
    }
}

void Newconfiginfo::on_pushButton_ones_add_clicked()
{
    bool wrong=false;
    if(ui->lineEdit_torque_max->text()!=""&& ui->lineEdit_torque_min->text()!=""&&
            ui->lineEdit_torque_max->text().toDouble()<ui->lineEdit_torque_min->text().toDouble())
    {
        wrong = true;
        ui->lineEdit_torque_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
        ui->lineEdit_torque_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
    }
    else
    {
        ui->lineEdit_torque_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
        ui->lineEdit_torque_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
    }
    if(ui->lineEdit_angle_max->text()!=""&& ui->lineEdit_angle_min->text()!=""&&
            ui->lineEdit_angle_max->text().toDouble()<ui->lineEdit_angle_min->text().toDouble())
    {
        wrong = true;
        ui->lineEdit_angle_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
        ui->lineEdit_angle_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
    }
    else
    {
        ui->lineEdit_angle_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
        ui->lineEdit_angle_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
    }
    if(!wrong)
    {
        bound_save();
        if(PronumNow+1<100)
        {
            PronumNow=PronumNow+1;
            bound_show();
        }
    }
}

void Newconfiginfo::on_pushButton_ones_minus_clicked()
{
    bool wrong=false;
    if(ui->lineEdit_torque_max->text()!=""&& ui->lineEdit_torque_min->text()!=""&&
            ui->lineEdit_torque_max->text().toDouble()<ui->lineEdit_torque_min->text().toDouble())
    {
        wrong = true;
        ui->lineEdit_torque_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
        ui->lineEdit_torque_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
    }
    else
    {
        ui->lineEdit_torque_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
        ui->lineEdit_torque_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
    }
    if(ui->lineEdit_angle_max->text()!=""&& ui->lineEdit_angle_min->text()!=""&&
            ui->lineEdit_angle_max->text().toDouble()<ui->lineEdit_angle_min->text().toDouble())
    {
        wrong = true;
        ui->lineEdit_angle_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
        ui->lineEdit_angle_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
    }
    else
    {
        ui->lineEdit_angle_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
        ui->lineEdit_angle_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
    }
    if(!wrong)
    {
        bound_save();
        if(PronumNow-1>=0)
        {
            PronumNow=PronumNow-1;
            bound_show();
        }
    }
}

void Newconfiginfo::on_pushButton_88_clicked()
{
    QSettings *config_bound = new QSettings("/config_bound.ini", QSettings::IniFormat);
    for(int i=0;i<100;i++)
    {
        bound[i][0]=config_bound->value("ProNumber"+QString::number(i)+"/Torque_max").toString();
        bound[i][1]=config_bound->value("ProNumber"+QString::number(i)+"/Torque_min").toString();
        bound[i][2]=config_bound->value("ProNumber"+QString::number(i)+"/Angle_max").toString();
        bound[i][3]=config_bound->value("ProNumber"+QString::number(i)+"/Angle_min").toString();
        for(int j=0;j<4;j++)
            bound_temp[i][j]=bound[i][j];
    }
    delete config_bound;
    bound_enabled_temp=bound_enabled;
    if(bound_enabled)
    {
        ui->pushButton_bound->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/13.bmp);");
    }
    else
    {
        ui->pushButton_bound->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/14.bmp);");
    }
    PronumNow = 0;
    bound_show();
    bound_update();
    ui->lineEdit_torque_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
    ui->lineEdit_torque_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
    ui->lineEdit_angle_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
    ui->lineEdit_angle_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
}

void Newconfiginfo::on_pushButton_87_clicked()
{
    bound_save();
    whichButtonClick = "bound_save";
    e3 = new QGraphicsOpacityEffect(this);
    e3->setOpacity(0.5);
    ui->label_100->setGraphicsEffect(e3);
    ui->label_100->show();
    ui->label_100->setGeometry(0,0,1366,768);

    SaveWhat = "config_bound";
    save = new Save(this);
    connect(save,SIGNAL(sendSaveBound(bool)),this,SLOT(receiveBound(bool)));
    save->show();
}

void Newconfiginfo::receiveBound(bool isSave)
{
    bool wrong=false;
    if(isSave)
    {
        if(ui->lineEdit_torque_max->text()!=""&& ui->lineEdit_torque_min->text()!=""&&
                ui->lineEdit_torque_max->text().toDouble()<ui->lineEdit_torque_min->text().toDouble())
        {
            wrong = true;
            ui->lineEdit_torque_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
            ui->lineEdit_torque_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
        }
        else
        {
            ui->lineEdit_torque_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
            ui->lineEdit_torque_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
        }
        if(ui->lineEdit_angle_max->text()!=""&& ui->lineEdit_angle_min->text()!=""&&
                ui->lineEdit_angle_max->text().toDouble()<ui->lineEdit_angle_min->text().toDouble())
        {
            wrong = true;
            ui->lineEdit_angle_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
            ui->lineEdit_angle_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(255, 0, 0);");
        }
        else
        {
            ui->lineEdit_angle_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
            ui->lineEdit_angle_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
        }
        if(!wrong)
        {
            QSettings *config_bound = new QSettings("/config_bound.ini", QSettings::IniFormat);
            for(int i=0;i<100;i++)
            {
                config_bound->setValue("ProNumber"+QString::number(i)+"/Torque_max",bound[i][0]);
                config_bound->setValue("ProNumber"+QString::number(i)+"/Torque_min",bound[i][1]);
                config_bound->setValue("ProNumber"+QString::number(i)+"/Angle_max",bound[i][2]);
                config_bound->setValue("ProNumber"+QString::number(i)+"/Angle_min",bound[i][3]);
                for(int j=0;j<4;j++)
                    bound_temp[i][j]=bound[i][j];
            }
            bound_enabled = bound_enabled_temp;
            QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
            //        qDebug()<<"bound_enabled"<<bound_enabled;
            config->setValue("baseinfo/bound",bound_enabled?"1":"0");
            delete config;
            bound_update();
            delete config_bound;
            isboundchange = true;
        }
    }
    else
    {
        ui->lineEdit_torque_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
        ui->lineEdit_torque_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
        ui->lineEdit_angle_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
        ui->lineEdit_angle_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
    }
    ui->label_100->hide();
    delete e3;
    delete save;
    if(!wrong)
    {
        if(whichButtonClick == "advancedback")
        {
            on_pushButton_3_clicked();
        }
        else if (whichButtonClick == "advanceset")
        {
            on_pushButton_58_clicked();
        }
        else if(whichButtonClick == "passwordchange")
        {
            on_pushButton_100_clicked();
        }
        else if(whichButtonClick == "testinterface")
        {
            on_pushButton_66_clicked();
        }
        else if(whichButtonClick == "fisupdatecolumn")
        {
            on_pushButton_67_clicked();
        }
        else if(whichButtonClick == "master_slave")
        {
            on_pushButton_86_clicked();
        }
        else if(whichButtonClick == "bound_save")
        {
            isboundchange = false;
        }
    }
}

void Newconfiginfo::boundIsChange()
{
    bound_save();
    QSettings *config_bound = new QSettings("/config_bound.ini", QSettings::IniFormat);
    bool change = false;
    for(int i=0;i<100;i++)
    {
        if(bound[i][0]!=config_bound->value("ProNumber"+QString::number(i)+"/Torque_max").toString()||
                bound[i][1]!=config_bound->value("ProNumber"+QString::number(i)+"/Torque_min").toString()||
                bound[i][2]!=config_bound->value("ProNumber"+QString::number(i)+"/Angle_max").toString()||
                bound[i][3]!=config_bound->value("ProNumber"+QString::number(i)+"/Angle_min").toString())
            change = true;
    }
    delete config_bound;
    if(bound_enabled_temp != bound_enabled)
        change = true;
    if (change)
    {
        e3 = new QGraphicsOpacityEffect(this);
        e3->setOpacity(0.5);
        ui->label_100->setGraphicsEffect(e3);
        ui->label_100->show();
        ui->label_100->setGeometry(0,0,1366,768);

        SaveWhat = "config_bound";
        save = new Save(this);
        connect(save,SIGNAL(sendSaveBound(bool)),this,SLOT(receiveBound(bool)));
        save->show();
//        isboundchange = true;
    }
    else
    {
        isboundchange = true;
        if (whichButtonClick == "advancedback")
        {
            on_pushButton_3_clicked();
        }
        else if (whichButtonClick == "advanceset")
        {
            on_pushButton_58_clicked();
        }
        else if (whichButtonClick == "passwordchange")
        {
            on_pushButton_100_clicked();
        }
        else if (whichButtonClick == "testinterface")
        {
            on_pushButton_66_clicked();
        }
        else if (whichButtonClick == "fisupdatecolumn")
        {
            on_pushButton_67_clicked();
        }
        else if (whichButtonClick == "advanceset")
        {
            on_pushButton_58_clicked();
        }
        else if (whichButtonClick == "master_slave")
        {
            on_pushButton_86_clicked();
        }
    }
}

void Newconfiginfo::on_pushButton_bound_prev_clicked()
{
    if(bound_pages!=0)
    {
        if(--bound_current_page != 0)
            show_bound();
        else
            bound_current_page=1;
    }
}

void Newconfiginfo::on_pushButton_bound_next_clicked()
{
    if(++bound_current_page != bound_pages+1)
    {
        show_bound();
    }
    else
        bound_current_page = bound_pages;
}

void Newconfiginfo::show_bound()
{
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<5;j++)
        {
            tableWidgetItem[i][j]->setText("");
        }
//        header_vertical[i]="";
    }

    if(bound_pages != 0)
    {
        current =0;
        for(iter=list.begin()+10*(bound_current_page-1); iter!=list.end() && iter!=list.begin()+10*(bound_current_page); iter++)
        {
            tableWidgetItem[current][0]->setText("P"+QString::number(*iter));
//            header_vertical[current]="P"+QString::number(*iter);
            for(int j=1;j<5;j++)
            {
                tableWidgetItem[current][j]->setText(bound_temp[*iter][j-1]);
            }
            current++;
        }
    }
//    ui->tableWidget->setVerticalHeaderLabels(header_vertical);
}

void Newconfiginfo::bound_init()
{
    isboundchange = true;
    bound_enabled_temp=bound_enabled;
    bound_pages=0;
    bound_current_page=0;
    current =0;
    iter=0;
    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setShowGrid(true);//显示表格线

    ui->tableWidget->setColumnWidth(0,69);
    for(int i=1;i<5;i++)
        ui->tableWidget->setColumnWidth(i,125);
    for(int i=0;i<10;i++)
        ui->tableWidget->setRowHeight(i,52);

    ui->tableWidget->verticalHeader()->setVisible(false);

    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color: rgb(51, 153, 255);"
                                                     "color: rgb(248, 248, 255);border: 0px; font:14pt}");
//    ui->tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section {background-color: rgb(248, 248, 255);"
//                                                     "color:black; border: 0px; font:14pt}");
    ui->tableWidget->horizontalHeader()->setFixedHeight(51);
//    ui->tableWidget->verticalHeader()->setFixedWidth(49);

//    ui->tableWidget->verticalHeader()->setMovable(false);
    ui->tableWidget->horizontalHeader()->setMovable(false);

//    ui->tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条
    ui->tableWidget->setAutoScroll(false);//去掉自动滚动

    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
//    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::Fixed);

//    table->setStyleSheet("QTableCornerButton::section{background-color:red;}");

//    tableWidget->setTextAlignment.
    // 创建表格项目，并插入到指定单元
//    QTableWidgetItem *tableWidgetItem = new QTableWidgetItem("qt");
//    tableWidget->setItem(1, 1, tableWidgetItem);
    // 创建表格项目，并将它们作为标头
//    QTableWidgetItem *headerV = new QTableWidgetItem("P1");
//    tableWidget->setVerticalHeaderItem(0,headerV);
//    QTableWidgetItem *headerH0 = new QTableWidgetItem(tr("扭矩上限"));
//    QTableWidgetItem *headerH1 = new QTableWidgetItem(tr("扭矩下限"));
//    QTableWidgetItem *headerH2 = new QTableWidgetItem(tr("角度上限"));
//    QTableWidgetItem *headerH3 = new QTableWidgetItem(tr("角度上限"));
//    tableWidget->setHorizontalHeaderItem(0,headerH0);
//    tableWidget->setHorizontalHeaderItem(1,headerH1);
//    tableWidget->setHorizontalHeaderItem(2,headerH2);
//    tableWidget->setHorizontalHeaderItem(3,headerH3);

    for(int i=0;i<10;++i)
        for(int j=0;j<5;++j)
        {
//            ui->tableWidget->item(i,j)->setText("");
//            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignCenter);
            tableWidgetItem[i][j] = new QTableWidgetItem("");
            ui->tableWidget->setItem(i,j,tableWidgetItem[i][j]);
            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignCenter);
        }

    //列名
    QStringList header;
    header<<"P"<<tr("扭矩上限(Nm)")<<tr("扭矩下限(Nm)")<<tr("角度上限(Deg)")<<tr("角度下限(Deg)");
    ui->tableWidget->setHorizontalHeaderLabels(header);

//    header_vertical<<""<<""<<""<<""<<""<<""<<""<<""<<""<<"";
//    ui->tableWidget->setVerticalHeaderLabels(header_vertical);
}

void Newconfiginfo::bound_update()
{
    list.clear();
    for(int i=0;i<100;i++)
    {
        if(bound_temp[i][0]!=""||bound_temp[i][1]!=""||bound_temp[i][2]!=""||bound_temp[i][3]!="")
        {
            list<<i;
        }
    }

    bound_pages = 0;
    bound_current_page = 0;

    if(list.size()%10==0 && list.size()!=0)
    {
        bound_pages=list.size()/10;
        bound_current_page = 1;
    }
    else if(list.size()%10!=0 && list.size() != 0)
    {
        bound_pages=list.size()/10+1;
        bound_current_page = 1;
    }
    ui->label_bound_current_page->setText(QString::number(bound_current_page));
    ui->label_bound_pages->setText(QString::number(bound_pages));
//    qDebug()<<bound_pages<<bound_current_page;
    show_bound();
}

void Newconfiginfo::on_pushButton_bound_clicked()
{
//    qDebug()<<"bound_enabled_temp:"<<bound_enabled_temp;
    if(bound_enabled_temp)
    {
        bound_enabled_temp=false;
        ui->pushButton_bound->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/14.bmp);");
    }
    else
    {
        bound_enabled_temp=true;
        ui->pushButton_bound->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/13.bmp);");
    }
}

void Newconfiginfo::on_pushButton_56_clicked()
{
    ui->lineEdit_torque_max->setText("");
    ui->lineEdit_torque_min->setText("");
    ui->lineEdit_angle_max->setText("");
    ui->lineEdit_angle_min->setText("");
    ui->lineEdit_torque_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
    ui->lineEdit_torque_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
    ui->lineEdit_angle_max->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
    ui->lineEdit_angle_min->setStyleSheet("font: 14pt \"黑体\";border-width:1px; border-style:solid;border-color:rgb(51, 153, 255);");
}

