#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QWidget>

namespace Ui {
class Schedule;
}

class Schedule : public QWidget
{
    Q_OBJECT

public:
    explicit Schedule(QWidget *parent = nullptr);
    ~Schedule();
    void AddData(const QString& value);
    void SaveToFile(const QString& fileName);
    bool LoadFromFile(const QString& fileName);
    void ResetSchedule();
    void ReTranslate();

private:
    Ui::Schedule *ui;
};

#endif // SCHEDULE_H
