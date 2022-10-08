#ifndef STEPEDITBEV_H
#define STEPEDITBEV_H

#include <QWidget>
#include <QTableWidget>
#include <QDebug>
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include "stepparameter.h"
#include "save.h"
#include "GlobalVarible.h"
#include "parsejson.h"

#include "./json/serializer.h"
#include "./json/parser.h"

namespace Ui {
class stepeditbev;
}

class stepeditbev : public QWidget
{
    Q_OBJECT

public:
    explicit stepeditbev(QWidget *parent = 0);
    ~stepeditbev();

signals:
    void stepParaInit();
private slots:
    void on_pushButton_return_clicked();

    void on_pushButton_append_clicked();

    void on_pushButton_remove_clicked();

    void on_pushButton_insert_clicked();

    void on_radioButton_manual_clicked();

    void on_radioButton_auto_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_update_clicked();

public slots:
    void receiveStep(QVariantMap);
    void closeStepParameter();
    void saveStep(bool);

private:
    Ui::stepeditbev *ui;
    void initTable();
    void newStepParameter();
    void newSave();
    void initUI(QVariantMap);
    QGraphicsOpacityEffect *e3;
    StepParameter *stepParameter;
    int insertRow;
    enum operate{add,insert,update};
    int currentOperate;
    QVariantList stepList;
    QVariantMap currentStep;
    Save * save;
    QString whichSave;
};

#endif // STEPEDITBEV_H
