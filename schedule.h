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

private:
    Ui::Schedule *ui;

};

#endif // SCHEDULE_H
