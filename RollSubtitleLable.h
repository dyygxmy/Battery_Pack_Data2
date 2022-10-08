#ifndef ROLLSUBTITLELABLE_H
#define ROLLSUBTITLELABLE_H

#include <QWidget>
#include <QLabel>
#include <QObject>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>
class RollSubtitleLable : public QLabel
{
    Q_OBJECT
public:
    explicit RollSubtitleLable(QWidget *parent = 0);

signals:

public slots:
    void getText(QString);
    void paintEvent(QPaintEvent *);
    void updateIndex();
private:
    int charWidth;
    int curIndex;
    QString showText;

};

#endif // ROLLSUBTITLELABLE_H
