#ifndef POINTSTABLE_H
#define POINTSTABLE_H

#include <QWidget>
#include "MatchCreator.h"

namespace Ui {
class PointsTable;
}

class PointsTable : public QWidget
{
    Q_OBJECT

public:
    explicit PointsTable(QWidget *parent = nullptr);
    ~PointsTable();

private:
    Ui::PointsTable *ui;
    enum ColumnList:int
    {
        PlayerNumber=0,
        PlayerName=1,
        WinMatches=2,
        Points=3
    };
public:
    void CreatePointsTable(tournamentManager::backend::matchCreator::Base::Match::MatchCreator& creator);
    void ResetPointTable();
    void SaveToFile(const QString& fileName);
    bool LoadFromFile(const QString& fileName);
    void ReTranslate();
public slots:
    void CalculateValue(int row, int column);
};

#endif // POINTSTABLE_H
