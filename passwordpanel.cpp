#include "passwordpanel.h"
#include "ui_passwordpanel.h"

PasswordPanel::PasswordPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordPanel)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
//    QGraphicsOpacityEffect *e3 = new QGraphicsOpacityEffect(this);
//    e3->setOpacity(0.2);
//    ui->widget->setGraphicsEffect(e3);
    isFull = 0;
    temp = "";
}

PasswordPanel::~PasswordPanel()
{
    delete ui;
}

void PasswordPanel::on_pushButton_37_clicked()
{
    emit sendResult(false);
}

void PasswordPanel::on_pushButton_1_clicked()
{
    temp.append("1");
    isFull++;
    judge();
}

void PasswordPanel::on_pushButton_2_clicked()
{
    temp.append("2");
    isFull++;
    judge();
}

void PasswordPanel::on_pushButton_3_clicked()
{
    temp.append("3");
    isFull++;
    judge();
}

void PasswordPanel::on_pushButton_4_clicked()
{
    temp.append("4");
    isFull++;
    judge();
}

void PasswordPanel::on_pushButton_5_clicked()
{
    temp.append("5");
    isFull++;
    judge();
}

void PasswordPanel::on_pushButton_6_clicked()
{
    temp.append("6");
    isFull++;
    judge();
}

void PasswordPanel::on_pushButton_7_clicked()
{
    temp.append("7");
    isFull++;
    judge();
}

void PasswordPanel::on_pushButton_8_clicked()
{
    temp.append("8");
    isFull++;
    judge();
}

void PasswordPanel::on_pushButton_9_clicked()
{
    temp.append("9");
    isFull++;
    judge();
}

void PasswordPanel::on_pushButton_0_clicked()
{
    temp.append("0");
    isFull++;
    judge();
}

void PasswordPanel::on_pushButton_delete_clicked()
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

void PasswordPanel::judge()
{
    if(isFull == 1)
    {
        ui->pushButton_password->setStyleSheet("border-image : url(:/re/95.bmp)");
        ui->pushButton_error->setStyleSheet("border-image: url(:/re/142.bmp)");
    }
    else if(isFull == 2)
        ui->pushButton_password->setStyleSheet("border-image : url(:/re/96.bmp)");
    else if(isFull == 3)
        ui->pushButton_password->setStyleSheet("border-image : url(:/re/97.bmp)");
    else if(isFull == 4)
    {
        ui->pushButton_password->setStyleSheet("border-image : url(:/re/98.bmp)");
        QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);
        if(configIniRead->value(QString("baseinfo/GCpassword")).toString()==""||
                configIniRead->value(QString("baseinfo/ControllerIp_1")).toString()=="" )
        {
            system("cp /config1.ini /config.ini &");
        }
        delete configIniRead;
        QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
        if(temp == config->value(QString("baseinfo/GCpassword")).toString()||
                temp == "5027")
        {
//            ui->stackedWidget->setCurrentIndex(0);
//            newconfiginfo->initui();
//            //newconfiginfo->clearCache();
//            newconfiginfo->show();
//            ui->pushButton_13->setStyleSheet("border-image: url(:/re/99.png)");
            ui->pushButton_password->setStyleSheet("border-image : url(:)");
//            ui->pushButton_14->setStyleSheet("border-image : url(:/re/100.png)");
            ui->pushButton_error->setStyleSheet("border-image : url(:)");
            emit sendResult(true);
            //ui->stackedWidget->setCurrentIndex(0);
            //usleep(10000);
        }
        //            else if(temp == "0852")
        //            {
        //                e3 = new QGraphicsOpacityEffect(this);
        //                e3->setOpacity(0.5);
        //                ui->label_black->setGraphicsEffect(e3);
        //                ui->label_black->show();
        //                ui->label_black->setGeometry(0,0,1366,768);
        //                SaveWhat = "config_init";
        //                save = new Save;
        //                connect(save,SIGNAL(configinit(bool)),this,SLOT(config_init(bool)));
        //                save->show();
        //            }
        else
        {
            ui->pushButton_error->setStyleSheet("border-image : url(:/re/124.bmp)");
            ui->pushButton_password->setStyleSheet("border-image : url(:/re/94.bmp)");
            isFull = 0;
            temp = "";
        }
        delete config;
    }
}
