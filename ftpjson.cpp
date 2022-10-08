#include "ftpjson.h"
#include "GlobalVarible.h"

FtpJson::FtpJson(QObject *parent) :
    QObject(parent)
{
    json_thread.start();
    this->moveToThread(&json_thread);
}

void FtpJson::myjson()
{
    qDebug()<< "FtpJson thread start";
    bzero(&screwid_enable,sizeof(screwid_enable));
//    channel_is_true = false;
    pro_is_true = false;
    connect(&jsonParse,SIGNAL(timeout()),this,SLOT(jsontimeout()));
    jsonParse.start(1000);
}

void FtpJson::jsontimeout()
{
    FindFile("/tmp/Curve");
}

void FtpJson::FindFile(QString path)
{
    QDir dir(path);
    if (!dir.exists())
       return ;
    dir.setFilter(QDir::Dirs|QDir::Files);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    int i=0;

    do{
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.fileName()=="."||fileInfo.fileName()=="..")
        {
            i++;
            continue;
        }
        bool bisDir=fileInfo.isDir();
        if(bisDir)
        {
            //nFiles++;
            // qDebug() << QString("%1 %2 %3").arg(fileInfo.size(), 10)
           // qDebug() << "find file2";
            FindFile(fileInfo.filePath());
            //qDebug() << "find fil3";
        }
        else
        {
            qDebug()<<"find "<<fileInfo.filePath();
            //nFiles++;
            // qDebug() << QString("%1 %2 %3").arg(fileInfo.size(), 10)
            // .arg(fileInfo.fileName(),10).arg(fileInfo.path())<<endl;
           // qDebug() << "find file parsing!!";
            if(fileInfo.fileName().mid(3,17) != VIN_PIN_SQL)
            {
                VIN_PIN_SQL = fileInfo.fileName().mid(3,17);
//                isParse = false;
                emit coming_VIN(VIN_PIN_SQL);
                screwid[0]="";
                screwid[1]="";
                screwid[2]="";
                bzero(&screwid_enable,sizeof(screwid_enable));
                if(matchVin())
                {
                    parseFile(fileInfo.absoluteFilePath(),fileInfo.fileName());
                }
                else
                {                    
                    qDebug()<<"match VIN FAIL: "<<VIN_PIN_SQL;
                    VIN_PIN_SQL="";
                  //没匹配上
                    emit VIN_Match_Wrong("VIN","");
//                    unlink(fileInfo.absoluteFilePath().toLocal8Bit().data());
                    system(QString("mv "+ fileInfo.absoluteFilePath()+" /tmp/wrongCurve/").toLocal8Bit().data());
                    qDebug() << "file "+fileInfo.fileName()<<" VIN match fail";
                }
            }
            else
                parseFile(fileInfo.absoluteFilePath(),fileInfo.fileName());
        }
        i++;
    }while(i<list.size());
}

void FtpJson::parseFile(QString filepath,QString fileName)
{   
    Q_UNUSED(fileName);
//    int num = fileName.mid(21,3).toInt();
    QJson::Parser parser;
    bool ok;

    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly| QIODevice::Text))
    {
        qDebug() << "Cannot open testdsn file for Reading";
    }
    QByteArray json = file.readAll();
    if(json.simplified().right(4) == "}] }")
    {
        QMap<QString,int>::iterator iter;
        for(iter = wrongMap.begin(); iter != wrongMap.end(); ++iter)
        {
            if(filepath == iter.key())
            {
                wrongMap.erase(iter);
                qDebug()<<"delete "+filepath+" from wrongMap";
                break;
            }
        }
        QVariantMap result = parser.parse(json, &ok).toMap();
        if (!ok) {
//            qFatal("An error occurred during parsing");
            qDebug()<<"An error occurred during parsing"<<json;
//            exit (1);
            system(QString("mv "+ filepath+" /tmp/wrongCurve/").toLocal8Bit().data());
            qDebug() << "parser fail move file "+filepath+" to /tmp/wrongCurve";
            return;
        }

        QString Date_time,DATE_blc,TIME_blc,STATE_blc,MI_blc,WI_blc,Cycle_CSR,Program,Channel,MI_MAX,MI_MIN;

        STATE_blc = result["result"].toString();
        Channel = QString::number(result["node id"].toDouble()*10);
        Program = result["prg nr"].toString();
        Date_time = result["date"].toString();
        DATE_blc = Date_time.mid(0,10);
        TIME_blc = Date_time.mid(11,8);
        Cycle_CSR = result["cycle"].toString();

//        qDebug() << "id code:" << result["id code"].toString().mid(0,17);

        QVariantMap tightening_steps=result["tightening steps"].toList().last().toMap();
        int size = tightening_steps["tightening functions"].toList().size();
    //    qDebug()<< size;
        for(int i=0;i<size;i++)
        {
            QVariantMap tightening_functions=tightening_steps["tightening functions"].toList().at(i).toMap();
            if(tightening_functions["name"].toString()=="MF TorqueMax")
            {
                MI_blc = tightening_functions["act"].toString();
                MI_MAX = tightening_functions["nom"].toString();
            }
            if(tightening_functions["name"].toString()=="MF TorqueMin")
                MI_MIN = tightening_functions["nom"].toString();
            if(tightening_functions["name"].toString()=="MF AngleMax")
                WI_blc = tightening_functions["act"].toString();
        }

        if(MI_blc == "")
        {
            for(int i=0;i<size;i++)
            {
                QVariantMap tightening_functions=tightening_steps["tightening functions"].toList().at(i).toMap();
                if(tightening_functions["name"].toString()=="TF Torque")
                    MI_blc = tightening_functions["act"].toString();
            }
            if(MI_blc == "")
                MI_blc = "0";
        }
        if(WI_blc  == "")
        {
            for(int i=0;i<size;i++)
            {
                QVariantMap tightening_functions=tightening_steps["tightening functions"].toList().at(i).toMap();
                if(tightening_functions["name"].toString()=="TF Angle")
                    WI_blc = tightening_functions["act"].toString();
            }
            if(WI_blc == "")
                WI_blc = "0";
        }
        if(MI_MAX == "")
            MI_MAX = "0";
        if(MI_MIN == "")
            MI_MIN = "0";

        if(Program != "99")
            screwid[Channel.toInt()-1] = configParse(Channel,Program,STATE_blc);
        else
        {
            emit VIN_Match_Wrong("99", Channel);
            if(screwid[Channel.toInt()-1]=="")
                screwid[Channel.toInt()-1]="100000000";
        }
        qDebug()<<"screwid:" << screwid[Channel.toInt()-1];

        if(screwid[Channel.toInt()-1] != "")
        {
            DATA_STRUCT demo;
            demo.data_model[0] = DATE_blc;
            demo.data_model[1] = TIME_blc;
            demo.data_model[2] = STATE_blc;
            demo.data_model[3] = MI_blc;
            demo.data_model[4] = WI_blc;
            demo.data_model[5] = screwid[Channel.toInt()-1];
            demo.data_model[6] = VIN_PIN_SQL;
            demo.data_model[7] = Cycle_CSR;
            demo.data_model[8] = json;
            demo.data_model[9] = Program;
            demo.data_model[10] = Channel;


                demo.data_model[11] = Type;
                if(STATE_blc == "OK")
                {
                    demo.data_model[12] = QString::number(BoltOrder[0]++);
                }
                else
                {
                    demo.data_model[12] = QString::number(BoltOrder[0]);
                }
                demo.data_model[13] = MI_MAX;
                demo.data_model[14] = MI_MIN;
                demo.data_model[15] = "0";


            qDebug()<<"emit date_model:"<<demo.data_model[0]<<demo.data_model[1]<<demo.data_model[2]<<demo.data_model[3]<<demo.data_model[4]<<demo.data_model[5]<<demo.data_model[6]<<demo.data_model[7]<<demo.data_model[9]<<demo.data_model[10];
            QVariant DataVar;
            DataVar.setValue(demo);
            emit sendfromjsonthread(DataVar);
            if(Program != "99"&&screwid[Channel.toInt()-1]!="200000000"&&screwid[Channel.toInt()-1]!="300000000")
            {
                emit send_mainwindow(MI_blc,WI_blc,STATE_blc,Channel.toInt(),screwid[Channel.toInt()-1].toInt(),Program.toInt());
                unlink(filepath.toLocal8Bit().data());
                qDebug() << "delete file "+filepath;
            }
            else if(screwid[Channel.toInt()-1]=="200000000"||screwid[Channel.toInt()-1]=="300000000")
            {
                emit VIN_Match_Wrong(Channel,Program);//匹配失败
                system(QString("mv "+ filepath+" /tmp/wrongCurve/").toLocal8Bit().data());
                qDebug() << "match fail move file "+filepath+" to /tmp/wrongCurve";
            }
            else if(Program =="99")
            {
                unlink(filepath.toLocal8Bit().data());
                qDebug() << "delete file Program 99 "+filepath;
            }
            else
            {
                unlink(filepath.toLocal8Bit().data());
                qDebug() << "delete file ????? "+filepath;
            }
        }
        else
        {
//            qDebug() << "What are you ? "<< "file: "+fileName;
//            unlink(filepath.toLocal8Bit().data());
            system(QString("mv "+ filepath+" /tmp/wrongCurve/").toLocal8Bit().data());
            qDebug() << "screwid match fail move file "+filepath+" to /tmp/wrongCurve";
        }

    }
    else
    {
        qDebug() << filepath <<" is incomplete or wrong";
        QMap<QString,int>::iterator iter;
        bool newfile = true;
        for(iter=wrongMap.begin(); iter != wrongMap.end(); ++iter)
        {
            if(filepath == iter.key())
            {
                newfile = false;
//                wrongMap[filepath]++;
                ++iter.value();
                if(iter.value()==5)
                {
                    wrongMap.erase(iter);
                    qDebug()<<"delete "+filepath+" from wrongMap >5 times";
                    system(QString("mv "+ filepath+" /tmp/wrongCurve/").toLocal8Bit().data());
//                    unlink(filepath.toLocal8Bit().data());
                    qDebug() << "move file "+filepath+" to /tmp/wrongCurve";
                }
                break;
            }
        }
        if(newfile)
        {
            wrongMap[filepath]=1;
            qDebug()<<"add newfile "+filepath+" to wrongMap";
        }
    }
}

QString FtpJson::configParse(QString chanel_tmp, QString program_tmp,QString state_ok)
{
    //匹配VIN码
    QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
    QString value = "";
    int j = 1;
    for(j = 1;j< 21;j++)
    {
        if(screwid_enable[j-1] == 3)
            continue;
        int channel = config->value(QString("carinfo").append(QString::number(whichar)).append("/Channel").append(QString::number(j))).toInt();
        int program = config->value(QString("carinfo").append(QString::number(whichar)).append("/ProNum").append(QString::number(j))).toInt();
        SCREWID_SQL = config->value(QString("carinfo").append(QString::number(whichar)).append("/LuoSuanNum").append(QString::number(j))).toString();
        lsNum = config->value(QString("carinfo").append(QString::number(whichar)).append("/LSNumber").append(QString::number(j))).toString();
        if(channel == chanel_tmp.toInt())
        {
            if(program_tmp.toInt() == program)
            {
                value  = SCREWID_SQL;
                if(state_ok == "OK")
                    screwid_enable[j-1] = 3;
                else if(state_ok == "NOK")
                    screwid_enable[j-1]++;
                break;
            }
            else
            {
                pro_is_true = true;
                continue;
            }
        }
        else
            continue;
    }
    if(j == 21)
    {
        //没有此通道的数据匹配上
        if(pro_is_true)
        {
            value = "200000000";
            pro_is_true = false;
        }
        else
            value = "300000000";
    }
    delete config;
    return value;
}

bool FtpJson::matchVin()
{
    QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
    bool flags = false;
    int i = 1;
    for(i = 1;i < 21;i++)
    {
        QString temp = config->value(QString("carinfo").append(QString::number(i)).append("/VIN")).toString();
        if(temp.length()!=5)
        {
            continue;
        }
        int replacetmp = 0;
        flags = true;
        for(int k = 3;k < 8;k++)
        {
            replacetmp = k-3;
            if(temp.at(replacetmp) == '?')
            {
                continue;
            }
            else
            {
                if(VIN_PIN_SQL.at(k) != temp.at(replacetmp))
                {
                    flags = false;
                    break;
                }else
                {
                    continue;
                }
            }
        }
        if(flags)
        {
            whichar = i;
            break;
        }
    }
    delete config;
    return flags;
}
