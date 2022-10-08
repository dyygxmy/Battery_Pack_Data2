#include "stepparameter.h"
#include "ui_stepparameter.h"

StepParameter::StepParameter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StepParameter)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    ui->groupBox->hide();
    ui->label_warning->hide();
    initPDM();
}

StepParameter::~StepParameter()
{
    delete ui;
}

void StepParameter::initUI(QVariantMap stepMap)
{
    initScanList();
    ui->lineEdit_regexp->hide();
    int stepType =stepMap["type"].toInt();
    if(stepType == 0)
    {
        ui->radioButton_manual->setChecked(true);
        on_radioButton_manual_clicked();
    }
    else if(stepType == 1)
    {
        ui->radioButton_request->setChecked(true);
        on_radioButton_request_clicked();
    }
    else if(stepType == 2)
    {
        ui->radioButton_completed->setChecked(true);
        on_radioButton_completed_clicked();
    }
    QString singleStep = stepMap["singleStep"].toString();

    ui->comboBox->setCurrentIndex(ui->comboBox->findText(singleStep));

    ui->lineEdit_readAdd->setText(stepMap["readAdd"].toString());
    ui->lineEdit_writeAdd->setText(stepMap["writeAdd"].toString());

    ui->lineEdit_plugTimes->setText(stepMap["plugTimes"].toString());
    ui->lineEdit_tightenTimes->setText(stepMap["tightenTimes"].toString());
    ui->lineEdit_boxNums->setText(stepMap["boxNums"].toString());
    ui->lineEdit_moduleNums->setText(stepMap["moduleNums"].toString());
    ui->lineEdit_w500IP->setText(stepMap["w500IP"].toString());

    ui->lineEdit_scanTimes->setText(stepMap["scanTimes"].toString());

//    ui->lineEdit_regexp->setText(stepMap["regexp"].toString());
    ui->comboBox_ScanRegwxp->setCurrentIndex(ui->comboBox_ScanRegwxp->findText(stepMap["regexp"].toString()));

    ui->lineEdit_regexp_2->setText(stepMap["regexp2"].toString());
    ui->lineEdit_dateDiff->setText(stepMap["datediff"].toString());
    ui->lineEdit_dateDiff2->setText(stepMap["datediff2"].toString());
    ui->lineEdit_VictionRegexp->setText(stepMap["VRegexp"].toString());

    ui->lineEdit_regexpCode->setText(stepMap["regexpCode"].toString());

    ui->lineEdit_regexp_VM1->setText(stepMap["regexpVM1"].toString());
    ui->lineEdit_regexp_VM2->setText(stepMap["regexpVM2"].toString());

    ui->lineEdit_lingjian->setText(stepMap["lingjian"].toString());
    ui->lineEdit_VariantDiff->setText(stepMap["VariantDiff"].toString());
    ui->lineEdit_dataDiff->setText(stepMap["dataDiff"].toString());
    ui->lineEdit_dataDiff_2->setText(stepMap["dataDiff1"].toString());
    ui->lineEdit_scanPass->setText(stepMap["scanPass"].toString());

    ui->checkBox_isTrue->setChecked(stepMap["isTrue"].toBool());

    ui->lineEdit_PDMShowTime->setText(stepMap["PDMShowTime"].toString());

    ui->lineEdit_PDMPass->setText(stepMap["PDMPass"].toString());

    ui->lineEdit_finishPass->setText(stepMap["finishPass"].toString());

    ui->lineEdit_JumpStep->setText(stepMap["jumpStep"].toString());
    ui->lineEdit_StationTrue->setText(stepMap["stationTrue"].toString());

    ui->lineEdit_Signature->setText(stepMap["Signature"].toString());//特征码
    ui->lineEdit_PackKeyCode->setText(stepMap["TightenPass"].toString());//拧紧型号

    ui->checkBox_Tightening_isFalse->setChecked(stepMap["isOFF"].toBool());

    ui->comboBox_IO->setCurrentIndex(ui->comboBox_IO->findText(stepMap["IO"].toString()));

    ui->comboBox_packTemp->setCurrentIndex(ui->comboBox_packTemp->findText(stepMap["whichPack"].toString()));

    ui->comboBox_PDM->setCurrentIndex(ui->comboBox_PDM->findText(stepMap["PDM"].toString()));

    ui->comboBox_PDM_2->setCurrentIndex(ui->comboBox_PDM_2->findText(stepMap["PDM"].toString()));

    ui->comboBox_PDM_name->setCurrentIndex(ui->comboBox_PDM_name->findText(stepMap["PDM"].toString()));
    ui->comboBox_PDM_name_holdOn->setCurrentIndex(ui->comboBox_PDM_name_holdOn->findText(stepMap["PDM"].toString()));
    ui->lineEdit_holdOn->setText(stepMap["holdOn"].toString());

    ui->comboBox_condition1->setCurrentIndex(ui->comboBox_condition1->findText(stepMap["condition1"].toString()));
    ui->comboBox_operator->setCurrentIndex(ui->comboBox_operator->findText(stepMap["operator"].toString()));
    ui->comboBox_condition2->setCurrentIndex(ui->comboBox_condition2->findText(stepMap["condition2"].toString()));
    ui->lineEdit_jump->setText(stepMap["jump"].toString());
    ui->lineEdit_bolt->setText(stepMap["boltSN"].toString());
    ui->lineEdit_program->setText(stepMap["program"].toString());
}


//comboBox控件的信号槽
void StepParameter::on_comboBox_currentIndexChanged(const QString &arg1)
{
    qDebug()<<"comboBox_currentIndexChanged"<<arg1;
    if(arg1 == "条码枪扫码")
    {
        ui->groupBox->show();//在下方显示QGroupBox控件(参数配置)
        ui->stackedWidget->setCurrentIndex(0);//QStackedWidget控件
    }
    else if(arg1 == "备用绑定流水号")
    {

    }
    else if(arg1 == "返修不合格跳过")
    {
        ui->groupBox->show();//在下方显示QGroupBox控件(参数配置)
        ui->stackedWidget->setCurrentIndex(9);//QStackedWidget控件
    }
    else if(arg1 == "挂起")
    {
        ui->groupBox->show();
        ui->stackedWidget->setCurrentIndex(10);
    }
    else if(arg1 == "等待I/O输入")
    {
        ui->groupBox->show();
        ui->comboBox_IO->clear();
        QStringList inputList;
        inputList<<tr("Input1")<<tr("Input2")<<tr("Input3")<<tr("Input4")<<tr("Input5")<<tr("Input6")<<tr("Input7")<<tr("Input8")<<tr("Input9")<<tr("Input10")<<tr("Input11")<<tr("Input12")<<tr("Input13")<<tr("Input14")<<tr("Input15")<<tr("Input16");
        ui->comboBox_IO->addItems(inputList);
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if(arg1 == "手工站站完成")
    {
        ui->groupBox->show();
        ui->stackedWidget->setCurrentIndex(8);
    }
    else if(arg1 == "I/O输出")
    {
        ui->groupBox->show();
        ui->comboBox_IO->clear();
        QStringList outputList;
        outputList<<tr("Output1")<<tr("Output2")<<tr("Output3")<<tr("Output4")<<tr("Output5")<<tr("Output6")<<tr("Output7");
        ui->comboBox_IO->addItems(outputList);
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if(arg1 == "显示PDM")
    {
        ui->groupBox->show();
        ui->stackedWidget->setCurrentIndex(2);
    }
    else if(arg1 == "辊道线数据绑定")
    {
        ui->groupBox->show();
        ui->comboBox_packTemp->clear();
        QStringList packTempList;
        packTempList<<tr("pack1")<<tr("pack2");
        ui->comboBox_packTemp->addItems(packTempList);
        ui->stackedWidget->setCurrentIndex(7);
    }
    else if(ui->radioButton_request->isChecked() || ui->radioButton_completed->isChecked())
    {
        ui->groupBox->show();
        ui->stackedWidget->setCurrentIndex(3);
        if(arg1 == "请求堵头安装位置号")
        {
            ui->stackedWidget_2->setCurrentIndex(1);
        }
        else if(arg1 == "请求机器人拧紧位置号")
        {
            ui->stackedWidget_2->setCurrentIndex(2);
        }
        else if(arg1 == "请求下箱体位置号")
        {
            ui->stackedWidget_2->setCurrentIndex(3);
        }
        else if(arg1 == "请求模组入箱位置号")
        {
            ui->stackedWidget_2->setCurrentIndex(4);
        }
        else if(arg1 == "请求记录堵头安装数据")
        {
            ui->stackedWidget_2->setCurrentIndex(5);
        }
        else if(arg1 == "请求下箱体条码扫描")
        {
            ui->stackedWidget_2->setCurrentIndex(6);
        }
        else
            ui->stackedWidget_2->setCurrentIndex(0);
    }
    else if(arg1 == "拧紧")
    {
        ui->groupBox->show();
        ui->stackedWidget->setCurrentIndex(4);
    }
    else if(arg1 == "跳转")
    {
        ui->groupBox->show();
        ui->comboBox_condition1->clear();
        QStringList condition1;
        condition1<<tr("当前PACK_SN");
        ui->comboBox_condition1->addItems(condition1);

        //        ui->comboBox_operator->clear();
        //        QStringList operators;
        //        operators<<tr("=")<<tr(">")<<tr("<");
        //        ui->comboBox_operator->addItems(operators);

        //        ui->comboBox_condition2->clear();
        //        QStringList condition2;
        //        condition2<<tr("2");
        //        ui->comboBox_condition2->addItems(condition2);

        ui->stackedWidget->setCurrentIndex(5);
    }
    else if(arg1 == "泄漏")
    {
        ui->groupBox->show();
        ui->stackedWidget->setCurrentIndex(6);
    }
    else
    {
        ui->groupBox->hide();
    }
}

void StepParameter::on_comboBox_condition1_currentIndexChanged(const QString &arg1)
{
    qDebug()<<"condition1_currentIndexChanged"<<arg1;
    if(arg1 == "当前PACK_SN")
    {
        ui->comboBox_operator->clear();
        QStringList operators;
        operators<<tr("=")<<tr(">")<<tr("<");
        ui->comboBox_operator->addItems(operators);

        ui->comboBox_condition2->clear();
        QStringList condition2;
        condition2<<tr("2");
        ui->comboBox_condition2->addItems(condition2);
    }
}

//PLC请求
void StepParameter::on_radioButton_request_clicked()
{
    ui->comboBox->clear();
    QStringList RequestList;
    RequestList<<tr("请求产品型号")<<tr("请求模组产品型号")<<tr("请求机器人拧紧位置号")
              <<tr("请求堵头安装位置号")<<tr("请求下箱体位置号")<<tr("请求模组入箱位置号")
             <<tr("请求捆绑流水号、料车号（AGV到位）")<<tr("请求下箱体条码扫描")<<tr("请求条码数据绑定")
            <<tr("请求记录堵头安装数据")<<tr("请求机器人涂胶位置号")<<tr("请求辊道托盘类型")
           <<tr("涂胶拧紧请求产品型号");
    ui->comboBox->addItems(RequestList);

}

//完成信号
void StepParameter::on_radioButton_completed_clicked()
{
    ui->comboBox->clear();
    QStringList CompletedList;
    CompletedList<<tr("辊道托盘到位")<<tr("托盘到位匹配产品型号")<<tr("站完成请求PCL放行托盘")
                <<tr("机器人到达拧紧位置")<<tr("已收到站完成信号")<<tr("机器人翻转下箱体完成")
               <<tr("模组入箱完成")<<tr("夹具抓取成功")<<tr("本次模组搬运完成")<<tr("手动/自动模式")<<tr("启动开班信号")
              <<tr("开班信号")<<tr("Pack1状态")<<tr("Pack2状态")<<tr("下线托盘型号")
             <<tr("涂胶数据绑定完成")<<tr("涂胶拧紧开班信号")<<tr("涂胶拧紧Pack1状态")<<tr("Pack数据")<<tr("Pack1SN")<<tr("Pack2SN");
    ui->comboBox->addItems(CompletedList);
}

//手动操作
void StepParameter::on_radioButton_manual_clicked()
{
    ui->comboBox->clear();
    QStringList ManualList;
    ManualList<<tr("向服务器请求产品型号")<<tr("条码枪扫码")<<tr("备用绑定流水号")<<tr("拧紧")<<tr("泄漏")
             <<tr("等待I/O输入")<<tr("I/O输出")<<tr("跳转")<<tr("显示PDM")<<tr("手工站站完成")
            <<tr("操作PACK1")<<tr("操作PACK2")<<tr("辊道线数据绑定")<<tr("挂起")<<tr("返修不合格跳过");
    ui->comboBox->addItems(ManualList);
}
/***********************************************/
//step data save
/***********************************************/
void StepParameter::on_pushButton_save_clicked()
{
    if(!judgment())
    {
        ui->label_warning->show();
        return;
    }
    int stepType = 0;
    if(ui->radioButton_request->isChecked())//PLC请求
        stepType = 1;
    else if(ui->radioButton_completed->isChecked())//完成信号
        stepType = 2;
    QVariantMap singleStep;
    singleStep["type"] = stepType;
    singleStep["singleStep"] = ui->comboBox->currentText();//选择的步骤名,存储到singleStep
    if(!ui->groupBox->isHidden())
    {
        if(ui->stackedWidget->currentIndex()==0)//barcode set interface
        {
            singleStep["scanTimes"]=ui->lineEdit_scanTimes->text();
            singleStep["PDM"]=ui->comboBox_PDM->currentText();
//            singleStep["regexp"]=ui->lineEdit_regexp->text();
            singleStep["regexp"]=ui->comboBox_ScanRegwxp->currentText();
            GscanRegExp = ui->comboBox_ScanRegwxp->currentText();
            singleStep["regexp2"]=ui->lineEdit_regexp_2->text();
            singleStep["datediff"]=ui->lineEdit_dateDiff->text();
            singleStep["datediff2"]=ui->lineEdit_dateDiff2->text();
            singleStep["VRegexp"]=ui->lineEdit_VictionRegexp->text();
            singleStep["regexpCode"]=ui->lineEdit_regexpCode->text();//条码匹配规则
            singleStep["scanPass"]=ui->lineEdit_scanPass->text();
            singleStep["parameter"]=ui->lineEdit_scanTimes->text()+","+ui->comboBox_PDM->currentText()+","+ui->lineEdit_regexp_2->text()+","+ui->lineEdit_regexpCode->text()+","+ui->lineEdit_scanPass->text()+","+ui->lineEdit_dateDiff->text()+","+ui->lineEdit_VictionRegexp->text();
        }
        else if(ui->stackedWidget->currentIndex()==1)//IO set interface
        {
            singleStep["IO"]=ui->comboBox_IO->currentText();
            singleStep["isTrue"]=QString::number(ui->checkBox_isTrue->isChecked());
            if(ui->checkBox_isTrue->isChecked())
            {
                singleStep["parameter"]=ui->comboBox_IO->currentText()+",true";
            }
            else
            {
                singleStep["parameter"]=ui->comboBox_IO->currentText()+",false";
            }
        }
        else if(ui->stackedWidget->currentIndex()==2)//PDM show set interface
        {
            singleStep["PDMShowTime"]=ui->lineEdit_PDMShowTime->text();
            singleStep["PDM"]=ui->comboBox_PDM_2->currentText();
            singleStep["PDMPass"]=ui->lineEdit_PDMPass->text();
            singleStep["parameter"]=ui->lineEdit_PDMShowTime->text()+","+ui->comboBox_PDM_2->currentText()+","+ui->lineEdit_PDMPass->text();
        }
        else if(ui->stackedWidget->currentIndex()==8)
        {
            singleStep["finishPass"]=ui->lineEdit_finishPass->text();
            singleStep["parameter"]=ui->lineEdit_finishPass->text();
        }
        else if(ui->stackedWidget->currentIndex()==9)
        {
            singleStep["jumpStep"]=ui->lineEdit_JumpStep->text();
            singleStep["stationTrue"]=ui->lineEdit_StationTrue->text();
            singleStep["parameter"]=ui->lineEdit_JumpStep->text()+","+ui->lineEdit_StationTrue->text();
        }
        else if(ui->stackedWidget->currentIndex()==10)
        {
            singleStep["PDM"]=ui->comboBox_PDM_name_holdOn->currentText();
            singleStep["holdOn"]=ui->lineEdit_holdOn->text();
            singleStep["parameter"]=ui->comboBox_PDM_name_holdOn->currentText()+","+ui->lineEdit_holdOn->text();
        }
        else if(ui->stackedWidget->currentIndex()==7)
        {
            singleStep["whichPack"]=ui->comboBox_packTemp->currentText();
            singleStep["parameter"]=ui->comboBox_packTemp->currentText();
        }
        else if(ui->stackedWidget->currentIndex()==3)//auto step set interface
        {
            singleStep["readAdd"]=ui->lineEdit_readAdd->text();
            singleStep["writeAdd"]=ui->lineEdit_writeAdd->text();
            QString addParameter = "";
            if(ui->stackedWidget_2->currentIndex()==1)
            {
                singleStep["plugTimes"]=ui->lineEdit_plugTimes->text();
                addParameter = ","+ui->lineEdit_plugTimes->text();
            }
            else if(ui->stackedWidget_2->currentIndex()==2)
            {
                singleStep["tightenTimes"]=ui->lineEdit_tightenTimes->text();
                addParameter = ","+ui->lineEdit_tightenTimes->text();
            }
            else if(ui->stackedWidget_2->currentIndex()==3)
            {
                singleStep["boxNums"]=ui->lineEdit_boxNums->text();
                addParameter = ","+ui->lineEdit_boxNums->text();
            }
            else if(ui->stackedWidget_2->currentIndex()==4)
            {
                singleStep["moduleNums"]=ui->lineEdit_moduleNums->text();
                addParameter = ","+ui->lineEdit_moduleNums->text();
            }
            else if(ui->stackedWidget_2->currentIndex()==5)
            {
                singleStep["w500IP"]=ui->lineEdit_w500IP->text();
                addParameter = ","+ui->lineEdit_w500IP->text();
            }
            else if(ui->stackedWidget_2->currentIndex()==6)
            {
                singleStep["regexpVM1"]=ui->lineEdit_regexp_VM1->text();
                singleStep["regexpVM2"]=ui->lineEdit_regexp_VM2->text();
                singleStep["VariantDiff"]=ui->lineEdit_VariantDiff->text();
                singleStep["dataDiff"]=ui->lineEdit_dataDiff->text();
                singleStep["dataDiff1"]=ui->lineEdit_dataDiff_2->text();
                singleStep["lingjian"]=ui->lineEdit_lingjian->text();
                addParameter = ","+ui->lineEdit_regexp_VM1->text()+","+ui->lineEdit_regexp_VM2->text();
            }
            singleStep["parameter"]=ui->lineEdit_readAdd->text()+","+ui->lineEdit_writeAdd->text()+addParameter;
        }
        else if(ui->stackedWidget->currentIndex()==4)
        {
            singleStep["Signature"]=ui->lineEdit_Signature->text();
            singleStep["TightenPass"] = ui->lineEdit_PackKeyCode->text();//拧紧型号
            singleStep["isOFF"]=QString::number(ui->checkBox_Tightening_isFalse->isChecked());
            if(ui->checkBox_Tightening_isFalse->isChecked())
            {
                singleStep["parameter"]=ui->lineEdit_Signature->text()+","+"OFF"+","+
                        ui->lineEdit_PackKeyCode->text();//参数栏显示
            }
            else
            {
                singleStep["parameter"]=ui->lineEdit_Signature->text()+","+"ON"+","+
                        ui->lineEdit_PackKeyCode->text();//参数栏显示
            }
        }
        else if(ui->stackedWidget->currentIndex()==5)
        {
            singleStep["condition1"]=ui->comboBox_condition1->currentText();
            singleStep["operator"]=ui->comboBox_operator->currentText();
            singleStep["condition2"]=ui->comboBox_condition2->currentText();
            singleStep["jump"]=ui->lineEdit_jump->text();
            singleStep["parameter"]=ui->comboBox_condition1->currentText()+","+ui->comboBox_operator->currentText()+","+ui->comboBox_condition2->currentText()+","+ui->lineEdit_jump->text();
        }
        else if(ui->stackedWidget->currentIndex()==6)
        {
            singleStep["boltSN"]=ui->lineEdit_bolt->text();
            singleStep["program"]=ui->lineEdit_program->text();
            singleStep["PDM"]=ui->comboBox_PDM_name->currentText();
            singleStep["parameter"]=ui->lineEdit_bolt->text()+","+ui->lineEdit_program->text()+","+ui->comboBox_PDM_name->currentText();
        }
    }
    emit sendStep(singleStep);//配置好的singleStep清单发stepedit.cpp(PHEV) and stepeditbev.cpp(BEV)
}

bool StepParameter::judgment()
{
    if(!ui->radioButton_manual->isChecked() && !ui->radioButton_request->isChecked() && !ui->radioButton_completed->isChecked())
        return false;
    if(!ui->groupBox->isHidden())
    {
        if(ui->stackedWidget->currentIndex()==0)
        {
            if(ui->lineEdit_scanTimes->text().isEmpty() || ui->lineEdit_scanPass->text().isEmpty())
                return false;
        }
        else if(ui->stackedWidget->currentIndex()==2)
        {
            if(ui->lineEdit_PDMShowTime->text().isEmpty() || ui->lineEdit_PDMPass->text().isEmpty())
                return false;
        }
        else if(ui->stackedWidget->currentIndex()==8)
        {
            if(ui->lineEdit_finishPass->text().isEmpty())
                return false;
        }
        else if(ui->stackedWidget->currentIndex()==9)
        {
            if(ui->lineEdit_JumpStep->text().isEmpty() || ui->lineEdit_StationTrue->text().isEmpty())
                return false;
        }
        else if(ui->stackedWidget->currentIndex()==3)
        {
            if(ui->lineEdit_readAdd->text().isEmpty()||ui->lineEdit_writeAdd->text().isEmpty())
                return false;
            if(ui->stackedWidget_2->currentIndex()==1)
            {
                if(ui->lineEdit_plugTimes->text().isEmpty())
                    return false;
            }
            else if(ui->stackedWidget_2->currentIndex()==2)
            {
                if(ui->lineEdit_tightenTimes->text().isEmpty())
                    return false;
            }
            else if(ui->stackedWidget_2->currentIndex()==3)
            {
                if(ui->lineEdit_boxNums->text().isEmpty())
                    return false;
            }
            else if(ui->stackedWidget_2->currentIndex()==4)
            {
                if(ui->lineEdit_moduleNums->text().isEmpty())
                    return false;
            }
        }
        else if(ui->stackedWidget->currentIndex()==4)
        {
            if(ui->lineEdit_Signature->text().isEmpty() || ui->lineEdit_PackKeyCode->text().isEmpty())
                return false;
        }
        else if(ui->stackedWidget->currentIndex()==5)
        {
            if(ui->comboBox_condition1->currentText()=="" ||
                    ui->comboBox_operator->currentText()=="" ||
                    ui->comboBox_condition2->currentText()=="" ||
                    ui->lineEdit_jump->text()=="")
                return false;
        }
        else if(ui->stackedWidget->currentIndex()==6)
        {
            if(ui->lineEdit_bolt->text().isEmpty()||
                    ui->lineEdit_program->text().isEmpty())
                return false;
        }
    }
    return true;
}

void StepParameter::initPDM()
{
    QDir *dir = new QDir("/PDM");
    QStringList list = dir->entryList();
    QString string;
    for (int index = 0; index < list.size(); index++)
    {
        string = list.at(index);
        if(string == "." || string == "..")
            continue;
        ui->comboBox_PDM->addItem(string);
        ui->comboBox_PDM_2->addItem(string);
        ui->comboBox_PDM_name->addItem(string);
        ui->comboBox_PDM_name_holdOn->addItem(string);
    }
}

void StepParameter::on_pushButton_close_clicked()
{
    emit closeDiglag();
}

void StepParameter::initScanList()
{
    QString fileName = "/ScanList";
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly| QIODevice::Text))
    {
        qDebug()   << "Cannot open ScanList file for Reading";
    }
    else
    {
        QTextStream data(&file);
        QString line;
        while (!data.atEnd())//逐行读取文本，并去除每行的回车
            {
                line = data.readLine();
                line.remove('\n');
                ScanList<<line;
                ui->comboBox_ScanRegwxp->addItem(line);
            }
        qDebug()<<"ScanList"<<ScanList;
        file.close();
    }
}

void StepParameter::on_comboBox_ScanRegwxp_currentIndexChanged(const QString &arg1)
{
    qDebug()<<"comboBox_ScanRegwxp_currentIndexChanged"<<arg1;
}
