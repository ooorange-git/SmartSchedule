#ifndef SWAPCLASSDIALOG_H
#define SWAPCLASSDIALOG_H

#include "ui_swapclassdialog.h"
#include <QDialog>

struct SwapClassData{
    int day1;
    int day2;
    int classN1;
    int classN2;
};

namespace Ui {
    class SwapClassDialog;
}

class SwapClassDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SwapClassDialog(QWidget *parent = nullptr,int day1=1,int day2=1)
        : QDialog(parent)
        , ui(new Ui::SwapClassDialog)
    {
        ui->setupUi(this);
        this->setWindowTitle("换课");
        ui->day1Box->setCurrentIndex(day1-1);
        ui->day2Box->setCurrentIndex(day2-1);
        data.day1 = ui->day1Box->currentIndex()+1;
        data.day2 = ui->day2Box->currentIndex()+1;
        data.classN1 = ui->class1Box->currentIndex()+1;
        data.classN2 = ui->class2Box->currentIndex()+1;
        connect(ui->day1Box,QOverload<int>::of(&QComboBox::activated),this,&SwapClassDialog::on_day1);
        connect(ui->day2Box,QOverload<int>::of(&QComboBox::activated),this,&SwapClassDialog::on_day2);
        connect(ui->class1Box,QOverload<int>::of(&QComboBox::activated),this,&SwapClassDialog::on_class1);
        connect(ui->class2Box,QOverload<int>::of(&QComboBox::activated),this,&SwapClassDialog::on_class2);
    }

    ~SwapClassDialog(){
        delete ui;
    };

    SwapClassData getData(){
        return data;
    }

private slots:
    void on_day1(int index){data.day1 = index+1;};
    void on_day2(int index){data.day2 = index+1;};
    void on_class1(int index){data.classN1 = index+1;};
    void on_class2(int index){data.classN2 = index+1;};

private:
    SwapClassData data;
    Ui::SwapClassDialog *ui;
};

#endif // SWAPCLASSDIALOG_H
