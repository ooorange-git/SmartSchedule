#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "schedule.h"
#include "clock.h"

#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QSpinBox>
#include <QCoreApplication>
#include <QSettings>
#include <QDir>
#include <QScreen>
#include <QRect>
#include <QLineEdit>
#include <QDate>
#include <QProcess>
#include <physics.h>
#include <systemapi.h>

using namespace SystemAPI;

//==============业务逻辑=================

void MainWindow::restart(){
    if (socket) {
        socket->close();
        QLocalServer::removeServer("orange's schedule");
        socket = nullptr;
    }
    QString program = QCoreApplication::applicationFilePath();
    QStringList arguments = QCoreApplication::arguments();
    QString workingPath = QDir::currentPath();
    bool start = QProcess::startDetached(program,arguments,workingPath);
    if(start){
        qApp->quit();
    }else{
        QMessageBox::critical(this,"……","重启似乎失败了，请手动重启");
    }
}

int MainWindow::currentScheduleWeekChange(){
    return readFile("/config/currentScheduleWeek").section("+",1,1).toInt();
}

void MainWindow::turnOn(bool is){
    QString name = QCoreApplication::applicationName();
    QString path = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    if(is){
        settings.setValue(name, path);
    }else{
        settings.remove(name);
    }
}

void MainWindow::setSchedule(const QString &text){
    QString name = QObject::sender()->objectName();
    QString path="/config/";
    path+=name[1];
    QDir p;
    p.mkdir(path);
    path+="/";
    path+=name[2];
    writeFile(path,text);
    if(inited){
       m_s->updateLabel();
    }
}

void MainWindow::setCurrentSchedule(const QString &text){
    if(is_cleaning){
        return;
    }else{
        QString name = QObject::sender()->objectName();
        QString path="/config/current";
        path+=name[1];
        QDir p;
        p.mkdir(QCoreApplication::applicationDirPath()+path);
        path+="/";
        path+=name[2];
        writeFile(path,text);
    }

}

void MainWindow::setCurrentScheduleToolTip(){
    int isEmpty=1;
    int isSame=1;
    const auto children = ui->tabWidget->widget(1)->findChildren<QLineEdit*>();
    for(QLineEdit *edit : children){
        if(!edit->text().isEmpty()){
            isEmpty=0;
            break;
        }
    }

    if(!isEmpty){
        for(QLineEdit *edit : children){
            if(edit->text()!= ui->tabWidget->widget(0)->findChild<QLineEdit*>(edit->objectName().left(3))->text()){//判断两个课表是否内容相同
                isSame=0;
                break;
            }
        }
    }

    if(isEmpty){
        ui->tabWidget->setTabToolTip(1,"本周或设置周数内换课后的课表，每周结束自动恢复为标准课程表，目前什么也没有，将使用标准课表");
    }else{
        int week = Clock::weekNow();
        if(week==-1){
            ui->tabWidget->setTabToolTip(1,"本周或设置周数内换课后的实际课程表，每周结束自动恢复为标准课程表，当前未开学，设置无效");
        }else{
            if(isSame){
                ui->tabWidget->setTabToolTip(1,"本周或设置周数内换课后的课表，每周结束自动恢复为标准课程表，目前与标准课表相同");
            }else{
                if(currentScheduleWeekChange()==0){
                    ui->tabWidget->setTabToolTip(1,"本周(第"+QString::number(week)+"周)换课后的实际课程表,每周结束自动恢复为标准课程表");
                }else{
                    QString fileContent = readFile("/config/currentScheduleWeek");
                    QString stdWeek = fileContent.section("+",0,0);
                    QString change = fileContent.section("+",1,1);
                    int right = stdWeek.toInt()+change.toInt();
                    ui->tabWidget->setTabToolTip(1,"设置周数内(第"+stdWeek+"~"+QString::number(right)+"周)换课后的实际课程表,每周结束自动恢复为标准课程表");
                }
            }
        }
    }
}


int MainWindow::recordCurrentScheduleWeek(int changeWeek){
    QString fileContent = readFile("/config/currentScheduleWeek");
    QString stdWeek = fileContent.section("+",0,0);
    QString change = fileContent.section("+",1,1);
    int left = stdWeek.toInt();
    int right = stdWeek.toInt()+change.toInt();
    int week = Clock::weekNow();
    if(week<0){
        setCurrentScheduleToolTip();
        return 0;
    }
    if(!(week<=right && week>=left)){//过期
        writeFile("/config/currentScheduleWeek",QString::number(week)+"+"+QString::number(changeWeek+change.toInt()));
    }else{
        if(changeWeek==-1){
            writeFile("/config/currentScheduleWeek",QString::number(week)+"+0");
        }else{
            writeFile("/config/currentScheduleWeek",stdWeek+"+"+QString::number(changeWeek+change.toInt()));
        }
    }
    setCurrentScheduleToolTip();
    QTimer::singleShot(50,this,[this](){m_s->updateLabel();});
    return right+changeWeek;
}

void MainWindow::showWeek(){
    QLabel *sW = ui->week;
    int week = Clock::weekNow();
    if(week==-1){
        sW->setText("今天还未开学");
    }else{
        sW->setText("今天是开学第"+QString::number(week)+"周");
    }
}

void MainWindow::setLineEdit(QLineEdit *l){
    QString name= l->objectName();
    QString path = "/config/"+name[1]+"/"+name[2];
    l->setText(readFile(path));
}

void MainWindow::setCurrentLineEdit(QLineEdit *l){
    QString name = l->objectName();
    QString path = "/config/current"+name[1]+"/"+name[2];
    bool ok;
    QString content = readFile(path,&ok);
    if(ok){
       l->setText(readFile(path));
    }else{
        setLineEdit(l);
    }

}

void MainWindow::updateInfo(){
    showWeek();
    setCurrentScheduleToolTip();
}

void MainWindow::deleteOutOfDate(int week,int day){
    QFile::remove(QCoreApplication::applicationDirPath()+"/config/current"+QString::number(week)+"/"+QString::number(day));
}

void MainWindow::aboutButtonMenu(const QPoint &pos){
    QPoint p = ui->aboutButton->mapToGlobal(pos);
    QMenu m;
    QAction *a1 = m.addAction("千万别点");
    connect(a1,&QAction::triggered,this,[this]{m_s->usePhy();});
    m.exec(p);
}


//=====================构造/析构==================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qApp->setStyleSheet("QMessageBox { messagebox-text-interaction-flags: 5; }");
    ui->setupUi(this);
    QDir p;
    p.mkdir(QCoreApplication::applicationDirPath()+"/config");

    //设置背景色
    if(readFile("/config/blur").toInt()){
        if(QSysInfo::productVersion() == "6.1" && !readFile("/unable.txt").toInt()){
            setAttribute(Qt::WA_NoSystemBackground);
            setAttribute(Qt::WA_TranslucentBackground);
            SystemAPI::extendFrameIntoClientArea(this);
        }
    }else{
        QPalette pal = this->palette();
        pal.setColor(QPalette::Window,QColor(0xF0F0F4));
        this->setPalette(pal);
        this->setAutoFillBackground(1);
    }

    ui->tabWidget->setCurrentIndex(1);

    ui->aboutButton->setFixedWidth(120);
    turnOn(readFile("/config/TurnOn").toInt());

    QCheckBox *turnOn = ui->TurnOn;
    turnOn->setChecked(readFile("/config/TurnOn").toInt());

    ui->tabWidget->setTabText(0,"标准课程表");
    ui->tabWidget->setTabToolTip(0,"学校标准的课程表，可同步至当前课程表");
    ui->tabWidget->setTabText(1,"当前课程表");

    ui->checkBox->setChecked(readFile("/config/blur").toInt());
    ui->checkBox_2->setEnabled(ui->checkBox->isChecked());

    ui->aboutButton->setStyleSheet("QPushButton:hover{background:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FF8E55, stop:1 #FF4526);border-radius:4px;border:none} QPushButton{background-color:#FFFFFF;border-radius:4px;border:none;padding: 4px 8px;}");

    QComboBox *day = ui->day;
    if(Clock::goodDay()==QDate::currentDate().dayOfWeek()){
        day->setCurrentText("自动");
    }else{
        day->setCurrentIndex(Clock::goodDay());
    }

    ui->dateEdit->setDate(QDate::fromString(readFile("/config/startTerm").trimmed(),Qt::ISODate));

    ui->spinBox->setValue(readFile("/config/change").trimmed().toInt());

    ui->checkBox_2->setChecked(readFile("/config/useWallpaper").toInt());

    const auto stdChildren = ui->tabWidget->widget(0)->findChildren<QLineEdit*>();
    for(QLineEdit *edit : stdChildren){
        connect(edit,&QLineEdit::textEdited,this,&MainWindow::setSchedule);
        setLineEdit(edit);
    }

    const auto currentChildren = ui->tabWidget->widget(1)->findChildren<QLineEdit*>();
    bool acSetting = 1;//当前课表是否过期
    if(Clock::timeTrusted()){//当前时间是否可靠
        QString csw=readFile("/config/currentScheduleWeek");
        int fCsw = csw.section("+",0,0).toInt();
        int bCsw = csw.section("+",1,1).toInt();
        if((fCsw<=Clock::weekNow()) && (Clock::weekNow()<=fCsw+bCsw)){
            acSetting=1;
        }else{
            acSetting=0;
            for(QLineEdit *edit2 : currentChildren){
                deleteOutOfDate(edit2->objectName()[1].digitValue(),edit2->objectName()[2].digitValue());
                edit2->setText(ui->tabWidget->widget(0)->findChild<QLineEdit*>(edit2->objectName().left(3))->text());
            }
        }
    }

    for(QLineEdit *edit3 : currentChildren){
        connect(edit3,&QLineEdit::textChanged,this,&MainWindow::setCurrentSchedule);
        connect(edit3,&QLineEdit::textEdited,this,[this]() {
            recordCurrentScheduleWeek();
        });
        if(acSetting){
            setCurrentLineEdit(edit3);
        }
    }

    ui->aboutButton->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->aboutButton,&QPushButton::customContextMenuRequested,this,&MainWindow::aboutButtonMenu);

    setCurrentScheduleToolTip();
    showWeek();
    this->update();

    inited = 1;
    QTimer *timer_update = new QTimer(this);
    connect(timer_update,&QTimer::timeout,this,&MainWindow::updateInfo);
    timer_update->start(6000);

    if(!Clock::timeTrusted()){
        QTimer *timer_del = new QTimer(this);
        connect(timer_del,&QTimer::timeout,this,[this,timer_del](){
            const auto currentChildren = ui->tabWidget->widget(1)->findChildren<QLineEdit*>();
            bool acSetting = 1;//当前课表是否过期
            if(Clock::timeTrusted()){//当前时间是否可靠
                QString csw=readFile("/config/currentScheduleWeek");
                int fCsw = csw.section("+",0,0).toInt();
                int bCsw = csw.section("+",1,1).toInt();
                is_cleaning = 1;
                if((fCsw<=Clock::weekNow()) && (Clock::weekNow()<=fCsw+bCsw)){
                    acSetting=1;
                }else{
                    acSetting=0;
                    for(QLineEdit *edit2 : currentChildren){
                        deleteOutOfDate(edit2->objectName()[1].digitValue(),edit2->objectName()[2].digitValue());
                        edit2->setText(ui->tabWidget->widget(0)->findChild<QLineEdit*>(edit2->objectName().left(3))->text());
                    }
                    QFile::remove(QCoreApplication::applicationDirPath()+"/config/currentScheduleWeek");
                }
                for(QLineEdit *edit3 : currentChildren){
                    if(acSetting){
                        setCurrentLineEdit(edit3);
                    }
                }
                is_cleaning = 0;
                timer_del->deleteLater();
            }
        });

        timer_del->start(5000);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

//========================槽函数===================================

//关于
void MainWindow::on_aboutButton_clicked()
{
    QMessageBox aboutBox(this);
    aboutBox.setWindowIcon(m_s->windowIcon());
    aboutBox.setIconPixmap(m_s->windowIcon().pixmap(72,72));
    aboutBox.setWindowTitle("关于");
    aboutBox.setText("作者：ooorange\n希望对班级课表有帮助(゜-゜)つロ 干杯~\n版本:Beta 1.5\n更新日志:1.封装了部分函数，提高代码可读性\n2.修复了无法删除过期课程表的问题\n下期预告:应用内更新，自动下载，无需手动替换exe！\n本程序已在Github开源：访问仓库：\nhttps://github.com/ooorange-git/SmartSchedule");
    aboutBox.setStandardButtons(QMessageBox::Ok);
    aboutBox.exec();
}


void MainWindow::on_TurnOn_clicked(bool checked)
{
    turnOn(checked);
    writeFile("/config/TurnOn",QString::number(checked));
}

//今天上周？课槽函数
void MainWindow::on_day_textActivated(const QString &arg1)
{
    QDate currentDate = QDate::currentDate();
    if(writeFile("/config/day",arg1)){
        writeFile("/config/dayCheck",currentDate.toString("yyyyMMdd"));
    }
    m_s->updateLabel();
}


void MainWindow::on_dateEdit_userDateChanged(const QDate &date)
{
    writeFile("/config/startTerm",date.toString(Qt::ISODate));
    showWeek();
}


void MainWindow::on_spinBox_textChanged(const QString &arg1)
{
    writeFile("/config/change",arg1);
    showWeek();
}


void MainWindow::on_checkBox_clicked(bool checked)
{
    ui->checkBox_2->setEnabled(checked);
    writeFile("/config/blur",QString::number(checked));

    if(QMessageBox::question(this,"Tip:如长期未看到新窗口请手动重启","设置成功，重启程序后生效,是否立即重启？")==QMessageBox::Yes){
        restart();
    }
}


void MainWindow::on_checkBox_2_clicked(bool checked)
{
    writeFile("/config/useWallpaper",QString::number(checked));

    if(QMessageBox::question(this,"Tip:如长期未看到新窗口请手动重启","设置成功，重启程序后生效,是否立即重启？")==QMessageBox::Yes){
        restart();
    }
}

//同步
void MainWindow::on_pushButton_clicked()
{
    int number=0;
    const auto current = ui->tabWidget->widget(1)->findChildren<QLineEdit*>();
    for(QLineEdit *edit : current){
        if(!edit->text().isEmpty() && edit->text()!= ui->tabWidget->widget(0)->findChild<QLineEdit*>(edit->objectName().left(3))->text()){
            number=1;
            break;
        }
    }

    if(number){
        if(QMessageBox::question(this,"提示","当前课表内已有内容，是否仍要同步？")==QMessageBox::Yes){
            for(QLineEdit *edit2 : current){
                edit2->setText(ui->tabWidget->widget(0)->findChild<QLineEdit*>(edit2->objectName().left(3))->text());
                deleteOutOfDate(edit2->objectName()[1].digitValue(),edit2->objectName()[2].digitValue());
            }
            ui->tabWidget->setTabToolTip(1,"本周或设置周数内换课后的课表，每周结束自动恢复为标准课程表，目前与标准课表相同");
        }
    }else{
        for(QLineEdit *edit2 : current){
            edit2->setText(ui->tabWidget->widget(0)->findChild<QLineEdit*>(edit2->objectName().left(3))->text());
            deleteOutOfDate(edit2->objectName()[1].digitValue(),edit2->objectName()[2].digitValue());
            ui->tabWidget->setTabToolTip(1,"本周或设置周数内换课后的课表，每周结束自动恢复为标准课程表，目前与标准课表相同");
        }
    }
    QFile::remove(QCoreApplication::applicationDirPath()+"/config/currentScheduleWeek");
}



//延续一周
void MainWindow::on_pushButton_3_clicked()
{
    int isEmpty=1;
    int isSame=1;
    const auto children = ui->tabWidget->widget(1)->findChildren<QLineEdit*>();
    for(QLineEdit *edit : children){
        if(!edit->text().isEmpty()){
            isEmpty=0;
            break;
        }
    }

    if(!isEmpty){
        for(QLineEdit *edit : children){
            if(edit->text()!= ui->tabWidget->widget(0)->findChild<QLineEdit*>(edit->objectName().left(3))->text()){//判断两个课表是否内容相同
                isSame=0;
                break;
            }
        }
    }

    if(!(isEmpty || isSame)){
        QMessageBox::information(this,"提示","已延续一周至第"+QString::number(recordCurrentScheduleWeek(1))+"周");
        setCurrentScheduleToolTip();
    }else{
        QMessageBox::information(this,"提示","当前课表与标准课表相同或并无内容，设置无效");
    }
}

//取消延续
void MainWindow::on_pushButton_4_clicked()
{
    recordCurrentScheduleWeek(-1);
    QMessageBox::information(this,"提示","取消成功");
}
