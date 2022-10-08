#ifndef SHOWPICTUREDIALOG_H
#define SHOWPICTUREDIALOG_H

#include <QDialog>
#include <QImage>
#include <QDir>
#include <QSettings>
#include "save.h"
#include <QGraphicsOpacityEffect>

namespace Ui {
class showPictureDialog;
}

class showPictureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit showPictureDialog(QWidget *parent = 0);
    ~showPictureDialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_32_clicked();

    void on_pushButton_21_clicked();

    void receiveChoice(bool);
private:
    Ui::showPictureDialog *ui;
    QImage img;
    QDir dir;
    QStringList fileList;
    int fileCount;
    int fileIndex;
    QString filePath;
    void showPictureIni(QString);
    Save *save;
    QGraphicsOpacityEffect *e3;
};

#endif // SHOWPICTUREDIALOG_H
