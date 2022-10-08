#include "customitem.h"

CustomItem::CustomItem(QWidget *parent, const QPixmap& pixmap,
                       const QString& text, const int& id) :
    QWidget(parent)
{
    setMinimumSize(170,70);
    icon.setGeometry(0,0,70,70);
    icon.setPixmap(pixmap);
    icon.show();
    name.setGeometry(70,0,100,70);
    name.setText(text);
    name.show();
}
