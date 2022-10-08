#include "paintarea.h"

PaintArea::PaintArea(QWidget *parent) :
    QWidget(parent)
{
//    condition = "";
    max=0.0;
    min=0.0;
    ave=0.0;
    _ma=0.0;//数组里的最大值
    QPalette p = palette();
    p.setColor(QPalette::Window,QColor(51, 153, 255, 255));
    setPalette(p);
    setAutoFillBackground(true);

    setMinimumSize(1100,570);
}

void PaintArea::setQuery(QSqlQuery query1,QSqlQuery query2,QSqlQuery query3)
{
//    condition = tmp;
    max = 0.0;
    min = 0.0;
    ave=0.0;
    _ma=0.0;
    query_number = query1;
    query_datas = query2;
    query_bound = query3;
    int i = 0;
    while(query_datas.next())
    {
        datas[i] = query_datas.value(0).toDouble();
        if(datas[i] > _ma)
            _ma= datas[i];
        i++;
    }
    while(query_bound.next())
    {
        max = query_bound.value(0).toDouble();
        min = query_bound.value(1).toDouble();
    }
//    qDebug()<<"*************"<<max<<min;
    if(max >_ma)
        _ma = max;
    update();
}

void PaintArea::paintEvent(QPaintEvent *)
{
//    qDebug()<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&";
    int n = query_number.value(0).toInt();//n为数据个数
    //        ui->label_num->setText(QString::number(n));

    //        double _mi=inf;

    double a[n];//数据储存在数组a中，大小为n
    //            query_datas.exec("SELECT Torque FROM TighteningDatas where "+condition);

    int i;
    for(i=0;i<n;i++)
        a[i]=datas[i];

    QPainter painter(this);


    int pointx=65,pointy=540;//确定坐标轴起点坐标，这里定义(40,500)
    int width=1030-pointx,height=450;//确定坐标轴宽度跟高度 上文定义画布为600X300，宽高依此而定。

    //绘制坐标轴 坐标轴原点(35，280)
    QPen pen(QColor(51, 153, 255, 255),1,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    //        pen.setColor(QColor(QRgb(51, 153, 255)));
    //        pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(14,52,1061,515);//外围的矩形，从(5,5)起，到(590,290)结束，周围留了5的间隙。

    if(n!= 0)
    {
        painter.drawLine(pointx,pointy,width+pointx+30,pointy);//坐标轴x宽度为width
        painter.drawLine(pointx,pointy-height-30,pointx,pointy);//坐标轴y高度为height

        painter.setRenderHint(QPainter::Antialiasing, true);//设置反锯齿模式，好看一点
        painter.drawLine(width+pointx+30,pointy,width+pointx+30-15,pointy-8);
        painter.drawLine(width+pointx+30,pointy,width+pointx+30-15,pointy+8);

        painter.drawLine(pointx,pointy-height-30,pointx-8,pointy-height-30+15);
        painter.drawLine(pointx,pointy-height-30,pointx+8,pointy-height-30+15);

        double kx=(double)width/(n-1); //x轴的系数
        double ky=(double)height/_ma;//y方向的比例系数
        QPen pen,penPoint;
        pen.setColor(Qt::black);
        pen.setWidth(1);

        penPoint.setColor(QColor(51, 153, 255, 255));
        penPoint.setWidth(3);
        for(int i=0;i<n-1;i++)
        {
            //由于y轴是倒着的，所以y轴坐标要pointy-a[i]*ky 其中ky为比例系数
            painter.setPen(pen);//黑色笔用于连线
            painter.drawLine(pointx+kx*i,pointy-a[i]*ky,pointx+kx*(i+1),pointy-a[i+1]*ky);
            painter.setPen(penPoint);//蓝色的笔，用于标记各个点
            painter.drawPoint(pointx+kx*i,pointy-a[i]*ky);
        }
        painter.drawPoint(pointx+kx*(n-1),pointy-a[n-1]*ky);//绘制最后一个点

        //绘制刻度线
        QPen penDegree;
        penDegree.setColor(Qt::black);
        penDegree.setWidth(2);
        painter.setPen(penDegree);

        painter.drawText(pointx-45,pointy-height-20,QString("Nm"));   //Nm
        //画上x轴刻度线
        for(int i=0;i<10;i++)//分成10份
        {
            //选取合适的坐标，绘制一段长度为4的直线，用于表示刻度
            painter.drawLine(pointx+(i+1)*width/10,pointy,pointx+(i+1)*width/10,pointy+4);
            painter.drawText(pointx+(i+0.88)*width/10,
                             pointy+18,QString::number((int)((i+1)*((double)n/10))));
        }
        //y轴刻度线
        double _maStep=(double)_ma/10;//y轴刻度间隔需根据最大值来表示
        for(int i=0;i<10;i++)
        {
            //代码较长，但是掌握基本原理即可。
            //主要就是确定一个位置，然后画一条短短的直线表示刻度。
            painter.drawLine(pointx,pointy-(i+1)*height/10,
                             pointx-4,pointy-(i+1)*height/10);
            painter.drawText(pointx-45,pointy-(i+0.87)*height/10,
                             QString::number((double)(_maStep*(i+1)),'f',1));
        }
//        max = 0.0165;
//        min = 0.0135;
        ave = (max+min)/2;

        //绘制上下限
        QPen penAve;
        penAve.setColor(Qt::red);//选择红色
        penAve.setWidth(2);
        penAve.setStyle(Qt::DotLine);//线条类型为虚线
        painter.setPen(penAve);
        painter.drawLine(pointx,pointy-max*ky,pointx+width,pointy-max*ky);
        painter.drawLine(pointx,pointy-min*ky,pointx+width,pointy-min*ky);
        painter.drawLine(pointx+500,pointy-height-20,pointx+540,pointy-height-20);
        painter.drawText(pointx+540+5,pointy-height-20+0.13*height/10,
                         "上/下限");  //(±10%)

        penAve.setColor(QColor(241, 156, 31, 255));//选择橙色
        penAve.setStyle(Qt::DashDotDotLine);//线条类型为虚线
        painter.setPen(penAve);
        painter.drawLine(pointx,pointy-ave*1.075*ky,pointx+width,pointy-ave*1.075*ky);
        painter.drawLine(pointx,pointy-ave*0.925*ky,pointx+width,pointy-ave*0.925*ky);
        painter.drawLine(pointx+642,pointy-height-20,pointx+682,pointy-height-20);
        painter.drawText(pointx+682+5,pointy-height-20+0.13*height/10,
                         "上/下预警限");   //(±7.5%)
        //绘制平均线
        penAve.setColor(QColor(69, 192, 26, 255));//选择绿色
        penAve.setStyle(Qt::DashDotLine);//线条类型为虚线
        painter.setPen(penAve);
        painter.drawLine(pointx,pointy-ave*ky,pointx+width,pointy-ave*ky);
        painter.drawText(pointx+width+5,pointy-ave*ky+0.13*height/10,
                         QString::number(ave,'f',1));
        painter.drawLine(pointx+820,pointy-height-20,pointx+860,pointy-height-20);
        painter.drawText(pointx+860+5,pointy-height-20+0.13*height/10,
                         "工艺目标值");

    }

//    //绘制最大值和最小值
//    QPen penMaxMin;
//    penMaxMin.setColor(Qt::darkGreen);//暗绿色
//    painter.setPen(penMaxMin);
//    painter.drawText(pointx+kx*maxpos-kx,pointy-a[maxpos]*ky-5,
//                     "最大值"+QString::number(_ma));
//    painter.drawText(pointx+kx*minpos-kx,pointy-a[minpos]*ky+15,
//                     "最小值"+QString::number(_mi));

//    penMaxMin.setColor(Qt::red);
//    penMaxMin.setWidth(7);
//    painter.setPen(penMaxMin);
//    painter.drawPoint(pointx+kx*maxpos,pointy-a[maxpos]*ky);//标记最大值点
//    painter.drawPoint(pointx+kx*minpos,pointy-a[minpos]*ky);//标记最小值点

}

