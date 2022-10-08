#include "dialogbindtray.h"
#include "ui_dialogbindtray.h"

DialogBindTray::DialogBindTray(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBindTray)
{
    ui->setupUi(this);
}

DialogBindTray::~DialogBindTray()
{
    delete ui;
}
