#include "save.h"
#include "ui_save.h"
#include "GlobalVarible.h"
#include <QSettings>

Save::Save(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Save)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    // this->setCursor(Qt::BlankCursor); //去掉光标
    isEmergencyStop = true;
    ui->pushButton_cancel->hide();
    ui->pushButton_3->hide();
    QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
    StationName = config->value("baseinfo/StationName").toString();
    offSet = QString::number(config->value("baseinfo/OffSet").toInt());
    delete config;

    if(SaveWhat == "OPDELONE" || SaveWhat == "OPDELALL" || SaveWhat == "deall")
        ui->label_3->setText(tr("删除参数设置？"));
    else if(SaveWhat == "wifiunconnect")
    {
        ui->label_3->setText("无线网络未连接");
        ui->pushButton_2->setText("确定");
        ui->pushButton->hide();
    }
    else if(SaveWhat == "pdmwaring")
    {
        ui->label_3->setText(tr("名字重复!请修改"));
        ui->pushButton->setGeometry(120,110,80,26);
        ui->pushButton_2->hide();
    }
    else if(SaveWhat == "pdmdel")
    {
        ui->label_3->setText(tr("删除作业指导图？"));
    }
    else if(SaveWhat == "input")
    {
        ui->label_3->setText(tr("是否确认全部？"));
    }
    else if(SaveWhat == "shutdown")
    {
        ui->label_3->setText(tr("   请选择？"));
        ui->pushButton_cancel->show();
        ui->pushButton->setText(tr("关机"));
        ui->pushButton_2->setText(tr("重启"));
    }
    else if(SaveWhat == "sqlerror")
    {
        ui->label_3->setText(tr("数据库出错!请修复"));
        ui->pushButton->setGeometry(120,110,80,26);
        ui->pushButton->show();
        ui->pushButton->setText(tr("修复"));
        //ui->pushButton_3->show();
        ui->pushButton_2->hide();
    }
    else if(SaveWhat == "delete_car")
    {
        ui->label_3->setText(tr("请扫删除车辆条码"));
        ui->pushButton_2->setGeometry(120,110,80,26);
        ui->pushButton->hide();
        ui->pushButton_2->setText(tr("取消"));
    }
    else if(SaveWhat.mid(0,7) == "queue_1")
    {
        ui->label_3->setGeometry(90,35,221,21);
        ui->label_3->setText(tr("排序出错，该车已打过!"));
        ui->label_4->setText(SaveWhat.mid(8));
        ui->pushButton->hide();
        ui->pushButton_2->setGeometry(120,110,80,26);
        ui->pushButton_2->show();
        ui->pushButton_2->setText(tr("确定"));
    }
    else if(SaveWhat.mid(0,7) == "queue_2")
    {
        ui->label_3->setGeometry(90,35,221,21);
        ui->label_3->setText(tr("排序出错，该车已删除!"));
        ui->label_4->setText(SaveWhat.mid(8));
        ui->pushButton->hide();
        ui->pushButton_2->setGeometry(120,110,80,26);
        ui->pushButton_2->show();
        ui->pushButton_2->setText(tr("确定"));
    }
    else if(SaveWhat.mid(0,8) == "queue_no")
    {
        ui->label_3->setGeometry(90,35,221,21);
        ui->label_3->setText(tr("没有该车,切换手动模式"));
        ui->label_4->setText(SaveWhat.mid(8));
        ui->pushButton_2->hide();
        ui->pushButton->setGeometry(120,110,80,26);
        ui->pushButton->show();
        ui->pushButton->setText(tr("确定"));
    }
    else if(SaveWhat=="which_one")
    {
        ui->label_3->setText(tr("请选择确认的通道号？"));
        ui->pushButton_cancel->show();
        ui->pushButton->setText(tr("通道1"));
        ui->pushButton_2->setText(tr("通道2"));
    }
    else if(SaveWhat=="Logout")
    {
        ui->label_3->setText(tr("是否注销登录?"));
        ui->pushButton->setText(tr("是"));
        ui->pushButton_2->setText(tr("否"));
    }
    else if(SaveWhat =="User")
    {
        ui->label_3->setText(tr("保存用户设置？"));
    }
    else if(SaveWhat == "noLogin")
    {
        ui->label_3->setText(tr("没有用户登录！"));
        ui->pushButton->setGeometry(120,110,80,26);
        ui->pushButton->show();
        ui->pushButton->setText(tr("确定"));
        ui->pushButton_2->hide();
    }
    else if(SaveWhat == "Align")
    {
        ui->label_3->setText(tr("是否对齐队列?"));
        ui->pushButton->setText(tr("是"));
        ui->pushButton_2->setText(tr("否"));
    }
    else if(SaveWhat.left(10) == "QueueError")
    {
        ui->pushButton->setGeometry(120,110,80,26);
        ui->pushButton->show();
        ui->pushButton->setText(tr("确定"));
        ui->pushButton_2->hide();
        int ErrorCode = SaveWhat.right(1).toInt();
        if(ErrorCode == 1)
            ui->label_3->setText(tr("未连网，请求失败！"));
        else if(ErrorCode == 2)
            ui->label_3->setText(tr("序列化失败，请求失败！"));
        else if(ErrorCode == 3)
            ui->label_3->setText(tr("HTTP请求失败！"));
        else if(ErrorCode == 4)
            ui->label_3->setText(tr("解析失败，请求失败！"));
        else if(ErrorCode == 5)
            ui->label_3->setText(tr("回复操作失败！"));
        else if(ErrorCode == 6)
            ui->label_3->setText(tr("回复超时，请求失败！"));
    }
    else if(SaveWhat == "EmergencyStop")
    {
        ui->label_3->setText(tr("确认故障排除？"));
        ui->pushButton_2->hide();
        ui->pushButton->setGeometry(120,110,80,26);
        ui->pushButton->show();
        ui->pushButton->setText(tr("确定"));
    }
}

Save::~Save()
{
    delete ui;
}


//yes
void Save::on_pushButton_clicked()
{
    if(SaveWhat == "advanced")
        emit sendSaveAdvancedState(true);
    else if(SaveWhat == "baseinfo")
        emit sendSaveBaseinfo(true);
    else if(SaveWhat == "de")
        emit sendDeSingle(true);
    else if(SaveWhat == "")
        emit sendDeSingle(true);
    else if(SaveWhat == "save2")
        emit sendDeSingle(true);
    else if(SaveWhat == "deall")
        emit sendDeSingle(true);
    else if(SaveWhat == "OPDELONE")
        emit sendDeSingle(true);
    else if(SaveWhat == "OPDELALL")
        emit sendDeSingle(true);
    else if(SaveWhat == "pdm")
        emit sendDeSingle(true);
    else if(SaveWhat == "pdmwaring")
        emit sendDeSingle(true);
    else if(SaveWhat == "pdmdel")
        emit sendDeSingle(true);
    else if(SaveWhat == "pdmout")
        emit sendDeSingle(true);
    else if(SaveWhat == "pdmoutmain")
        emit sendDeSingle(true);
    else if(SaveWhat == "OPCANCEL")
        emit sendDeSingle(true);
    else if(SaveWhat == "CANCEL")
        emit sendDeSingle(true);
    else if(SaveWhat == "pdmoutabout")
        emit sendDeSingle(true);
    else if(SaveWhat == "Master_slave")
        emit sendSaveMasterSlaveState(true);
    else if(SaveWhat == "input")
        emit sendCloseInput(true);
    else if(SaveWhat == "shutdown")
        emit sendShutDown(1);
    else if(SaveWhat == "sqlerror")
        repairMySQL();
    else if(SaveWhat == "config_bound")
        emit sendSaveBound(true);
    else if(SaveWhat == "delete_car")
        emit sendDelete_car(true);
    else if(SaveWhat.mid(0,8) == "queue_no")
        emit change_mode();
    else if(SaveWhat == "which_one")
        emit choiceChannel(1);
    else if(SaveWhat=="Logout")
        emit sendLogout(true);
    else if(SaveWhat =="User")
        emit sendSaveUser(true);
    else if(SaveWhat == "noLogin")
        emit sendClose();
    else if(SaveWhat == "Align")
        alignQueue();
    else if(SaveWhat.left(10) == "QueueError")
        emit sendClose();
    else if(SaveWhat == "Save")
        emit sendSave(true);
    else if(SaveWhat == "EmergencyStop")
    {
        emit sendresetEmergencyStop(false);
        ui->pushButton->hide();
        if(isEmergencyStop)
            ui->label_3->setText(tr("等待急停按钮复位"));
        else
        {
            ui->label_3->setText(tr("重新启动AGV小车？"));
            ui->pushButton_2->setGeometry(120,110,80,26);
            ui->pushButton_2->show();
            ui->pushButton_2->setText(tr("确定"));
        }
    }
}


//no
void Save::on_pushButton_2_clicked()
{
    if(SaveWhat == "advanced")
        emit sendSaveAdvancedState(false);
    else if(SaveWhat == "baseinfo")
        emit sendSaveBaseinfo(false);
    else if(SaveWhat == "de")
        emit sendDeSingle(false);
    else if(SaveWhat == "")
        emit sendDeSingle(false);
    else if(SaveWhat == "save2")
        emit sendDeSingle(false);
    else if(SaveWhat == "deall")
        emit sendDeSingle(false);
    else if(SaveWhat == "OPDELONE")
        emit sendDeSingle(false);
    else if(SaveWhat == "OPDELALL")
        emit sendDeSingle(false);
    else if(SaveWhat == "pdm")
        emit sendDeSingle(false);
    else if(SaveWhat == "wifiunconnect")
        emit sendDeSingle(false);
    else if(SaveWhat == "pdmdel")
        emit sendDeSingle(false);
    else if(SaveWhat == "pdmout")
        emit sendDeSingle(false);
    else if(SaveWhat == "pdmoutmain")
        emit sendDeSingle(false);
    else if(SaveWhat == "OPCANCEL")
        emit sendDeSingle(false);
    else if(SaveWhat == "CANCEL")
        emit sendDeSingle(false);
    else if(SaveWhat == "pdmoutabout")
        emit sendDeSingle(false);
    else if(SaveWhat == "Master_slave")
        emit sendSaveMasterSlaveState(false);
    else if(SaveWhat == "input")
        emit sendCloseInput(false);
    else if(SaveWhat == "shutdown")
        emit sendShutDown(2);
    else if(SaveWhat == "config_bound")
        emit sendSaveBound(false);
    else if(SaveWhat == "delete_car")
        emit sendDelete_car(false);
    else if(SaveWhat == "which_one")
        emit choiceChannel(2);
    else if(SaveWhat=="Logout")
        emit sendLogout(false);
    else if(SaveWhat =="User")
        emit sendSaveUser(false);
    else if(SaveWhat == "Align")
        emit sendClose();
    else if(SaveWhat == "Save")
        emit sendSave(false);
    else if(SaveWhat == "EmergencyStop")
        emit sendresetEmergencyStop(true);
    //    else if(SaveWhat.mid(0,7)  == "queue_1"||
    //            SaveWhat.mid(0,7)  == "queue_2"||
    //            SaveWhat.mid(0,8)  == "queue_no")
    //        emit sendClose();
    //this->close();
}

void Save::on_pushButton_cancel_clicked()
{
    if(SaveWhat == "shutdown")
        emit sendShutDown(3);
    else if(SaveWhat == "which_one")
        emit choiceChannel(0);
    else if(SaveWhat == "delete_car")
        emit sendDelete_car(false);
}

void Save::show_VIN(QString VIN)
{
    //    if(isVIN)
    //    {
    ui->label_3->setText(VIN);
    //    }
    //    else
    //    {
    //        ui->label_3->setText(tr("没有此条码"));
    //    }
    ui->pushButton->setGeometry(120,110,80,26);
    ui->pushButton->setText(tr("确定"));
    ui->pushButton->show();
    ui->pushButton_2->hide();
    ui->pushButton_cancel->show();
}

void Save::repairMySQL()
{
    system(QString("/usr/local/mysql/bin/myisamchk -c -r -o -f /usr/local/mysql/var/Tighten/"+Localtable+".MYI").toLocal8Bit().data());
    qDebug()<<"repair Mysql";
    qDebug()<<"after repair Mysql reboot";
    system("reboot");
    //    SaveWhat = "shutdown";
    //    ui->pushButton_2->setGeometry(120,110,80,26);
    //    ui->pushButton_2->setText(tr("重启"));
    //    ui->label_3->setText(tr("请重启"));
    //    ui->pushButton->hide();
    //    ui->pushButton_2->show();
}

void Save::alignQueue()
{
    bool AlignSuccess = false;
    QString VIN = "";
    WIFIlock.lockForRead();
    if(WIFI_STATE)
    {
        WIFIlock.unlock();
        if(true)
        {
            QSqlDatabase db1=QSqlDatabase::addDatabase("QODBC","AlignQueueConnection");
            db1.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=1;SQL_ATTR_CONNECTION_TIMEOUT=1");
            db1.setDatabaseName("sqltighten");
            db1.setPort(1433);
            db1.setUserName(SqlUserName);
            db1.setPassword(SqlPassword);
            QSqlQuery query1 = QSqlQuery(db1);

            if(!db1.open())
            {
                if(!db1.open())
                {
                    qDebug()<<"sqlserver align "<<db1.lastError().text();
                }
                else
                {
                    qDebug()<< "sqlserver align open ok 2";
                }
            }else
            {
                qDebug()<< "sqlserver align open ok 1";
            }

            QSqlDatabase db2=QSqlDatabase::addDatabase("QMYSQL","AlignMysqlconnection");
            db2.setHostName("localhost");
            db2.setDatabaseName("Tighten");
            db2.setUserName("root");
            db2.setPassword("123456");
            QSqlQuery query2 = QSqlQuery(db2);
            //            QSqlQuery query3 = QSqlQuery(db2);

            if(!db2.open())
            {
                if(!db2.open())
                {
                    qDebug()<< "fisupdate align "<< db2.lastError().text();
                }else
                {
                    qDebug()<< "fisupdate align open ok 2";
                }
            }else
            {
                qDebug()<< "fisupdate align open ok 1";
            }

            if(db1.isOpen() && db2.isOpen() && QSqlDatabase::contains("AlignQueueConnection") && QSqlDatabase::contains("AlignMysqlconnection"))
            {
                QString aff="";
                if(StationName == "QFCJ")
                    aff= "SELECT VIN FROM dbo.SubAssembly WHERE RecordId = 2";
                else if(StationName == "FDJ")
                    aff= "SELECT VIN FROM dbo.SubAssembly WHERE RecordId = 1";
                if(!query1.exec(aff))
                    qDebug() << "SELECT VIN Fail " << query1.lastError();
                else
                {
                    if(query1.next())
                    {
                        qDebug()<<"VIN :"<<query1.value(0).toString();
                        if(!query2.exec("SELECT RecordID, UseFlag,VIN from TaskPreview WHERE RecordID = (SELECT RecordID FROM TaskPreview WHERE VIN = '"+query1.value(0).toString()+"')-"+offSet))
                            qDebug()<<"SELECT RecordID, UseFlag fail"<<query2.lastError();
                        else
                        {
                            if(query2.next())
                            {
                                qDebug()<<"RecordID:"<<query2.value(0).toInt()<<"UseFlag:"<<query2.value(1).toInt()<<"VIN:"<<query2.value(2).toString();
                                VIN  =  query2.value(2).toString();
                                AlignSuccess = true;
                                //                                qDebug()<<"********Save*emit*QueueAlign**********";
                                //                                QueueAlign(query2.value(2).toString(),false,"VIN");
                            }
                            else
                            {
                                qDebug()<<"There is no VIN:"<<query1.value(0).toString();
                            }
                        }
                    }
                }
            }

            if(db1.isOpen())
                db1.close();
            if(db2.isOpen())
                db2.close();
        }
        QSqlDatabase::removeDatabase("AlignQueueConnection");
        QSqlDatabase::removeDatabase("AlignMysqlconnection");
    }
    else
    {
        WIFIlock.unlock();
    }
    if(AlignSuccess)
    {
        qDebug()<<"********Save*emit*QueueAlign**********";
        emit sendClose();
        emit QueueAlign(VIN,false,"VIN");
    }
    else
    {
        ui->label_3->setText(tr("对齐失败，请稍后重试！"));
        ui->pushButton_2->setGeometry(120,110,80,26);
        ui->pushButton_2->setText(tr("确定"));
        ui->pushButton->hide();
        ui->pushButton_2->show();
    }
}

void Save::revChangeEmergencyStop()
{
    isEmergencyStop = false;
    if(ui->pushButton->isHidden()&&ui->pushButton_2->isHidden())
    {
        ui->label_3->setText(tr("重新启动AGV小车？"));
        ui->pushButton_2->setGeometry(120,110,80,26);
        ui->pushButton_2->show();
        ui->pushButton_2->setText(tr("确定"));
    }
}

void Save::on_pushButton_3_clicked()
{
    ui->pushButton_3->hide();
    this->close();
}
