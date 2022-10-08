#ifndef ROOTDIALOG_H
#define ROOTDIALOG_H

#include <QDialog>
#include <QApplication>
#include <mainwindow.h>
#include <newconfiginfo.h>
#include <QDateTime>
#include "passwordpanel.h"
#include <QPixmap>
#include <QScreen>        // 用来截取全屏
#include <QDesktopWidget>
#include "codeedit.h"
#include "stepedit.h"
#include "stepeditbev.h"
#include "showpicturedialog.h"
//#include <QGraphicsBlurEffect>

namespace Ui {
class RootDialog;
}

class RootDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RootDialog(QWidget *parent = 0);
    ~RootDialog();
    QString vari1_r;
signals:
    void sendKeyToStep(bool);
    void sendWarningSpearhead();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
    void timerUpdate();
    void wifishow(bool);

    void on_pushButton_11_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_code_clicked();

    void on_pushButton_step_clicked();

    void on_pushButton_step_BEV_clicked();

    void on_pushButton_code_2_clicked();

public slots:
    void batteryshow1(QString);
    void init(MainWindow *w, Newconfiginfo *newconfiginfo);
//    void shutdown(int);
    void receiveResult(bool);
    void logout(bool);
    void reboot();
    void poweroff();
    void closeDialog();
    void closeSave();
    void battery15();


private:
    Ui::RootDialog *ui;
    QTimer timer_showdown;
    int battry_num;
    QGraphicsOpacityEffect *e3;
    QGraphicsBlurEffect *e0;
    Save * save;
    bool isLogin;
    PasswordPanel * passwordPanel;
    bool isSystem;
    bool isProgram;
    bool isPdm;
    bool isHistory;
    bool isUser;
    enum enumFunctions{systemConfig,program,pdm,history,user};
    QPixmap fullScreenPixmap;
    QPushButton *pushButton_reboot;
    QPushButton *pushButton_poweroff;
    QPushButton *pushButton_blur;
//    enum keys{noKey,redKey,greenKey,blackKey};
    int tmpKey;

public:
    MainWindow *w;
    Newconfiginfo *c;
    QString EnterWhich;
    CodeEdit *codeEdit;
    StepEdit *stepEdit;
    stepeditbev *stepEditBev;
    showPictureDialog *fileDialog;

public slots:
    void receiveKey(int);
};

#endif // ROOTDIALOG_H
