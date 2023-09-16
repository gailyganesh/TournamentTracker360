#include "pointstable.h"
#include "ui_pointstable.h"
#include "mainwindow.h"

PointsTable::PointsTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointsTable)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
}

PointsTable::~PointsTable()
{
    delete ui;
}

void PointsTable::CreatePointsTable(Base::Match::MatchCreator& creator)
{
    for (int player=0; player<creator.mPlayersList.size(); player++)
    {
        uint rowCount=ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        auto playerInstance=creator.mPlayersList[player];
        for(int col=0;col<ui->tableWidget->columnCount();col++)
        {
            switch(col)
            {
            case ColumnList::PlayerNumber:
            {
                auto item = new QTableWidgetItem(QString::fromStdString(std::to_string(playerInstance.mNumber)));
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                ui->tableWidget->setItem(player,ColumnList::PlayerNumber,item);
                break;
            }
            case ColumnList::PlayerName:
            {
                auto item = new QTableWidgetItem(QString::fromStdString(playerInstance.mName));
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                ui->tableWidget->setItem(player,ColumnList::PlayerName,item);
                break;
            }
            case ColumnList::WinMatches:
            {
                ui->tableWidget->setItem(player,ColumnList::WinMatches,new QTableWidgetItem(QString::fromStdString(std::to_string(playerInstance.mWinMatches))));
                break;
            }
            case ColumnList::Points:
            {
                ui->tableWidget->setItem(player,ColumnList::Points,new QTableWidgetItem(QString::fromStdString(std::to_string(playerInstance.mPoints))));
                break;
            }
            }
        }
    }
}

void PointsTable::ResetPointTable()
{
//    for(int row=0; row<ui->tableWidget->rowCount(); row++)
//    {
//        ui->tableWidget->removeRow(row);
//    }
    while(ui->tableWidget->rowCount()!=0)
    {
        ui->tableWidget->removeRow(0);
    }
}

void PointsTable::SaveToFile(const QString& fileName)
{
    Common::WriteTableDataToFile(ui->tableWidget,fileName);
}

void PointsTable::ReTranslate()
{
    ui->retranslateUi(this);
}
