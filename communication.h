#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <qmath.h>
#include <QSettings>
#include <QDebug>
#include <QBuffer>
#include <QFileDialog>
#include <QIODevice>
#include <QDomDocument>
#include <QTextStream>
#include <iostream>

class Communication : public QObject
{
    Q_OBJECT

signals:
    void update();
    void xmlcreate();

public slots:
    void mycommunicationtimer();
    void connection();
    void CMD();
    QString ToSixteen(int decimal);

private:
    QThread communication_thread;

    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QByteArray rev_data;
    QByteArray data;

    char Work_state;//0: 休息 1：工作中
    char Work_Reset;//0:无信号 1:复位信号
    char Work_ShutDown;//0:无信号1:关机信号
    char Work_Restart;//0:无信号 1:重启信号


    int index;
    QByteArray six_data;
    int tran_data_len;//待发数据长度
    int rev_data_len;//待收数据长度
    int update_len;//更新文件长度
    QString update_filename;//更新文件名称
    QByteArray bag;//接收数据的拼接包
    QByteArray bag_buff;//拼接包转成数组
    int plus_flag;//八号命令接收数据是否拼接
    int bag_len;//拼接包长度
    int dowm_len;//接收数据长度
    int file_dowm;//xml 文件接收成功
    int update_dowm;//更新文件接收成功
    struct Parameter
    {
        QString BasicPara[23];//基本参数设置
        QString CarType[20][590];//20个车型信息
        QString PDM[820];//PDM信息
    };

public:
    explicit Communication(QObject *parent = 0);

    void updateINI();
    void checkUpdate();
    void update_program();
    void Analy_xml(Communication::Parameter *p);


};

#endif // COMMUNICATION_H
