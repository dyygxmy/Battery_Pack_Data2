#include "save.h"
#include "ui_save.h"
#include "GlobalVarible.h"
Save::Save(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Save)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    // this->setCursor(Qt::BlankCursor); //去掉光标
     ui->pushButton_cancel->hide();

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
        ui->label_3->setText(tr("PDM名字重复!请修改"));
        ui->pushButton->setGeometry(120,110,80,26);
        ui->pushButton_2->hide();
    }
    else if(SaveWhat == "pdmdel")
    {
        ui->label_3->setText(tr("删除PDM设置？"));
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
        ui->label_3->setText(tr("数据库出错!请找维修人员"));
        ui->pushButton->setGeometry(120,110,80,26);
        ui->pushButton->hide();
        ui->pushButton_2->hide();
    }
    else if(SaveWhat == "delete_car")
    {
        ui->label_3->setText(tr("请扫删除车辆条码"));
        ui->pushButton_2->setGeometry(120,110,80,26);
        ui->pushButton->hide();
        ui->pushButton_2->setText(tr("取消"));
    }
}

Save::~Save()
{
    delete ui;
}

void Save::on_pushButton_clicked()
{
    //yes

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
        emit sendShutDown(2);
    else if(SaveWhat == "config_bound")
        emit sendSaveBound(true);
    else if(SaveWhat == "delete_car")
        emit sendDelete_car(true);

}

void Save::on_pushButton_2_clicked()
{
    //no
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
    //this->close();
}

void Save::on_pushButton_cancel_clicked()
{
    emit sendShutDown(3);
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
}
