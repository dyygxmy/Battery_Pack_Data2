#include "stepedit.h"
#include "ui_stepedit.h"

StepEdit::StepEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StepEdit)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setGeometry(QRect(0, 0, 1366, 768));
    initTable();
    ui->label_black->hide();
    ui->label_warning->hide();

    ParseJson * parsejson = new ParseJson;
    currentStep = parsejson->parseJson();
    delete parsejson;
    if(currentStep.value("errorCode")!=1)
        initUI(currentStep);
}

void StepEdit::initUI(QVariantMap Step)
{
    stepList.clear();
    ui->lineEdit_plcDB->clear();
    ui->lineEdit_plcIP->clear();
    ui->tableWidget->setRowCount(0);
    if(Step["isManual"].toBool())
    {
        ui->radioButton_manual->setChecked(true);
        on_radioButton_manual_clicked();
    }
    else
    {
        ui->radioButton_auto->setChecked(true);
        on_radioButton_auto_clicked();
        ui->lineEdit_plcDB->setText(Step["plcDB"].toString());
        ui->lineEdit_plcIP->setText(Step["plcIP"].toString());
    }

    foreach (QVariant single, Step["steps"].toList()) {
        QVariantMap singleMap = single.toMap();
        stepList.append(singleMap);
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(row + 1);//总行数增加1
        ui->tableWidget->setRowHeight(row,43);//行
        QTableWidgetItem *tableWidgetItem = new QTableWidgetItem(singleMap["singleStep"].toString());
        QTableWidgetItem *tableWidgetItem1 = new QTableWidgetItem(singleMap["parameter"].toString());
        //QTableWidgetItem *tableWidgetItem2 = new QTableWidgetItem(singleMap["parameter"].toString());
        ui->tableWidget->setItem(row, 0, tableWidgetItem);
        ui->tableWidget->setItem(row, 1, tableWidgetItem1);
        //ui->tableWidget->setItem(row, 2, tableWidgetItem2);
    }
}

StepEdit::~StepEdit()
{
    delete ui;
}

void StepEdit::on_pushButton_return_clicked()
{
    QVariantMap Step;
    Step.insert("isManual",ui->radioButton_manual->isChecked());
    if(ui->radioButton_auto->isChecked())
    {
        Step.insert("plcDB",ui->lineEdit_plcDB->text());
        Step.insert("plcIP",ui->lineEdit_plcIP->text());
    }
    Step.insert("steps",stepList);
    if(currentStep != Step)
    {
        whichSave = "return";
        newSave();
    }
    else
    {
        ui->label_warning->hide();
        this->close();
    }
}

void StepEdit::initTable()
{
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(0);

    // 创建表格项目，并将它们作为表头
    QTableWidgetItem *headerH = new QTableWidgetItem(tr("步骤"));
    ui->tableWidget->setHorizontalHeaderItem(0,headerH);
    QTableWidgetItem *headerH1 = new QTableWidgetItem(tr("参数"));
    ui->tableWidget->setHorizontalHeaderItem(1,headerH1);

    ui->tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setColumnWidth(0,369);//第一列宽度
    ui->tableWidget->setColumnWidth(1,300);//第2列宽度
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color: rgb(51, 153, 255);"
                                                       "color: rgb(248, 248, 255);border: 0px; font:14pt}");
    ui->tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{border-bottom:1px groove #bdac96;border-top:0px;border-left:0px;border-right:1px groove #e9dbc4;background-color: rgb(250, 250, 250);font: 12pt}");
    ui->tableWidget->horizontalHeader()->setFixedHeight(43);
    ui->tableWidget->verticalHeader()->setFixedWidth(50);
//    ui->tableWidget->horizontalHeader()->setFixedWidth(369);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    insertRow = -1;
}

void StepEdit::newStepParameter()
{
    e3 = new QGraphicsOpacityEffect(this);
    e3->setOpacity(0.5);
    ui->label_black->setGraphicsEffect(e3);
    ui->label_black->show();
    ui->label_black->setGeometry(0,0,1366,768);
    stepParameter = new StepParameter(this);
    connect(stepParameter,SIGNAL(sendStep(QVariantMap)),this,SLOT(receiveStep(QVariantMap)));
    connect(stepParameter,SIGNAL(closeDiglag()),this,SLOT(closeStepParameter()));
    stepParameter->show();
}

void StepEdit::newSave()
{
    SaveWhat = "Save";
    e3 = new QGraphicsOpacityEffect(this);
    e3->setOpacity(0.5);
    ui->label_black->setGraphicsEffect(e3);
    ui->label_black->show();
    ui->label_black->setGeometry(0,0,1366,768);

    save = new Save(this);
    connect(save,SIGNAL(sendSave(bool)),this,SLOT(saveStep(bool)));
    save->show();
}

void StepEdit::on_pushButton_append_clicked()
{
    newStepParameter();
    insertRow = ui->tableWidget->rowCount();
    currentOperate = add;
}

void StepEdit::on_pushButton_insert_clicked()
{
    if(ui->tableWidget->currentRow()!= -1)
    {
        newStepParameter();
        insertRow = ui->tableWidget->currentRow();
        currentOperate = insert;
    }
}

void StepEdit::on_pushButton_update_clicked()
{
    if(ui->tableWidget->currentRow()!= -1)
    {
        newStepParameter();
        insertRow = ui->tableWidget->currentRow();
        stepParameter->initUI(stepList[insertRow].toMap());
        currentOperate = update;
    }
}

void StepEdit::on_pushButton_remove_clicked()
{
    int rowIndex = ui->tableWidget->currentRow();
    if (rowIndex != -1)
    {
        ui->tableWidget->removeRow(rowIndex);
        stepList.removeAt(rowIndex);
    }
}

void StepEdit::on_radioButton_manual_clicked()
{
    ui->label_plcDB->hide();
    ui->label_plcIP->hide();
    ui->lineEdit_plcDB->hide();
    ui->lineEdit_plcIP->hide();
}

void StepEdit::on_radioButton_auto_clicked()
{
    ui->label_plcDB->show();
    ui->label_plcIP->show();
    ui->lineEdit_plcDB->show();
    ui->lineEdit_plcIP->show();
}

void StepEdit::on_pushButton_save_clicked()
{
    whichSave = "save";
    newSave();
}

void StepEdit::saveStep(bool isSave)
{
    if(isSave)
    {
        QVariantMap Step;
        Step.insert("isManual",ui->radioButton_manual->isChecked());
        if(ui->radioButton_auto->isChecked())
        {
            if(ui->lineEdit_plcDB->text().isEmpty() || ui->lineEdit_plcIP->text().isEmpty())
            {
                ui->label_warning->show();
                delete e3;
                ui->label_black->hide();
                delete save;
                return;
            }
            Step.insert("plcDB",ui->lineEdit_plcDB->text());
            Step.insert("plcIP",ui->lineEdit_plcIP->text());
        }
        Step.insert("steps",stepList);

        QJson::Serializer serializer;
        bool ok;
        QByteArray json = serializer.serialize(Step, &ok);

        if (ok)
        {
            qDebug() << json;
            QFile stepJson("/step.json");
            if(stepJson.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                QTextStream text_stream(&stepJson);
                text_stream << json << endl;
                stepJson.flush();
                stepJson.close();
                currentStep = Step;
                emit stepParaInit();//step parameter init
            }
            else
                qDebug()<<"step.json write open fail";
        }
        else
            qCritical() << "Something went wrong:" << serializer.errorMessage();
    }
    else
    {
        if(whichSave == "return")
            initUI(currentStep);
    }

    if(whichSave == "return")
    {
        ui->label_warning->hide();
        this->close();
    }
    delete e3;
    ui->label_black->hide();
    delete save;
}


//接收stepparameter.cpp中添加的步骤
void StepEdit::receiveStep(QVariantMap singleMap)
{
    QTableWidgetItem *tableWidgetItem = new QTableWidgetItem(singleMap["singleStep"].toString());
    QTableWidgetItem *tableWidgetItem1 = new QTableWidgetItem(singleMap["parameter"].toString());
    //QTableWidgetItem *tableWidgetItem2 = new QTableWidgetItem(singleMap["parameter"].toString());
    if(currentOperate == add)
    {
        ui->tableWidget->setRowCount(insertRow + 1);//总行数增加1
        ui->tableWidget->setRowHeight(insertRow,43);//行
        stepList.append(singleMap);
    }
    else if(currentOperate == insert)
    {
        ui->tableWidget->insertRow(insertRow);
        ui->tableWidget->setRowHeight(insertRow,43);//行
        stepList.insert(insertRow,singleMap);
    }
    else if(currentOperate == update)
    {
        stepList.replace(insertRow,singleMap);
    }
    ui->tableWidget->setItem(insertRow, 0, tableWidgetItem);
    ui->tableWidget->setItem(insertRow, 1, tableWidgetItem1);
    //ui->tableWidget->setItem(insertRow, 2, tableWidgetItem2);
    closeStepParameter();
}

void StepEdit::closeStepParameter()
{
    delete e3;
    ui->label_black->hide();
    delete stepParameter;
}
