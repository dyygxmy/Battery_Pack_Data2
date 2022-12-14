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
    if(stepName == "??????")
        return 100;
    else if(stepName =="???????????????")
        return 101;
    else if(stepName =="??????")
        return 102;
    else if(stepName == "?????????????????????")
        return 99;
    else if(stepName =="??????I/O??????")
        return 103;
    else if(stepName =="I/O??????")
        return 104;
    else if(stepName =="??????")  //
        return 105;
    else if(stepName =="??????PDM")
        return 106;
    else if(stepName =="??????")
        return 107;
    else if(stepName =="?????????????????????")
        return 108;
    else if(stepName =="?????????????????????")
        return 111;
    else if(stepName =="??????PACK1")
        return 149;
    else if(stepName =="??????PACK2")
        return 150;
    else if(stepName =="??????????????????????????????")
        return 151;
    else if(stepName =="??????????????????")
        return 152;
    else if(stepName == "??????????????????")
        return 15;
    else if(stepName == "??????????????????")
        return 22;
    else if(stepName == "???????????????PCL????????????")
        return 30;
    else if(stepName == "??????????????????")
        return 1;
    else if(stepName == "????????????????????????")
        return 21;
    else if(stepName == "??????????????????????????????")
        return 2;
    else if(stepName == "???????????????????????????")
        return 9;
    else if(stepName == "????????????????????????")
        return 26;
    else if(stepName == "???????????????????????????")
        return 16;
    else if(stepName == "????????????????????????????????????AGV?????????")
        return 25;
    else if(stepName == "???????????????????????????")
        return 27;
    else if(stepName == "????????????????????????")
        return 28;
    else if(stepName == "????????????????????????")
        return 29;
    else if(stepName == "??????????????????????????????")
        return 10;

    else if(stepName == "??????????????????")
        return 15;
    else if(stepName == "??????????????????????????????")
        return 18;
    else if(stepName == "???????????????PCL????????????")
        return 30;
    else if(stepName == "???????????????????????????")
        return 3;
    else if(stepName == "????????????????????????")
        return 4;
    else if(stepName == "??????????????????????????????")
        return 6;
    else if(stepName == "??????????????????")
        return 37;
    else if(stepName == "??????????????????")
        return 17;
    else if(stepName == "????????????????????????")
        return 20;
    else if(stepName == "??????/????????????")
        return 8;
    else if(stepName == "??????????????????")
        return 49;
    else if(stepName == "????????????")
        return 50;
    else if(stepName == "Pack1??????")
        return 51;
    else if(stepName == "Pack2??????")
        return 52;
    else if(stepName == "??????????????????????????????")
        return 53;
    else if(stepName == "????????????????????????")
        return 54;
    else if(stepName == "????????????????????????")
        return 201;
    else if(stepName == "????????????Pack1??????")
        return 202;
    else if(stepName == "??????????????????????????????")
        return 203;
    else if(stepName == "Pack??????")
        return 204;
    else if(stepName == "Pack1SN")
        return 205;
    else if(stepName == "Pack2SN")
        return 206;
    else
        return 0;
}


QVariantMap ParseJson::parseJsonEncoder()
{

    QFile encoderJson("/encoder.json");

    if(encoderJson.open(QIODevice::ReadOnly))
    {
        QByteArray json = encoderJson.readAll();

        QJson::Parser parser;
        bool ok;

        QVariantMap currentStep = parser.parse(json, &ok).toMap();
        qDebug();
        if (!ok)
        {
            qDebug()<<"encoder.json An error occurred during parsing";
        }
        else
        {
            return currentStep;
            qDebug()<<currentStep;
        }
    }
    else
    {
        qDebug()<<"encoder.json encoderJson read open fail";
    }
    QVariantMap errorMap;
    errorMap["errorCode"] = 1;
    return errorMap;
}
