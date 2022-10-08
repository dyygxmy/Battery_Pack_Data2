#include "ftpjsonparse.h"

FtpJsonParse::FtpJsonParse(QObject *parent) :
    QObject(parent)
{
    jsonParse_thread.start();
    this->moveToThread(&jsonParse_thread);
}

void FtpJsonParse::ftpStart()
{
    qDebug() << "PLCFtpJsonParse thread start!!!!!!!!!!!!" ;
    bzero(&screwid_enable,sizeof(screwid_enable));
    //    channel_is_true = false;
    pro_is_true = false;
    groupNum = -1;
    connect(&ftpupload,SIGNAL(timeout()),this,SLOT(ftptimeouts()));
    ftpupload.start(1000);
}

void FtpJsonParse::mysqlopen()
{
    if(QSqlDatabase::contains("ftpPLCMysqlconnection")){
        db2 = QSqlDatabase::database("ftpPLCMysqlconnection");
    }else{
        db2=QSqlDatabase::addDatabase("QMYSQL","ftpPLCMysqlconnection");
        db2.setHostName("localhost");
        db2.setDatabaseName("Tighten");
        db2.setUserName("root");
        db2.setPassword("123456");
        query2 = QSqlQuery(db2);
    }

    if(!db2.open())
    {
        if(!db2.open())
        {
            qDebug()<< "ftp localmysql "<< db2.lastError().text();
        }else
        {
            qDebug()<< "ftp localmysql ok2 ";
        }
    }else
    {
        qDebug()<< "ftp localmysql ok1 ";
    }
}

/*
*     数据库关闭
*/
void FtpJsonParse::sqlclose()
{
    if(db2.isOpen())
        db2.close();
}

void FtpJsonParse::ftptimeouts()
{
    FindFile("/tmp/Curve");
}

void FtpJsonParse::FindFile(QString path)
{
    //qDebug() << "find file1";
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
            qDebug() << "find fil3";
        }
        else
        {
            //nFiles++;
            // qDebug() << QString("%1 %2 %3").arg(fileInfo.size(), 10)
            // .arg(fileInfo.fileName(),10).arg(fileInfo.path())<<endl;
            qDebug() << "find file parsing!!" << fileInfo.baseName();
            parseFile(fileInfo.absoluteFilePath(),fileInfo.baseName());

        }
        i++;
    }while(i<list.size());
}

void FtpJsonParse::parseFile(QString filepath,QString fileName)
{
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

        if(fileName.mid(3,3)=="BIW")
        {
            system(QString("mv "+ filepath+" /tmp/wrongCurve/").toLocal8Bit().data());
            qDebug() << "manual move file "+filepath+" to /tmp/wrongCurve";
            return;
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

        QString MI[2],WI[2],State[2],Program[2];

        for(int m=0;m<2;m++)
        {
            QString Date_time,DATE_blc,TIME_blc,STATE_blc,MI_blc,WI_blc,Cycle_CSR,Program_blc,Channel,Curve;
            QVariantMap curveMap = result["channels"].toList().at(m).toMap();
//            Curve = result["channels"].toList().at(m).toString();
            QVariantList curveList;
            curveList << curveMap;
            QJson::Serializer serializer;
            QByteArray curveJson = serializer.serialize(curveList,&ok);
            Curve = curveJson.mid(1,curveJson.length()-2);
//            qDebug() << "curve*********" << Curve;
            STATE_blc = curveMap["result"].toString();
            Channel = QString::number(curveMap["node id"].toDouble()*10);
            Program_blc = curveMap["prg nr"].toString();
            Date_time = curveMap["date"].toString();
            DATE_blc = Date_time.mid(0,10);
            TIME_blc = Date_time.mid(11,8);
            Cycle_CSR = curveMap["cycle"].toString();

            //        qDebug() << "id code:" << result["id code"].toString().mid(0,17);

            QVariantMap tightening_steps=curveMap["tightening steps"].toList().last().toMap();
            int size = tightening_steps["tightening functions"].toList().size();
            //    qDebug()<< size;
            for(int i=0;i<size;i++)
            {
                QVariantMap tightening_functions=tightening_steps["tightening functions"].toList().at(i).toMap();
                if(tightening_functions["name"].toString()=="MF AngleMax")
                    WI_blc = tightening_functions["act"].toString();
                if(tightening_functions["name"].toString()=="MF TorqueMax")
                    MI_blc = tightening_functions["act"].toString();
            }
            if(MI_blc == "")
                MI_blc = "0";
            if(WI_blc  == "")
                WI_blc = "0";

            MI[m] = MI_blc;
            WI[m] = WI_blc;
            State[m] = STATE_blc;
            Program[m] = Program_blc;

            if(Program_blc != "99" && fileName.mid(3,17)==VIN_PIN_SQL)
            {
                qDebug()<<"***********"<<Channel<<Program_blc;
                struct Return screw_groupnum= configParse(Channel,Program_blc,STATE_blc);
                screwid[Channel.toInt()-1] = screw_groupnum.screw;
                groupNum = screw_groupnum.groupnum;
            }
            else if(Program_blc == "99")
            {
                emit VIN_Match_Wrong("99", Channel);
                if(screwid[Channel.toInt()-1]=="")
                    screwid[Channel.toInt()-1]="100000000";
            }
            else if(fileName.mid(3,17)!=VIN_PIN_SQL)
            {
                screwid[Channel.toInt()-1] ="400000000";
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
                demo.data_model[6] = fileName.mid(3,17);
                demo.data_model[7] = Cycle_CSR;
                demo.data_model[8] = Curve;
                demo.data_model[9] = Program_blc;
                demo.data_model[10] = Channel;
                qDebug()<<"emit date_model:"<<demo.data_model[0]<<demo.data_model[1]<<demo.data_model[2]<<demo.data_model[3]<<demo.data_model[4]<<demo.data_model[5]<<demo.data_model[6]<<demo.data_model[7]<<demo.data_model[9]<<demo.data_model[10];
                QVariant DataVar;
                DataVar.setValue(demo);
                emit sendfromjsonthread(DataVar);
            }
            else
            {
//                qDebug() << "What are you ? "<< "file: "+fileName;
                //            unlink(filepath.toLocal8Bit().data());
//                system(QString("mv "+ filepath+" /tmp/wrongCurve/").toLocal8Bit().data());
//                qDebug() << "screwid match fail move file "+filepath+" to /tmp/wrongCurve";
            }
        }

        if(screwid[0] != "" && screwid[1] != "")
        {
            if(screwid[0]!="200000000"&&screwid[0]!="300000000"&&screwid[1]!="200000000"&&screwid[1]!="300000000"&&fileName.mid(3,17)==VIN_PIN_SQL)
            {
                emit send_mainwindow(MI[0],WI[0],State[0],MI[1],WI[1],State[1]);
                unlink(filepath.toLocal8Bit().data());
                qDebug() << "delete file "+filepath;
            }
            else if(screwid[0]=="200000000"||screwid[0]=="300000000")
            {
                emit VIN_Match_Wrong("1",Program[0]);//匹配失败
                system(QString("mv "+ filepath+" /tmp/wrongCurve/").toLocal8Bit().data());
                qDebug() << "match fail channel1 move file "+filepath+" to /tmp/wrongCurve";
            }
            else if(screwid[1]=="200000000"||screwid[1]=="300000000")
            {
                emit VIN_Match_Wrong("2",Program[1]);//匹配失败
                system(QString("mv "+ filepath+" /tmp/wrongCurve/").toLocal8Bit().data());
                qDebug() << "match fail channel2 move file "+filepath+" to /tmp/wrongCurve";
            }
            else if(fileName.mid(3,17) != VIN_PIN_SQL)
            {
                unlink(filepath.toLocal8Bit().data());
                qDebug() << "delete file wrong VIN "+filepath;
            }
        }
        else
        {
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

struct Return FtpJsonParse::configParse(QString channel_tmp, QString program_tmp,QString state_ok)
{
    Q_UNUSED(state_ok);
    //匹配VIN码
    QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
    QString value = "";
    int groupnum=0;
    int j = 0;
    for(j = 0;j< 20;j++)
    {
        if(1 == channel_tmp.toInt())
        {
            QString program = carInfor[channel_tmp.toInt()-1].proNo[j];//config->value(QString("carinfo").append(QString::number(whichar)).append("/ProNum").append(QString::number(j))).toInt();
            SCREWID_SQL = carInfor[channel_tmp.toInt()-1].boltSN[j];//config->value(QString("carinfo").append(QString::number(whichar)).append("/LuoSuanNum").append(QString::number(j))).toString();
            if(program_tmp == program)
            {
                value  = SCREWID_SQL;
                groupnum = j;
                break;
            }
            else
            {
                pro_is_true = true;
                continue;
            }
        }
        else if(2 == channel_tmp.toInt())
        {
            QString program = carInfor[channel_tmp.toInt()-1].proNo[j];//config->value(QString("carinfo").append(QString::number(whichar)).append("/ProNum").append(QString::number(j))).toInt();
            SCREWID_SQL = carInfor[channel_tmp.toInt()-1].boltSN[j];//config->value(QString("carinfo").append(QString::number(whichar)).append("/LuoSuanNum").append(QString::number(j))).toString();
            if(program_tmp == program)
            {
                value  = SCREWID_SQL;
                groupnum = j;
                break;
            }
            else
            {
                pro_is_true = true;
                continue;
            }
        }
    }
    if(j == 20)
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
    struct Return returntmp;
    returntmp.screw=value;
    returntmp.groupnum=groupnum;

    return returntmp;
}
