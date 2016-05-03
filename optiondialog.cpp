#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    // this->setCursor(Qt::BlankCursor); //去掉光标
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::on_pushButton_clicked()
{
    emit sendoptioninfo(ui->lineEdit_bx1name->text(),ui->lineEdit_bx1code->text(),true);
}

void OptionDialog::on_pushButton_2_clicked()
{
    //CHA
    emit sendoptioninfo("","",false);
}

void OptionDialog::setOptext(QString a, QString b)
{
   ui->lineEdit_bx1name->setText(a);
   ui->lineEdit_bx1code->setText(b);
}
