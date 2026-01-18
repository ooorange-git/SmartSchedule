#include "schedule.h"
#include "ui_schedule.h"
#include <QDate>
#include <QFile>
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

Schedule::~Schedule()
{
    delete ui;
}
