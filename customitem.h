#ifndef CUSTOMITEM_H
#define CUSTOMITEM_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>

class CustomItem : public QWidget
{
    Q_OBJECT
public:
    explicit CustomItem(QWidget *parent, const QPixmap& pixmap,
                        const QString& text, const int& id);

signals:

public slots:

private:
    QLabel icon;
    QLabel name;
    int id;


};

#endif // CUSTOMITEM_H
