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
    QRegExp rx("[0-9]{1,15}");
    QValidator *validator = new QRegExpValidator(rx, this);
    QRegExp rx3("^((([1-9]{0,1}|2[0-4]|1\\d)\\d|25[0-5])(\\.|$)){4}");
    QValidator *validator3 = new QRegExpValidator(rx3, this);
    ui->lineEdit_controllerIp_ch1->setValidator(validator3);
    ui->lineEdit_controllerIp_ch2->setValidator(validator3);
    ui->lineEdit_RfidIp->setValidator(validator3);
    ui->lineEdit_RfidPort->setValidator(validator);
    ui->lineEdit_DataServerIp->setValidator(validator3);
    ui->lineEdit_CurveServerIp->setValidator(validator3);
    ui->lineEdit_AndonServerIp->setValidator(validator3);
    ui->lineEdit_BarcodeGun->setValidator(validator);
    ui->lineEdit_port->setValidator(validator);

    if(SaveWhat == "Controller")
        ui->stackedWidget->setCurrentIndex(0);
//    else if(SaveWhat == "RFID")
//        ui->stackedWidget->setCurrentIndex(1);
    else if(SaveWhat == "server")
        ui->stackedWidget->setCurrentIndex(2);
    else if(SaveWhat == "Barcode")
        ui->stackedWidget->setCurrentIndex(3);


        ui->label_21->hide();
        ui->lineEdit_CurveServerIp->hide();
        ui->label_29->hide();
        ui->lineEdit_AndonServerIp->hide();
        ui->label_31->show();
        ui->lineEdit_port->show();
        ui->label_31->setGeometry(157,166,141,41);
        ui->lineEdit_port->setGeometry(301,170,161,34);


    clickTimes = 0;
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
    if(SaveWhat == "Controller")
    {
        QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
        if(ui->comboBox->currentText()=="OFF")
            configIniRead->setValue("baseinfo/ControlType_1","OFF");
        else if(ui->comboBox->currentText()=="Bosch_CS351")
            configIniRead->setValue("baseinfo/ControlType_1","CS351");
        else if(ui->comboBox->currentText()=="Bosch_Nexo")
            configIniRead->setValue("baseinfo/ControlType_1","Nexo");
        else if(ui->comboBox->currentText()=="Bosch_SB356")
            configIniRead->setValue("baseinfo/ControlType_1","SB356");
        else if(ui->comboBox->currentText()=="Bosch_SB356_PLC")
            configIniRead->setValue("baseinfo/ControlType_1","SB356_PLC");
        else if(ui->comboBox->currentText()=="Apex_Cleco")
            configIniRead->setValue("baseinfo/ControlType_1","Cleco");
        else if(ui->comboBox->currentText()=="Atlas_PF_D")
            configIniRead->setValue("baseinfo/ControlType_1","PFD");
        else if(ui->comboBox->currentText()=="Atlas_PF_C")
            configIniRead->setValue("baseinfo/ControlType_1","PFC");
        else if(ui->comboBox->currentText()=="Atlas_PM_M")
            configIniRead->setValue("baseinfo/ControlType_1","PMM");
        else if(ui->comboBox->currentText()=="Atlas_PM")
            configIniRead->setValue("baseinfo/ControlType_1","PM");
        else if(ui->comboBox->currentText()=="Atlas_PM600")
            configIniRead->setValue("baseinfo/ControlType_1","PM600");
        else if(ui->comboBox->currentText()=="Ingersoll_Rand")
            configIniRead->setValue("baseinfo/ControlType_1","IR");
        else if(ui->comboBox->currentText()=="GWK")
            configIniRead->setValue("baseinfo/ControlType_1","GWK");
        else if(ui->comboBox->currentText()=="PF_XML")
            configIniRead->setValue("baseinfo/ControlType_1","PF_XML");
        else if(ui->comboBox->currentText()=="PF_232")
            configIniRead->setValue("baseinfo/ControlType_1","PF_232");


        if(ui->comboBox_2->currentText()=="OFF")
            configIniRead->setValue("baseinfo/ControlType_2","OFF");
        else if(ui->comboBox_2->currentText()=="Bosch_CS351")
            configIniRead->setValue("baseinfo/ControlType_2","CS351");
        else if(ui->comboBox_2->currentText()=="Bosch_Nexo")
            configIniRead->setValue("baseinfo/ControlType_2","Nexo");
        else if(ui->comboBox_2->currentText()=="Bosch_SB356")
            configIniRead->setValue("baseinfo/ControlType_2","SB356");
        else if(ui->comboBox_2->currentText()=="Bosch_SB356_PLC")
            configIniRead->setValue("baseinfo/ControlType_2","SB356_PLC");
        else if(ui->comboBox_2->currentText()=="Apex_Cleco")
            configIniRead->setValue("baseinfo/ControlType_2","Cleco");
        else if(ui->comboBox_2->currentText()=="Atlas_PF_D")
            configIniRead->setValue("baseinfo/ControlType_2","PFD");
        else if(ui->comboBox_2->currentText()=="Atlas_PF_C")
            configIniRead->setValue("baseinfo/ControlType_2","PFC");
        else if(ui->comboBox_2->currentText()=="Atlas_PM_M")
            configIniRead->setValue("baseinfo/ControlType_2","PMM");
        else if(ui->comboBox_2->currentText()=="Atlas_PM")
            configIniRead->setValue("baseinfo/ControlType_2","PM");
        else if(ui->comboBox_2->currentText()=="Atlas_PM600")
            configIniRead->setValue("baseinfo/ControlType_2","PM600");
        else if(ui->comboBox_2->currentText()=="Ingersoll_Rand")
            configIniRead->setValue("baseinfo/ControlType_2","IR");
        else if(ui->comboBox_2->currentText()=="GWK")
            configIniRead->setValue("baseinfo/ControlType_2","GWK");
        else if(ui->comboBox_2->currentText()=="PF_XML")
            configIniRead->setValue("baseinfo/ControlType_2","PF_XML");
        else if(ui->comboBox_2->currentText()=="PF_232")
            configIniRead->setValue("baseinfo/ControlType_2","PF_232");

        if(ui->comboBox_3->currentIndex()==0)
            configIniRead->setValue("baseinfo/factory","SVW2");

        delete configIniRead;
        emit sendBaseinfo(ui->lineEdit_controllerIp_ch1->text(),ui->lineEdit_controllerIp_ch2->text(),"","");
    }
//    else if(SaveWhat == "RFID")
//        emit sendBaseinfo(ui->lineEdit_RfidIp->text(),ui->lineEdit_RfidPort->text(),"");
    else if (SaveWhat == "server")
        emit sendBaseinfo(ui->lineEdit_DataServerIp->text(),ui->lineEdit_CurveServerIp->text(),ui->lineEdit_AndonServerIp->text(),ui->lineEdit_port->text());
    else if (SaveWhat == "Barcode")
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
        emit sendBaseinfo(ui->lineEdit_RfidIp->text(),ui->lineEdit_RfidPort->text(),ui->lineEdit_BarcodeGun->text(),"");
    }
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

void BasicSet::setConfigValue351(QString controller1_Ip, QString controller2_Ip)
{
    ui->lineEdit_controllerIp_ch1->setText(controller1_Ip);
    ui->lineEdit_controllerIp_ch2->setText(controller2_Ip);
    QStringList ControllerList;
    ControllerList<<"OFF"<<"Bosch_CS351"<<"Bosch_Nexo"<<"Bosch_SB356"<<"Bosch_SB356_PLC"<<"Apex_Cleco"<<"Atlas_PF_D"<<"Atlas_PF_C"<<"Atlas_PM_M"<<"Atlas_PM"<<"Atlas_PM600"<<"Ingersoll_Rand"<<"GWK"<<"PF_XML"<<"PF_232";
    ui->comboBox->addItems(ControllerList);
    ui->comboBox_2->addItems(ControllerList);
    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
    if(configIniRead->value("baseinfo/ControlType_1").toString()=="OFF")
        ui->comboBox->setCurrentIndex(0);
    else if(configIniRead->value("baseinfo/ControlType_1").toString()=="CS351"||configIniRead->value("baseinfo/ControlType_1").toString()=="")
        ui->comboBox->setCurrentIndex(1);
    else if(configIniRead->value("baseinfo/ControlType_1").toString()=="Nexo")
        ui->comboBox->setCurrentIndex(2);
    else if(configIniRead->value("baseinfo/ControlType_1").toString()=="SB356")
        ui->comboBox->setCurrentIndex(3);
    else if(configIniRead->value("baseinfo/ControlType_1").toString()=="SB356_PLC")
        ui->comboBox->setCurrentIndex(4);
    else if(configIniRead->value("baseinfo/ControlType_1").toString()=="Cleco")
        ui->comboBox->setCurrentIndex(5);
    else if(configIniRead->value("baseinfo/ControlType_1").toString()=="PFD")
        ui->comboBox->setCurrentIndex(6);
    else if(configIniRead->value("baseinfo/ControlType_1").toString()=="PFC")
        ui->comboBox->setCurrentIndex(7);
    else if(configIniRead->value("baseinfo/ControlType_1").toString()=="PMM")
        ui->comboBox->setCurrentIndex(8);
    else if(configIniRead->value("baseinfo/ControlType_1").toString()=="PM")
        ui->comboBox->setCurrentIndex(9);
    else if(configIniRead->value("baseinfo/ControlType_1").toString()=="PM600")
        ui->comboBox->setCurrentIndex(10);
    else if(configIniRead->value("baseinfo/ControlType_1").toString()=="IR")
        ui->comboBox->setCurrentIndex(11);
    else if(configIniRead->value("baseinfo/ControlType_1").toString()=="GWK")
        ui->comboBox->setCurrentIndex(12);
    else if(configIniRead->value("baseinfo/ControlType_1").toString()=="PF_XML")
        ui->comboBox->setCurrentIndex(13);
    else if(configIniRead->value("baseinfo/ControlType_1").toString()=="PF_232")
        ui->comboBox->setCurrentIndex(14);

    if(configIniRead->value("baseinfo/ControlType_2").toString()=="OFF"||configIniRead->value("baseinfo/ControlType_2").toString()=="")
    {
        ui->comboBox_2->setCurrentIndex(0);
        ui->label_7->hide();
        ui->lineEdit_controllerIp_ch2->hide();
    }
    else if(configIniRead->value("baseinfo/ControlType_2").toString()=="CS351")
        ui->comboBox_2->setCurrentIndex(1);
    else if(configIniRead->value("baseinfo/ControlType_2").toString()=="Nexo")
        ui->comboBox_2->setCurrentIndex(2);
    else if(configIniRead->value("baseinfo/ControlType_2").toString()=="SB356")
        ui->comboBox_2->setCurrentIndex(3);
    else if(configIniRead->value("baseinfo/ControlType_2").toString()=="SB356_PLC")
        ui->comboBox_2->setCurrentIndex(4);
    else if(configIniRead->value("baseinfo/ControlType_2").toString()=="Cleco")
        ui->comboBox_2->setCurrentIndex(5);
    else if(configIniRead->value("baseinfo/ControlType_2").toString()=="PFD")
        ui->comboBox_2->setCurrentIndex(6);
    else if(configIniRead->value("baseinfo/ControlType_2").toString()=="PFC")
        ui->comboBox_2->setCurrentIndex(7);
    else if(configIniRead->value("baseinfo/ControlType_2").toString()=="PMM")
        ui->comboBox_2->setCurrentIndex(8);
    else if(configIniRead->value("baseinfo/ControlType_2").toString()=="PM")
        ui->comboBox_2->setCurrentIndex(9);
    else if(configIniRead->value("baseinfo/ControlType_2").toString()=="PM600")
        ui->comboBox_2->setCurrentIndex(10);
    else if(configIniRead->value("baseinfo/ControlType_2").toString()=="IR")
        ui->comboBox_2->setCurrentIndex(11);
    else if(configIniRead->value("baseinfo/ControlType_2").toString()=="GWK")
        ui->comboBox_2->setCurrentIndex(12);
    else if(configIniRead->value("baseinfo/ControlType_2").toString()=="PF_XML")
        ui->comboBox_2->setCurrentIndex(13);
    else if(configIniRead->value("baseinfo/ControlType_2").toString()=="PF_232")
        ui->comboBox_2->setCurrentIndex(14);

    QStringList FactoryList;
    FactoryList<<tr("大众二厂");
    ui->comboBox_3->addItems(FactoryList);

        ui->comboBox_3->setCurrentIndex(0);



    delete configIniRead;
}

void BasicSet::setSerialOrRfidMode(QString tmp1, QString tmp2, QString tmp3)
{
    if(isRFID)
    {
        ui->lineEdit_RfidIp->setText(tmp1);
        ui->lineEdit_RfidPort->setText(tmp2);
    }
    else
    {
        ui->lineEdit_RfidIp->setStyleSheet("font: 14pt \"黑体\";border-width:1px;border-style:solid;border-color:rgb(150, 150, 150); ");
        ui->lineEdit_RfidIp->setEnabled(false);
        ui->lineEdit_RfidPort->setStyleSheet("font: 14pt \"黑体\";border-width:1px;border-style:solid;border-color:rgb(150, 150, 150); ");
        ui->lineEdit_RfidPort->setEnabled(false);
    }
    if(isBarCode)
    {
        setchecking();
        ui->lineEdit_BarcodeGun->setText(tmp3);
    }
    else
    {
        ui->lineEdit_BarcodeGun->setStyleSheet("font: 14pt \"黑体\";border-width:1px;border-style:solid;border-color:rgb(150, 150, 150); ");
        ui->lineEdit_BarcodeGun->setEnabled(false);
        ui->radioButton->setStyleSheet("QRadioButton{font: 14pt \"黑体\";spacing: 10 }  QRadioButton::indicator { width: 25px; height: 25px;image: url(:/LCD_CS351/LCD_CS351/logo/Radio_gray.png);}");
        ui->radioButton_2->setStyleSheet("QRadioButton{font: 14pt \"黑体\";spacing: 10 }  QRadioButton::indicator { width: 25px; height: 25px;image: url(:/LCD_CS351/LCD_CS351/logo/Radio_gray.png);}");
    }
}

void BasicSet::setServerValue(QString tmp1, QString tmp2, QString tmp3, QString tmp4)
{
    ui->lineEdit_DataServerIp->setText(tmp1);
    ui->lineEdit_CurveServerIp->setText(tmp2);
    ui->lineEdit_AndonServerIp->setText(tmp3);
    ui->lineEdit_port->setText(tmp4);
}

void BasicSet::on_pushButton_clicked()
{
    clickTimes++;
    if(clickTimes!=10)
        return;
    ui->stackedWidget->setCurrentIndex(1);
}

void BasicSet::on_pushButton_2_clicked()
{
    emit sendTruncate();
}

void BasicSet::receiveResult(bool tmp)
{
    if(tmp)
        ui->label_5->setText("Success!");
    else
        ui->label_5->setText("Fail!");
}
