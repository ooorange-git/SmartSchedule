#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "mainwindow.h"
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
    void SetMain(MainWindow *w){ m_mainwindow=w; };

private slots:
    void showContextMenu(const QPoint &pos);
    void onMainWindow();
    void onUpdate();
    void onQuit();

private:
    Ui::Schedule *ui;
    QMenu *contextMenu=nullptr;
    void createContextMenu();
    MainWindow *m_mainwindow;
};

#endif // SCHEDULE_H
