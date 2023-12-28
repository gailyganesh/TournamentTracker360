#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include "xlsxdocument.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setCentralWidget(ui->tabWidget);
    ui->tableWidget->setColumnWidth(0,200);
    ui->tableWidget->setColumnWidth(1,ui->tableWidget->width()-100);
    ui->fileToolButton->setDefaultAction(ui->actionLoad);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_triggered()
{
    ResetPlayersList();
    QString fileName= QFileDialog::getOpenFileName(this, tr("Load Players List"), QDir::homePath(),"*.xlsx");
    QFile file(fileName);
    ui->lineEdit->setText(fileName);
    LoadPlayerListFromFile(fileName);
}

void MainWindow::on_actionLoad_All_triggered()
{
    QString fileName= QFileDialog::getOpenFileName(this, tr("Load Players List"), QDir::homePath(),"*.xlsx");
    QFile file(fileName);
    ui->lineEdit->setText(fileName);
    ResetPlayersList();
    LoadPlayerListFromFile(fileName);
    ResetScheduleIfRequired();
    ResetPointsTableIfRequired();
    ui->tabWidget->addTab(sch, tr("Schedules"));
    if(!sch->LoadFromFile(fileName))
    {
        QMessageBox::critical(this,tr("Error"), tr("Schedule Sheet is Not Available"));
    }
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);

    ui->tabWidget->addTab(pointsTable, tr("PointsTable"));
    if(!pointsTable->LoadFromFile(fileName))
    {
        QMessageBox::critical(this,tr("Error"), tr("PointsTable Sheet is Not Available"));
    }
}

void MainWindow::on_actionSave_Player_List_triggered()
{
    QString fileName= QFileDialog::getSaveFileName(this, tr("Save Players List"), QDir::homePath(),"*.xlsx");
    Common::WriteTableDataToFile(ui->tableWidget, fileName);
}

void MainWindow::on_actionSave_Schedule_triggered()
{
    if(sch==nullptr)
    {
        QMessageBox::critical(this,tr("Error"), tr("Schedule Tab is Not Available"));
    }
    QString fileName= QFileDialog::getSaveFileName(this, tr("Save Schedule"), QDir::homePath(),"*.xlsx");
    sch->SaveToFile(fileName);
}

void MainWindow::on_actionSave_Points_Table_triggered()
{
    if(pointsTable==nullptr)
    {
        QMessageBox::critical(this,tr("Error"), tr("Points Table Tab is Not Available"));
    }
    QString fileName= QFileDialog::getSaveFileName(this, tr("Save Points Table"), QDir::homePath(),"*.xlsx");
    pointsTable->SaveToFile(fileName);
}

void MainWindow::on_actionSave_All_in_One_triggered()
{
    if(sch==nullptr)
    {
        QMessageBox::critical(this,tr("Error"), tr("Schedule Tab is Not Available"));
    }
    if(pointsTable==nullptr)
    {
        QMessageBox::critical(this,tr("Error"), tr("Points Table Tab is Not Available"));
    }
    QString fileName= QFileDialog::getSaveFileName(this, tr("Save Points Table"), QDir::homePath(),"*.xlsx");

    pointsTable->SaveToFile(fileName);
    sch->SaveToFile(fileName);
    Common::WriteTableDataToFile(ui->tableWidget, fileName);
}

void MainWindow::on_actionEnglish_triggered()
{
    if(!translator.isEmpty())
    {
        qApp->removeTranslator(&translator);
        if(sch!=nullptr)
        {
            sch->ReTranslate();
        }
        if(pointsTable!=nullptr)
        {
            pointsTable->ReTranslate();
        }
        ui->retranslateUi(this);
    }
}

void MainWindow::on_actionDeutsch_triggered()
{
    if(translator.isEmpty())
    {
        const QString baseName = "Tournament_de_DE";
        if (translator.load(baseName+".qm"))
        {
            qApp->installTranslator(&translator);
            if(sch!=nullptr)
            {
                sch->ReTranslate();
            }
            if(pointsTable!=nullptr)
            {
                pointsTable->ReTranslate();
            }
            ui->retranslateUi(this);
            return;
        }
    }
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
}

void MainWindow::on_addButton_clicked()
{
    AddPlayerToTable();
}

void MainWindow::on_removeButton_clicked()
{
    QList<QTableWidgetItem *> selectedItems = ui->tableWidget->selectedItems();
    QList<int> selectedRows;

    foreach (const QTableWidgetItem *item, selectedItems)
    {
        if (!selectedRows.contains(item->row()))
        {
            selectedRows.append(item->row());
        }
    }

    // Sort the selected row indices in descending order
    std::sort(selectedRows.begin(), selectedRows.end(), std::greater<int>());

    // Remove selected rows
    foreach (int row, selectedRows)
    {
        ui->tableWidget->removeRow(row);
    }
}

void MainWindow::on_doneButton_clicked()
{
    //CreateCoulmns();
    CreatePlayers();
    mCreator.mNo_Matches=ui->spinBox->value();

    if(mCreator.mNo_Matches==0)
    {
        QMessageBox::critical(this,tr("Error"),tr("Please Set Number of Matches. 0 Matches can't created"));
        return;
    }
    if(mCreator.mPlayersList.size()<=1)
    {
        QMessageBox::critical(this,tr("Error"),tr("Atleast two players are needed to create the Match"));
        return;
    }
    bool isRepeatationAllowed=false;
    mCreator.GenerateTeams();
    if(!mCreator.IsMatchCalculationPossible())
    {
        QMessageBox::StandardButton reply=QMessageBox::question(this,tr("Error"),tr("Without repeatation couldn't create Matches. Do you want continue with repeatation?"),QMessageBox::Yes |QMessageBox::No);
        if(reply==QMessageBox::No)
        {
            return;
        }
        else if(QMessageBox::Yes)
        {
            isRepeatationAllowed=true;
        }

    }
    CreateSchedule(isRepeatationAllowed);
    CreatePointsTable();
}

void MainWindow::CreatePlayers()
{
    if(!mCreator.mPlayersList.empty())
    {
        mCreator.RestMatches();
        mCreator.RestPlayerList();
    }

    for(int row=0;row<ui->tableWidget->rowCount();row++)
    {
        int playerNumber;
        std::string PlayerName;
        for(int col=0;col<ui->tableWidget->columnCount();col++)
        {
            switch(col)
            {
            case ColumnList::PlayerNumber:
            {
                playerNumber=ui->tableWidget->item(row,ColumnList::PlayerNumber)->text().toInt();
                break;
            }
            case ColumnList::PlayerName:
            {
                PlayerName=ui->tableWidget->item(row,ColumnList::PlayerName)->text().toStdString();
                break;
            }
            }
        }
        if(!mCreator.AddToPlayersList(playerNumber,PlayerName))
        {
            std::string message = tr("Already player exist: ").toStdString()+PlayerName+"("+std::to_string(playerNumber)+")";
            QMessageBox::critical(this,tr("Error"), QString(message.c_str()));
            ResetScheduleIfRequired();
            ResetPointsTableIfRequired();
            return;
        }
    }
}

void MainWindow::CreateSchedule(bool isRepeatationAllowed)
{
    ResetScheduleIfRequired();
    mCreator.CalculateMatches(isRepeatationAllowed);
    for(int match=0; match<mCreator.mMatches.size(); match++)
    {
        auto match_ = mCreator.mMatches[match];
        QString value=QString::fromStdString(match_.first.mMembers[0].mName+"("+std::to_string(match_.first.mMembers[0].mNumber)+")"+"&"+match_.first.mMembers[1].mName+"("+std::to_string(match_.first.mMembers[1].mNumber)+")"+
                                               " vs "
                                               +match_.second.mMembers[0].mName+"("+std::to_string(match_.second.mMembers[0].mNumber)+")"+"&"+match_.second.mMembers[1].mName+"("+std::to_string(match_.second.mMembers[1].mNumber)+")");
        std::cout<<value.toStdString()<<std::endl;
        sch->AddData(value);
    }

    ui->tabWidget->addTab(sch, tr("Schedules"));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void MainWindow::CreatePointsTable()
{
    ResetPointsTableIfRequired();
    pointsTable->CreatePointsTable(mCreator);
    ui->tabWidget->addTab(pointsTable, tr("PointsTables"));
}

void MainWindow::ResetPlayersList()
{
    while(ui->tableWidget->rowCount()!=0)
    {
        ui->tableWidget->removeRow(0);
    }
}

void MainWindow::ResetPointsTableIfRequired()
{
    if (pointsTable==nullptr)
    {
        pointsTable=new PointsTable();
    }
    else
    {
        pointsTable->ResetPointTable(); //done clicked not first time
    }
}

void MainWindow::ResetScheduleIfRequired()
{
    if (sch==nullptr)
    {
        sch=new Schedule();
    }
    else
    {
        sch->ResetSchedule(); //done clicked not first time
    }
}

void MainWindow::AddPlayerToTable(std::string number,std::string name)
{
    uint rowCount=ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(rowCount+1);
    std::vector<QTableWidgetItem*> row;
    if(number.empty() && name.empty())
    {
        row.push_back(new QTableWidgetItem(tr("player number")));
        row.push_back(new QTableWidgetItem(tr("Enter player name")));
    }
    else
    {
        row.push_back(new QTableWidgetItem(QString(number.c_str())));
        row.push_back(new QTableWidgetItem(QString(name.c_str())));
    }
    for(int i=0; i<ui->tableWidget->columnCount(); i++)
    {
        ui->tableWidget->setItem(rowCount,i,row[i]);
    }
}

void MainWindow::LoadPlayerListFromFile(const QString& fileName)
{
    QXlsx::Document xlsx(fileName);

    if(xlsx.load() && xlsx.selectSheet("playerList"))
    {
        int No_Row=xlsx.dimension().rowCount();
        int No_Column=xlsx.dimension().columnCount();

        for(int row_id=1; row_id<=No_Row; ++row_id)
        {
            for(int col_id=1; col_id<=No_Column;++col_id)
            {
                if(row_id==1)
                {
                    QXlsx::Cell* cell = xlsx.cellAt(row_id, col_id); // get cell pointer.
                    if ( cell == NULL )
                    {
                        QMessageBox::critical(this,tr("Error"), tr("File is corruputed"));
                    }
                }
                else
                {
                    mFileData[row_id].push_back(xlsx.read(row_id,col_id).toString().toStdString());
                }
            }
            if(row_id!=1)
            {
                AddPlayerToTable(mFileData[row_id].at(0), mFileData[row_id].at(1));
            }
        }
    }
}

void Common::WriteTableDataToFile(QTableWidget* tableWidget, const QString& fileName)
{
    QXlsx::Document xlsxW(fileName);
    xlsxW.addSheet(tableWidget->parent()->objectName());
    for(int col=0;col<tableWidget->columnCount();col++)
    {
        QXlsx::Format format;
        format.setFontBold(true);
        format.setFontSize(12);
        xlsxW.write(1,col+1,tableWidget->horizontalHeaderItem(col)->text(),format);
    }
    for(int row=0;row<tableWidget->rowCount();row++)
    {
        for(int col=0;col<tableWidget->columnCount();col++)
        {
            xlsxW.write(row+2,col+1,tableWidget->item(row,col)->text());
        }
    }
    xlsxW.saveAs(fileName); // save the document as 'Test.xlsx'
}
