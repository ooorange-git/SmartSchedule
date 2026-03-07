#include "schedule.h"
#include "ui_schedule.h"
#include "mainwindow.h"
#include <QDate>
#include <QFile>
#include <QAction>
#include <QMenu>
#include <QDebug>
#include <QTimer>

void SetLabel(QFile& file,QLabel *l){
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    l->setText(content);
}

// mainwindow.cpp
void Schedule::startDailyTimer()
{
    QDateTime now = QDateTime::currentDateTime();
    // 构造下一个午夜 0:00:00.000
    QDateTime nextMidnight = now.addDays(1);   // 先加一天
    nextMidnight.setTime(QTime(0, 0, 0, 0));   // 设置为当天的 00:00:00
    // 注意：上面先 addDays(1) 再 setTime(0,0) 得到的是明天凌晨

    // 计算到下一个午夜的时间差（毫秒）
    qint64 msecsToNext = now.msecsTo(nextMidnight);
    if (msecsToNext <= 0) {
        // 理论上不会发生，但万一当前时间刚好是午夜 00:00，msecsToNext 可能为 0？
        // 保险起见，如果 ≤0 则设为 24 小时（86400000 毫秒）
        msecsToNext = 86400000;
    }

    // 使用 singleShot 定时触发
    QTimer::singleShot(msecsToNext, this, [this]() {
        updateLabel();          // 执行需要的操作
        startDailyTimer();             // 重新设置下一次
    });
}

void Schedule::updateLabel(){
    if(!ui) return;

    int day=MainWindow::goodDay();
    QLabel* c[8]={ui->C1,ui->C2,ui->C3,ui->C4,ui->C5,ui->C6,ui->C7,ui->C8};

    QFont f("微软雅黑",14);
    f.setHintingPreference(QFont::PreferNoHinting);
    f.setStyleStrategy(QFont::PreferAntialias);

    for(int i=0;i<7;i++){
        c[i]->setFont(f);
    }
    ui->Noon->setFont(f);
    ui->BB1->setFont(f);
    ui->BB2->setFont(f);

    if(day>=1 && day <=5){
        for(int i=0;i<7;i++){
            QString name="config/";
            name+=QString::number(day);
            name+="\\";
            name+=QString::number(i+1);
            QFile file(name);
            SetLabel(file,c[i]);
        }
    }else{
        c[1]->setText("今");
        ui->Noon->setText("天");
        c[5]->setText("无");
        ui->BB2->setText("课");
        c[0]->setVisible(0);
        c[2]->setVisible(0);
        c[3]->setVisible(0);
        c[4]->setVisible(0);
        c[6]->setVisible(0);
        c[7]->setVisible(0);
        ui->BB1->setVisible(0);
    }

}

Schedule::Schedule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Schedule)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnBottomHint);
    setWindowTitle(" ");

    //右键菜单
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,this, &Schedule::showContextMenu);
    createContextMenu();

    //设置课程
    updateLabel();


    //定时器
    startDailyTimer();


}

void Schedule::createContextMenu()
{
    contextMenu = new QMenu(this);
    QAction *action1 = contextMenu->addAction("打开主窗口");
    QAction *action2 = contextMenu->addAction("刷新");
    contextMenu->addSeparator();
    QAction *action3 = contextMenu->addAction("退出");
    connect(action1, &QAction::triggered, this, &Schedule::onMainWindow);
    connect(action2, &QAction::triggered, this, &Schedule::onUpdate);
    connect(action3, &QAction::triggered, this, &Schedule::onQuit);
}

void Schedule::showContextMenu(const QPoint &pos)
{
    contextMenu->exec(mapToGlobal(pos));
}

void Schedule::onUpdate(){
    Schedule::updateLabel();
}

void Schedule::onQuit(){
    qApp->quit();
}

void Schedule::onMainWindow(){
    if (m_mainwindow->isMinimized())
        m_mainwindow->showNormal();  // 从最小化恢复
    else
        m_mainwindow->show();
    m_mainwindow->raise();
    m_mainwindow->activateWindow();

}

Schedule::~Schedule()
{
    qDebug()<<1;
    delete ui;
}
