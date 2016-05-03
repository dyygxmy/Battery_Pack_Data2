#include <QSettings>
#include "basicset.h"
#include "ui_basicset.h"
#include "GlobalVarible.h"
#include <QDebug>
BasicSet::BasicSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BasicSet)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
   // this->setCursor(Qt::BlankCursor); //去掉光标
    QRegExp rx("[0-9.]{7,15}");
    QValidator *validator = new QRegExpValidator(rx, this );
    ui->lineEdit_cs351Ip->setValidator(validator);
    ui->lineEdit_PortA->setValidator(validator);
    ui->lineEdit_PortB->setValidator(validator);
    ui->lineEdit_RfidIp->setValidator(validator);
    ui->lineEdit_RfidPort->setValidator(validator);
    ui->lineEdit_DataServerIp->setValidator(validator);
    ui->lineEdit_CurveServerIp->setValidator(validator);
    ui->lineEdit_AndonServerIp->setValidator(validator);
    ui->lineEdit_BarcodeGun->setValidator(validator);

    if(SaveWhat == "cs351")
        ui->stackedWidget->setCurrentIndex(0);
    else if(SaveWhat == "RFID")
        ui->stackedWidget->setCurrentIndex(1);
    else if(SaveWhat == "server")
        ui->stackedWidget->setCurrentIndex(2);
    else if(SaveWhat == "Gun")
        ui->stackedWidget->setCurrentIndex(3);

}

BasicSet::~BasicSet()
{
    delete ui;
}

void BasicSet::on_pushButton_37_clicked()
{

    emit sendbaseinfocancel();

}

void BasicSet::on_pushButton_38_clicked()
{
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    if(SerialGunMode)
    {
        if(ui->radioButton_2->isChecked())
        {
           SerialGunMode = false;
           configIniRead->setValue("baseinfo/serialMode","0");
        }

    }
    else
    {
        if(ui->radioButton->isChecked())
        {
           SerialGunMode = true;
           configIniRead->setValue("baseinfo/serialMode","1");
        }
    }
    delete configIniRead;
    if(SaveWhat == "cs351")
        emit sendBaseinfo(ui->lineEdit_cs351Ip->text(),ui->lineEdit_PortA->text(),ui->lineEdit_PortB->text());
    else if(SaveWhat == "RFID")
        emit sendBaseinfo(ui->lineEdit_RfidIp->text(),ui->lineEdit_RfidPort->text(),"");
    else if (SaveWhat == "server")
        emit sendBaseinfo(ui->lineEdit_DataServerIp->text(),ui->lineEdit_CurveServerIp->text(),ui->lineEdit_AndonServerIp->text());
    else if (SaveWhat == "Gun")
        emit sendBaseinfo(ui->lineEdit_BarcodeGun->text(),"","");
   // this->close();
}

void BasicSet::on_radioButton_clicked(bool checked)
{
    if(checked)
    {
       if(!SerialGunMode)
       {
           // serial gun set wifi
         ui->radioButton_2->setChecked(false);
       }
    }
}

void BasicSet::on_radioButton_2_clicked(bool checked)
{
    if(checked)
    {
        if(SerialGunMode)
        {
            //serial gun set no_wifi
          ui->radioButton->setChecked(false);
        }
    }
}
void BasicSet::setchecking()
{
    if(SerialGunMode)
    {
      ui->radioButton->setChecked(true);
      ui->radioButton_2->setChecked(false);
    }
    else
    {
        ui->radioButton_2->setChecked(true);
        ui->radioButton->setChecked(false);
    }
}

void BasicSet::setConfigValue351(QString tmp1, QString tmp2, QString tmp3)
{

    ui->lineEdit_cs351Ip->setText(tmp1);
    ui->lineEdit_PortA->setText(tmp2);
    ui->lineEdit_PortB->setText(tmp3);
}

void BasicSet::setSerialOrRfidMode(QString tmp1, QString tmp2)
{
    if(isRFID == 1)
    {
        ui->lineEdit_RfidIp->setText(tmp1);
        ui->lineEdit_RfidPort->setText(tmp2);
    }
    else
    {
        ui->lineEdit_BarcodeGun->setText(tmp1);
    }
}

void BasicSet::setServerValue(QString tmp1, QString tmp2, QString tmp3)
{
    ui->lineEdit_DataServerIp->setText(tmp1);
    ui->lineEdit_CurveServerIp->setText(tmp2);
    ui->lineEdit_AndonServerIp->setText(tmp3);
}
