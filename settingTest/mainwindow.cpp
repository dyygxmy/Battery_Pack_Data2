#include "mainwindow.h"
#include <QSettings>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    QSettings* configIniRead = new QSettings("/root/Desktop/agvsConfig.ini",QSettings::IniFormat);
    configIniRead->setValue("main/make",12345);
    //configIniRead->setValue("main/vinum",1321);
    configIniRead->setValue("long/vinum","h");

}

MainWindow::~MainWindow()
{

}
