#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_about_clicked(){
    QString dlgTitle="关于";
    QString strInfo="本程序使用Qt编写";
    QMessageBox::information(this, dlgTitle,strInfo,QMessageBox::Ok,QMessageBox::NoButton);
}
