#include "xmlcreate.h"
#include "GlobalVarible.h"

XMLCreate::XMLCreate(QObject *parent) :
    QObject(parent)
{
    xmlcreate_thread.start();
    this->moveToThread(&xmlcreate_thread);
}

void XMLCreate::XML_CreateStart()
{
    qDebug()<<"xmlcreate_thread start";
    updateXML();
}

//生成新filename.xml
void XMLCreate::updateXML()
{
    qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@  xml update  @@@@@@@@@@@@@@@@@@@@@@@@@@";

    QSettings *configIniRead = new QSettings("/config.ini", QSettings::IniFormat);

    struct Parameter demo1;

    //基本信息
    demo1.BasicPara[0] = configIniRead->value("baseinfo/StationName").toString();
    demo1.BasicPara[1] = configIniRead->value("baseinfo/StationId").toString();
    demo1.BasicPara[2] = configIniRead->value("baseinfo/LocalIp").toString();
    demo1.BasicPara[3] = configIniRead->value("baseinfo/ControlType").toString();
    demo1.BasicPara[4] = configIniRead->value("baseinfo/cs351Ip").toString();
    demo1.BasicPara[5] = configIniRead->value("baseinfo/PortA").toString();
    demo1.BasicPara[6] = configIniRead->value("baseinfo/PortB").toString();
    demo1.BasicPara[7] = configIniRead->value("baseinfo/isRFID").toString();
    demo1.BasicPara[8] = configIniRead->value("baseinfo/RfidIp").toString();
    demo1.BasicPara[9] = configIniRead->value("baseinfo/RfidPort").toString();
    demo1.BasicPara[10] = configIniRead->value("baseinfo/TackTime").toString();
    demo1.BasicPara[11] = configIniRead->value("baseinfo/netmask").toString();
    demo1.BasicPara[12] = configIniRead->value("baseinfo/gateway").toString();
    demo1.BasicPara[13] = configIniRead->value("baseinfo/SSID").toString();
    demo1.BasicPara[14] = configIniRead->value("baseinfo/psk").toString();
    demo1.BasicPara[15] = configIniRead->value("baseinfo/DataServerIp").toString();
    demo1.BasicPara[16] = configIniRead->value("baseinfo/CurveServerIp").toString();
    demo1.BasicPara[17] = configIniRead->value("baseinfo/AndonServerIp").toString();
    demo1.BasicPara[18] = configIniRead->value("baseinfo/NexoCurvePath").toString();
    demo1.BasicPara[19] = configIniRead->value("baseinfo/GCpassword").toString();
    demo1.BasicPara[20] = configIniRead->value("baseinfo/JSpassword").toString();
    demo1.BasicPara[21] = configIniRead->value("baseinfo/SN").toString();
    //demo1.BasicPara[22] = configIniRead->value("baseinfo/Version").toString();
    demo1.BasicPara[22] = Version;

    //车型
    int i;
    int j;
    int k;
    for(i=0;i<21;i++)
    {
        demo1.CarType[i][0] = configIniRead->value("carinfo"+QString::number(i+1)+"/carcx").toString();
        demo1.CarType[i][1] = configIniRead->value("carinfo"+QString::number(i+1)+"/G9").toString();
        demo1.CarType[i][2] = configIniRead->value("carinfo"+QString::number(i+1)+"/VIN").toString();
        demo1.CarType[i][3] = configIniRead->value("carinfo"+QString::number(i+1)+"/pdmyinyong").toString();
        if(configIniRead->value("carinfo"+QString::number(i+1)+"/IsOptionalStation").toString()=="")
            demo1.CarType[i][4] = "0";
        else
            demo1.CarType[i][4] = configIniRead->value("carinfo"+QString::number(i+1)+"/IsOptionalStation").toString();


        //非选配
        for(k=1;k<21;k++)
        {
            if(configIniRead->value("carinfo"+QString::number(i+1)+"/ProNum"+QString::number(k)).toString()=="")
                demo1.CarType[i][5+4*(k-1)] = "0";
            else
                demo1.CarType[i][5+4*(k-1)] = configIniRead->value("carinfo"+QString::number(i+1)+"/ProNum"+QString::number(k)).toString();
            if(configIniRead->value("carinfo"+QString::number(i+1)+"/LuoSuanNum"+QString::number(k)).toString()=="")
                demo1.CarType[i][6+4*(k-1)] = "0";
            else
                demo1.CarType[i][6+4*(k-1)] = configIniRead->value("carinfo"+QString::number(i+1)+"/LuoSuanNum"+QString::number(k)).toString();
            if(configIniRead->value("carinfo"+QString::number(i+1)+"/LSNumber"+QString::number(k)).toString()=="")
                demo1.CarType[i][7+4*(k-1)] = "0";
            else
                demo1.CarType[i][7+4*(k-1)] = configIniRead->value("carinfo"+QString::number(i+1)+"/LSNumber"+QString::number(k)).toString();
//            if(configIniRead->value("carinfo"+QString::number(i+1)+"/Taotong"+QString::number(k)).toString()=="")
//                demo1.CarType[i][8+4*(k-1)] = "0";
            if(configIniRead->value("carinfo"+QString::number(i+1)+"/Taotong"+QString::number(k)).toInt()!=1 &&
               configIniRead->value("carinfo"+QString::number(i+1)+"/Taotong"+QString::number(k)).toInt()!=2 &&
               configIniRead->value("carinfo"+QString::number(i+1)+"/Taotong"+QString::number(k)).toInt()!=3 &&
               configIniRead->value("carinfo"+QString::number(i+1)+"/Taotong"+QString::number(k)).toInt()!=4)
                demo1.CarType[i][8+4*(k-1)] = "0";
            else
                demo1.CarType[i][8+4*(k-1)] = configIniRead->value("carinfo"+QString::number(i+1)+"/Taotong"+QString::number(k)).toString();
        }
        //选配
        for(k=1;k<6;k++)
        {
            demo1.CarType[i][85+101*(k-1)] = configIniRead->value("carinfo"+QString::number(i+1)+"/pdmyinyong"+QString::number(k)).toString();

            for(j=1;j<21;j++)
            {
                if(configIniRead->value("carinfo"+QString::number(i+1)+"/OPProNum"+QString::number(k)+QString::number(j)).toString()=="")
                    demo1.CarType[i][86+101*(k-1)+3*(j-1)]="0";
                else
                    demo1.CarType[i][86+101*(k-1)+3*(j-1)] = configIniRead->value("carinfo"+QString::number(i+1)+"/OPProNum"+QString::number(k)+QString::number(j)).toString();
                if(configIniRead->value("carinfo"+QString::number(i+1)+"/OPLuoSuanNum"+QString::number(k)+QString::number(j)).toString()=="")
                    demo1.CarType[i][87+101*(k-1)+3*(j-1)]="0";
                else
                    demo1.CarType[i][87+101*(k-1)+3*(j-1)] = configIniRead->value("carinfo"+QString::number(i+1)+"/OPLuoSuanNum"+QString::number(k)+QString::number(j)).toString();
                if(configIniRead->value("carinfo"+QString::number(i+1)+"/OPLSNumber"+QString::number(k)+QString::number(j)).toString()=="")
                    demo1.CarType[i][88+101*(k-1)+3*(j-1)]="0";
                else
                    demo1.CarType[i][88+101*(k-1)+3*(j-1)] = configIniRead->value("carinfo"+QString::number(i+1)+"/OPLSNumber"+QString::number(k)+QString::number(j)).toString();
            }
            //必选
            for(j=1;j<11;j++)
            {
                demo1.CarType[i][89+101*(k-1)+57+2*(j-1)] = configIniRead->value("carinfo"+QString::number(i+1)+"/bxuanname"+QString::number(k)+QString::number(j)).toString();
                demo1.CarType[i][90+101*(k-1)+57+2*(j-1)] = configIniRead->value("carinfo"+QString::number(i+1)+"/bxuancode"+QString::number(k)+QString::number(j)).toString();
            }
            //可选
            for(j=1;j<11;j++)
            {
                demo1.CarType[i][91+101*(k-1)+57+18+2*(j-1)] = configIniRead->value("carinfo"+QString::number(i+1)+"/kxuanname"+QString::number(k)+QString::number(j)).toString();
                demo1.CarType[i][92+101*(k-1)+57+18+2*(j-1)] = configIniRead->value("carinfo"+QString::number(i+1)+"/kxuancode"+QString::number(k)+QString::number(j)).toString();
            }
        }
    }

    //PDM图
    int n = configIniRead->value("baseinfo/pdmmax").toInt();  //PDM   info数
    int c = configIniRead->value("baseinfo/pdmnumber").toInt();   //PDM数
    demo1.PDM[0] = QString::number(c);
    int coord[c]; //坐标数 数组
    int m=0;
    for(i=1; i<n+1; i++)
    {
        if (configIniRead->value("pdminfo"+QString::number(i)+"/pdmname").toString() == "")
            continue;
        coord[m]= configIniRead->value("pdminfo"+QString::number(i)+"/num").toInt();
        m++;
    }
    //qDebug()<< "m" << m;
    //qDebug()<<"coord[c]"<<coord[0]<<coord[1]<<coord[2]<<coord[3];
    int coords=0;
    QString PDMpath;

    QByteArray ba;
    //    QString PDMba;
    m = 1;
    for(i=1; i<n+1;i++)
    {
        if(configIniRead->value("pdminfo"+QString::number(i)+"/pdmname").toString()=="")
        {
            //qDebug()<<"i1"<<i;
            continue;
        }
        //qDebug()<<"i2"<<i;
        if(m!= 1)
        {
            //qDebug()<<"coords1"<<coords;
            //qDebug()<<"i"<<i;
            coords=coords+coord[m-2];
            //qDebug()<<"coords2"<<coords;
        }

        demo1.PDM[1+2*coords+3*(m-1)] = configIniRead->value("pdminfo"+QString::number(i)+"/pdmname").toString();
        PDMpath = configIniRead->value("pdminfo"+QString::number(i)+"/pdmpath").toString();

        QImage img("/PDM/"+PDMpath);
        ba.clear();
        QBuffer buf(&ba);
        img.save(&buf,"bmp");//按照JPG解码保存数据
        //QByteArray cc=qCompress(ba,1);
        QByteArray hh="";
        hh=ba.toBase64();//base64数据
        QString PDMba(hh);


        //qDebug()<<"PDM"<< PDMba.length() <<  PDMba.mid(0,200) << endl;


        //        QByteArray array = QByteArray::fromBase64(PDMba.toAscii());
        //        QBuffer buffer(&array);
        //        buffer.open( QIODevice::ReadOnly );
        //        QImageReader reader(&buffer, "jpg");
        //        QImage image = reader.read();


        demo1.PDM[2+2*coords+3*(m-1)] = PDMba;
        demo1.PDM[3+2*coords+3*(m-1)] = QString::number(coord[m-1]);

        //坐标
        for(k=1;k<(coord[m-1]+1);k++)
        {
            demo1.PDM[4+2*coords+3*(m-1)+2*(k-1)] = configIniRead->value("pdminfo"+QString::number(i)+"/tempx"+QString::number(k)).toString();
            demo1.PDM[5+2*coords+3*(m-1)+2*(k-1)] = configIniRead->value("pdminfo"+QString::number(i)+"/tempy"+QString::number(k)).toString();
        }
        m++;
    }

    delete configIniRead;
    creat_xml(&demo1);



}

void XMLCreate::creat_xml(struct Parameter *p)
{
    int x,y;
    QFile file("/filename.xml");//在默认路径下创建文件
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        //QMessageBox::information(NULL, QString("title"), QString("create error!"));
        qDebug()<<"create error!";
    }
    QDomDocument document;
    QDomText text;
    QString strHeader( "version=\"1.0\" encoding=\"gb2312\"" );//设置XML文件的头
    document.appendChild( document.createProcessingInstruction("xml", strHeader) );
    QDomElement item0 = document.createElement("ControlFormConfig");//创建首节点
    document.appendChild(item0);//生成首节点

    /*************************************基本参数******************************************************/
    QDomElement item1_1 = document.createElement("LineName");
    QDomElement item1_2 = document.createElement("StationNum");
    QDomElement item1_3 = document.createElement("StationIP");
    QDomElement item1_4 = document.createElement("ControlType");
    QDomElement item1_5 = document.createElement("ControlIP");
    QDomElement item1_6 = document.createElement("ControlPort");
    QDomElement item1_7 = document.createElement("ControlLocalPort");
    QDomElement item1_8 = document.createElement("ScanModel");
    QDomElement item1_9 = document.createElement("RFIDIP");
    QDomElement item1_10 = document.createElement("RFIDPort");
    QDomElement item1_11 = document.createElement("CycleTime");
    QDomElement item1_12 = document.createElement("SubnetMask");
    QDomElement item1_13 = document.createElement("DefaultGateway");
    QDomElement item1_14 = document.createElement("WifiName");
    QDomElement item1_15 = document.createElement("WifiPassword");
    QDomElement item1_16 = document.createElement("DataServerIP");
    QDomElement item1_17 = document.createElement("CurveServerIP");
    QDomElement item1_18 = document.createElement("AndonServerIP");
    QDomElement item1_19 = document.createElement("NexoCurvePath");
    QDomElement item1_20 = document.createElement("AdminPassword");
    QDomElement item1_21 = document.createElement("TechPassword");
    QDomElement item1_22 = document.createElement("SN");
    QDomElement item1_23 = document.createElement("Version");
    item0.appendChild(item1_1);
    item0.appendChild(item1_2);
    item0.appendChild(item1_3);
    item0.appendChild(item1_4);
    item0.appendChild(item1_5);
    item0.appendChild(item1_6);
    item0.appendChild(item1_7);
    item0.appendChild(item1_8);
    item0.appendChild(item1_9);
    item0.appendChild(item1_10);
    item0.appendChild(item1_11);
    item0.appendChild(item1_12);
    item0.appendChild(item1_13);
    item0.appendChild(item1_14);
    item0.appendChild(item1_15);
    item0.appendChild(item1_16);
    item0.appendChild(item1_17);
    item0.appendChild(item1_18);
    item0.appendChild(item1_19);
    item0.appendChild(item1_20);
    item0.appendChild(item1_21);
    item0.appendChild(item1_22);
    item0.appendChild(item1_23);
    //写入基本参数
    text = document.createTextNode(p->BasicPara[0]);
    item1_1.appendChild(text);
    text = document.createTextNode(p->BasicPara[1]);
    item1_2.appendChild(text);
    text = document.createTextNode(p->BasicPara[2]);
    item1_3.appendChild(text);
    text = document.createTextNode(p->BasicPara[3]);
    item1_4.appendChild(text);
    text = document.createTextNode(p->BasicPara[4]);
    item1_5.appendChild(text);
    text = document.createTextNode(p->BasicPara[5]);
    item1_6.appendChild(text);
    text = document.createTextNode(p->BasicPara[6]);
    item1_7.appendChild(text);
    text = document.createTextNode(p->BasicPara[7]);
    item1_8.appendChild(text);
    text = document.createTextNode(p->BasicPara[8]);
    item1_9.appendChild(text);
    text = document.createTextNode(p->BasicPara[9]);
    item1_10.appendChild(text);
    text = document.createTextNode(p->BasicPara[10]);
    item1_11.appendChild(text);
    text = document.createTextNode(p->BasicPara[11]);
    item1_12.appendChild(text);
    text = document.createTextNode(p->BasicPara[12]);
    item1_13.appendChild(text);
    text = document.createTextNode(p->BasicPara[13]);
    item1_14.appendChild(text);
    text = document.createTextNode(p->BasicPara[14]);
    item1_15.appendChild(text);
    text = document.createTextNode(p->BasicPara[15]);
    item1_16.appendChild(text);
    text = document.createTextNode(p->BasicPara[16]);
    item1_17.appendChild(text);
    text = document.createTextNode(p->BasicPara[17]);
    item1_18.appendChild(text);
    text = document.createTextNode(p->BasicPara[18]);
    item1_19.appendChild(text);
    text = document.createTextNode(p->BasicPara[19]);
    item1_20.appendChild(text);
    text = document.createTextNode(p->BasicPara[20]);
    item1_21.appendChild(text);
    text = document.createTextNode(p->BasicPara[21]);
    item1_22.appendChild(text);
    text = document.createTextNode(p->BasicPara[22]);
    item1_23.appendChild(text);

    /*************************************车型列表******************************************************/
    QDomElement item2 = document.createElement("CarTypeInfoList");
    item0.appendChild(item2);
    int i;
    for(i=0;i<20;i++)//循环生成车型信息
    {
        create_car(p,document,item2,i);//调用函数生成车型信息
    }

    /*************************************PDM列表******************************************************/
    int PDM_NUM=0;
    int PDM_NUM0=0;
    QDomElement item3 = document.createElement("PDMList");
    item0.appendChild(item3);
    for(x=0;x<(p->PDM[0].toInt());x++)//按PDM数量循环生成PDM信息
    {
        QDomElement item3_1 = document.createElement("PDMItem");
        item3.appendChild(item3_1);
        QDomElement item3_1_1 = document.createElement("Name");
        QDomElement item3_1_2 = document.createElement("Images");
        QDomElement item3_1_3 = document.createElement("Items");
        item3_1.appendChild(item3_1_1);
        item3_1.appendChild(item3_1_2);
        item3_1.appendChild(item3_1_3);
        PDM_NUM++;
        text = document.createTextNode(p->PDM[PDM_NUM]);
        item3_1_1.appendChild(text);
        PDM_NUM++;
        text = document.createTextNode(p->PDM[PDM_NUM]);
        item3_1_2.appendChild(text);
        PDM_NUM++;
        PDM_NUM0=PDM_NUM;
        for(y=0 ;y<(p->PDM[PDM_NUM0].toInt());y++)//按PDM点数循环生成坐标信息
        {
            QDomElement item3_1_3_1 = document.createElement("Point");
            item3_1_3.appendChild(item3_1_3_1);
            QDomElement item3_1_3_1_1 = document.createElement("X");
            QDomElement item3_1_3_1_2 = document.createElement("Y");
            item3_1_3_1.appendChild(item3_1_3_1_1);
            item3_1_3_1.appendChild(item3_1_3_1_2);
            PDM_NUM++;
            text = document.createTextNode(p->PDM[PDM_NUM]);//坐标X
            item3_1_3_1_1.appendChild(text);
            PDM_NUM++;
            text = document.createTextNode(p->PDM[PDM_NUM]);//坐标Y
            item3_1_3_1_2.appendChild(text);
        }
    }
    QTextStream out( &file );
    document.save(out,4);
    file.close();
}

void XMLCreate::create_car(struct Parameter *demo,QDomDocument document,QDomElement item2,int n)
{
    int a,b,c,d,e;
    QDomText text;

    QDomElement item2_1 = document.createElement("CarTypeInfo");
    item2.appendChild(item2_1);
    //生成基本车型信息
    QDomElement item2_1_1 = document.createElement("CarName");
    QDomElement item2_1_2 = document.createElement("G9");
    QDomElement item2_1_3 = document.createElement("VIN");
    QDomElement item2_1_4 = document.createElement("PDM");
    QDomElement item2_1_5 = document.createElement("IsOptionalStation");
    //写入基本车型信息内容
    text = document.createTextNode(demo->CarType[n][0]);
    item2_1_1.appendChild(text);
    text = document.createTextNode(demo->CarType[n][1]);
    item2_1_2.appendChild(text);
    text = document.createTextNode(demo->CarType[n][2]);
    item2_1_3.appendChild(text);
    text = document.createTextNode(demo->CarType[n][3]);
    item2_1_4.appendChild(text);
    text = document.createTextNode(demo->CarType[n][4]);
    item2_1_5.appendChild(text);

    QDomElement item2_1_6 = document.createElement("ProgramList");
    QDomElement item2_1_7 = document.createElement("OptionProList");
    item2_1.appendChild(item2_1_1);
    item2_1.appendChild(item2_1_2);
    item2_1.appendChild(item2_1_3);
    item2_1.appendChild(item2_1_4);
    item2_1.appendChild(item2_1_5);
    item2_1.appendChild(item2_1_6);
    item2_1.appendChild(item2_1_7);
    for(a=0;a<=19;a++)//未选配创建20个程序
    {
        QDomElement item2_1_6_1 = document.createElement("ProgramNum");
        item2_1_6.appendChild(item2_1_6_1);

        QDomElement item2_1_6_1_1 = document.createElement("PID");
        QDomElement item2_1_6_1_2 = document.createElement("BoltNumber");
        QDomElement item2_1_6_1_3 = document.createElement("BoltCount");
        QDomElement item2_1_6_1_4 = document.createElement("Sleeve");
        item2_1_6_1.appendChild(item2_1_6_1_1);
        item2_1_6_1.appendChild(item2_1_6_1_2);
        item2_1_6_1.appendChild(item2_1_6_1_3);
        item2_1_6_1.appendChild(item2_1_6_1_4);
        //写入程序信息
        text = document.createTextNode(demo->CarType[n][5+4*a]);
        item2_1_6_1_1.appendChild(text);
        text = document.createTextNode(demo->CarType[n][6+4*a]);
        item2_1_6_1_2.appendChild(text);
        text = document.createTextNode(demo->CarType[n][7+4*a]);
        item2_1_6_1_3.appendChild(text);
        text = document.createTextNode(demo->CarType[n][8+4*a]);
        item2_1_6_1_4.appendChild(text);
    }
    for(b=0;b<=4;b++)//选配创建5个程序槽
    {
        QDomElement item2_1_7_1 = document.createElement("OptionalInfo");
        item2_1_7.appendChild(item2_1_7_1);

        QDomElement item2_1_7_1_1 = document.createElement("PDM");
        QDomElement item2_1_7_1_2 = document.createElement("ProgramList");
        QDomElement item2_1_7_1_3 = document.createElement("MustList");
        QDomElement item2_1_7_1_4 = document.createElement("OptionList");
        item2_1_7_1.appendChild(item2_1_7_1_1);
        item2_1_7_1.appendChild(item2_1_7_1_2);
        item2_1_7_1.appendChild(item2_1_7_1_3);
        item2_1_7_1.appendChild(item2_1_7_1_4);

        text = document.createTextNode(demo->CarType[n][85+101*b]);//选配PDM
        item2_1_7_1_1.appendChild(text);

        for(c=0;c<=19;c++)//每个槽20个程序
        {
            QDomElement item2_1_7_1_2_1 = document.createElement("ProgramNum");
            item2_1_7_1_2.appendChild(item2_1_7_1_2_1);

            QDomElement item2_1_7_1_2_1_1 = document.createElement("PID");
            QDomElement item2_1_7_1_2_1_2 = document.createElement("BoltNumber");
            QDomElement item2_1_7_1_2_1_3 = document.createElement("BoltCount");

            item2_1_7_1_2_1.appendChild(item2_1_7_1_2_1_1);
            item2_1_7_1_2_1.appendChild(item2_1_7_1_2_1_2);
            item2_1_7_1_2_1.appendChild(item2_1_7_1_2_1_3);
            //写入程序信息内容
            text = document.createTextNode(demo->CarType[n][86+3*c+101*b]);
            item2_1_7_1_2_1_1.appendChild(text);
            text = document.createTextNode(demo->CarType[n][87+3*c+101*b]);
            item2_1_7_1_2_1_2.appendChild(text);
            text = document.createTextNode(demo->CarType[n][88+3*c+101*b]);
            item2_1_7_1_2_1_3.appendChild(text);
        }
        for(d=0;d<=9;d++)//10个必选信息
        {
            QDomElement item2_1_7_1_3_1 = document.createElement("OptionalCell");
            item2_1_7_1_3.appendChild(item2_1_7_1_3_1);

            QDomElement item2_1_7_1_3_1_1 = document.createElement("Key");
            QDomElement item2_1_7_1_3_1_2 = document.createElement("Value");
            item2_1_7_1_3_1.appendChild(item2_1_7_1_3_1_1);
            item2_1_7_1_3_1.appendChild(item2_1_7_1_3_1_2);
            //写入必选内容
            text = document.createTextNode(demo->CarType[n][146+2*d+101*b]);
            item2_1_7_1_3_1_1.appendChild(text);
            text = document.createTextNode(demo->CarType[n][147+2*d+101*b]);
            item2_1_7_1_3_1_2.appendChild(text);
        }
        for(e=0;e<=9;e++)//10个可选信息
        {
            QDomElement item2_1_7_1_4_1 = document.createElement("OptionalCell");
            item2_1_7_1_4.appendChild(item2_1_7_1_4_1);

            QDomElement item2_1_7_1_4_1_1 = document.createElement("Key");
            QDomElement item2_1_7_1_4_1_2 = document.createElement("Value");
            item2_1_7_1_4_1.appendChild(item2_1_7_1_4_1_1);
            item2_1_7_1_4_1.appendChild(item2_1_7_1_4_1_2);
            //写入可选内容
            text = document.createTextNode(demo->CarType[n][166+2*e+101*b]);
            item2_1_7_1_4_1_1.appendChild(text);
            text = document.createTextNode(demo->CarType[n][167+2*e+101*b]);
            item2_1_7_1_4_1_2.appendChild(text);
        }
    }
}
