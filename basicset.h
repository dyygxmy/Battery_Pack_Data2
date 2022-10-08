#ifndef BASICSET_H
#define BASICSET_H

#include <QDialog>

namespace Ui {
class BasicSet;
}

class BasicSet : public QDialog
{
    Q_OBJECT
    
public:
    explicit BasicSet(QWidget *parent = 0);
    ~BasicSet();
public:
signals:
    void sendBaseinfo(QString,QString,QString,QString);
    void sendbaseinfocancel();
    void sendTruncate();
    
private slots:
    void on_pushButton_37_clicked();

    void on_pushButton_38_clicked();

    void on_radioButton_clicked(bool checked);

    void on_radioButton_2_clicked(bool checked);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_new_clicked();

private:
    Ui::BasicSet *ui;
    int clickTimes;
public:
    void   setchecking();
    void   setConfigValue351(QString,QString);
    void   setSerialOrRfidMode(QString, QString, QString);
    void   setServerValue(QString,QString,QString,QString);

public slots:
    void receiveResult(bool);
};

#endif // BASICSET_H
