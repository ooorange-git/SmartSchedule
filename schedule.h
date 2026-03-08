#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QPoint>

class MainWindow;

namespace Ui {
class Schedule;
}

class Schedule : public QWidget
{
    Q_OBJECT

public:
    explicit Schedule(QWidget *parent = nullptr);
    ~Schedule();
    void updateLabel();
    void startDailyTimer();
    void SetMain(MainWindow *w){ m_mainwindow=w; };

private slots:
    void showContextMenu(const QPoint &pos);
    void onMainWindow();
    void onUpdate();
    void onQuit();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::Schedule *ui;
    QMenu *contextMenu=nullptr;
    void createContextMenu();
    MainWindow *m_mainwindow;
};

#endif // SCHEDULE_H
