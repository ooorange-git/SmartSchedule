#include "schedule.h"
#include "ui_schedule.h"
#include "mainwindow.h"
#include <QDate>
#include <QFile>
#include <QAction>
#include <QMenu>
#include <QDebug>
#include <QCloseEvent>
#include <QTimer>

void SetLabel(QFile& file,QLabel *l){
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    if(content.contains('/')){
        QStringList sl=content.split('/');
        if(MainWindow::weekNow(nullptr)%2==0){
            l->setText(sl[1]);
        }else{
            l->setText(sl[0]);
        }
        return;
    }
    l->setText(content);
}



void Schedule::showAll(){
    ui->Noon->setVisible(1);
    ui->BB1->setVisible(1);
    ui->BB2->setVisible(1);
    ui->C1->setVisible(1);
    ui->C2->setVisible(1);
    ui->C3->setVisible(1);
    ui->C4->setVisible(1);
    ui->C5->setVisible(1);
    ui->C6->setVisible(1);
    ui->C7->setVisible(1);
    ui->C8->setVisible(1);
}

void Schedule::monday(){
    QLabel* labels[8]={ui->C1,ui->BB1,ui->C3,ui->C4,ui->C5,ui->C6,ui->C7,ui->C8};
    showAll();
    ui->C2->setText("升旗仪式");
    ui->Noon->setText("午休");
    ui->BB2->setText("双师体育课");
    for(int i=0;i<8;i++){
        QString name=QCoreApplication::applicationDirPath()+"/config/1/"+QString::number(i+1);
        QFile file(name);
        SetLabel(file,labels[i]);
    }
}

void Schedule::weekday(int d){
    QLabel* labels[8]={ui->C1,ui->C2,ui->C3,ui->C4,ui->C5,ui->C6,ui->C7,ui->C8};
    showAll();
    ui->BB1->setText("大课间");
    ui->Noon->setText("午休");
    ui->BB2->setText("双师体育课");
    for(int i=0;i<8;i++){
        QString name=QCoreApplication::applicationDirPath()+"/config/"+QString::number(d)+"/"+QString::number(i+1);
        QFile file(name);
        SetLabel(file,labels[i]);
    }
}

void Schedule::weekEnd(){
    QLabel* c[8]={ui->C1,ui->C2,ui->C3,ui->C4,ui->C5,ui->C6,ui->C7,ui->C8};
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

void Schedule::updateLabel(){
    if(!ui) return;
    int day=MainWindow::goodDay();

    QLabel* c[8]={ui->C1,ui->C2,ui->C3,ui->C4,ui->C5,ui->C6,ui->C7,ui->C8};

    QFont f("LXGW WenKai Lite Medium",14);
    f.setHintingPreference(QFont::PreferNoHinting);
    f.setStyleStrategy(QFont::PreferAntialias);

    for(int i=0;i<8;i++){c[i]->setFont(f);}
    ui->Noon->setFont(f);
    ui->BB1->setFont(f);
    ui->BB2->setFont(f);

    if(day==1){
        monday();
    }else if(day>=2 && day <=5){
        weekday(day);
    }else{
        weekEnd();
    }
}


Schedule::Schedule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Schedule)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnBottomHint);
    setWindowTitle(" ");
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window,QColor("#F0F0F4"));
    this->setPalette(pal);
    this->setAutoFillBackground(1);

    //右键菜单
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,this, &Schedule::showContextMenu);
    createContextMenu();

    //设置课程
    updateLabel();

    timer_update = new QTimer(this);
    connect(timer_update,&QTimer::timeout,this,&Schedule::updateLabel);
    timer_update->start(10000);

}

void Schedule::createContextMenu()
{
    contextMenu = new QMenu(this);
    QAction *action1 = contextMenu->addAction("打开设置");
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

void Schedule::closeEvent(QCloseEvent *event){
    if(event->isAccepted()){
        qApp->quit();
    }
}

Schedule::~Schedule()
{
    delete ui;
}
