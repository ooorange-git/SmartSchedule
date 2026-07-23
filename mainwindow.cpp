#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "schedule.h"
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
#include <windows.h>
#include <dwmapi.h>

//==============乱七八糟业务逻辑===========================

void MainWindow::writeFile(QString path,QString content){
    QFile file(QCoreApplication::applicationDirPath()+path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out<<content;
    file.close();
}
//格式“/xxx/xx”
QString MainWindow::readFile(QString path){
    QFile file(QCoreApplication::applicationDirPath()+path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream out(&file);
    QString content = out.readAll();
    file.close();
    return content.trimmed();
}

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
    file.close();
    file2.close();
    if(dayInFile=="自动"){
        return date.dayOfWeek();
    }else{
        if(cD==checkInFile){
            return dayInFile.toInt();
        }else{
            return date.dayOfWeek();
        }
    }
}

//该函数能返回精确周数含用户手动加减！！！
int MainWindow::weekNow(){
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
    }else{
        QFile file2(QCoreApplication::applicationDirPath()+"/config/change");
        if(file2.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in2(&file2);
            int c=in2.readAll().toInt();
            file2.close();
            return 1387+c;
        }else{
            return 1387;
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

void MainWindow::setCurrentSchedule(const QString &text){
    QString name = QObject::sender()->objectName();
    QString path=QCoreApplication::applicationDirPath()+"/config/current";
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
        if(weekNow()==-1){
            ui->tabWidget->setTabToolTip(1,"本周或设置周数内换课后的实际课程表，每周结束自动恢复为标准课程表，当前未开学，设置无效");
        }else{
            if(isSame){
                ui->tabWidget->setTabToolTip(1,"本周或设置周数内换课后的课表，每周结束自动恢复为标准课程表，目前与标准课表相同");
            }else{
                if(currentScheduleWeekChange==0){
                    ui->tabWidget->setTabToolTip(1,"本周(第"+QString::number(weekNow())+"周)换课后的实际课程表,每周结束自动恢复为标准课程表");
                }else{
                    ui->tabWidget->setTabToolTip(1,"设置周数内(第"+QString::number(weekNow())+"~"+QString::number(weekNow()+currentScheduleWeekChange)+"周)换课后的实际课程表,每周结束自动恢复为标准课程表");
                }
            }
        }
    }
}

void MainWindow::recordCurrentScheduleWeek(){
    writeFile("/config/currentScheduleWeek",QString::number(weekNow())+"+"+QString::number(currentScheduleWeekChange));
    if(weekNow()!=-1){
        if(currentScheduleWeekChange==0){
            ui->tabWidget->setTabToolTip(1,"本周(第"+QString::number(weekNow())+"周)换课后的实际课程表,每周结束自动恢复为标准课程表");
        }else{
            ui->tabWidget->setTabToolTip(1,"设置周数内(第"+QString::number(weekNow())+"~"+QString::number(weekNow()+currentScheduleWeekChange)+"周)换课后的实际课程表,每周结束自动恢复为标准课程表");
        }
    }else{
        ui->tabWidget->setTabToolTip(1,"本周或设置周数内换课后的实际课程表,每周结束自动恢复为标准课程表,当前未开学,设置无效");
    }
}

void MainWindow::showWeek(){
    QLabel *sW = ui->week;
    if(weekNow()!=-1){
        sW->setText("今天是开学第"+QString::number(weekNow())+"周");
    }else{
        sW->setText("今天还未开学");
    }
}

void MainWindow::setLineEdit(QLineEdit *l){
    QString name= l->objectName();
    QString path = "/config/"+name[1]+"/"+name[2];
    l->setText(readFile(path));
}

void MainWindow::setCurrentLineEdit(QLineEdit *l){
    QString name=l->objectName();
    QString path = "/config/current"+name[1]+"/"+name[2];
    l->setText(readFile(path));
}

void MainWindow::updateInfo(){
    showWeek();
    setCurrentScheduleToolTip();
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
        if(QSysInfo::productVersion() == "6.1"){
            setAttribute(Qt::WA_NoSystemBackground);
            HWND hwnd = reinterpret_cast<HWND>(this->winId());
            MARGINS margins = {-1,-1,-1,-1};
            DwmExtendFrameIntoClientArea(hwnd, &margins);
        }
    }else{
        QPalette pal = this->palette();
        pal.setColor(QPalette::Window,QColor(0xF0F0F4));
        this->setPalette(pal);
        this->setAutoFillBackground(1);
    }

    currentScheduleWeekChange=readFile("/config/currentScheduleWeek").section("+",1,1).toInt();
    ui->aboutButton->setFixedWidth(120);
    turnOn(readFile("/config/TurnOn").toInt());

    QCheckBox *turnOn = ui->TurnOn;
    turnOn->setChecked(readFile("/config/TurnOn").toInt());

    ui->tabWidget->setTabText(0,"标准课程表");
    ui->tabWidget->setTabToolTip(0,"学校标准的课程表，可同步至当前课程表");
    ui->tabWidget->setTabText(1,"当前课程表");

    ui->checkBox->setChecked(readFile("/config/blur").toInt());

    ui->aboutButton->setStyleSheet("QPushButton:hover{background:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FF8E55, stop:1 #FF4526);border-radius:4px;border:none} QPushButton{background-color:#FFFFFF;border-radius:4px;border:none;padding: 4px 8px;}");

    QComboBox *day = ui->day;
    if(MainWindow::goodDay()==QDate::currentDate().dayOfWeek()){
        day->setCurrentText("自动");
    }else{
        day->setCurrentIndex(goodDay());
    }

    ui->dateEdit->setDate(QDate::fromString(readFile("/config/startTerm").trimmed(),Qt::ISODate));

    ui->spinBox->setValue(readFile("/config/change").trimmed().toInt());

    ui->checkBox_2->setChecked(readFile("/config/useWallpaper").toInt());

    ui->ifEmpty->setCurrentIndex(readFile("/config/ifEmpty").toInt());

    const auto stdChildren = ui->tabWidget->widget(0)->findChildren<QLineEdit*>();
    for(QLineEdit *edit : stdChildren){
        connect(edit,&QLineEdit::textEdited,this,&MainWindow::setSchedule);
        setLineEdit(edit);
    }

    const auto currentChildren = ui->tabWidget->widget(1)->findChildren<QLineEdit*>();
    bool acSetting = 1;//当前课表表是否过期
    if(QDate::currentDate().year()>=2026){//当前时间是否可靠
        QString csw=readFile("/config/currentScheduleWeek");
        if((csw.toInt()<=weekNow()) && (weekNow()<=(csw.toInt()+csw.section("+",1,1).toInt()))){
            acSetting=1;
        }else{
            acSetting=0;
            for(QLineEdit *edit2 : currentChildren){
                edit2->setText(ui->tabWidget->widget(0)->findChild<QLineEdit*>(edit2->objectName().left(3))->text());
            }
        }
    }

    for(QLineEdit *edit3 : currentChildren){
        connect(edit3,&QLineEdit::textChanged,this,&MainWindow::setCurrentSchedule);
        connect(edit3,&QLineEdit::textEdited,this,&MainWindow::recordCurrentScheduleWeek);
        if(acSetting){
            setCurrentLineEdit(edit3);
        }
    }

    setCurrentScheduleToolTip();
    showWeek();
    this->update();

    QTimer *timer_update = new QTimer(this);
    connect(timer_update,&QTimer::timeout,this,&MainWindow::updateInfo);
    timer_update->start(6000);
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
    aboutBox.setText("作者：ooorange\n希望对班级课表有帮助(゜-゜)つロ 干杯~\n版本:Release1.2\n更新日志:1.修复了Win7无法显示设置窗口毛玻璃的问题\n2.优化了设置窗口启动慢的问题\n3.实装壁纸模糊\n4.修复了Beta1.2版本启动位置错误问题\n5.修复了关闭按钮位置错误问题\n6.提升窗口更新频率\n7.吃了一些巧乐兹\n本程序已在Github开源：访问仓库：\nhttps://github.com/ooorange-git/SmartSchedule");
    aboutBox.setStandardButtons(QMessageBox::Ok);
    aboutBox.exec();
}


void MainWindow::on_TurnOn_clicked(bool checked)
{
    turnOn(checked);
    QFile file(QCoreApplication::applicationDirPath()+"/config/TurnOn");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        if(checked){
            out << 1;
        }else{
            out << 0;
        }
        file.close();
    }else{
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
    }else{
        QMessageBox::warning(this,"错误","设置失败，请检查程序所在的驱动器是否有充足的空间后重试");
    }

    if(QMessageBox::question(this,"Tip:如长期未看到新窗口请手动重启","设置成功，重启程序后生效,是否立即重启？")==QMessageBox::Yes){
        restart();
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


void MainWindow::on_checkBox_2_clicked(bool checked)
{
    QFile file(QCoreApplication::applicationDirPath()+"/config/useWallpaper");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&file);
        if(checked){
            out<<1;
        }else{
            out<<0;
        }
        file.close();
    }else{
        QMessageBox::warning(this,"---放大一点 ---嗯对","设置失败，请检查程序所在驱动器是否有足够的存储空间或是否处于系统文件夹等无权限访问文件夹内");
    }
    if(QMessageBox::question(this,"Tip:如长期未看到新窗口请手动重启","设置成功，重启程序后生效,是否立即重启？")==QMessageBox::Yes){
        restart();
    }
}


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
            }
            ui->tabWidget->setTabToolTip(1,"本周或设置周数内换课后的课表，目前与标准课表相同");
        }
    }else{
        for(QLineEdit *edit2 : current){
            edit2->setText(ui->tabWidget->widget(0)->findChild<QLineEdit*>(edit2->objectName().left(3))->text());
            ui->tabWidget->setTabToolTip(1,"本周或设置周数内换课后的课表，目前与标准课表相同");
        }
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    int number=0;
    const auto children = ui->tabWidget->widget(1)->findChildren<QLineEdit*>();
    for(QLineEdit *edit : children){
        if(!edit->text().isEmpty()){
            number=1;
            break;
        }
    }
    if(number){
        if(QMessageBox::question(this,"提示","是否要清空当前课表？")==QMessageBox::Yes){
            for(QLineEdit *edit : children){
                edit->clear();
            }
            QFile::remove(QCoreApplication::applicationDirPath()+"/config/currentScheduleWeek");
            ui->tabWidget->setTabToolTip(1,"本周或设置周数内换课后的课表，目前什么也没有，将使用标准课表");
        }
    }
}


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
        currentScheduleWeekChange++;
        QMessageBox::information(this,"提示","已延续一周至第"+QString::number(weekNow()+currentScheduleWeekChange)+"周");
        recordCurrentScheduleWeek();
    }else{
        QMessageBox::information(this,"提示","当前课表与标准课表相同或并无内容，设置无效");
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    currentScheduleWeekChange=0;
    recordCurrentScheduleWeek();
    QMessageBox::information(this,"提示","取消成功");
}


void MainWindow::on_ifEmpty_activated(int index)
{
    writeFile("/config/ifEmpty",QString::number(index));
}

