#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "schedule.h"
#include "qdebug.h"
#include<QMessageBox>
#include<QFile>
#include<QTextStream>
#include<QSpinBox>
#include<QCoreApplication>
#include<QSettings>
#include<QDir>
#include<QScreen>
#include<QRect>
#include<QLineEdit>
#include<QDate>

int MainWindow::goodDay(){
    QFile file(QCoreApplication::applicationDirPath()+"/config/day");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString dayInFile=in.readAll();//文件中设置的星期

    QFile file2(QCoreApplication::applicationDirPath()+"/config/dayCheck");
    file2.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in2(&file2);
    int checkInFile=in2.readAll().toInt();

    QDate date=QDate::currentDate();
    int cD=date.toString("yyyyMMdd").toInt();

    if(dayInFile=="自动"){
        return date.dayOfWeek();
    }else{
        if(cD==checkInFile){
            return dayInFile.toInt();
        }else{
            return date.dayOfWeek();
        }
    }
    file.close();
    file2.close();
}

//该函数能返回精确周数含用户手动加减！！！
int MainWindow::weekNow(QDateEdit *l){
    int total=1;
    QFile file(QCoreApplication::applicationDirPath()+"/config/startTerm");
    QDate d2=QDate::currentDate();
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        QDate d1=QDate::fromString(in.readAll().trimmed(),Qt::ISODate);
        if(d1>d2){
            return -1;
        }
        for(;d1!=d2;d1=d1.addDays(1)){
            if(d1.dayOfWeek()==7){
                total++;
            }
        }
    }
    file.close();

    QFile file2(QCoreApplication::applicationDirPath()+"/config/change");
    file2.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in2(&file2);
    int c=in2.readAll().toInt();
    file2.close();
    total+=c;
    return total;
}

void turnOn(bool is){
    QString name = QCoreApplication::applicationName();
    QString path = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    if(is){
        settings.setValue(name, path);
    }else{
        settings.remove(name);
    }
}

void MainWindow::SetSchedule(const QString &text){
    QString name = QObject::sender()->objectName();

    QString path=QCoreApplication::applicationDirPath()+"/config/";
    path+=name[1];
    QDir p;
    p.mkdir(path);
    path+="/";
    path+=name[2];

    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out<<text;
        file.close();
    }
}


void SetCheckboxState(QFile& file,QCheckBox *cb){
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    cb->setChecked(content.toInt());
}

void MainWindow::showWeek(){
    QLabel *sW = ui->week;
    if(weekNow(ui->dateEdit)!=-1){
        sW->setText("今天是开学第"+QString::number(weekNow(ui->dateEdit))+"周");
    }else{
        sW->setText("今天还未开学");
    }
}

void MainWindow::setLineEdit(QLineEdit *l){
    QString name= l->objectName();
    QString path = QCoreApplication::applicationDirPath()+"/config/"+name[1]+"/"+name[2];
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream text(&file);
    l->setText(text.readAll());
    file.close();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置背景色
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window,QColor("#F0F0F4"));
    this->setPalette(pal);
    this->setAutoFillBackground(1);

    //创建config文件夹
    QDir p;
    p.mkdir(QCoreApplication::applicationDirPath()+"/config");

    QFile file(QCoreApplication::applicationDirPath()+"/config/TurnOn");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in1(&file);
    int c=in1.readAll().toInt();
    turnOn(c);
    file.close();

    QCheckBox *turnOn = ui->TurnOn;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/TurnOn");
    SetCheckboxState(file,turnOn);

    ui->aboutButton->setStyleSheet("QPushButton:hover{background:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FF8E55, stop:1 #FF4526);border-radius:4px;border:none} QPushButton{background-color:#FFFFFF;border-radius:4px;border:none;padding: 4px 8px;}");

    QComboBox *day = ui->day;
    if(MainWindow::goodDay()==QDate::currentDate().dayOfWeek()){
        day->setCurrentText("自动");
    }else{
        day->setCurrentIndex(goodDay());
    }

    QDateEdit *StartDate = ui->dateEdit;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/startTerm");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){StartDate->setDate(QDate::fromString(QTextStream(&file).readAll().trimmed(),Qt::ISODate));}

    QSpinBox *w=ui->spinBox;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/change");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){w->setValue(QTextStream(&file).readAll().trimmed().toInt());}

    for(QLineEdit *edit : findChildren<QLineEdit*>()){
        if(edit->objectName()=="qt_spinbox_lineedit"){
            continue;
        }
        connect(edit,&QLineEdit::textEdited,this,&MainWindow::SetSchedule);
        setLineEdit(edit);
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

//关于
void MainWindow::on_aboutButton_clicked()
{
    QMessageBox::information(this,"关于","作者：ooorange\n希望对班级课表有帮助(゜-゜)つロ 干杯~\n版本:Release1.1.8\n更新日志:1.代码史诗级优化，减少350行多余代码\n2.优化了窗口边框\n3.优化了部分文字严谨性\n4.优化按钮样式\n5.所有窗口都可自由缩放！");
}


void MainWindow::on_TurnOn_clicked(bool checked)
{
    QFile file(QCoreApplication::applicationDirPath()+"/config/TurnOn");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        if(checked){
            out << 1;
            turnOn(1);
        }else{
            out << 0;
            turnOn(0);
        }
        file.close();
        QMessageBox::information(this,"设置成功","设置成功，重启程序生效");
    }
    else{
        QMessageBox::warning(this,"错误","设置失败，请检查程序所在的驱动器是否有充足的空间后重试");
    }
}


void MainWindow::on_checkBox_clicked(bool checked)
{
    QFile file(QCoreApplication::applicationDirPath()+"/config/blur");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        if(checked){
            out << 1;
        }else{
            out << 0;
        }
        file.close();
    }
    else{
        QMessageBox::warning(this,"错误","设置失败，请检查程序所在的驱动器是否有充足的空间后重试");
    }
}




void MainWindow::on_day_textActivated(const QString &arg1)
{
    QFile file1(QCoreApplication::applicationDirPath()+"/config/day");
    QFile file2(QCoreApplication::applicationDirPath()+"/config/dayCheck");
    QDate currentDate = QDate::currentDate();
    if (file1.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&file1);
        out<<arg1;
        file1.close();
        if (file2.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file2);
            out<<currentDate.toString("yyyyMMdd");
            file2.close();
        }
        else{
            QMessageBox::warning(this,"错误","设置失败，请检查程序所在的驱动器是否有充足的空间后重试");
        }
    }
    else{
        QMessageBox::warning(this,"错误","设置失败，请检查程序所在的驱动器是否有充足的空间后重试");
    }
    m_s->updateLabel();
}


void MainWindow::on_dateEdit_userDateChanged(const QDate &date)
{
    QFile file(QCoreApplication::applicationDirPath()+"/config/startTerm");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&file);
        out<<date.toString(Qt::ISODate);
        file.close();
    }else{
        QMessageBox::warning(this,"俺寻思不对啊","设置失败，请检查程序所在驱动器是否有足够的存储空间或是否处于系统文件夹等无权限访问文件夹内");
    }
    showWeek();
}


void MainWindow::on_spinBox_textChanged(const QString &arg1)
{
    QFile file(QCoreApplication::applicationDirPath()+"/config/change");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&file);
        out<<arg1;
        file.close();
    }else{
        QMessageBox::warning(this,"---放大一点 ---嗯对","设置失败，请检查程序所在驱动器是否有足够的存储空间或是否处于系统文件夹等无权限访问文件夹内");
    }
    showWeek();
}

