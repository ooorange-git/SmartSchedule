#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "qpropertyanimation.h"
#include "qpushbutton.h"
#include "swapclassdialog.h"
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
    int CURRENT_BLUR_SETTING = 0;

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
    bool is_exist(QString path);
    Ui::Schedule *ui;
    QPoint m_dragP;
    bool m_bDragging;
    bool usingPhy=0;
    QMenu *contextMenu=nullptr;
    void createContextMenu();
    MainWindow *m_mainwindow=nullptr;
    QPushButton *closeB=nullptr;
    QGraphicsOpacityEffect *e = new QGraphicsOpacityEffect(closeB);
    QPropertyAnimation *anim = nullptr;
    QPropertyAnimation *anim2 = nullptr;
    bool m_pressing=0;
    QPoint firstPos;
    void setShadow();
    void recordWindowSize();
    void SetWindowSize();
    QPixmap BluredWallPaper;
    QPixmap noise;
    void setBackground();
    QPixmap addNoise(int size);
    void hideCloseButton();
    void initPosition();
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayMenu;
    void setSystemTrayIcon();
    void setLabel(QLabel *l,int today);
    void setLabelText(QLabel *l,QString content);
    void swapClass();
    Physics *p = nullptr;
    bool inited=0;
};

#endif // SCHEDULE_H
