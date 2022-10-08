#ifndef STEPPARAMETER_H
#define STEPPARAMETER_H

#include <QDialog>
#include <QDebug>
#include <QSettings>
#include <QDir>

namespace Ui {
class StepParameter;
}

class StepParameter : public QDialog
{
    Q_OBJECT

public:
    explicit StepParameter(QWidget *parent = 0);
    ~StepParameter();
    void initUI(QVariantMap);

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_radioButton_request_clicked();

    void on_radioButton_completed_clicked();

    void on_radioButton_manual_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_close_clicked();

    void on_comboBox_condition1_currentIndexChanged(const QString &arg1);

signals:
    void sendStep(QVariantMap);
    void closeDiglag();

private:
    Ui::StepParameter *ui;
    bool judgment();
    void initPDM();
};

#endif // STEPPARAMETER_H
