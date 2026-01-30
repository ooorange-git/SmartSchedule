#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QPoint>

namespace Ui {
class Schedule;
}

class Schedule : public QWidget
{
    Q_OBJECT

public:
    explicit Schedule(QWidget *parent = nullptr);
    ~Schedule();

private slots:
    void showContextMenu(const QPoint &pos);
    void onMainWindow();
    void onUpdate();

private:
    Ui::Schedule *ui;
    QMenu *contextMenu=nullptr;
    void createContextMenu();

};

#endif // SCHEDULE_H
