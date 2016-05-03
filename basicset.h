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
    void sendBaseinfo(QString,QString,QString);
    void sendbaseinfocancel();
    
private slots:
    void on_pushButton_37_clicked();

    void on_pushButton_38_clicked();

    void on_radioButton_clicked(bool checked);

    void on_radioButton_2_clicked(bool checked);

private:
    Ui::BasicSet *ui;
public:
    void   setchecking();
    void   setConfigValue351(QString,QString,QString);
    void   setSerialOrRfidMode(QString,QString);
    void   setServerValue(QString,QString,QString);
};

#endif // BASICSET_H
