#ifndef DIALOGBINDTRAY_H
#define DIALOGBINDTRAY_H

#include <QDialog>

namespace Ui {
class DialogBindTray;
}

class DialogBindTray : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBindTray(QWidget *parent = 0);
    ~DialogBindTray();

private:
    Ui::DialogBindTray *ui;
};

#endif // DIALOGBINDTRAY_H
