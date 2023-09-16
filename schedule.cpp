#include "schedule.h"
#include "ui_schedule.h"
#include "mainwindow.h"

Schedule::Schedule(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Schedule)
{
    ui->setupUi(this);
    ui->scheduleTableWidget->setColumnWidth(0,500);
}

Schedule::~Schedule()
{
    delete ui;
}

void Schedule::AddData(const QString& value)
{
    uint rowCount=ui->scheduleTableWidget->rowCount();
    ui->scheduleTableWidget->setRowCount(rowCount+1);
    ui->scheduleTableWidget->setItem(rowCount,0,new QTableWidgetItem(value));
}

void Schedule::Reset()
{
    while(ui->scheduleTableWidget->rowCount()!=0)
    {
        ui->scheduleTableWidget->removeRow(0);
    }
}

void Schedule::SaveToFile(const QString& fileName)
{
    Common::WriteTableDataToFile(ui->scheduleTableWidget,fileName);
}

void Schedule::ReTranslate()
{
    ui->retranslateUi(this);
}
