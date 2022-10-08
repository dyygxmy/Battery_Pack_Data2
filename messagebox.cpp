#include "messagebox.h"

DTMessageBox::DTMessageBox(QDialog *parent) :
    QDialog(parent)
{
    this->setFixedSize(350,350);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
#if 0
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
#endif

    mainLayout   = new QGridLayout(this) ;
    layoutPHEVEN = new QGridLayout(this) ;
    layoutIO     = new QGridLayout(this) ;

    stackLayout  = new QStackedLayout(this) ;

    WidgetPHEVEN = new QWidget(this);
    btnSave   = new QPushButton("保存",this) ;
    btnCancel = new QPushButton(this) ;
    editList  = new QLineEdit(this) ;
    editYEN   = new QLineEdit(this) ;
    editYTol  = new QLineEdit(this) ;
    editAEN   = new QLineEdit(this) ;
    editATol  = new QLineEdit(this) ;
    editOEN   = new QLineEdit(this) ;
    editOTol  = new QLineEdit(this) ;

    initWidgetIO();

    btnSave->setFixedSize(51,41);
    btnCancel->setFixedSize(38,38);
    editList->setFixedHeight(40);

    editYEN  ->setPlaceholderText("角度");
    editYTol ->setPlaceholderText("公差");
    editAEN  ->setPlaceholderText("Y轴");
    editATol ->setPlaceholderText("公差");
    editOEN  ->setEnabled(false);
    editOEN  ->setText("NULL");
    editOTol ->setEnabled(false);
    editOTol ->setText("NULL");
    editList->setPlaceholderText("请输入项目名称");

    btnCancel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);

    //格栅布局
    layoutPHEVEN->addWidget(editYEN,0,0,1,1);
    layoutPHEVEN->addWidget(editYTol,0,1,1,1);
    layoutPHEVEN->addWidget(editAEN,1,0,1,1);
    layoutPHEVEN->addWidget(editATol,1,1,1,1);
    layoutPHEVEN->addWidget(editOEN,2,0,1,1);
    layoutPHEVEN->addWidget(editOTol,2,1,1,1);

    WidgetPHEVEN->setLayout(layoutPHEVEN);

    stackLayout->addWidget(WidgetPHEVEN);
    stackLayout->addWidget(editList);
    stackLayout->addWidget(widgetIO);

    mainLayout->addWidget(btnCancel,0,8,1,1);
    mainLayout->addLayout(stackLayout,1,0,3,8);
    mainLayout->addWidget(btnSave,5,7,1,2);

    setLayout(mainLayout);

    connect(btnSave, SIGNAL(clicked()),
            this, SLOT(slotBtnSaveIsClick()));
    connect(btnCancel,SIGNAL(clicked()),this,SLOT(slotBtnCancelIsClick()));

    initStyleSheet();

}

void DTMessageBox::initStyleSheet()
{
    this->setStyleSheet("QWidget{background-color:rgb(248, 248, 255);}"
                        "QGroupBox {font: 14pt \"黑体\";margin-top: 1ex;"
                                   "border-width:0.5px;border-style:solid;"
                                   "border-color: rgb(51, 153, 255);}"
                         "QGroupBox::title {subcontrol-origin: margin;"
                                   "subcontrol-position: top left;left:15px;"
                                   "margin-left: 5px;margin-right: 5px;padding:1px;}");

    editList->setStyleSheet("border:1px solid  rgb(51,153,255);");

    btnSave ->setStyleSheet("background-color:rgb(51,153,255);"
                             "border-image: url(:/LCD_CS351/LCD_CS351/35_all/67.bmp);"
                             "font: 14pt \"黑体\";color: rgb(248, 248, 255);");

    btnCancel->setStyleSheet("background-color:rgb(245,245,245);"
                             "border-image: url(:/LCD_CS351/LCD_CS351/35_all/cha.bmp);");
}

void DTMessageBox::initWidgetIO()
{
    //别挣扎了，groupBox说认它做爹也不行，除非写绝对位置。
    //为了方便后期维护，我选择加个布局
#if 0
    QHBoxLayout *oldPosLayout = new QHBoxLayout(this);
    QHBoxLayout *newPosLayout = new QHBoxLayout(this);
    widgetIO    = new QWidget(this);
    groupBoxOld = new QGroupBox("history",this) ;
    groupBoxNew = new QGroupBox("new",this) ;
    oldPosIO    = new DTColorLumpWidget(this) ;
    newPosIO    = new DTColorLumpWidget(this) ;

    oldPosLayout->addWidget(oldPosIO);
    newPosLayout->addWidget(newPosIO);

    groupBoxOld->setLayout(oldPosLayout);
    groupBoxNew->setLayout(newPosLayout);

    layoutIO->addWidget(groupBoxOld);
    layoutIO->addWidget(groupBoxNew);
    widgetIO->setLayout(layoutIO);
#else
    QGridLayout *layoutPosIO = new QGridLayout(this);
    editPosIO = new QLineEdit(this) ;
    labelPosIO= new QLabel("位置号",this);
    widgetIO  = new QWidget(this);

    layoutPosIO->addWidget(labelPosIO,0,0,1,2);
    layoutPosIO->addWidget(editPosIO,0,1,1,5);

    widgetIO->setLayout(layoutPosIO);
#endif

}

//接收codeedit页面传过来的参数
void DTMessageBox::setParameter(QVariantMap map)
{
    stackLayout->setCurrentIndex(0);
    editType  = map.value("type").toString();
    indexRow  = map.value("row").toInt();
    indexItem = map.value("item").toString();

    if(indexItem.contains("IO"))
    {
        stackLayout->setCurrentIndex(2);
#if 0
        newPosIO->setColor("0010011011110011");
#else
        editPosIO->setText(map.value("posIO").toString());
#endif
    }

    if(editType == "listWidget")
    {
        stackLayout->setCurrentIndex(1);
    }
    else if(editType == "edit")
    {
        editYEN ->setText(map.value("YEN").toString());
        editYTol->setText(map.value("YTol").toString());
        editAEN ->setText(map.value("AEN").toString());
        editATol->setText(map.value("ATol").toString());
//        editYEN ->setText(QString::number(GAENCODERRemp));
//        editAEN ->setText(QString::number(GYENCODERTemp));
//        qDebug()<<"editAen"<<editAEN->text()<<editYEN->text()<<GAENCODERRemp<<GYENCODERTemp;
    }
    else {
        editAEN ->clear();
        editATol->clear();
        editYEN ->clear();
        editYTol->clear();
//        editYEN ->setText(QString::number(GAENCODERRemp));
//        editAEN ->setText(QString::number(GYENCODERTemp));
//        qDebug()<<"editAen"<<editAEN->text()<<editYEN->text()<<GAENCODERRemp<<GYENCODERTemp;
    }
}

void DTMessageBox::slotBtnSaveIsClick()
{
    QVariantMap data;
    data.insert("row" ,QString("%1").arg(indexRow));
    data.insert("YEN" ,editYEN->text());
    data.insert("YTol",editYTol->text());
    data.insert("AEN" ,editAEN->text());
    data.insert("ATol",editATol->text());
    data.insert("OEN" ,editOEN->text());
    data.insert("OTol",editOTol->text());
    data.insert("type",editType);
    data.insert("item",editList->text());  //添加listwidgetItem用的
    data.insert("posIO",editPosIO->text()); //IO类用的
    Q_EMIT signalUpdateDate(data);

}

//关闭
void DTMessageBox::slotBtnCancelIsClick()
{
    Q_EMIT signalCancel();
}

void DTMessageBox::slotrecEncoderFromTight(qint64 y, qint64 a,QByteArray c)
{
    editYEN ->setText(QString::number(a));
    editAEN ->setText(QString::number(y));
    editPosIO->setText(c);
    qDebug()<<"this is messgaebox rec"<<y<<a<<c;
}

/*****************************************************
 * 20180507
 * Claire
 *
******************************************************/
DTColorLumpWidget::DTColorLumpWidget(QWidget *parent) :
    QWidget(parent)
{
    layout = new QGridLayout(this);

    //第一行
    for(int i=0;i<8;i++)
    {
        label[i] = new QLabel(this);
        layout->addWidget(label[i],0,i,1,1);
    }

    //第二行
    for(int j=8;j<16;j++)
    {
        label[j] = new QLabel(this);
        layout->addWidget(label[j],1,j-8,1,1);
    }
}

//
void DTColorLumpWidget::setColor(QString str)
{
    int len = str.length();

    //如果不加判断，一旦收到的字串长度大于16，会导致label[i]的数组溢出
    if(len>16){
        len = 16 ;}

    //根据字串中字符0/1显示颜色，如果包含0和1以外的字符，报错
    for(int i=0; i<len;i++)
    {
        QString color = str.at(i);
        if(color == "0")
        {
            label[i]->setStyleSheet("QLabel{background-color:rgb(224,224,224)}");
        }
        else if(color == "1")
        {
            label[i]->setStyleSheet("QLabel{background-color:rgb(51,153,255)}");
        }
        else {
            qDebug()<<"字符串有误";
        }
    }
}
