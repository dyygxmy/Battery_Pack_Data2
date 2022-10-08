#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include "GlobalVarible.h"

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OptionDialog(QWidget *parent = 0);
    ~OptionDialog();
   void  setOptext(QString ,QString);
    
private slots:

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

public:
signals:
    void sendoptioninfo(QString,QString,bool);

private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H
