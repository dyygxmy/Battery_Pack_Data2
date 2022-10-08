#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QWidget>
#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QGridLayout>
#include <QVariantMap>
#include <QPalette>
#include <QDesktopWidget>
#include <QApplication>
#include <QLabel>
#include <QGroupBox>

#include <QDebug>

class DTColorLumpWidget;
class DTMessageBox : public QDialog
{
    Q_OBJECT
public:
    explicit DTMessageBox(QDialog *parent = 0);

    void setParameter (QVariantMap) ;
    void initStyleSheet( ) ;
    void initWidgetIO( ) ;

    QGridLayout *mainLayout;
    QGridLayout *layoutPHEVEN;
    QGridLayout *layoutIO;
#if 0
    QGroupBox   *groupBoxOld;
    QGroupBox   *groupBoxNew;
#endif
    QStackedLayout *stackLayout;
#if 0
    DTColorLumpWidget *oldPosIO;
    DTColorLumpWidget *newPosIO;
#endif
    QWidget     *WidgetPHEVEN;
    QWidget     *widgetIO;
    QPushButton *btnSave;
    QPushButton *btnCancel;
    QLineEdit   *editList;
    QLineEdit   *editYEN;
    QLineEdit   *editYTol;
    QLineEdit   *editAEN;
    QLineEdit   *editATol;
    QLineEdit   *editOEN;
    QLineEdit   *editOTol;
    QLabel      *labelPosIO;
    QLineEdit   *editPosIO;

    int indexRow;
    QString indexItem;
    QString editType;


signals:
    void signalUpdateDate(QVariantMap);
    void signalCancel();


public slots:
    void slotBtnSaveIsClick();
    void slotBtnCancelIsClick();
    void slotrecEncoderFromTight(qint64,qint64,QByteArray);
};

class DTColorLumpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DTColorLumpWidget(QWidget *parent = 0);

    QGridLayout *layout;
    QLabel *label[16];

public:
    void setColor(QString);

signals:

public slots:
};

#endif // MESSAGEBOX_H

