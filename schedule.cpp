#include "schedule.h"
#include "ui_schedule.h"
#include "mainwindow.h"
#include "xlsxdocument.h"

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

void Schedule::ResetSchedule()
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

bool Schedule::LoadFromFile(const QString& fileName)
{
    ResetSchedule();
    QXlsx::Document xlsx(fileName);

    if(xlsx.load() && xlsx.selectSheet("Schedule"))
    {
        int No_Row=xlsx.dimension().rowCount();
        int No_Column=xlsx.dimension().columnCount();
        for(int row_id=2; row_id<=No_Row; ++row_id)
        {
            uint rowCount=ui->scheduleTableWidget->rowCount();
            ui->scheduleTableWidget->setRowCount(rowCount+1);
            for(int col_id=1; col_id<=No_Column;++col_id)
            {
                auto item = new QTableWidgetItem(xlsx.read(row_id,col_id).toString());
                ui->scheduleTableWidget->setItem(rowCount,col_id-1,item);
            }
        }
        return true;
    }
    return false;
}

void Schedule::ReTranslate()
{
    ui->retranslateUi(this);
}
