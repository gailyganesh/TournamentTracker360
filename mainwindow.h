#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QLocale>
#include <QTranslator>
#include "MatchCreator.h"
#include "Schedule.h"
#include "pointstable.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace Common {
void WriteTableDataToFile(QTableWidget* tableWidget, const QString& fileName);
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();

    void on_removeButton_clicked();

    void on_doneButton_clicked();

    void on_actionLoad_triggered();

    void on_actionSave_Player_List_triggered();

    void on_actionSave_Schedule_triggered();

    void on_actionSave_Points_Table_triggered();

    void on_actionEnglish_triggered();

    void on_actionDeutsch_triggered();

    void on_actionSave_All_in_One_triggered();

private:
    Ui::MainWindow *ui;
    Schedule *sch=nullptr;
    PointsTable *pointsTable=nullptr;

    std::map<int,std::vector<std::string>> mFileData;

    QTranslator translator;


    tournament_manager::backend::matchCreatorLib::Base::Match::MatchCreator mCreator;
    enum ColumnList:int
    {
        PlayerNumber=0,
        PlayerName=1,
        WinMatches=2,
        Points=3
    };
private:
    void CreatePlayers();
    void CreateSchedule(bool isRepeatationAllowed=false);
    void CreatePointsTable();
    void ResetPointsTableIfRequired();
    void ResetScheduleIfRequired();
    void AddPlayerToTable(std::string number="",std::string name="");
    void ReadPlayerList(const QString& fileName);
};
#endif // MAINWINDOW_H
