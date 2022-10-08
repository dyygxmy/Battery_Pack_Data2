#include "parsejson.h"

ParseJson::ParseJson(QObject *parent) :
    QObject(parent)
{
}
/***********************************
 *
 * read json parameter
 *
***********************************/
QVariantMap ParseJson::parseJson()
{
    if(GPackType == "PHEV")
    {
        QFile stepJson("/step.json");
        if(stepJson.open(QIODevice::ReadOnly))
        {
            QByteArray json = stepJson.readAll();

            QJson::Parser parser;
            bool ok;

            QVariantMap currentStep = parser.parse(json, &ok).toMap();
            if (!ok)
            {
                qDebug()<<"step.json An error occurred during parsing";
            }
            else
            {
                return currentStep;
            }
        }
        else
        {
            qDebug()<<"step.json stepJson read open fail";
        }
    }
    else
    {
        QFile stepJson("/stepbev.json");
        if(stepJson.open(QIODevice::ReadOnly))
        {
            QByteArray json = stepJson.readAll();

            QJson::Parser parser;
            bool ok;

            QVariantMap currentStep = parser.parse(json, &ok).toMap();
            if (!ok)
            {
                qDebug()<<"step.json An error occurred during parsing";
            }
            else
            {
                return currentStep;
            }
        }
        else
        {
            qDebug()<<"step.json stepJson read open fail";
        }
    }
    QVariantMap errorMap;
    errorMap["errorCode"] = 1;
    return errorMap;
}

QVariantMap ParseJson::parseJsonbev()
{
    QFile stepJson("/stepbev.json");
    if(stepJson.open(QIODevice::ReadOnly))
    {
        QByteArray json = stepJson.readAll();

        QJson::Parser parser;
        bool ok;

        QVariantMap currentStep = parser.parse(json, &ok).toMap();
        if (!ok)
        {
            qDebug()<<"step.json An error occurred during parsing";
        }
        else
        {
            return currentStep;
        }
    }
    else
    {
        qDebug()<<"step.json stepJson read open fail";
    }
    QVariantMap errorMap;
    errorMap["errorCode"] = 1;
    return errorMap;
}

/********************************************
 * step name change to type code
 *
 * *****************************************/
int ParseJson::stepNameToTypeCode(QString stepName)
{
    if(stepName == "挂起")
        return 100;
    else if(stepName =="条码枪扫码")
        return 101;
    else if(stepName =="拧紧")
        return 102;
    else if(stepName =="等待I/O输入")
        return 103;
    else if(stepName =="I/O输出")
        return 104;
    else if(stepName =="跳转")  //
        return 105;
    else if(stepName =="显示PDM")
        return 106;
    else if(stepName =="泄漏")
        return 107;
    else if(stepName =="辊道线数据绑定")
        return 108;
    else if(stepName =="操作PACK1")
        return 149;
    else if(stepName =="操作PACK2")
        return 150;
    else if(stepName =="向服务器请求产品型号")
        return 151;
    else if(stepName =="手工站站完成")
        return 152;
    else if(stepName == "辊道托盘到位")
        return 15;
    else if(stepName == "下线托盘型号")
        return 22;
    else if(stepName == "站完成请求PCL放行托盘")
        return 30;
    else if(stepName == "请求产品型号")
        return 1;
    else if(stepName == "请求模组产品型号")
        return 21;
    else if(stepName == "请求机器人拧紧位置号")
        return 2;
    else if(stepName == "请求堵头安装位置号")
        return 9;
    else if(stepName == "请求下箱体位置号")
        return 26;
    else if(stepName == "请求模组入箱位置号")
        return 16;
    else if(stepName == "请求捆绑流水号、料车号（AGV到位）")
        return 25;
    else if(stepName == "请求下箱体条码扫描")
        return 27;
    else if(stepName == "请求条码数据绑定")
        return 28;
    else if(stepName == "请求辊道托盘类型")
        return 29;
    else if(stepName == "请求记录堵头安装数据")
        return 10;

    else if(stepName == "辊道托盘到位")
        return 15;
    else if(stepName == "托盘到位匹配产品型号")
        return 18;
    else if(stepName == "站完成请求PCL放行托盘")
        return 30;
    else if(stepName == "机器人到达拧紧位置")
        return 3;
    else if(stepName == "已收到站完成信号")
        return 4;
    else if(stepName == "机器人翻转下箱体完成")
        return 6;
    else if(stepName == "模组入箱完成")
        return 17;
    else if(stepName == "本次模组搬运完成")
        return 20;
    else if(stepName == "手动/自动模式")
        return 8;
    else if(stepName == "启动开班信号")
        return 49;
    else if(stepName == "开班信号")
        return 50;
    else if(stepName == "Pack1状态")
        return 51;
    else if(stepName == "Pack2状态")
        return 52;
    else if(stepName == "请求机器人涂胶位置号")
        return 53;
    else if(stepName == "涂胶数据绑定完成")
        return 54;
    else if(stepName == "涂胶拧紧开班信号")
        return 201;
    else if(stepName == "涂胶拧紧Pack1状态")
        return 202;
    else if(stepName == "涂胶拧紧请求产品型号")
        return 203;
    else if(stepName == "Pack1SN")
        return 205;
    else if(stepName == "Pack2SN")
        return 206;
    else
        return 0;
}
