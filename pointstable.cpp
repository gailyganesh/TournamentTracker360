#include "pointstable.h"
#include "ui_pointstable.h"
#include "mainwindow.h"
#include <string>
#include <sstream>
#include <regex>

PointsTable::PointsTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointsTable)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    connect(ui->tableWidget, &QTableWidget::cellChanged, this, &PointsTable::CalculateValue);
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

int calculateExpression(const std::string& expression) {
    std::istringstream iss(expression);
    char op;
    int num1, num2;
    iss >> num1 >> op >> num2;

    switch (op) {
    case '+':
        return num1 + num2;
    case '-':
        return num1 - num2;
    case '*':
        return num1 * num2;
    case '/':
        if (num2 != 0) {
            return num1 / num2;
        }
        else {
            std::cerr << "Error: Division by zero" << std::endl;
            return 0;
        }
    default:
        std::cerr << "Error: Invalid operator" << std::endl;
        return 0;
    }
}

bool isValidExpressionFormat(const std::string& expression)
{
    std::regex pattern("\\d+[-+*/]\\d+");
    return std::regex_match(expression, pattern);
}

void PointsTable::CalculateValue(int row, int columns)
{
    if ((columns == ColumnList::WinMatches) || (columns == ColumnList::Points))
    {
        auto str = ui->tableWidget->item(row, columns)->text().toStdString();
        if (isValidExpressionFormat(str))
        {
            int value = calculateExpression(ui->tableWidget->item(row, columns)->text().toStdString());
            ui->tableWidget->setItem(row, columns, new QTableWidgetItem(QString::fromStdString(std::to_string(value))));
        }
    }
}
