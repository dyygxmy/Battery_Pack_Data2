#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QTimer>
#include "RollSubtitleLable.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setGeometry(QRect(0, 0, 1366, 768));

#if DYTEST1^1
       ui->lineEdit->hide();
       ui->pushButton->hide();
       ui->but_ac1->hide();
       ui->but_bd2->hide();
       ui->but_bd3->hide();
       ui->but_bd4_1->hide();
       ui->but_bd4_2->hide();
       ui->but_bd4_3->hide();
       ui->but_bd4_4->hide();
#endif

#if DYTEST3
       ui->pushButton->show();
       ui->but_bd2->show();
#endif


    //    sharedMemory.setKey("QSharedMemoryIDCode");

    ui->stackedWidget_2->setCurrentIndex(0);
    ui->stackedWidget_6->setCurrentIndex(0);

    controlMode = false;
    workmode = false;

    //    mainWindowInit();
    showhome();
    this->setFixedSize(1366,768);
    //    this->setWindowTitle("B.I.W @Data2.1.0");
    connectMysql();
    ui->stackedWidget->setCurrentIndex(0);
    optionOrNot = 0;
    whichar = 0;
    equeloptionbnum= 0;
    equeloptionknum= 0;
    pdmnowromisOk = true;
    pdmflicker = true;
    battry_num= 0;
    huCh1 = 0;
    huCh2 = 0;
    huCh3 = 0;
    OKCount = 0;
    recCount = 0;
    packCount = 0;
    packNumber = 0;//记录当前小车合格数（计入产量）
    isTestOverFlag = false;
    isGostRun = false;
    ui->label_black->hide();
    ui->pushButton_reset->hide();
    ui->pushButton_17->hide();
    ui->pushButton_errorGo->hide();
    ui->pushButton_errorContinue->hide();
    ui->pushButton_errorContinue_2->hide();
    ui->pushButton_deep1->hide();
    ui->pushButton_readRFID->hide();
    ui->pushButton_deep2->hide();
    ui->pushButton_deepUp->hide();
    ui->pushButton_deepDown->hide();
    ui->pushButton_Go->hide();
    ui->label_6->hide();
    ui->label_13->hide();
    ui->label_14->hide();
    ui->label_15->hide();
    ui->label_AEN->hide();
    ui->label_YEN->hide();
    ui->label_tishu->hide();
    ui->label_m020Pack1Sn->hide();
    ui->label_m020Pqck2SN->hide();
    ui->label_m020Result->hide();
    ui->lineEdit_M020LineEdit->hide();
    ui->pushButton_M020SendPost->hide();
    testAdd = 10;

    gunPower(true);

    ScrewWhichExit = 0;
    ui->progressBar->setValue(0);
    // ui->label_9->hide();
    // ui->label_ss2->hide();
    ui->label_wifi->hide();
    connect(this,SIGNAL(sendRollLable(QString)),ui->label_showState,SLOT(getText(QString)));
    m_CurrentValue = m_MaxValue = m_UpdateInterval = 0;
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(UpdateSlot()));
    connect(&timerpdm, SIGNAL(timeout()), this, SLOT(PdmFlicker()));
    connect(&packTime, SIGNAL(timeout()), this, SLOT(packTimeStart()));

    connect(&timer_Info,SIGNAL(timeout()),this,SLOT(send_Info()));
    timer_Info.start(1000);

    In_Output = new InOutput;
    In_Output->writeIOOutput("all_led",false);
    //In_Output->writeIOOutput("y_led",true);
    connect(&FlashTime_r,SIGNAL(timeout()),this,SLOT(onLEDFlash_r()));
    connect(&FlashTime_g,SIGNAL(timeout()),this,SLOT(onLEDFlash_g()));
    connect(&FlashTime_y,SIGNAL(timeout()),this,SLOT(onLEDFlash_y()));
    //FlashTime_y.start(500);//开机黄闪
    LEDIsON_r = false;
    LEDIsON_g = false;
    LEDIsON_y = false;
    vinAttributeCode = "";
    VINhead="LSV";
    vinAttributeBit = 3;
    ui->stackedWidget_4->setCurrentIndex(0);
    PDMBoltNum = 0;
    currentBoltSum = 0;
    currentFirstBolt = 0;
    pdmflickerNum=0;
    nokBoltFlag = false;
    ttChangeFlag = false;

    groupAllBoltNumCh=0;
    tempnumdpm = 0;
    whichpdmnumnow = 0;
    PDMCurrentState ="OK";

    systemStatus = 1;
    preSystemStatus = -1;

    linkCount = 0;

    isRFIDConnected = false;
    isFirst = true;
    isSaveShow = false;
    enTaotongFlag = false;
    enBarcodeFlag = false;
    enIOCtlFlag = false;
    taotong_Value = 0;
    IO_value = 0;
    barcode_Value = false;
    taotongEnValue = 0;
    airTestStatus = 0;
    airButtonNum = 0;
    packSN = "";
    isEmergencyStop = false;
    if(ControlType_1 == "CS351")
    {
        ui->label_tighten->setText(ControlType_1);
        vinreverse = new VinReverse;
        QTimer::singleShot(0,vinreverse,SLOT(newconnects()));
        connect(this,SIGNAL(sendnexo(QString)),vinreverse,SLOT(receiveVin(QString)));
    }
    else
    {
        ui->label_tighten->setText("OFF");
        TightenIsReady(true);
    }
}

void MainWindow::newconfigInit(Newconfiginfo *newconfig)
{
    newconfiginfo = newconfig;
}

/*****************断开重新连接*******************/
void MainWindow::receiveOperate()
{   
    TightenIsReady(true);
    if(SYSS == "ING")
    {
        if(carInfor.boltNum[groupNumCh]>0)
        {
            emit sendnexo(serialNums);
//            emit sendOperate1(true,groupNumCh);   //之前的通道号会发0
            emit sendOperate1(true,channelNum.toInt());
        }
    }
}
/*****************拧紧枪是否准备*******************/
void MainWindow::TightenIsReady(bool isready)
{
    qDebug() << "&&&&&&&&&&&&&&&& TightenIsReady &&&&&&&&&&&&&&&&" << isready;
    emit sendCS351StatusToStep(isready);
    if(isready)
    {
        GTightStatus = 1;
        linkCount = 1;
        ui->label_ss1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/68.bmp);");
        if(SYSS == "Ready")
        {
            lock.lockForWrite();
            StationStatus = 2;
            lock.unlock();
            ui->pushButton_16->setText("Ready");
            ui->pushButton_16->setStyleSheet("font: 30pt ; background-color: rgb(51, 153, 255); color: rgb(248, 248, 255); border-width:0px;   border-style:solid; border-color: rgb(51, 153, 255);  ");
        }
        else if(SYSS == "ING")
        {
            lock.lockForWrite();
            StationStatus = 2;
            lock.unlock();
            ui->pushButton_16->setText("ING");
            ui->pushButton_16->setStyleSheet("font: 50pt ; background-color: rgb(250, 225, 0); color: rgb(248, 248, 255); border-width:0px;   border-style:solid; border-color: rgb(51, 153, 255);  ");
        }
        else if(SYSS == "OK")
        {
            lock.lockForWrite();
            StationStatus = 2;
            lock.unlock();
            ui->pushButton_16->setText("OK");
            ui->pushButton_16->setStyleSheet("font: 60pt ; background-color: rgb(25, 125, 44); color: rgb(248, 248, 255); border-width:0px;   border-style:solid; border-color: rgb(51, 153, 255);  ");
        }
        else if(SYSS == "NOK")
        {
            lock.lockForWrite();
            StationStatus = 1;
            lock.unlock();
            ui->pushButton_16->setText("NOK");
            ui->pushButton_16->setStyleSheet("font: 40pt ; background-color: rgb(255, 0, 0); color: rgb(248, 248, 255); border-width:0px;   border-style:solid; border-color: rgb(51, 153, 255);  ");
        }
        In_Output->writeIOOutput("y_led",false);
        //        ui->label_tiaomastate->setText("");
        systemStatus = 0;
    }
    else
    {
        GTightStatus = 2;
        linkCount = 0;
        lock.lockForWrite();
        StationStatus = 0;
        lock.unlock();
        In_Output->writeIOOutput("y_led",true);
        //        ui->label_tiaomastate->setText(tr("拧紧控制器断开连接"));
        systemStatus = 1;
        ui->label_ss1->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/69.bmp);");
        ui->pushButton_16->setText("Not Ready");
        ui->pushButton_16->setStyleSheet("font: 19pt ; background-color: rgb(250, 0, 0); color: rgb(248, 248, 255); border-width:0px;   border-style:solid; border-color: rgb(51, 153, 255);  ");
    }
}
/************************************/
//PLC connect
//
/************************************/
void MainWindow::PLCIsReady(bool isready)
{
    if(isready)
    {
        qDebug()<<"this is white";
        ui->label_ss3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/68.bmp);");
    }
    else
    {
        qDebug()<<"this is black";
        ui->label_ss3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/69.bmp);");
    }
}

/**********************************连接mysql数据库*/
void ::MainWindow::connectMysql()
{
    db=QSqlDatabase::addDatabase("QMYSQL","mysqlconnectionmatch");
    db.setHostName("localhost");
    db.setDatabaseName("Tighten");
    db.setUserName("root");
    db.setPassword("123456");
    if(!db.open()){
        qDebug()<< db.lastError().text();
        qDebug()<< "mainwindow open mysql fail";
    }else{
        qDebug()<< "mainwindow open mysql ok";
    }
    query = QSqlQuery(db);
    query1 =QSqlQuery(db);

    if(!query.exec("select count(*) from "+Localtable))
    {
        qDebug()<<"Mysql start error";
        system(QString("/usr/local/mysql/bin/myisamchk -c -r -o -f /usr/local/mysql/var/Tighten/"+Localtable+".MYI").toLocal8Bit().data());
        //        signal_mysqlerror_do();
        qDebug()<<"repair Mysql";
    }
    else
        qDebug()<<"mysql normal";
}

void MainWindow::mysqlopen()
{
    if(QSqlDatabase::contains("mysqlconnectionmatch")){
        db = QSqlDatabase::database("mysqlconnectionmatch");
    }else{
        db = QSqlDatabase::addDatabase("QMYSQL","mysqlconnectionmatch");
        db.setHostName("localhost");
        db.setDatabaseName("Tighten");
        db.setUserName("root");
        db.setPassword("123456");
        query = QSqlQuery(db);
        query1 =QSqlQuery(db);
    }

    if(!db.open())
    {
        if(!db.open())
        {
            qDebug()<< "mainwindow localmysql "<< db.lastError().text();
        }else
        {
            qDebug()<< "mainwindow localmysql open ok 2";
        }
    }else
    {
        //qDebug()<< "mainwindow localmysql open ok 1";
    }
}

void MainWindow::mysqlclose()
{
    if(db.isOpen())
        db.close();
}

/****************************/
//flags: =1 code valide
//       =0 code invalide
//whichtmp:choose car number
/****************************/
void MainWindow::VinPinCodeView(bool flags, int whichtmp)
{
    if(flags)
    {
        int channel;
        int kxuannumtmp = 0;
        QString outBuf[4];
        for(int i=0;i<4;i++)  //clear car buff information
        {
            outBuf[i] = "0";
        }
        qDebug()<<"112222222222222222222";
        tempnumdpm = 0;
        groupNumCh = 0;
        groupAllBoltNumCh = 0;
        carInfor.pdmName = "";
        for(int j=0;j<20;j++)
        {
            carInfor.proNo[j] = "";
            carInfor.boltNum[j] = 0;
            carInfor.ttNum[j] = 0;
            carInfor.IONum[j] = 0;
            carInfor.barcodeEnable[j] = false;
        }

        BoltOrder[0] = 1;
        BoltOrder[1] = 1;
        whichar = whichtmp;
        currentFirstBolt = 0;
        VIN_PIN_SQL = serialNums;
        QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
        optionOrNot = config->value(QString("carinfo").append(QString::number(whichar)).append("/IsOptionalStation")).toInt();
        Type = config->value(QString("carinfo").append(QString::number(whichar)).append("/carcx")).toString();//car name
        //匹配成功,再判断是否选配
        if(optionOrNot)
        {
            //选配匹配 数据库中的vin码
            //取出选配，查询FIS 数据库，比对 必选fis信息
            QString  tmpserial;

            if(serialNums.length()==14)
            {

                selectVin = "select FisMatch from "+tablePreview+" where KNR ='";
                tmpserial = serialNums.right(8);
            }
            else if(serialNums.length()==17)
            {

                selectVin = "select FisMatch from "+tablePreview+" where VIN ='";

                tmpserial = serialNums;
            }

            query.exec(selectVin+tmpserial.append("'"));
            if(query.next())  //查到了匹配
            {
                qDebug() << "****select KNR******" << selectVin + tmpserial;
                QString Style ="";
                QByteArray jsonData;
                QJson::Parser parser;
                bool ok = false;
                QVariantMap FisMatch;

                jsonData = query.value(0).toByteArray();
                qDebug()<< "jsonData" <<jsonData;
                FisMatch = parser.parse(jsonData, &ok).toMap();
                if (!ok) {
                    qDebug()<<"An error occurred during parsing"<<jsonData;
                }

                record = query.record();
                int k =1;
                for(k=1;k<6;k++)//optional NO (1~5)    选配
                {
                    equeloptionbnum = 0;
                    equeloptionknum = 0;
                    bxuannumtmp = 0;
                    int boltNumber = 0; //bolt number
                    for(int n = 1;n < 21;n++) //optional program NO and bolt number
                    {
                        boltNumber = config->value(QString("carinfo").append(QString::number(whichar)).append("/OPLSNumber").append(QString::number(k)).append(QString::number(n))).toInt();
                        if(boltNumber)
                        {
                            break;
                        }
                    }
                    whichoption = k;
                    qDebug() << "****optiongal para******" << k << boltNumber ;
                    if(boltNumber) //bolt number > 0
                    {
                        for(int t = 1;t < 6;t++)//optional       必选
                        {
                            QString bxname = config->value(QString("carinfo").append(QString::number(whichar)).append("/bxuanname").append(QString::number(whichoption)).append(QString::number(t))).toString();
                            QString bxcode = config->value(QString("carinfo").append(QString::number(whichar)).append("/bxuancode").append(QString::number(whichoption)).append(QString::number(t))).toString();
                            if(bxname!="" && bxcode!="")
                            {
                                qDebug()<<"bxname"<<bxname<<"bxcode"<<bxcode;
                                bxuannumtmp++;
                                if(ok)
                                {
                                    if(FisMatch[bxname].toString() == bxcode)
                                    {
                                        equeloptionbnum++;
                                    }
                                    else
                                    {
                                        qDebug() << "bixuan match fail" ;
                                    }
                                }

                            }
                            else //parameter name NULL
                            {}
                        }
                        equeloptionknum = 0;
                        kxuannumtmp = 0;
                        if(bxuannumtmp == equeloptionbnum)//bixuan parameter equal
                        {
                            qDebug()<<"bixuan equal";
                            for(int j = 1;j < 6;j++)     // 可选
                            {
                                QString kxname = config->value(QString("carinfo").append(QString::number(whichar)).append("/kxuanname").append(QString::number(whichoption)).append(QString::number(j))).toString();
                                QString kxcode = config->value(QString("carinfo").append(QString::number(whichar)).append("/kxuancode").append(QString::number(whichoption)).append(QString::number(j))).toString();
                                if(kxname != "" && kxcode != "")
                                {
                                    qDebug()<<"kxname"<<kxname<<"kxcode"<<kxcode;
                                    kxuannumtmp ++;

                                    if(ok)
                                    {
                                        if(FisMatch[kxname].toString() == kxcode)
                                        {
                                            qDebug()<<"kexuan success";
                                            equeloptionknum = 5;
                                        }
                                        else
                                        {
                                            qDebug() << "kexuan match fail" ;
                                            equeloptionknum = 0;
                                        }
                                    }


                                    for(int m = 0;m< record.count();m++)
                                    {
                                        field = record.field(m);
                                        if(field.name() == kxname)
                                        {
                                            if(field.value().toString() == kxcode)
                                            {
                                                equeloptionknum = 5;
                                                break;
                                            }
                                            else
                                            {
                                                qDebug() << "kexuan match fail" ;
                                                equeloptionknum = 0;
                                            }
                                        }
                                        else
                                        {}
                                    }

                                    if(equeloptionknum == 5)//kexuan success jump out for(int j = 1;j < 6;j++)
                                    {
                                        break;
                                    }
                                }
                                else
                                {
                                    equeloptionknum ++;
                                }
                            }
                            if( (bxuannumtmp == 0)&&(kxuannumtmp == 0) ) //bixuan and kexuan name all name NULL
                            {
                                equeloptionknum = 0;
                                qDebug() << "bixuan and kexuan name all name NULL";
                            }
                            else if(equeloptionknum==5)
                            {
                                break; //jump out optional NO (1~5)
                            }
                        }
                        else //bixuan parameter optional error
                        {}
                    }
                    else
                    {}  //bolt number = 0  next optional
                }
                if( (equeloptionbnum == bxuannumtmp) && (equeloptionknum == 5))
                {
                    qDebug() << "optional success" << whichar << whichoption<<equeloptionbnum;

                    carInfor.pdmName =  config->value(QString("carinfo").append(QString::number(whichar)).append("/pdmyinyong").append(QString::number(whichoption).append("1"))).toString();
                    carInfor.pdmName =  config->value(QString("carinfo").append(QString::number(whichar)).append("/pdmyinyong").append(QString::number(whichoption).append("2"))).toString();
                    for(int j = 1;j< 21;j++)
                    {
                        channel = config->value(QString("carinfo").append(QString::number(whichar)).append("/OPChannel").append(QString::number(whichoption)).append(QString::number(j))).toInt();
                        if(channel == 1)
                        {
                            carInfor.proNo[j-1] = config->value(QString("carinfo").append(QString::number(whichar)).append("/OPProNum").append(QString::number(whichoption)).append(QString::number(j))).toString();
                            carInfor.boltNum[j-1] = config->value(QString("carinfo").append(QString::number(whichar)).append("/OPLSNumber").append(QString::number(whichoption)).append(QString::number(j))).toInt();
                            if((carInfor.proNo[j-1].toInt())&&(carInfor.boltNum[j-1]))
                            {
                                carInfor.boltSN[j-1] = config->value(QString("carinfo").append(QString::number(whichar)).append("/OPLuoSuanNum").append(QString::number(whichoption)).append(QString::number(j))).toString();
                                carInfor.ttNum[j-1]  = config->value(QString("carinfo").append(QString::number(whichar)).append("/OPTaotong").append(QString::number(whichoption)).append(QString::number(j))).toInt();
                                qDebug()<<"000000000*"<<carInfor.boltSN[j-1];
                                qDebug()<<"000000000**"<<carInfor.proNo[j-1];
                                qDebug()<<"000000000***"<<carInfor.boltNum[j-1];
                                qDebug()<<"000000000****"<<carInfor.ttNum[j-1];
                                groupAllBoltNumCh += carInfor.boltNum[j-1];
                            }
                        }
                    }
                    BoltTotalNum = groupAllBoltNumCh ;
                    if(BoltTotalNum == 0)
                    {
                        systemStatus = 14;
                    }
                }
                else //optional fail
                {
                    qDebug() << "***********optional fail***************"<< equeloptionbnum << bxuannumtmp << equeloptionknum;
                    equeloptionknum = 0;
                    equeloptionbnum = 0;
                    bxuannumtmp = 0;
                    //                    ui->label_tiaomastate->setText(tr("选配匹配失败"));
                    qDebug()<<"optional match fail";
                    systemStatus = 11;
                    ISmaintenance = false;
                    gunPower(true);

                }
            }
            else//local fis fail
            {
                qDebug() << "***********local fis fail***************";
                //                ui->label_tiaomastate->setText(tr("本地无此车信息"));
                systemStatus = 12;
                ISmaintenance = false;
                qDebug()<<"local fis match fail";
                gunPower(true);
            }
        }
        else
        {
            //不选配
            //设置获取匹配成功后的 程序号
            //再判断是否有套筒
            //判断是否有套筒

            carInfor.pdmName =  config->value(QString("carinfo").append(QString::number(whichar)).append("/pdmyinyong0")).toString();
            for(int m=1;m<21;m++)
            {
                channel = config->value(QString("carinfo").append(QString::number(whichar)).append("/Channel").append(QString::number(m))).toInt();
                carInfor.proNo[m-1] = config->value(QString("carinfo").append(QString::number(whichar)).append("/ProNum").append(QString::number(m))).toString();
                carInfor.boltNum[m-1] = config->value(QString("carinfo").append(QString::number(whichar)).append("/LSNumber").append(QString::number(m))).toInt();
                if((carInfor.proNo[m-1].toInt())&&(carInfor.boltNum[m-1]))
                {
                    carInfor.boltSN[m-1] = config->value(QString("carinfo").append(QString::number(whichar)).append("/LuoSuanNum").append(QString::number(m))).toString();
                    carInfor.ttNum[m-1] = config->value(QString("carinfo").append(QString::number(whichar)).append("/Taotong").append(QString::number(m))).toInt();
                    carInfor.IONum[m-1] = config->value(QString("carinfo").append(QString::number(whichar)).append("/Channel").append(QString::number(m))).toInt();
                    carInfor.barcodeEnable[m-1] = config->value(QString("carinfo").append(QString::number(whichar)).append("/Barcode").append(QString::number(m))).toBool();
                    qDebug()<<"000000000*"<<carInfor.boltSN[m-1];
                    qDebug()<<"000000000**"<<carInfor.proNo[m-1];
                    qDebug()<<"000000000***"<<carInfor.boltNum[m-1];
                    qDebug()<<"000000000****"<<carInfor.ttNum[m-1];
                    qDebug()<<"000000000***"<<carInfor.IONum[m-1];
                    qDebug()<<"GGGGGGGGGGG";
                    groupAllBoltNumCh += carInfor.boltNum[m-1];
                }
            }
            BoltTotalNum = groupAllBoltNumCh;
        }
        if(groupAllBoltNumCh > 0) //read program group and system ING
        {
            ui->stackedWidget_6->setCurrentIndex(0);
            int j = 0;
            for(j = 0;j< 20;j++)
            {
                if(carInfor.boltNum[j]>0)//bolt number
                {
                    QString pathpdm;
                    int k = 1;
                    for(k = 1;k<100;k++)
                    {
                        if(carInfor.pdmName == config->value(QString("pdminfo").append(QString::number((k))).append("/pdmname")))
                        {
                            tempnumdpm = config->value(QString("pdminfo").append(QString::number((k))).append("/num")).toInt();//PDM bolt number
                            NOKflag = false;
                            pathpdm = config->value(QString("pdminfo").append(QString::number((k))).append("/pdmpath")).toString();//PDM name
                            break;
                        }
                    }
                    ui->label_pdmarea->setStyleSheet(QString("border-image: url(/PDM/").append(pathpdm).append(")"));
                    ui->label_pdmarea->clear();
                    /************发给web端空白螺栓****************/
                    for(int i = 0;i < tempnumdpm;i++)
                    {
                        butt[i] = new QPushButton(ui->stackedWidget_2);
                        label1[i] = new QLabel(ui->stackedWidget_2);
                        label2[i] = new QLabel(ui->stackedWidget_2);

                        butt[i]->raise();
                        butt[i]->setFlat(true);
                        label1[i]->setAlignment(Qt::AlignLeft);
                        label2[i]->setAlignment(Qt::AlignLeft);
                        butt[i]->setFocusPolicy(Qt::NoFocus);
                        label1[i]->setFocusPolicy(Qt::NoFocus);
                        label2[i]->setFocusPolicy(Qt::NoFocus);
                        butt[i]->setAttribute(Qt::WA_TransparentForMouseEvents);
                        label1[i]->setAttribute(Qt::WA_TransparentForMouseEvents);
                        label2[i]->setAttribute(Qt::WA_TransparentForMouseEvents);

                        butt[i]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/w01.png);font: 18pt;");
                        butt[i]->setText(QString::number((i+1)));

                        int tempx = config->value(QString("pdminfo").append(QString::number(k)).append("/tempx").append(QString::number((i+1)))).toInt();
                        int tempy =  config->value(QString("pdminfo").append(QString::number(k)).append("/tempy").append(QString::number((i+1)))).toInt();
                        double x=0.0;
                        double y=0.0;
                        x = (double)tempx/1000 * 1166;
                        y = (double)tempy/1000 * 598;

                        butt[i]->setGeometry(x,y,46,46);
                        label1[i]->setGeometry(x+23,y,140,23);
                        label2[i]->setGeometry(x+23,y+23,140,23);
                        label1[i]->setStyleSheet("background:transparent;");
                        label2[i]->setStyleSheet("background:transparent;");
                        butt[i]->show();
                        label1[i]->show();
                        label2[i]->show();
                    }
                    SYSS = "ING";
                    groupNumCh = j;//boltSN number
                    taotong_Value = carInfor.ttNum[j];
                    IO_value = carInfor.IONum[j];
                    barcode_Value = carInfor.barcodeEnable[j];
                    ui->label_pronum->setText(carInfor.proNo[j]);
                    ui->label_lsnum->setText(QString::number(carInfor.boltNum[j]));
                    taotongEnValue = 0;
                    if(taotong_Value > 0 && taotong_Value < 9)
                    {
                        enTaotongFlag = true;
                    }
                    else
                    {
                        taotongEnValue |= 0x01;
                        enTaotongFlag = false;
                    }
                    if( (IO_value > 0)&&(IO_value < 6) ) //collect IO variable
                    {
                        enIOCtlFlag = true;
                    }
                    else
                    {
                        taotongEnValue |= 0x02;
                        enIOCtlFlag = false;
                    }
                    if(barcode_Value)
                    {
                        enBarcodeFlag = true;
                    }
                    else
                    {
                        taotongEnValue |= 0x04;
                        enBarcodeFlag = false;
                    }
                    if(enTaotongFlag||enIOCtlFlag||enBarcodeFlag)
                    {
                        if(enTaotongFlag||enIOCtlFlag)
                        {
                            TaoTongState = true;
                            emit send_TT_IO_value(taotong_Value,IO_value,groupNumCh);
                            qDebug() << "here waitting taotong"<<taotong_Value<<IO_value;
                        }
                        break;
                    }
                    else
                    {
                        TaoTongState = false;
                        emit sendOperate1(true,j);
                        qDebug()<<"*******send enable ch1*********group:"<<j;
                        break;
                    }
                }
                else
                {
                    continue;
                }
            }
            if(j == 20)
            {
                systemStatus = 14;//螺栓数量为0
            }
            if(SYSS == "ING")
            {
                qDebug() <<"*********** SYSS ing **********";
                emit sendnexo(serialNums);//download VIN
                ui->progressBar->setValue(0);
                m_CurrentValue  = 0;
                progressBarStart(100, Tacktime*10);
                ISmaintenance = true;
                QualifiedNum = 0;
                whichpdmnumnow = 0;
                timerpdm.start(500);     //PDM bolt flick
                ui->pushButton_16->setStyleSheet("font: 50pt ; background-color: rgb(250, 225, 0); color: rgb(248, 248, 255); border-width:0px;   border-style:solid; border-color: rgb(51, 153, 255);  ");
                ui->pushButton_16->setText("ING");
                In_Output->writeIOOutput("w_led",true);
            }
            else
            {
                qDebug() << "*******vin invalid********";
                ISmaintenance = false;
                gunPower(true);
            }
        }
        delete config;
    }
    else//vin match fail
    {
        systemStatus = 13;
        ISmaintenance = false;
        qDebug()<<"################vin match fail";
        gunPower(true);
        emit sendCmdToStep(2,SYSS,1);//step type2 finish
    }
}

/****************************/
//flags: =1 code valide
//       =0 code invalide
//whichtmp:choose car number
/****************************/
void MainWindow::VinPinCodeViewSB356(QString vinCode, int channelNum,int count)
{
    ui->label_17->setText(vinCode);
    if(timerpdm.isActive())
        timerpdm.stop();
    ui->label_hege->setText("");
    pdmnowromisOk = true;
    In_Output->writeIOOutput("r_led",false);
    In_Output->writeIOOutput("g_led",false);

    for(int j=0;j<tempnumdpm;j++)
    {
        delete butt[j];
        delete label1[j];
        delete label2[j];
    }
    PDMCurrentState = "OK";
    qDebug()<<"113333333333333333333"<<vinCode<<channelNum<<count;
    tempnumdpm = 0;
    groupNumCh = 0;
    groupAllBoltNumCh = 0;
    carInfor.pdmName = "";
    for(int j=0;j<20;j++)
    {
        carInfor.proNo[j] = "";
        carInfor.boltNum[j] = 0;
        carInfor.ttNum[j] = 0;
        carInfor.IONum[j] = 0;
        carInfor.barcodeEnable[j] = false;
    }
    BoltOrder[0] = 1;
    BoltOrder[1] = 1;
    whichar = count;
    QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
    Type = config->value(QString("carinfo").append(QString::number(whichar)).append("/carcx")).toString();//car name
    carInfor.pdmName =  config->value(QString("carinfo").append(QString::number(whichar)).append("/pdmyinyong0")).toString();
    for(int m=1;m<21;m++)
    {
        carInfor.proNo[m-1] = config->value(QString("carinfo").append(QString::number(whichar)).append("/ProNum").append(QString::number(m))).toString();
        carInfor.boltNum[m-1] = config->value(QString("carinfo").append(QString::number(whichar)).append("/LSNumber").append(QString::number(m))).toInt();
        if((carInfor.proNo[m-1].toInt())&&(carInfor.boltNum[m-1]))
        {
            carInfor.boltSN[m-1] = config->value(QString("carinfo").append(QString::number(whichar)).append("/LuoSuanNum").append(QString::number(m))).toString();
            carInfor.ttNum[m-1] = config->value(QString("carinfo").append(QString::number(whichar)).append("/Taotong").append(QString::number(m))).toInt();
            carInfor.IONum[m-1] = config->value(QString("carinfo").append(QString::number(whichar)).append("/Channel").append(QString::number(m))).toInt();
            carInfor.barcodeEnable[m-1] = config->value(QString("carinfo").append(QString::number(whichar)).append("/Barcode").append(QString::number(m))).toBool();
            qDebug()<<"000000000*"<<carInfor.boltSN[m-1];
            qDebug()<<"000000000**"<<carInfor.proNo[m-1];
            qDebug()<<"000000000***"<<carInfor.boltNum[m-1];
            qDebug()<<"000000000****"<<carInfor.ttNum[m-1];
            qDebug()<<"000000000***"<<carInfor.IONum[m-1];
            qDebug()<<"TTTTTTTTTT";
            groupAllBoltNumCh += carInfor.boltNum[m-1];
            if(m == 1)
            {
                bool ch[5];
                QString strChannel = "";
                ch[0] = config->value(QString("carinfo").append(QString::number(whichar)).append("/ch0_1")).toBool();
                ch[1] = config->value(QString("carinfo").append(QString::number(whichar)).append("/ch1_1")).toBool();
                ch[2] = config->value(QString("carinfo").append(QString::number(whichar)).append("/ch2_1")).toBool();
                ch[3] = config->value(QString("carinfo").append(QString::number(whichar)).append("/ch3_1")).toBool();
                ch[4] = config->value(QString("carinfo").append(QString::number(whichar)).append("/ch4_1")).toBool();
                for(int i=0;i<5;i++)
                {
                    if(ch[i])
                    {
                        strChannel.append(QString::number(i+1));
                    }
                }
                channelNum = strChannel.toInt();
                qDebug()<<"************************&&&&&&&&&&&&&&&&&&&"<<strChannel<<channelNum;
            }
        }
    }
    BoltTotalNum = groupAllBoltNumCh;
    int j = 0;
    if(groupAllBoltNumCh > 0) //read program group and system ING
    {
        ui->stackedWidget_6->setCurrentIndex(0);
        for(j = 0;j< 20;j++)
        {
            if(carInfor.boltNum[j]>0)//bolt number
            {
                QString pathpdm;
                int k = 1;
                for(k = 1;k<100;k++)
                {
                    if(carInfor.pdmName == config->value(QString("pdminfo").append(QString::number((k))).append("/pdmname")))
                    {
                        tempnumdpm = config->value(QString("pdminfo").append(QString::number((k))).append("/num")).toInt();//PDM bolt number
                        NOKflag = false;
                        pathpdm = config->value(QString("pdminfo").append(QString::number((k))).append("/pdmpath")).toString();//PDM name
                        break;
                    }
                }
                ui->label_pdmarea->setStyleSheet(QString("border-image: url(/PDM/").append(pathpdm).append(")"));
                ui->label_pdmarea->clear();
                /************发给web端空白螺栓****************/
                for(int i = 0;i < tempnumdpm;i++)
                {
                    butt[i] = new QPushButton(ui->stackedWidget_2);
                    label1[i] = new QLabel(ui->stackedWidget_2);
                    label2[i] = new QLabel(ui->stackedWidget_2);

                    butt[i]->raise();
                    butt[i]->setFlat(true);
                    label1[i]->setAlignment(Qt::AlignLeft);
                    label2[i]->setAlignment(Qt::AlignLeft);
                    butt[i]->setFocusPolicy(Qt::NoFocus);
                    label1[i]->setFocusPolicy(Qt::NoFocus);
                    label2[i]->setFocusPolicy(Qt::NoFocus);
                    butt[i]->setAttribute(Qt::WA_TransparentForMouseEvents);
                    label1[i]->setAttribute(Qt::WA_TransparentForMouseEvents);
                    label2[i]->setAttribute(Qt::WA_TransparentForMouseEvents);

                    butt[i]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/w01.png);font: 18pt;");
                    butt[i]->setText(QString::number((i+1)));

                    int tempx = config->value(QString("pdminfo").append(QString::number(k)).append("/tempx").append(QString::number((i+1)))).toInt();
                    int tempy =  config->value(QString("pdminfo").append(QString::number(k)).append("/tempy").append(QString::number((i+1)))).toInt();
                    double x=0.0;
                    double y=0.0;
                    x = (double)tempx/1000 * 1166;
                    y = (double)tempy/1000 * 598;

                    butt[i]->setGeometry(x,y,46,46);
                    label1[i]->setGeometry(x+23,y,140,23);
                    label2[i]->setGeometry(x+23,y+23,140,23);
                    label1[i]->setStyleSheet("background:transparent;");
                    label2[i]->setStyleSheet("background:transparent;");
                    butt[i]->show();
                    label1[i]->show();
                    label2[i]->show();
                }
                ui->label_pronum->setText(carInfor.proNo[j]);
                ui->label_lsnum->setText(QString::number(carInfor.boltNum[j]));
                SYSS = "ING";
                emit sendOperate1(true,channelNum);
                ui->pushButton_16->setStyleSheet("font: 50pt ; background-color: rgb(250, 225, 0); color: rgb(248, 248, 255); border-width:0px;   border-style:solid; border-color: rgb(51, 153, 255);  ");
                ui->pushButton_16->setText("ING");
                In_Output->writeIOOutput("w_led",true);
                qDebug() <<"*********** SYSS ing **********";
                ui->progressBar->setValue(0);
                m_CurrentValue  = 0;
                progressBarStart(100, Tacktime*10);
                QualifiedNum = 0;
                whichpdmnumnow = 0;
                timerpdm.start(500);     //PDM bolt flick
                break;
            }
        }
    }
    else
    {
        systemStatus = 14;//螺栓数量为0
    }
    delete config;
}
/*****************匹配G9码*********************/
void MainWindow::searchG9Num()
{
    //匹配G9
    int i = 0;
    QString temp;
    bool flagss = false;
    QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
    for(i = 1;i < 21;i++)
    {
        temp = config->value(QString("carinfo").append(QString::number(i)).append("/G9")).toString();
        //qDebug()<<"this is G9 to temp"<<temp;
        qDebug()<<"this is test G9"<<tempG9<<tempG9.size()<<temp;
        if(temp.length()!=tempG9.size())    //change 2018-0427
//        if(temp.length()!=4)
            continue;
        flagss = true;       
        for(int j=0;j<tempG9.size();j++)    //change 2018-0427
//        for(int j=0;j<4;j++)    //change 2018-0427
        {
            if(temp.at(j) == '?')
                continue;
            else
            {
                if(temp.at(j) == tempG9.at(j))
                {
                    flagss = true;
                    continue;
                }
                else
                {
                    flagss = false;
                    break;
                }
            }
        }
        if(flagss)
        {
            break;
        }

    }
    delete config;
    VinPinCodeView(flagss,i);
}

/*******************匹配VIN码***************************/
void MainWindow::searchVinNum()
{
    //匹配VIN码
    QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
    bool flags = false;
    int i = 1;

    vinAttributeCode = serialNums.mid(vinAttributeBit,5);
    for(i = 1;i < 21;i++)
    {
        QString temp = config->value(QString("carinfo").append(QString::number(i)).append("/VIN")).toString();
        if(temp.length()!=5)
        {
            continue;
        }
        flags = true;
        for(int k = 0;k < 5;k++)
        {
            if(temp.at(k) == '?')
            {
                continue;
            }
            else
            {
                if(vinAttributeCode.at(k) != temp.at(k))
                {
                    flags = false;
                    break;
                }
                else
                {
                    continue;
                }
            }

        }
        if(flags)
        {
            break;
        }
    }
    delete config;
    VinPinCodeView(flags,i);

}
/*******************校验PIN码*************************/
bool MainWindow::pinCodeVerify(QByteArray pinBuf,int len)
{
    int sum,sumOdd,sumEven,result,i;
    QByteArray PinTempBuf;
    sum = 0;
    sumOdd = 0;
    sumEven = 0;
    for(i=0;i<len;i++)
    {
        switch(pinBuf[i])
        {
        case '0':
            PinTempBuf[i] = 0;
            break;
        case '1':
        case 'A':
        case 'J':
        case 'S':
            PinTempBuf[i] = 1;
            break;
        case '2':
        case 'B':
        case 'K':
        case 'T':
            PinTempBuf[i] = 2;
            break;
        case '3':
        case 'C':
        case 'L':
        case 'U':
            PinTempBuf[i] = 3;
            break;
        case '4':
        case 'D':
        case 'M':
        case 'V':
            PinTempBuf[i] = 4;
            break;
        case '5':
        case 'E':
        case 'N':
        case 'W':
            PinTempBuf[i] = 5;
            break;
        case '6':
        case 'F':
        case 'O':
        case 'X':
            PinTempBuf[i] = 6;
            break;
        case '7':
        case 'G':
        case 'P':
        case 'Y':
            PinTempBuf[i] = 7;
            break;
        case '8':
        case 'H':
        case 'Q':
        case 'Z':
            PinTempBuf[i] = 8;
            break;
        case '9':
        case 'R':
        case 'I':
            PinTempBuf[i] = 9;
            break;
        default:
            break;
        }
    }
    for(i=0;i<len-1;i+=2)
    {
        sumOdd += PinTempBuf[i];
    }
    for(i=1;i<len-1;i+=2)
    {
        sumEven += PinTempBuf[i];
    }
    sum = sumOdd*3 + sumEven;
    result = (10-sum%10)%10;
    if(result == PinTempBuf[len-1])
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool MainWindow::pinCodeRightVerify(QString pinBuf1,int len)
{
    int sum,sumOdd,sumEven,result,i;
    QByteArray pinTemp;
    QString pinTempBuf="87654321",pinBuf = "12345678";
    pinBuf = pinBuf1.mid(6,len);
    sum = 0;
    sumOdd = 0;
    sumEven = 0;
    for(i=0;i<len;i++)
    {
        pinTemp = pinBuf.mid(i,1).toLatin1();
        switch(pinTemp[0])
        {
        case '0':
            pinTempBuf.replace(i,1,"0");
            break;
        case '1':
        case 'A':
        case 'J':
        case 'S':
            pinTempBuf.replace(i,1,"1");
            break;
        case '2':
        case 'B':
        case 'K':
        case 'T':
            pinTempBuf.replace(i,1,"2");
            break;
        case '3':
        case 'C':
        case 'L':
        case 'U':
            pinTempBuf.replace(i,1,"3");
            break;
        case '4':
        case 'D':
        case 'M':
        case 'V':
            pinTempBuf.replace(i,1,"4");
            break;
        case '5':
        case 'E':
        case 'N':
        case 'W':
            pinTempBuf.replace(i,1,"5");
            break;
        case '6':
        case 'F':
        case 'O':
        case 'X':
            pinTempBuf.replace(i,1,"6");
            break;
        case '7':
        case 'G':
        case 'P':
        case 'Y':
            pinTempBuf.replace(i,1,"7");
            break;
        case '8':
        case 'H':
        case 'Q':
        case 'Z':
            pinTempBuf.replace(i,1,"8");
            break;
        case '9':
        case 'R':
        case 'I':
            pinTempBuf.replace(i,1,"9");
            break;
        default:
            break;
        }
    }
    for(i=0;i<len-1;i+=2)
    {
        sumOdd += pinTempBuf.mid(i,1).toInt();
    }
    for(i=1;i<len-1;i+=2)
    {
        sumEven += pinTempBuf.mid(i,1).toInt();
    }
    sum = sumOdd*3 + sumEven;
    result = (10-sum%10)%10;
    if(result == pinTempBuf.mid(len-1,1).toInt())
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*******************校验VIN码*************************/
int MainWindow::VIN_VerifyFunc( char *buf)
{
    //校验VIN码
    unsigned char i;
    unsigned char VinTempBuf[17];
    unsigned short  Sum_Num;

    for(i=0;i<3;i++)
    {
        if(buf[i]!=VINhead[i])
            return 0;
    }

    for(i=0;i<6;i++)//right 6 bit range(0~9)
    {
        if((buf[i+11]>'9')||(buf[i+11]<'0'))
        {
            return 0;
        }
    }

    for(i=0;i<17;i++)
    {
        if(i == 8)
        {
            if(buf[i]=='X') //校验位不需要
            {
                VinTempBuf[i] = 'X';
                continue;
            }
        }
        switch(buf[i])
        {
        case '0':
            VinTempBuf[i] = 0;
            break;
        case '1':
        case 'A':
        case 'J':
            VinTempBuf[i] = 1;
            break;
        case '2':
        case 'B':
        case 'K':
        case 'S':
            VinTempBuf[i] = 2;
            break;
        case '3':
        case 'C':
        case 'L':
        case 'T':
            VinTempBuf[i] = 3;
            break;
        case '4':
        case 'D':
        case 'M':
        case 'U':
            VinTempBuf[i] = 4;
            break;
        case '5':
        case 'E':
        case 'N':
        case 'V':
            VinTempBuf[i] = 5;
            break;
        case '6':
        case 'F':
        case 'W':
            VinTempBuf[i] = 6;
            break;
        case '7':
        case 'G':
        case 'P':
        case 'X':
            VinTempBuf[i] = 7;
            break;
        case '8':
        case 'H':
        case 'Y':
            VinTempBuf[i] = 8;
            break;
        case '9':
        case 'R':
        case 'Z':
            VinTempBuf[i] = 9;
            break;
        default:
            return 0;//Vin code valid
            //				break;
        }
    }
    Sum_Num  = VinTempBuf[7]*10;
    Sum_Num += VinTempBuf[9]*9;
    Sum_Num += (VinTempBuf[0]+VinTempBuf[10])*8;
    Sum_Num += (VinTempBuf[1]+VinTempBuf[11])*7;
    Sum_Num += (VinTempBuf[2]+VinTempBuf[12])*6;
    Sum_Num += (VinTempBuf[3]+VinTempBuf[13])*5;
    Sum_Num += (VinTempBuf[4]+VinTempBuf[14])*4;
    Sum_Num += (VinTempBuf[5]+VinTempBuf[15])*3;
    Sum_Num += (VinTempBuf[6]+VinTempBuf[16])*2;
    Sum_Num %=11; //求余数
    qDebug()<<"***********sum"<<Sum_Num<<VinTempBuf[8];
    if(Sum_Num == 10)
    {
        if(VinTempBuf[8] == 'X')
            return 1;//Vin code valid
        else
            return 0;//Vin code invalid
    }
    else
    {
        if(VinTempBuf[8] == Sum_Num)
            return 1;//Vin code valid
        else
            return 0;//Vin code invalid
    }
}

/********************获取扫描枪的条码**************************/
void MainWindow::getSerialNum(QString serialNum,bool equeled,QString tempp)
{
    qDebug()<<"***********************&&&&&&&&&&&&&&&&&&&&&"<<serialNum;
    systemStatus = 0;//clear error status
    serialNums = serialNum;
    if(1)      //RFID
    {  // 接收RFID G9 pin
        if(serialNums =="0"||serialNums =="-1")
            return;
        tempG9  = tempp;
        ui->label_17->setText(serialNums);
        if(equeled)//pin码重复
        {
            systemStatus = 9;
        }
        else //不重复匹配
        {
            if(SYSS == "OK" || SYSS == "NOK")
            {
                if(timerpdm.isActive())
                    timerpdm.stop();
                QualifiedNum = 0;
                ui->label_hege->setText("");
                pdmnowromisOk = true;
                In_Output->writeIOOutput("w_led",false);
                In_Output->writeIOOutput("r_led",false);
                In_Output->writeIOOutput("g_led",false);

                for(int j=0;j<tempnumdpm;j++)
                {
                    delete butt[j];
                    delete label1[j];
                    delete label2[j];
                }
                qDebug()<<"114444444444444444444444";
                tempnumdpm=0;
                PDMCurrentState = "OK";
            }
            searchG9Num();
        }
    }
    else if(isBarCode) //条码枪
    {
        //接收ＶＩＮ
        serialNums = serialNum;
        //设置条码
        ui->label_17->setText(serialNums);
        if(equeled)
        {
            systemStatus = 9;
            gunPower(true);
            ISmaintenance = false;
        }
        else
        {
            qDebug()<<"*****************************************"<<serialNums;
            if(SYSS == "OK" || SYSS == "NOK")
            {
                if(timerpdm.isActive())
                    timerpdm.stop();
                QualifiedNum = 0;
                ui->label_hege->setText("");
                pdmnowromisOk = true;
                In_Output->writeIOOutput("w_led",false);
                In_Output->writeIOOutput("r_led",false);
                In_Output->writeIOOutput("g_led",false);

                for(int j=0;j<tempnumdpm;j++)
                {
                    delete butt[j];
                    delete label1[j];
                    delete label2[j];
                }
                qDebug()<<"115555555555555555555";
                tempnumdpm=0;
                PDMCurrentState = "OK";

            }
            searchVinNum();
        }
    }
}

void MainWindow::closeSave()
{
    ui->label_black->hide();
    delete e3;
    delete save;
    SaveWhat = "";
    isSaveShow = false;
}

MainWindow::~MainWindow()
{
    qDebug()<<"~MainWindow()";
    delete ui;
}

/**********************PDM闪烁*****************************/
//PDMBoltBuf[i]=0,stop;=1,w and y;=2,r and y;=4,g and y
/**********************PDM闪烁*****************************/
void MainWindow::PdmFlicker()
{
    if(pdmflicker)
    {
        pdmflicker = false;
    }
    else
    {
        pdmflicker = true;
    }


    if(whichpdmnumnow == tempnumdpm)
    {
        timerpdm.stop();
    }
    else
    {
        if(whichpdmnumnow != tempnumdpm)
        {
            if(PDMCurrentState == "NOK")
            {
                if(pdmflicker)
                {
                    butt[whichpdmnumnow]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/r01.png);font: 18pt;color:rgb(248,255,255)");
                }
                else
                {
                    butt[whichpdmnumnow]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/y01.png);font: 18pt;");
                }
            }
            else
            {
                if(pdmflicker)
                {
                    butt[whichpdmnumnow]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/y01.png);font: 18pt;");
                }
                else
                {
                    butt[whichpdmnumnow]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/w01.png);font: 18pt;");
                }
            }
        }
    }
}
/**********************一个条码拧紧完成OK后操作*****************************/
void MainWindow::timerDelay()
{
    ui->label_pronum->setText("");
    ui->label_lsnum->setText("");
    if(timerpdm.isActive())
    {
        timerpdm.stop();
    }

    qDebug()<<"this is tempumdpm"<<tempnumdpm;
    for(int j=0;j<tempnumdpm;j++)
    {
        qDebug()<<"hahahahhahahahahahahah";
        delete butt[j];
        delete label1[j];
        delete label2[j];
    }
    for(int j=0;j<airButtonNum;j++)
    {
        delete butt[j];
        delete label1[j];
        delete label2[j];
    }
    airButtonNum = 0;
    tempnumdpm = 0;
    PDMCurrentState = "OK";

    ui->label_hege->setText("");
    BoltOrder[0] = 1;
    BoltOrder[1] = 1;
    SYSS = "Ready";
    if(CsIsConnect)
    {
        ui->pushButton_16->setText("Ready");
        ui->pushButton_16->setStyleSheet("font: 30pt ; background-color: rgb(51, 153, 255); color: rgb(248, 248, 255); border-width:0px;   border-style:solid; border-color: rgb(51, 153, 255);  ");
    }
    QualifiedNum = 0;
    ui->label_hege->setText("");
    pdmnowromisOk = true;
    enablenumberLeft = 0;
    //delete config;
    In_Output->writeIOOutput("w_led",false);
    In_Output->writeIOOutput("r_led",false);
    In_Output->writeIOOutput("g_led",false);
    TaoTongState = false;
}


/*******************************************/
//more channel 1,2,3,4 接收拧紧的结果，做相应的逻辑处理
//ch 0,1,2,3
/*******************************************/
void MainWindow::fromsecondthreaddata(QString MI,QString WI,QString IsOk,int ch,int groupNum)
{
    //0 dat  1 time   2 state   3  扭矩 4 角度 5 螺栓编号   6 vin pin 码   7 循环号  8 曲线

    //status[whichpdmnumnow[0]][4] = data_model.DATE_b+" "+data_model.TIME_b;//螺栓拧紧时间
    if(GBlotErrorCount > 2)//螺栓连续不合格次数
    {
        emit sendOperate1(false,GiswhichChannel);
        GisBoltErrorCountFlag = true;
        ui->label_tishu->show();
    }
    else
    {

    }
    if(IsOk == "NOK") // 本次螺栓不合格nok
    {
        if(whichpdmnumnow < tempnumdpm)
        {
            label1[whichpdmnumnow]->setText(QString("  T:"+MI+"Nm"));
            label2[whichpdmnumnow]->setText(QString("  A:"+WI+"Deg"));
            label1[whichpdmnumnow]->setStyleSheet("font: 14pt;background-color: rgb(248, 248, 248);border-width:1px 1px 0 1px; border-style:solid; border-color:rgb(255, 0, 0);");
            label2[whichpdmnumnow]->setStyleSheet("font: 14pt;background-color: rgb(248, 248, 248);border-width:0 1px 1px 1px; border-style:solid; border-color:rgb(255, 0, 0);");
            butt[whichpdmnumnow]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/r01.png);font: 18pt;color:rgb(248,255,255)");
        }
        In_Output->writeIOOutput("n_led",true);
        PDMCurrentState = "NOK";
    }
    else  //本次螺栓合格ok
    {
        GProgramIndex++;
        //更新螺栓定位配置信息
        emit sendNewCoordinates(GProgramIndex);//更新坐标
        if(MI!= "-1" && WI != "-1")
        {
            QualifiedNum++;//合格数加1
            ui->label_hege->setText(QString::number(QualifiedNum));
        }
        if(carInfor.boltNum[groupNum] > 0)
        {
            carInfor.boltNum[groupNum]--;   //结果ok 数量减1
        }
        if(groupAllBoltNumCh>0)
        {
            groupAllBoltNumCh--;
        }
        if(MI == "-1" && WI == "-1")//nok确认1 个
        {
            pdmnowromisOk = false;
            if(whichpdmnumnow < tempnumdpm)
            {
                butt[whichpdmnumnow]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/r01.png);font: 18pt;color:rgb(248,255,255)");//red button
                if(PDMCurrentState != "NOK")
                {
                    label1[whichpdmnumnow]->setText(QString("  T:-1Nm"));
                    label2[whichpdmnumnow]->setText(QString("  A:-1Deg"));
                    label1[whichpdmnumnow]->setStyleSheet("font: 14pt;background-color: rgb(248, 248, 248);border-width:1px 1px 0 1px; border-style:solid; border-color:rgb(255, 0, 0);");
                    label2[whichpdmnumnow]->setStyleSheet("font: 14pt;background-color: rgb(248, 248, 248);border-width:0 1px 1px 1px; border-style:solid; border-color:rgb(255, 0, 0);");
                    //存入数据库
                }
                whichpdmnumnow++;
            }
        }
        else//正常
        {
            if(whichpdmnumnow < tempnumdpm)
            {
                GBoltOkNum++;
                butt[whichpdmnumnow]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/g01.png);font: 18pt;color:rgb(248,255,255)");//green button
                label1[whichpdmnumnow]->setText(QString("  T:"+MI+"Nm"));
                label2[whichpdmnumnow]->setText(QString("  A:"+WI+"Deg"));
                label1[whichpdmnumnow]->setStyleSheet("font: 14pt;background-color: rgb(248, 248, 248);border-width:1px 1px 0 1px; border-style:solid; border-color:rgb(25, 125, 44);");
                label2[whichpdmnumnow]->setStyleSheet("font: 14pt;background-color: rgb(248, 248, 248);border-width:0 1px 1px 1px; border-style:solid; border-color:rgb(25, 125, 44);");
                whichpdmnumnow++;
            }
        }

        In_Output->writeIOOutput("n_led",false);
        PDMCurrentState = "OK";
        qDebug() << "******carInfor.boltNum[n]***data***"<<carInfor.boltNum[groupNum]<<ch<<groupNum<<GBoltOkNum<<BoltTotalNum<<GProgramIndex;
        if(carInfor.boltNum[groupNum] == 0)
        {
            QualifiedNum = 0;
            groupNum++;
            qDebug() << "********groupNum*****" << groupNum <<groupAllBoltNumCh;
            int k = 0;
            if((groupNum == 21)||(groupAllBoltNumCh==0))
            {
                k = 20;
            }
            else
            {
                for( k = groupNum;k <20;k++)
                {
                    qDebug() << "****send next****" <<carInfor.boltNum[k]<<carInfor.ttNum[k]<<k;
                    if(carInfor.boltNum[k])
                    {
                        groupNumCh = k;
                        taotongEnValue = 0;
                        taotong_Value = carInfor.ttNum[k];
                        IO_value = carInfor.IONum[k];
                        barcode_Value = carInfor.barcodeEnable[k];
                        ui->label_pronum->setText(carInfor.proNo[k]);
                        ui->label_lsnum->setText(QString::number(carInfor.boltNum[k]));
                        if( (taotong_Value > 0) && (taotong_Value < 9) )
                        {
                            enTaotongFlag = true;
                        }
                        else
                        {
                            taotongEnValue |= 0x01;
                            enTaotongFlag = false;
                        }
                        if( (IO_value > 0)&&(IO_value < 6) ) //collect IO variable
                        {
                            enIOCtlFlag = true;
                        }
                        else
                        {
                            taotongEnValue |= 0x02;
                            enIOCtlFlag = false;
                        }
                        if(barcode_Value)
                        {
                            enBarcodeFlag = true;
                        }
                        else
                        {
                            taotongEnValue |= 0x04;
                            enBarcodeFlag = false;
                        }
                        if(enTaotongFlag||enIOCtlFlag||enBarcodeFlag)
                        {
                            if(enTaotongFlag||enIOCtlFlag)
                            {
                                TaoTongState = true;
                                emit send_TT_IO_value(taotong_Value,IO_value,groupNumCh);
                                qDebug() << "here waitting taotong"<<taotong_Value<<IO_value<<taotongEnValue;
                            }
                            break;
                        }
                        else
                        {
                            TaoTongState = false;
                            emit sendOperate1(true,k);
                            qDebug()<<"*******send enable ch1*********group:"<<k;
                            break;
                        }
                    }
                }
            }
            if(k == 20)
            {
                if(!pdmnowromisOk)//NOK
                {
                    lock.lockForWrite();
                    StationStatus = 1;
                    lock.unlock();
                    ui->pushButton_16->setStyleSheet("font: 40pt ; background-color: rgb(255, 0, 0); color: rgb(248, 248, 255); border-width:0px;   border-style:solid; border-color: rgb(51, 153, 255);  ");
                    ui->pushButton_16->setText("NOK");
                    SYSS = "NOK";
                    In_Output->writeIOOutput("r_led",true);
                }
                else//OK
                {
                    lock.lockForWrite();
                    StationStatus =2;
                    lock.unlock();
                    ui->pushButton_16->setText("OK");
                    ui->pushButton_16->setStyleSheet("font: 60pt ; background-color: rgb(25, 125, 44); color: rgb(248, 248, 255); border-width:0px;   border-style:solid; border-color: rgb(51, 153, 255);  ");
                    SYSS = "OK";
                    In_Output->writeIOOutput("g_led",true);
                }
                emit sendOperate1(false,k);
                TaoTongState = false;//bolt finished close taotong
                ISmaintenance = false;
                In_Output->writeIOOutput("w_led",false);
                progressBarStop();//progress Bar stop
                gunPower(true);//open code bar power
                emit sendCmdToStep(2,SYSS,1);//step type2 finish
            }
        }
        else
        {
            if(MI == "-1" && WI == "-1") //more bolt number
            {
                qDebug()<<"this is number bolt"<<groupNum;
                emit sendOperate1(true,groupNum);
            }
        }
    }
}
/*******************************************/
//more channel 1,2,3,4 接收拧紧机发来的部分拧紧数据
//ch 0,1,2,3
/*******************************************/
void MainWindow::fromsecondthreaddataSB356(QString MI,QString WI,QString IsOk,int ch,int groupNum)
{
    if(IsOk == "OK")
    {
        GBoltOkNum++;
        QualifiedNum++;//合格数加1
        ui->label_hege->setText(QString::number(QualifiedNum));
    }
    GMI = (MI.toDouble()*100);
//    if(GErrorTight)
//    {
//        GrecEnableCount++;
//    }
//    GBoltAllNum++;
//    GrecEnableCount++;
//    if(IsOk != "OK")//不合格数据把通道号记下来
//    {
//        GErrorChannel.append(QString::number(ch));
//        qDebug()<<"this is nok"<<GErrorChannel;
//    }
    if(carInfor.boltNum[groupNum] > 0)
    {
        carInfor.boltNum[groupNum]--;   //结果ok 数量减1
    }
    if(groupAllBoltNumCh>0)
    {
        groupAllBoltNumCh--;
    }
    if(IsOk == "NOK") // 本次螺栓不合格nok
    {
        if(whichpdmnumnow < tempnumdpm)
        {
            label1[whichpdmnumnow]->setText(QString("  T:"+MI+"Nm"));
            label2[whichpdmnumnow]->setText(QString("  A:"+WI+"Deg"));
            label1[whichpdmnumnow]->setStyleSheet("font: 14pt;background-color: rgb(248, 248, 248);border-width:1px 1px 0 1px; border-style:solid; border-color:rgb(255, 0, 0);");
            label2[whichpdmnumnow]->setStyleSheet("font: 14pt;background-color: rgb(248, 248, 248);border-width:0 1px 1px 1px; border-style:solid; border-color:rgb(255, 0, 0);");
            butt[whichpdmnumnow]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/r01.png);font: 18pt;color:rgb(248,255,255)");
        }
        In_Output->writeIOOutput("n_led",true);
        pdmnowromisOk = false;
    }
    else//正常
    {
        if(whichpdmnumnow < tempnumdpm)
        {
            label1[whichpdmnumnow]->setText(QString("  T:"+MI+"Nm"));
            label2[whichpdmnumnow]->setText(QString("  A:"+WI+"Deg"));
            label1[whichpdmnumnow]->setStyleSheet("font: 14pt;background-color: rgb(248, 248, 248);border-width:1px 1px 0 1px; border-style:solid; border-color:rgb(25, 125, 44);");
            label2[whichpdmnumnow]->setStyleSheet("font: 14pt;background-color: rgb(248, 248, 248);border-width:0 1px 1px 1px; border-style:solid; border-color:rgb(25, 125, 44);");
            butt[whichpdmnumnow]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/g01.png);font: 18pt;color:rgb(248,255,255)");//green button
        }
        In_Output->writeIOOutput("n_led",false);
    }
    whichpdmnumnow++;
    qDebug() << "******carInfor.boltNum[n]***SB356***"<<carInfor.boltNum[groupNum]<<ch<<groupNum<<"whichpdmnumnow"<<whichpdmnumnow<<GBoltOkNum<<IsOk<<GBoltAllNum;
    if(carInfor.boltNum[groupNum] == 0)
    {
        QualifiedNum = 0;
        groupNum++;
        qDebug() << "********groupNum*****" << groupNum <<groupAllBoltNumCh;
        int k = 0;
        if((groupNum == 21)||(groupAllBoltNumCh==0))
        {
            k = 20;
        }
        else
        {
            for( k = groupNum;k <20;k++)
            {
                qDebug() << "****send next****" <<carInfor.boltNum[k]<<carInfor.ttNum[k]<<k;
                if(carInfor.boltNum[k])
                {
                    groupNumCh = k;
                    ui->label_pronum->setText(carInfor.proNo[k]);
                    ui->label_lsnum->setText(QString::number(carInfor.boltNum[k]));
                    break;
                }
            }
        }
        if(k == 20)
        {
            if(!pdmnowromisOk)//NOK
            {
                ui->pushButton_16->setStyleSheet("font: 40pt ; background-color: rgb(255, 0, 0); color: rgb(248, 248, 255); border-width:0px;   border-style:solid; border-color: rgb(51, 153, 255);  ");
                ui->pushButton_16->setText("NOK");
                SYSS = "NOK";
                In_Output->writeIOOutput("r_led",true);
            }
            else//OK
            {
                ui->pushButton_16->setText("OK");
                ui->pushButton_16->setStyleSheet("font: 60pt ; background-color: rgb(25, 125, 44); color: rgb(248, 248, 255); border-width:0px;   border-style:solid; border-color: rgb(51, 153, 255);  ");
                SYSS = "OK";
                In_Output->writeIOOutput("g_led",true);
            }
            ISmaintenance = false;
            In_Output->writeIOOutput("w_led",false);
            progressBarStop();//progress Bar stop
            emit sendCmdToStep(2,SYSS,1);//step type2 finish
        }
    }
    else
    {}
}
/*******************************************/
//more channel 1,2,3,4
//ch 0,1,2,3
/*******************************************/
void MainWindow::fromsecondthreaddataAirTest(QString MI,QString WI,QString IsOk,int ch)
{

    if(GPackType == "PHEV")
    {
        recCount++;
        if(IsOk == "NOK")
        {

        }
        else
        {
            OKCount++;
        }
        if(recCount >= 2)
        {
            recCount = 0;
            if(OKCount == 2)
            {
                //greenflash
                 onLEDFlashAllOFF(500,"flash_green",true);//合格绿闪
            }
            else
            {
                //redflash
                onLEDFlashAllOFF(500,"flash_red",true);//不合格红闪
            }
            OKCount =0;
        }
    }
    else
    {
        if(IsOk == "NOK")
        {
            // send redflash
            onLEDFlashAllOFF(500,"flash_red",true);//不合格红闪
        }
        else
        {
            //send greenflash
             onLEDFlashAllOFF(500,"flash_green",true);//合格绿闪
        }
    }
    if(IsOk == "NOK") // 本次螺栓不合格nok
    {
        pdmnowromisOk = false;
        butt[ch]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/r01.png);font: 18pt;color:rgb(248,255,255)");
    }
    else//正常
    {
        butt[ch]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/g01.png);font: 18pt;color:rgb(248,255,255)");//green button
    }
    label1[ch]->setText(QString("   VL:"+MI+""));
    label2[ch]->setText(QString("   PM:"+WI+""));
    label1[ch]->setStyleSheet("font: 14pt;background-color: rgb(248, 248, 248);border-width:1px 1px 0 1px; border-style:solid; border-color:rgb(255, 0, 0);");
    label2[ch]->setStyleSheet("font: 14pt;background-color: rgb(248, 248, 248);border-width:0 1px 1px 1px; border-style:solid; border-color:rgb(255, 0, 0);");

    if(GPackType == "PHEV")
    {
        if(ch == 0)
        {
            if(IsOk == "OK") // 本次螺栓合格ok
            {
                qDebug()<<"ch1 is ok";
                airTestStatus |= ch+1;
            }
            else
            {
                airTestStatus = 0;
                huCh1++;
                if(huCh1 > 2)
                {
                    qDebug()<<"this is ch1 error times 3";
                    emit sendCmdToStep(3,"",pdmnowromisOk);//
                    huCh1 = 0;
                }
            }
        }
        if(ch == 1)
        {
            if(IsOk == "OK") // 本次螺栓合格ok
            {
                qDebug()<<"rh2 is ok";
                airTestStatus |= ch+1;
            }
            else
            {
                airTestStatus = 0;
                huCh2++;
                if(huCh2 > 2)
                {
                    qDebug()<<"this is ch2 error times 3";
                    emit sendCmdToStep(3,"",pdmnowromisOk);//
                    huCh2 = 0;
                }
            }
        }

        if(airTestStatus == 0x03)
        {
            emit sendCmdToStep(3,"",pdmnowromisOk);//
            huCh1 = 0;
            huCh2 = 0;
        }
        else if(airTestStatus == 0x02)
        {
            airTestStatus = 0;
        }
    }
    else                    //-----bev
    {
        butt[0]->hide();
        label1[0]->hide();
        label2[0]->hide();
        if(IsOk == "OK") // 本次螺栓合格ok
        {
            qDebug()<<"bev air test is ok";
            emit sendCmdToStep(3,"",pdmnowromisOk);//
            huCh3 = 0;
        }
        else
        {
            huCh3++;
            if(huCh3 > 2)
            {
                qDebug()<<"this is bev air test error times 3";
                emit sendCmdToStep(3,"",pdmnowromisOk);//
                huCh3 = 0;
            }
        }
    }
//    if(){
//        onLEDFlashAllOFF(500,"flash_green",true);//合格绿闪
//    }else{
//        onLEDFlashAllOFF(500,"flash_red",true);//不合格红闪
//    }
}
void MainWindow::ShowTime()
{
    //    qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@ShowTime#################";
    qDebug()<<"changeTime";
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
    ui->label_12->setText(str);
}


void MainWindow::UpdateSlot()
{
    m_CurrentValue++;
    TimeLeft = Tacktime-m_CurrentValue/10;
    if( m_CurrentValue == m_MaxValue )
    {
        progressBarStop();
    }

    ui->progressBar->setValue(m_CurrentValue);

}

void MainWindow::progressBarStart(int interval/* =100 */, int maxValue/* =600 */)
{
    TimeLeft=Tacktime;
    m_UpdateInterval = interval;
    m_MaxValue = maxValue;
    m_Timer.start(m_UpdateInterval);
    ui->progressBar->setRange(0, m_MaxValue);
    ui->progressBar->setValue(0);
}

void MainWindow::progressBarStop()
{
    ui->progressBar->setValue(0);
    m_Timer.stop();
    m_CurrentValue  = 0;
    TimeLeft=0;
}
//rfid Ui
void MainWindow::setRfidState(bool istrue)
{
    if(istrue)
    {
        isRFIDConnected = true;
        ui->label_ss4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/68.bmp);");
    }
    else
    {
        isRFIDConnected = false;
        ui->label_ss4->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/69.bmp);");
    }
}

/*******************************************/
//NOK all bolt
/*******************************************/
void MainWindow::on_pushButton_reset_clicked()
{
    //no ok 确认全部
    qDebug()<<"normal reset";
    for(int j=whichpdmnumnow;j<tempnumdpm;j++)
    {
        qDebug() << "******nok all*******"<<j;
        butt[j]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/r01.png);font: 18pt;color:rgb(248,255,255)");
        if(label1[j]->text() == "")
        {
            label1[j]->setText(QString("   T:-1Nm"));
            label2[j]->setText(QString("   A:-1Deg"));
            label1[j]->setStyleSheet("font: 14pt;background-color: rgb(248, 248, 248);border-width:1px 1px 0 1px; border-style:solid; border-color:rgb(255, 0, 0);");
            label2[j]->setStyleSheet("font: 14pt;background-color: rgb(248, 248, 248);border-width:0 1px 1px 1px; border-style:solid; border-color:rgb(255, 0, 0);");
        }
    }
    qDebug() << "******nok all*******";
    ISRESET = true;
    emit sendOperate1(false,1);
    emit sendNokAll(1);//send to SqlThreadSVW2::
    gunPower(true);
    ISmaintenance = false;
    TaoTongState = false; //NOK all bolt close taotong
    SYSS = "NOK";
    ui->pushButton_16->setStyleSheet("font: 40pt ; background-color: rgb(255, 0, 0); color: rgb(248, 248, 255); border-width:0px;   border-style:solid; border-color: rgb(51, 153, 255);  ");
    ui->pushButton_16->setText("NOK");
    In_Output->writeIOOutput("r_led",true);
    In_Output->writeIOOutput("w_led",false);
    In_Output->writeIOOutput("n_led",false);
    if(timerpdm.isActive())
        timerpdm.stop();
    progressBarStop();
    emit sendCmdToStep(2,SYSS,1);//step type2 finish
}
// ING 状态确认
void MainWindow::resetUiDo()
{

}



//重启系统
void MainWindow::signal_mysqlerror_do()
{
    if(!isSaveShow)
    {
        emit sendOperate1(false,groupNumCh);
        e3 = new QGraphicsOpacityEffect(this);
        e3->setOpacity(0.5);
        ui->label_black->setGraphicsEffect(e3);
        ui->label_black->show();
        ui->label_black->setGeometry(0,0,1366,768);
        isSaveShow = true;
        SaveWhat = "sqlerror";
        save = new Save;
        save->show();
    }
}

void MainWindow::mainWindowInit()
{
    ui->stackedWidget_6->setCurrentIndex(0);
    //    QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
    ui->label_gongduan->setText(stationName);//config->value(QString("baseinfo/StationName")).toString());
    ui->label_gongwei->setText(QSettings("/config.ini", QSettings::IniFormat).value(QString("baseinfo/showStation")).toString()); //config->value(QString("baseinfo/StationId")).toString());
    if(!controlMode)
    {
        if(vari1 == "MM")//modual ready
        {
            ui->label_17->setText(tr("等待托盘到位..."));
        }
        else
        {
            ui->label_17->setText(tr("等待小车进站..."));
        }
        if(isRFID)
        {
            ui->pushButton_tiaoma->setText("RFID");
            ui->label_11->show();
            ui->label_ss4->show();
        }
        else if(isBarCode)
        {
            ui->pushButton_tiaoma->setText(tr("条码枪"));
            ui->label_11->hide();
            ui->label_ss4->hide();
        }
    }
    else
    {
        ui->pushButton_tiaoma->setText(tr("手动"));
        ui->label_17->setText(tr("当前手动模式..."));
    }
    if(vari1 == "M")
    {
        qDebug()<<"this is manual station";
        ui->label_10->setText(tr("系统"));
        ui->label_ss3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/68.bmp);");
    }
    else
    {
        qDebug()<<"this is aoto station";
        ui->label_ss3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/69.bmp);");
    }
    //    Tacktime = config->value("baseinfo/TackTime").toInt();
    //    delete config;
    QDateTime dateTime = QDateTime::currentDateTime();
    int year=dateTime.date().year();
    if(year<2017)
        ui->label_time->setText(tr("系统时间错误"));
    else
    {
        ui->label_time->clear();
    }
}

void MainWindow::initBack()
{
    //    ui->stackedWidget_6->setCurrentIndex(0);
    //    QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
    ui->label_gongduan->setText(stationName);//;config->value(QString("baseinfo/StationName")).toString());
    ui->label_gongwei->setText(QSettings("/config.ini", QSettings::IniFormat).value(QString("baseinfo/showStation")).toString());//config->value(QString("baseinfo/StationId")).toString());

    ui->label_ss3->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/68.bmp);");

    //    Tacktime = config->value("baseinfo/TackTime").toInt();


    //    delete config;

    QDateTime dateTime = QDateTime::currentDateTime();
    int year=dateTime.date().year();
    if(year<2017)
        ui->label_time->setText(tr("系统时间错误"));
    else
    {
        ui->label_time->clear();
    }
}

//处理屏幕右上角的放行按钮
void MainWindow::configwarning(bool isconfigone)
{
    qDebug() << "come hererer!" << isconfigone;
    if(SYSS == "ING")//nok one bolt 不合格跳过一个
    {
        ConfigOneOrAll = isconfigone;
        if(!isconfigone)
        {
            if(!isSaveShow)
            {
                ISWARNING = true;
                e3 = new QGraphicsOpacityEffect(this);
                e3->setOpacity(0.5);
                ui->label_black->setGraphicsEffect(e3);
                ui->label_black->show();
                ui->label_black->setGeometry(0,0,1366,768);
                isSaveShow = true;
                SaveWhat = "input";
                save = new Save;
                connect(save,SIGNAL(sendCloseInput(bool)),this,SLOT(receiveCloseInput(bool)));
                save->show();
            }
        }
        else
        {
            //nok确认1个
            if(!GIsTestStartFlag){
                emit  sendfromsecondthread(carInfor.boltSN[groupNumCh],VIN_PIN_SQL,carInfor.proNo[groupNumCh],1);
            }
                ISRESET = true;
                qDebug()<<"this is error enabled";
                GBlotErrorCount = 0;
            fromsecondthreaddata("-1","-1","ok",0,groupNumCh);
        }
    }
    else//放行操作
    {
        qDebug() << "come hererer11111" << isconfigone;
        emit sendMoveButtonToStep();
    }
}

/*****************套筒选择信号对应使能发送********************************/
void MainWindow::taotong_main(int type)
{
    qDebug()<<"33333 rev taotong 333333333"<<type<<taotongEnValue;
    if(type == 1)//receive taotong collect value
    {
        taotongEnValue |= 0x01;
    }
    else if(type == 2)//receive IO collect value
    {
        taotongEnValue |= 0x02;
    }
    else if(type == 4)//receive part code
    {
        taotongEnValue |= 0x04;
    }
    else if(type == 0)
    {
        qDebug() << "************taotong and IO and partCode send disable";
        taotongEnValue &= ~0x01;
        systemStatus = 15;
        emit sendOperate1(false,groupNumCh);
    }
    if(taotongEnValue == 0x07)
    {
        qDebug() << "************taotong and IO and partCode send enable *************";
        systemStatus = 0;
        emit sendOperate1(true,groupNumCh);
        if(enIOCtlFlag)
        {
            emit send_IO_Reset();
        }
    }
}

void MainWindow::receiveCloseInput(bool tmp)
{
    if(tmp)
    {
        if(!ConfigOneOrAll)
        {
            on_pushButton_reset_clicked();
            ui->label_black->hide();
            delete e3;
            delete save;
        }
        else
        {
            ui->label_black->hide();
            delete e3;
            delete save;
        }

        ISWARNING = false;
    }
    else
    {
        ui->label_black->hide();
        delete e3;
        delete save;
        ISWARNING = false;
    }
    SaveWhat ="";
    isSaveShow = false;
}

void MainWindow::wifishow(bool wificonnect)
{
    if(wificonnect)
    {
        ui->label_wifi->show();
    }
    else
    {
        ui->label_wifi->hide();
    }
}
void MainWindow::datashow(bool dataconnect)
{
    if(dataconnect)
    {
        ui->label_ss2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/68.bmp);");
    }
    else
    {
        ui->label_ss2->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/69.bmp);");
    }
}

void MainWindow::batteryshow1(QString power)
{
    if(power == "17")
    {

        ui->label_battery->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/battery2.png);background:transparent;");
    }
    else if(power == "16")
    {
        ui->label_battery->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/battery1.png);background:transparent;");
    }
    else if(power == "05")
    {
        ui->label_battery->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/battery.png);background:transparent;");
    }
    else if(power == "04")
    {
        ui->label_battery->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/battery_2.png);background:transparent;");
    }
    else if(power == "03")
    {
        ui->label_battery->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/battery_red.png);background:transparent;");
    }
    else if(power == "02"||power == "01")
    {
        //15 分钟后关机
        ui->label_battery->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/logo/battery_red.png);background:transparent;");
        //        timer_showdown.start(900000);
    }
}

void MainWindow::showhome()
{
    ui->label_pdmarea->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/DWIN_SET/home_SVW2.bmp);");
    ui->label_pdmarea->clear();
}

void MainWindow::send_Info()
{
    ShowTime();
    int OK;
    if(SYSS == "ING") //当前状态 ING or OK or NOK
        OK = 1;
    else if(SYSS == "OK")
        OK = 2;
    else if(SYSS == "NOK")
        OK = 3;
    else
        OK = 2;
    //    qDebug()<<"################sendInfo########################1";
    emit sendInfo(linkCount&0x01,linkCount&0x02,TimeLeft,OK);
    //    qDebug()<<"################sendInfo########################2";
    /************************************************************************/
    //    1.拧紧控制器断开连接；
    //    2.PLC断开连接；
    //    3.RFID断开连接；
    //    9.条码重复；
    //    10.条码校验失败；
    //    11.选配匹配失败；
    //    12.本地无此车信息；
    //    13.特征码匹配失败；
    //    14.车型螺栓数量为0；
    //    15.请正确选择套筒；
    //    16.VIN码匹配失败；
    //    17.通道匹配失败；
    /************************************************************************/
    if(preSystemStatus != systemStatus)
    {
        preSystemStatus = systemStatus;
        qDebug() << "*********systemStatus**********"<<systemStatus << preSystemStatus;
        switch(systemStatus)
        {
        case 1:
            ui->label_tiaomastate->setText(tr("拧紧控制器断开连接"));
            break;
        case 2:
            ui->label_tiaomastate->setText(tr("PLC断开连接"));
            break;
        case 3:
            ui->label_tiaomastate->setText(tr("PLC read error"));
            break;
        case 9:
            ui->label_tiaomastate->setText(tr("条码重复"));
            break;
        case 10:
            ui->label_tiaomastate->setText(tr("条码校验失败"));
            break;
        case 11:
            ui->label_tiaomastate->setText(tr("选配匹配失败"));
            break;
        case 12:
            ui->label_tiaomastate->setText(tr("本地无此车信息"));
            break;
        case 13:
            ui->label_tiaomastate->setText(tr("特征码匹配失败"));
            break;
        case 14:
            ui->label_tiaomastate->setText(tr("螺栓参数设置异常"));
            break;
        case 15:
            ui->label_tiaomastate->setText(tr("请正确选择套筒"));
            break;
        case 16:
            ui->label_tiaomastate->setText(tr("VIN码匹配失败"));
            break;
        case 17:
            ui->label_tiaomastate->setText(tr("通道匹配失败"));
            break;
        case 18:
            ui->label_tiaomastate->setText(tr("获取Job信息失败"));
            break;
        case 19:
            ui->label_tiaomastate->setText(tr("不存在条码信息"));
            break;
        case 20:
            ui->label_tiaomastate->setText(tr("条码不属于本站"));
            break;
        case 21:
            ui->label_tiaomastate->setText(tr("此辆车不操作"));
            //            ui->label_tiaomastate->setText(tr("Job匹配失败"));
            break;
        default:
            ui->label_tiaomastate->setText("");
            break;
        }
    }
}

void MainWindow::on_pushButton_18_clicked()
{
    this->close();
}


void MainWindow::gunPower(bool isOn)
{
    if(isOn)
    {
        if(SerialGunMode)
        {
            system("echo 0 > /sys/class/leds/control_uart2/brightness &");
        }
    }
    else
    {
    }
}

/********************************************/
//scan barcode
/********************************************/
void MainWindow::displayScanBarcode(QString name,QString type)
{
    qDebug()<<"&&&&&&&&&&&&&&&&& display num &&&&&&&&&&&&&&&&&&&&"<<name<<type;
    if(type == "1")//请扫描条码
    {
        timerDelay();
//        ui->label_17->setText(packSN);
        ui->label_pdmarea->setStyleSheet(QString("border-image: url(/PDM/").append(name).append(")"));
        ui->label_pdmarea->clear();
    }
    else if(type == "DisplayPressureResults"){//显示压力结果
        if(name == "newshow"){
            ui->label_17->setText(packSN);
        }else{
            ui->label_17->setText(name);
        }
    }else if(type == "startAirTest"){//检验通过，请开始密封测试
        ui->label_17->setText(name);
    }
    else if(type == "21")//display packSN1
    {
        timerDelay();
        qDebug()<<"this is display pack";
        packSN = name;
        ui->label_17->setText(packSN);
        ui->label_pdmarea->clear();
        ui->label_pdmarea->setStyleSheet("font: 100pt \"黑体\";background:transparent;");
        ui->label_pdmarea->setText(tr("操作PACK1"));
    }
    else if(type == "301")
    {
        timerDelay();
        ui->label_17->setText("条码匹配失败");
        ui->label_pdmarea->clear();
        ui->label_pdmarea->setStyleSheet("font: 50pt \"黑体\";background:transparent;");
        ui->label_pdmarea->setText(name);
    }
    else if(type == "220")
    {
        timerDelay();
        ui->label_17->setText("AGV放行");
    }
    else if(type == "201")//display packSN1
    {
        timerDelay();
        ui->label_pdmarea->clear();
        ui->label_pdmarea->setStyleSheet("font: 100pt \"黑体\";background:transparent;");
        ui->label_pdmarea->setText(tr("请求拧紧位置"));
    }
    else if(type == "202")//display packSN1
    {
        timerDelay();
        ui->label_pdmarea->clear();
        ui->label_pdmarea->setStyleSheet("font: 100pt \"黑体\";background:transparent;");
        ui->label_pdmarea->setText(tr("拧紧完成"));
    }
    else if(type == "203")//display packSN1
    {
        timerDelay();
        ui->label_pdmarea->clear();
        ui->label_pdmarea->setStyleSheet("font: 100pt \"黑体\";background:transparent;");
        ui->label_pdmarea->setText(tr("请求堵头位置"));
    }
    else if(type == "204")//display packSN1
    {
        timerDelay();
        ui->label_pdmarea->clear();
        ui->label_pdmarea->setStyleSheet("font: 100pt \"黑体\";background:transparent;");
        ui->label_pdmarea->setText(name);
    }
    else if(type == "205")//display packSN1
    {
        timerDelay();
        ui->label_pdmarea->clear();
        ui->label_pdmarea->setStyleSheet("font: 100pt \"黑体\";background:transparent;");
        ui->label_pdmarea->setText(tr("请求下箱体位置号"));
    }
    else if(type == "206")//display
    {
        timerDelay();
        ui->label_pdmarea->clear();
        ui->label_pdmarea->setStyleSheet("font: 100pt \"黑体\";background:transparent;");
        ui->label_pdmarea->setText(tr("操作错误！"));
    }
    else if(type == "22")//display packSN2
    {
        timerDelay();
        packSN = name;
        ui->label_17->setText(packSN);
        ui->label_pdmarea->clear();
        ui->label_pdmarea->setStyleSheet("font: 60pt \"黑体\";background:transparent;");
        ui->label_pdmarea->setText(tr("操作PACK2"));
    }
    else if(type == "15")
    {
        timerDelay();
        ui->label_17->setText(tr("等待托盘到位..."));
        showhome();
    }
    else if(type == "30")
    {
        timerDelay();
        qDebug()<<"this is put car";
        ui->label_pdmarea->setStyleSheet("font: 60pt \"黑体\";background:transparent;");
        if("MM" == vari1)
            ui->label_pdmarea->setText(tr("托盘完成放行"));
        if("M" == vari1)
            ui->label_pdmarea->setText(tr("手动站AGV完成放行"));
        if("A" == vari1)
            ui->label_pdmarea->setText(tr("自动站AGV完成放行"));

         //onLEDFlashAllOFF(0,"0",false);//关掉红绿黄灯
         //FlashTime_y.start(500);//完成放行黄闪
    }
    else if(type == "122")
    {
        ui->pushButton_errorGo->show();
        ui->pushButton_errorContinue_2->show();
    }
    else if(type == "111")
    {
        qDebug()<<"this is send pack error to mainwindow"<<GWhichPackError;
        ui->label_pdmarea->setStyleSheet("font: 60pt \"黑体\";background:transparent;color: rgb(255, 0, 0);");
        if(GWhichPackError == 1)
        {
            ui->label_pdmarea->setText(tr("PACK1异常，等待放行!!!"));
        }
        else if(GWhichPackError == 2)
        {
            ui->label_pdmarea->setText(tr("PACK2异常，等待放行!!!"));
        }
        else if(GWhichPackError == 3)
        {
            ui->label_pdmarea->setText(tr("PACK12异常，等待放行!!!"));
        }
        else if(GWhichPackError == 4)
        {
            ui->label_pdmarea->setText(tr("PACK不存在，等待放行!!!"));
            isGostRun=true;
            emit sendDrectGo();
            //showhome();
            QTimer::singleShot(3000,this,SLOT(directShowHome()));
        }
        else
        {
            ui->label_pdmarea->setText(tr("PACK异常，等待放行!!!"));
        }
        if (!isGostRun)
        {
            ui->pushButton_errorGo->show();
            ui->pushButton_errorContinue->show();
            ui->pushButton_errorContinue_2->show();
            isGostRun=false;
        }
        isGostRun=false;
    }
    else if(type == "31")
    {
        timerDelay();
        ui->label_17->setText(tr("等待小车进站..."));
        showhome();
    }
    else if(type == "4")//wait IO input
    {
        ui->label_17->setText(tr("等待触发信号"));
    }
    else if(type == "7")//display picture
    {
        timerDelay();
        ui->label_17->setText(packSN);
        ui->label_pdmarea->setStyleSheet(QString("border-image: url(/PDM/").append(name).append(")"));
        ui->label_pdmarea->clear();
    }
    else if(type == "8")//display air test
    {
        timerDelay();
        ui->label_17->setText(packSN);
        ui->label_pdmarea->setStyleSheet(QString("border-image: url(/PDM/").append(name).append(")"));
        ui->label_pdmarea->clear();
        int x,y;

        airTestStatus = 0;
        pdmnowromisOk = true;
        for(int i=0;i<2;i++)
        {
            qDebug()<<i<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@################";
            butt[i] = new QPushButton(ui->stackedWidget_2);
            label1[i] = new QLabel(ui->stackedWidget_2);
            label2[i] = new QLabel(ui->stackedWidget_2);
            butt[i]->raise();
            butt[i]->setFlat(true);
            label1[i]->setAlignment(Qt::AlignLeft);
            label2[i]->setAlignment(Qt::AlignLeft);
            butt[i]->setFocusPolicy(Qt::NoFocus);
            label1[i]->setFocusPolicy(Qt::NoFocus);
            label2[i]->setFocusPolicy(Qt::NoFocus);
            butt[i]->setAttribute(Qt::WA_TransparentForMouseEvents);
            label1[i]->setAttribute(Qt::WA_TransparentForMouseEvents);
            label2[i]->setAttribute(Qt::WA_TransparentForMouseEvents);
            butt[i]->setStyleSheet("border-image: url(:/LCD_CS351/LCD_CS351/35_all/w01.png);font: 18pt;");
            //            butt[i]->setText(QString::number((i+1)));
            if(i==0)
            {
                butt[i]->setText("1");
            }
            else
            {
                butt[i]->setText("2");
            }
            x = 10;
            y = 10 + 50*i;
            butt[i]->setGeometry(x,y,46,46);
            label1[i]->setGeometry(x+23,y,140,23);
            label2[i]->setGeometry(x+23,y+23,140,23);
            label1[i]->setStyleSheet("background:transparent;");
            label2[i]->setStyleSheet("background:transparent;");
            butt[i]->show();
            label1[i]->show();
            label2[i]->show();
        }
        airButtonNum = 2;
    }
    else if(type == "显示托盘请求状态"){
        ui->label_17->setText(name);
    }
    else if(type == "displayOnly"){
        ui->label_17->setText(name);
    }else if(type == "scanStepError"){
        if(ui->label_tishu->isHidden()){
            ui->label_tishu->show();
        }
        ui->label_tishu->setText(name);
        QTimer::singleShot(20*1000,this,SLOT(closeTishu()));
    }
    else
    {
        ui->label_17->setText(type);
    }
}
void MainWindow::directShowHome()
{
    showhome();
}
/*************************/
//OK Button
/*************************/
void MainWindow::on_pushButton_16_clicked()
{
#if DYTEST4
    showWarningSpearhead();
    showWarningSpearhead();
#endif
    configwarning(true);
    emit sendToPlcReadyClicked();
#if DYTEST1
    //模拟做个托盘RFID读取，测试用
    QString errorCode = "0000";
    QByteArray code = (QString("BMCE-0")+QString::number(testAdd)).toLocal8Bit();
    testAdd++;
    emit sendTrayNUMTest(errorCode,code);
#endif

#if DYTEST2
    displayScanBarcode("条码校验失败\n实际扫码#3Q0915646A\n需要扫码3#5NG915183C","301");
#endif
}

/*************************/
//EmergencyStop  急停
/*************************/
void MainWindow::showEmergencyStop(bool isMove)
{
    //FlashTime_r.start(200);
    //In_Output->writeIOOutput("r_led",true);
    if(!isMove)
    {
        isEmergencyStop = true;
        e3 = new QGraphicsOpacityEffect(this);
        e3->setOpacity(0.5);
        ui->label_black->setGraphicsEffect(e3);
        ui->label_black->show();
        ui->label_black->setGeometry(0,0,1366,768);
        isSaveShow = true;
        SaveWhat = "EmergencyStop";
        save = new Save;
        connect(this,SIGNAL(sendChangeEmergencyStop()),save,SLOT(revChangeEmergencyStop()));
        connect(save,SIGNAL(sendresetEmergencyStop(bool)),this,SLOT(resetEmergencyStop(bool)));
        save->show();
    }
    else
    {
        emit sendChangeEmergencyStop();
    }
}

void MainWindow::resetEmergencyStop(bool isReset)
{
    if(isReset)
        closeSave();
    emit sendStepResetEmergencyStop(isReset);
}


void MainWindow::on_pushButton_errorGo_clicked()
{
    if(Station.mid(0,2) == "OP")
    {
        emit sendErrorGo(1);
        sleep(1);
        ui->pushButton_errorGo->hide();
        ui->pushButton_errorContinue_2->hide();
    }
    else
    {
        emit sendErrorGo(2);
        sleep(1);
        ui->pushButton_errorGo->hide();
        ui->pushButton_errorContinue->hide();
        ui->pushButton_errorContinue_2->hide();
    }

}

void MainWindow::on_pushButton_errorContinue_clicked()
{
    emit sendErrorContinue();
    sleep(1);
    ui->pushButton_errorGo->hide();
    ui->pushButton_errorContinue->hide();
    ui->pushButton_errorContinue_2->hide();
}

void MainWindow::on_pushButton_errorContinue_2_clicked()
{
    if(Station.mid(0,2) == "OP")
    {
        qDebug()<<"bmceErrorContinue";
        emit sendErrorContinue();
        sleep(1);
        ui->pushButton_errorGo->hide();
        ui->pushButton_errorContinue_2->hide();
    }
    else
    {
        GWorkStatus = true;
        emit sendErrorContinue();
        qDebug()<<"this is GworkStatus"<<GWorkStatus;
        sleep(1);
        ui->pushButton_errorGo->hide();
        ui->pushButton_errorContinue->hide();
        ui->pushButton_errorContinue_2->hide();
    }
}

//收到EKS钥匙信号
void MainWindow::EKSStausChange(int eksStatus)
{
    if(eksStatus == greenKey || eksStatus == redKey){
        if(GisBoltErrorCountFlag)//有过3次不合格
        {
            GisBoltErrorCountFlag = false;
            GBlotErrorCount = 0;//螺栓连续错误个数清0，又可以重新打
            emit sendOperate1(true,GiswhichChannel);
            ui->label_tishu->hide();
        }
        else
        {
            ui->pushButton_readRFID->show();
            ui->pushButton_deep1->show();
            ui->pushButton_deep2->show();
            ui->pushButton_deepUp->show();
            ui->pushButton_deepDown->show();
            ui->label_6->show();
            ui->label_13->show();
            ui->label_14->show();
            ui->label_15->show();
        }
    }
}

//跳过PACK1
void MainWindow::on_pushButton_deep1_clicked()
{
    sendEKSStatusEvent(1);
    ui->pushButton_readRFID->hide();
    ui->pushButton_deep1->hide();
    ui->pushButton_deep2->hide();
    ui->pushButton_deepUp->hide();
    ui->pushButton_deepDown->hide();
    ui->label_6->hide();
    ui->label_13->hide();
    ui->label_14->hide();
    ui->label_15->hide();

}

//跳过PACK2
void MainWindow::on_pushButton_deep2_clicked()
{
    sendEKSStatusEvent(2);
    ui->pushButton_readRFID->hide();
    ui->pushButton_deep1->hide();
    ui->pushButton_deep2->hide();
    ui->pushButton_deepUp->hide();
    ui->pushButton_deepDown->hide();
    ui->label_6->hide();
    ui->label_13->hide();
    ui->label_14->hide();
    ui->label_15->hide();
}

//点击上一步
void MainWindow::on_pushButton_deepUp_clicked()
{
    sendEKSStatusEvent(3);
    ui->pushButton_readRFID->hide();
    ui->pushButton_deep1->hide();
    ui->pushButton_deep2->hide();
    ui->pushButton_deepUp->hide();
    ui->pushButton_deepDown->hide();
    ui->label_6->hide();
    ui->label_13->hide();
    ui->label_14->hide();
    ui->label_15->hide();
}
//点击下一步
void MainWindow::on_pushButton_deepDown_clicked()
{
    sendEKSStatusEvent(4);
    ui->pushButton_readRFID->hide();
    ui->pushButton_deep1->hide();
    ui->pushButton_deep2->hide();
    ui->pushButton_deepUp->hide();
    ui->pushButton_deepDown->hide();
    ui->label_6->hide();
    ui->label_13->hide();
    ui->label_14->hide();
    ui->label_15->hide();
}

//重新读取RFID
void MainWindow::on_pushButton_readRFID_clicked()
{
    emit sendRFIDRead();
    emit sendOperate1(false,0);
    ui->pushButton_readRFID->hide();
    ui->pushButton_deep1->hide();
    ui->pushButton_deep2->hide();
    ui->pushButton_deepUp->hide();
    ui->pushButton_deepDown->hide();
    ui->label_6->hide();
    ui->label_13->hide();
    ui->label_14->hide();
    ui->label_15->hide();
}

//在label_showState上滚动显示文字内容
void MainWindow::onShowState(QString states)
{
    qDebug()<<"this onShowState states"<<states;
    QString next = ui->label_showState->text();
    if(states == "handleRecvStatusMess is true"){
        emit sendRollLable(next+" "+"handleRecvStatusMess is true");
        qDebug()<<"this is onShowState states true"<<states;
    }else{
        qDebug()<<"this is onShowState states false"<<states;
        emit sendRollLable(states);
    }
}

//收到添加挂起步骤的信号
void MainWindow::onGetSuspend()
{
    ui->label_pdmarea->clear();
    ui->pushButton_Go->show();
}

//继续操作
void MainWindow::on_pushButton_Go_clicked()
{
    ui->pushButton_Go->hide();
    GisTightenGoOn = 1;
}

//红灯闪
void MainWindow::onLEDFlash_r()
{
    if(LEDIsON_r){
        LEDIsON_r = false;
    }else{
        LEDIsON_r = true;
    }
    In_Output->writeIOOutput("r_led",LEDIsON_r);
    //qDebug()<<"red led on";
    //onShowState("red on");//启动按钮亮
}

//绿灯闪
void MainWindow::onLEDFlash_g()
{
    if(LEDIsON_g){
        LEDIsON_g = false;
    }else{
        LEDIsON_g = true;
    }
    In_Output->writeIOOutput("g_led",LEDIsON_g);
    //onShowState("green on");
    //qDebug()<<"green led on";
}

//黄灯闪
void MainWindow::onLEDFlash_y()
{
    if(LEDIsON_y){
        LEDIsON_y = false;
    }else{
        LEDIsON_y = true;
    }
    In_Output->writeIOOutput("y_led",LEDIsON_y);
    //qDebug()<<"yellow led on";
    //onShowState("yellow on");
}

//关掉红绿黄灯
void MainWindow::onLEDFlashAllOFF(int frequency,QString command,bool flag)
{
    FlashTime_r.stop();
    FlashTime_g.stop();
    FlashTime_y.stop();
    In_Output->writeIOOutput("r_led",false);
    In_Output->writeIOOutput("g_led",false);
    In_Output->writeIOOutput("y_led",false);
    QString flagToString = "";
    if(flag){
        flagToString = "true";
    }else{
        flagToString = "false";
    }
    //onShowState("frequency:" + QString::number(frequency) +",command:"+ command + ",flag:" +flagToString);

    if(command == "light_red"){//亮红灯
        In_Output->writeIOOutput("r_led",flag);
    }else if(command == "light_green"){//亮绿灯
        In_Output->writeIOOutput("g_led",flag);
    }else if(command == "light_yellow"){//亮黄灯
        In_Output->writeIOOutput("y_led",flag);
    }else if(command == "flash_red"){//红闪
        FlashTime_r.start(frequency);
    }else if(command == "flash_green"){//绿闪
        FlashTime_g.start(frequency);
    }else if(command == "flash_yellow"){//黄闪
        FlashTime_y.start(frequency);
    }
}

void MainWindow::recEncoderFromTight(qint64 yEN, qint64 aEN)
{
    ui->label_AEN->show();
    ui->label_YEN->show();
    ui->label_AEN->setText(QString::number(aEN));
    ui->label_YEN->setText(QString::number(yEN));
}

void MainWindow::revM020FromStep()
{
    ui->label_pdmarea->hide();
    ui->label_m020Pack1Sn->show();
    ui->label_m020Pqck2SN->show();
    ui->label_m020Result->show();
    ui->lineEdit_M020LineEdit->show();
    ui->pushButton_M020SendPost->show();
}

void MainWindow::on_pushButton_M020SendPost_clicked()
{
    QString agvSN = ui->lineEdit_M020LineEdit->text();
    emit sendM020ToPost(agvSN);
    ui->label_m020Result->setText("");
    ui->label_m020Pack1Sn->setText("");
    ui->label_m020Pqck2SN->setText("");
}

void MainWindow::revm020FromInterface(QString result, QString pack1SN, QString pack2SN, QString packType)
{
    qDebug()<<"revm020FromInterface"<<result<<pack1SN<<pack2SN<<packType;
    if(result == "0")
    {
        ui->label_m020Result->setText("绑定流水号失败！！！");
    }
    else
    {
        ui->label_m020Result->setText("绑定流水号成功！！！");
        QString pack1SNTemp = "pack1SN:" + pack1SN;
        QString pack2SNTemp = "pack2SN:" + pack2SN;
        ui->label_m020Pack1Sn->setText(pack1SNTemp);
        ui->label_m020Pqck2SN->setText(pack2SNTemp);
    }
}

void MainWindow::revStartNewPack()
{
    packTime.start(1000);
}

//处理产量数据统计新增逻辑 在界面上显示
void MainWindow::revEndNewPack(QString packType,QString pack1SN,QString pack1Status,QString pack2SN,QString pack2Status,QDateTime timeStartPack1,QDateTime timeStartPack2,QDateTime timeEnd)
{
    packTime.stop();
    //GpackCount = packCount;
    packNumber = 0;
    qDebug()<<"addPackCount:"<<packType<<pack1SN<<pack1Status<<pack2SN<<pack2Status<<timeStartPack1<<timeStartPack2<<timeEnd;
    double diffTime = 0;
    if(pack1SN.size() > 0 || pack2SN.size() > 0){//有包
        if(packType == "BEV"){
            diffTime = timeStartPack1.secsTo(timeEnd);
            if("1" == pack1Status){
                packNumber++;
            }
        }else{//PHEV
            if(Station.mid(0,2) == "OP" ||  Station.mid(0,2) == "VM"){
                diffTime = timeStartPack1.secsTo(timeEnd);
            }else{
                diffTime = timeStartPack1.secsTo(timeEnd)/2;
            }
            if("1" == pack1Status || "1" == pack2Status){
                if("1" == pack1Status && "1" == pack2Status){//两个包合格
                    packNumber+=2;
                }else{//一个包合格
                    packNumber++;
                }
            }
        }
    }
    GOutputToday += packNumber;
    QString aaa = "当前节拍：";
    QString bbb = "当天产量：";
    aaa.append(QString::number(diffTime));
    bbb.append(QString::number(GOutputToday));
    ui->label_18->setText(aaa);
    ui->label_19->setText(bbb);

    //存储产量信息到产量表
    if(pack1SN.size() > 0){
        if(pack2SN.size() > 0){//两个包
            emit sendpackMesToSql(pack1SN,packType,timeStartPack1,timeStartPack2,pack1Status,diffTime);
            emit sendPackMesToInterface(pack1SN,packType,timeStartPack1.toString("yyyy-MM-dd hh:mm:ss"),timeStartPack2.toString("yyyy-MM-dd hh:mm:ss"),diffTime,pack1Status.toInt());
        }else{//只有PACK1
            emit sendpackMesToSql(pack1SN,packType,timeStartPack1,timeEnd,pack1Status,diffTime);
            emit sendPackMesToInterface(pack1SN,packType,timeStartPack1.toString("yyyy-MM-dd hh:mm:ss"),timeEnd.toString("yyyy-MM-dd hh:mm:ss"),diffTime,pack1Status.toInt());
        }
    }
    if(pack2SN.size() > 0){
        emit sendpackMesToSql(pack2SN,packType,timeStartPack2,timeEnd,pack2Status,diffTime);
        emit sendPackMesToInterface(pack2SN,packType,timeStartPack2.toString("yyyy-MM-dd hh:mm:ss"),timeEnd.toString("yyyy-MM-dd hh:mm:ss"),diffTime,pack2Status.toInt());
    }

    packCount = 0;
}


void MainWindow::displayDailyProduction()
{
    ui->label_19->setText("当天产量："+QString::number(GOutputToday));
}

void MainWindow::revPackNumberSum()
{
    packNumber++;

}

void MainWindow::packTimeStart()
{
    packCount++;
    QString aaa = "当前节拍：";
    aaa.append(QString::number(packCount)).append("秒");
    ui->label_18->setText(aaa);
}

//测试发条码
void MainWindow::on_pushButton_clicked()
{
#if DYTEST1
    QByteArray va = "3#5NG915183C    #H004S0941#*K8CV6D-V0107.04.2003012178*=";
    QString str = ui->lineEdit->text();
    if(str.size() > 0){
        va = str.toLocal8Bit().data();
    }
    emit sendCodeTest(va);
#endif


#if DYTEST3
    EKSBuff = "";
    emit saveEKSStatus();
#endif
}

void MainWindow::on_but_bd2_clicked()
{
#if DYTEST1
    QByteArray va = "#3Q0915646A    ###060319*288 LS301AAXC46*=";
    QString str = ui->lineEdit->text();
    if(str.size() > 0){
        va = str.toLocal8Bit().data();
    }
    emit sendCodeTest(va);
#endif

#if DYTEST3
    EKSBuff = "123456";
    emit saveEKSStatus();
#endif

}

void MainWindow::on_but_bd3_clicked()
{
#if DYTEST1
    QByteArray va = "#5QE919891     ##432514305#040919*288 AX902584479*=";
    QString str = ui->lineEdit->text();
    if(str.size() > 0){
        va = str.toLocal8Bit().data();
    }
    emit sendCodeTest(va);
#endif
}

void MainWindow::on_but_bd4_1_clicked()
{
#if DYTEST1
    QByteArray va = "#3Q0915646B    ###040919*288 LS300YLIKK1*=";
    QString str = ui->lineEdit->text();
    if(str.size() > 0){
        va = str.toLocal8Bit().data();
    }
    emit sendCodeTest(va);
#endif
}

void MainWindow::on_but_bd4_2_clicked()
{
#if DYTEST1
    QByteArray va = "#3Q0915646B    ###040919*288 LS300YLIKK2*=";
    QString str = ui->lineEdit->text();
    if(str.size() > 0){
        va = str.toLocal8Bit().data();
    }
    emit sendCodeTest(va);
#endif
}

void MainWindow::on_but_bd4_3_clicked()
{
#if DYTEST1
    QByteArray va = "#3Q0915646B    ###040919*288 LS300YLIKK3*=";
    QString str = ui->lineEdit->text();
    if(str.size() > 0){
        va = str.toLocal8Bit().data();
    }
    emit sendCodeTest(va);
#endif
}

void MainWindow::on_but_bd4_4_clicked()
{
#if DYTEST1
    QByteArray va = "#3Q0915646B    ###040919*288 LS300YLIKK4*=";
    QString str = ui->lineEdit->text();
    if(str.size() > 0){
        va = str.toLocal8Bit().data();
    }
    emit sendCodeTest(va);
#endif
}

void MainWindow::on_but_ac1_clicked()
{
#if DYTEST1
    QByteArray va = "3#5KE915227A    ###060319*288 LS301AAXC46*=";
    QString str = ui->lineEdit->text();
    if(str.size() > 0){
        va = str.toLocal8Bit().data();
    }
    emit sendCodeTest(va);
#endif
}

//显示枪头剩余次数低的警告
void MainWindow::showWarningSpearhead()
{
    qDebug()<<"MainWindow::showWarningSpearhead";
//    emit sendOperate1(false,groupNumCh);//切掉使能
    e3 = new QGraphicsOpacityEffect(this);
    e3->setOpacity(0.5);
    ui->label_black->setGraphicsEffect(e3);
    ui->label_black->show();
    ui->label_black->setGeometry(0,0,1366,768);
    isSaveShow = true;
    SaveWhat = "WarningSpearhead";
    save = new Save(this);
    connect(save,SIGNAL(sendClose()),this,SLOT(closeSave()));
    save->show();
}


void MainWindow::closeTishu()
{
    QString text = ui->label_tishu->text();
    if((text.contains("别扫码") || text.contains("无订单信息"))&& !ui->label_tishu->isHidden()){
        ui->label_tishu->hide();
    }
}
