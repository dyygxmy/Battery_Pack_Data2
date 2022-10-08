#ifndef SAVE_H
#define SAVE_H

#include <QDialog>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class Save;
}

class Save : public QDialog
{
    Q_OBJECT
    
public:
    explicit Save(QWidget *parent = 0);
    ~Save();
public slots:
    void revChangeEmergencyStop();
signals:
    void sendSaveAdvancedState(bool);
    void sendSaveBaseinfo(bool);
    void sendDeSingle(bool);
    void sendSaveMasterSlaveState(bool);
    void sendCloseInput(bool);
    void sendShutDown(int);
    void sendSaveBound(bool);
    void sendDelete_car(bool);
    void sendClose();
    void change_mode();
    void choiceChannel(int);
    void sendLogout(bool);
    void sendSaveUser(bool);
    void QueueAlign(QString,bool,QString);//发给UI线程
    void sendSave(bool);
    void sendresetEmergencyStop(bool);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_cancel_clicked();
    void show_VIN(QString);
    void repairMySQL();
    void alignQueue();

    void on_pushButton_3_clicked();

private:
    Ui::Save *ui;
    QString StationName;
    QString offSet;
    bool isEmergencyStop;
};

#endif // SAVE_H
