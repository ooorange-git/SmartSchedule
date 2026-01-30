#include "schedule.h"
#include "ui_schedule.h"
#include <QDate>
#include <QFile>
#include <QAction>
#include <QMenu>
#include <QDebug>

void SetLabel(QFile& file,QLabel *l){
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    l->setText(content);
}


Schedule::Schedule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Schedule)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint);
    setWindowTitle(" ");
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,this, &Schedule::showContextMenu);
    createContextMenu();
    QDate d(QDate::currentDate());
    int day=d.dayOfWeek();
    QLabel* c[8]={ui->C1,ui->C2,ui->C3,ui->C4,ui->C5,ui->C6,ui->C7,ui->C8};
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

void Schedule::createContextMenu()
{
    contextMenu = new QMenu(this);
    QAction *action1 = contextMenu->addAction("打开主窗口");
    QAction *action2 = contextMenu->addAction("刷新");
    contextMenu->addSeparator();
    QAction *action3 = contextMenu->addAction("退出");
    connect(action1, &QAction::triggered, this, &Schedule::onMainWindow);
    connect(action2, &QAction::triggered, this, &Schedule::onUpdate);
    connect(action3, &QAction::triggered, this, &QWidget::close);
}

void Schedule::showContextMenu(const QPoint &pos)
{
    contextMenu->exec(mapToGlobal(pos));
}

void Schedule::onUpdate(){
    qDebug()<<"a";
}

void Schedule::onMainWindow(){
    qDebug()<<"a";
}

Schedule::~Schedule()
{
    delete ui;
}
