#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "qpropertyanimation.h"
#include "qpushbutton.h"
#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QPoint>
#include <QGraphicsEffect>
#include <QLabel>

class MainWindow;
class Physics;

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
    void monday();
    void weekday();
    void weekEnd();
    void showAll();
    void startDailyTimer();
    void setMainWindow(MainWindow *w){ m_mainwindow=w; };
    static bool enableSetWindowCompositionAttribute(QWidget *w);
    bool enableBlurBehindWindow();
    void usePhy();
private slots:
    void showContextMenu(const QPoint &pos);
    void onMainWindow();
    void onUpdate();
    void onQuit();

protected:
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QString readFile(QString path);
    void writeFile(QString path,QString content);
    Ui::Schedule *ui;
    QPoint m_dragP;
    bool m_bDragging;
    bool usingPhy=0;
    QMenu *contextMenu=nullptr;
    void createContextMenu();
    QTimer *timer_update;
    MainWindow *m_mainwindow=nullptr;
    QPushButton *closeB=nullptr;
    QGraphicsOpacityEffect *e = new QGraphicsOpacityEffect(closeB);
    QPropertyAnimation *anim = new QPropertyAnimation(e,"opacity");
    QPropertyAnimation *anim2 = new QPropertyAnimation(e,"opacity");
    bool m_pressing=0;
    QPoint firstPos;
    void setShadow();
    void recordWindowSize();
    void SetWindowSize();
    QPixmap BluredWallPaper;
    void setBackground();
    void hideCloseButton();
    void initPosition();
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayMenu;
    void setSystemTrayIcon();
    void SetLabel(QLabel *l,int today);
    Physics *p = nullptr;
};

#endif // SCHEDULE_H
