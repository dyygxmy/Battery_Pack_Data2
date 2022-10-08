#include "showpicturedialog.h"
#include "ui_showpicturedialog.h"
#include <QFileInfo>
#include "GlobalVarible.h"

showPictureDialog::showPictureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showPictureDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setGeometry(QRect(0, 0, 1366, 768));
    filePath = "/config.ini";
    fileIndex = 0;
    ui->label_5->hide();
    QSettings config(filePath,QSettings::IniFormat);
    QString allSuff;
    //有配置就取配置，没配置就配置一个给个默认值
    if(config.contains("baseinfo/showPicture")){
        allSuff = config.value("baseinfo/showPicture").toString();
        ui->lineEdit_pdmname->setText(allSuff);
    }else{
        allSuff = ui->lineEdit_pdmname->text();
        config.setValue("baseinfo/showPicture",allSuff);
    }

    dir.setPath("/PDM/");
    if(!dir.exists()){
        dir.mkpath(dir.absolutePath());
    }
    showPictureIni(allSuff);
}

showPictureDialog::~showPictureDialog()
{
    delete ui;
}

//按设定格式初始显示的图片
void showPictureDialog::showPictureIni(QString allSuff)
{
    QStringList sufList = allSuff.trimmed().split(",");
    QStringList fileListTemp = dir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries);
    fileList.clear();
    for(int i = 0;i<fileListTemp.size();i++){
        QFileInfo fileInfo(fileListTemp.at(i));
        QString suf = fileInfo.suffix();
        if(sufList.contains(suf)){
            fileList<<fileListTemp.at(i);
        }
    }
    fileCount = fileList.size();
    if(fileCount > 0){
        img.load(dir.filePath(fileList.at(fileIndex)));
        ui->label_2->setText("共有"+QString::number(fileCount)+"张图片，当前第"+QString::number(fileIndex+1)+"张");
        ui->label_3->setText(dir.absoluteFilePath(fileList.at(fileIndex)));
    }else{
        ui->label_2->setText("共有"+QString::number(fileCount)+"张图片，当前第"+QString::number(fileIndex)+"张");
    }
    ui->label->setPixmap(QPixmap::fromImage(img));
}

//退出
void showPictureDialog::on_pushButton_clicked()
{
    this->close();
}

//上一张
void showPictureDialog::on_pushButton_2_clicked()
{
    if(0 == fileIndex){
        fileIndex = fileCount - 1;
    }else{
        fileIndex--;
    }
    if(fileCount > 0){
        img.load(dir.filePath(fileList.at(fileIndex)));
        ui->label_2->setText("共有"+QString::number(fileCount)+"张图片，当前第"+QString::number(fileIndex+1)+"张");
        ui->label_3->setText(dir.absoluteFilePath(fileList.at(fileIndex)));
    }
    ui->label->setPixmap(QPixmap::fromImage(img));
}

//下一张
void showPictureDialog::on_pushButton_3_clicked()
{
    if(fileIndex == fileCount - 1){
        fileIndex = 0;
    }else{
        fileIndex++;
    }
    if(fileCount > 0){
        img.load(dir.filePath(fileList.at(fileIndex)));
        ui->label_2->setText("共有"+QString::number(fileCount)+"张图片，当前第"+QString::number(fileIndex+1)+"张");
        ui->label_3->setText(dir.absoluteFilePath(fileList.at(fileIndex)));
    }
    ui->label->setPixmap(QPixmap::fromImage(img));
}

//保存
void showPictureDialog::on_pushButton_32_clicked()
{
    e3 = new QGraphicsOpacityEffect(this);
    e3->setOpacity(0.5);
    ui->label_5->setGraphicsEffect(e3);
    ui->label_5->show();
    ui->label_5->setGeometry(0,0,1366,768);
    SaveWhat = "baseinfo";
    save = new Save(this);
    connect(save,SIGNAL(sendSaveBaseinfo(bool)),this,SLOT(receiveChoice(bool)));
    save->show();
}

//选择是
void showPictureDialog::receiveChoice(bool cho)
{
    if(cho){
        fileIndex = 0;
        QSettings config(filePath,QSettings::IniFormat);
        config.setValue("baseinfo/showPicture",ui->lineEdit_pdmname->text());
        showPictureIni(ui->lineEdit_pdmname->text());
    }
    ui->label_5->hide();
    e3->deleteLater();
    save->deleteLater();
}

//取消
void showPictureDialog::on_pushButton_21_clicked()
{
    QSettings config(filePath,QSettings::IniFormat);
    ui->lineEdit_pdmname->setText(config.value("baseinfo/showPicture").toString());
}
