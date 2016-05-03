#ifndef XMLCREATE_H
#define XMLCREATE_H

#include <QObject>
#include <QThread>
#include <QSettings>
#include <QDebug>
#include <QIODevice>
#include <QDomDocument>
#include <QFile>
#include <QBuffer>
#include <QImage>

class XMLCreate : public QObject
{
    Q_OBJECT
public:
    explicit XMLCreate(QObject *parent = 0);
    
signals:
    
public slots:
    void XML_CreateStart();
    void updateXML();

private:
    QThread xmlcreate_thread;
    struct Parameter
    {
        QString BasicPara[23];//基本参数设置
        QString CarType[20][590];//20个车型信息
        QString PDM[820];//PDM信息
    };

public:
    void creat_xml(XMLCreate::Parameter *p);
    void create_car(XMLCreate::Parameter *demo,QDomDocument document,QDomElement item2,int n);
    
};

#endif // XMLCREATE_H
