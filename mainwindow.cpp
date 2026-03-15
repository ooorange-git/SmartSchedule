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

void SetSchedule(int a,int b,QString c){
    QString path=QCoreApplication::applicationDirPath()+"/config/";
    path+=QString::fromStdString(std::to_string(a));
    QDir p;
    p.mkdir(path);
    path+="/";
    path+=QString::fromStdString(std::to_string(b));

    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out<<c;
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


void SetLineEditState(QFile& file,QLineEdit *le){
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    le->setText(content);
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette pal = this->palette();
    pal.setColor(QPalette::Window,QColor("#F0F0F4"));
    this->setPalette(pal);
    this->setAutoFillBackground(1);

    QDir p;
    p.mkdir(QCoreApplication::applicationDirPath()+"/config");
    qDebug()<<QCoreApplication::applicationDirPath();

    QFile file(QCoreApplication::applicationDirPath()+"/config/TurnOn");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in1(&file);
    int c=in1.readAll().toInt();
    turnOn(c);
    file.close();

    // QSpinBox *toumindu = ui->spinBox;
    // file.setFileName(QCoreApplication::applicationDirPath()+"/config/toumindu");
    // file.open(QIODevice::ReadOnly | QIODevice::Text);
    // QTextStream in(&file);
    // QString content = in.readAll();
    // file.close();
    // toumindu->setValue(content.toInt());

    QCheckBox *turnOn = ui->TurnOn;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/TurnOn");
    SetCheckboxState(file,turnOn);

    // QCheckBox *RC = ui->RightClockB;
    // file.setFileName(QCoreApplication::applicationDirPath()+"/config/RightClock");
    // SetCheckboxState(file,RC);

    QComboBox *day = ui->day;
    if(MainWindow::goodDay()==QDate::currentDate().dayOfWeek()){
        day->setCurrentText("自动");
    }else{
        day->setCurrentIndex(goodDay());
    }

    QCheckBox *bl = ui->checkBox;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/blur");
    SetCheckboxState(file,bl);
    bl->setVisible(0);
    ui->label->setVisible(0);

    QLineEdit *L11 = ui->L11;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/1/1");
    SetLineEditState(file,L11);

    QLineEdit *L12 = ui->L12;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/1/2");
    SetLineEditState(file,L12);

    QLineEdit *L13 = ui->L13;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/1/3");
    SetLineEditState(file,L13);

    QLineEdit *L14 = ui->L14;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/1/4");
    SetLineEditState(file,L14);

    QLineEdit *L15 = ui->L15;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/1/5");
    SetLineEditState(file,L15);

    QLineEdit *L16 = ui->L16;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/1/6");
    SetLineEditState(file,L16);

    QLineEdit *L17 = ui->L17;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/1/7");
    SetLineEditState(file,L17);

    QLineEdit *L18 = ui->L18;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/1/8");
    SetLineEditState(file,L18);

    QLineEdit *L21 = ui->L21;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/2/1");
    SetLineEditState(file,L21);

    QLineEdit *L22 = ui->L22;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/2/2");
    SetLineEditState(file,L22);

    QLineEdit *L23 = ui->L23;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/2/3");
    SetLineEditState(file,L23);

    QLineEdit *L24 = ui->L24;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/2/4");
    SetLineEditState(file,L24);

    QLineEdit *L25 = ui->L25;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/2/5");
    SetLineEditState(file,L25);

    QLineEdit *L26 = ui->L26;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/2/6");
    SetLineEditState(file,L26);

    QLineEdit *L27 = ui->L27;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/2/7");
    SetLineEditState(file,L27);

    QLineEdit *L28 = ui->L28;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/2/8");
    SetLineEditState(file,L28);

    QLineEdit *L31 = ui->L31;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/3/1");
    SetLineEditState(file,L31);

    QLineEdit *L32 = ui->L32;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/3/2");
    SetLineEditState(file,L32);

    QLineEdit *L33 = ui->L33;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/3/3");
    SetLineEditState(file,L33);

    QLineEdit *L34 = ui->L34;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/3/4");
    SetLineEditState(file,L34);

    QLineEdit *L35 = ui->L35;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/3/5");
    SetLineEditState(file,L35);

    QLineEdit *L36 = ui->L36;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/3/6");
    SetLineEditState(file,L36);

    QLineEdit *L37 = ui->L37;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/3/7");
    SetLineEditState(file,L37);

    QLineEdit *L38 = ui->L38;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/3/8");
    SetLineEditState(file,L38);

    QLineEdit *L41 = ui->L41;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/4/1");
    SetLineEditState(file,L41);

    QLineEdit *L42 = ui->L42;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/4/2");
    SetLineEditState(file,L42);

    QLineEdit *L43 = ui->L43;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/4/3");
    SetLineEditState(file,L43);

    QLineEdit *L44 = ui->L44;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/4/4");
    SetLineEditState(file,L44);

    QLineEdit *L45 = ui->L45;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/4/5");
    SetLineEditState(file,L45);

    QLineEdit *L46 = ui->L46;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/4/6");
    SetLineEditState(file,L46);

    QLineEdit *L47 = ui->L47;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/4/7");
    SetLineEditState(file,L47);

    QLineEdit *L48 = ui->L48;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/4/8");
    SetLineEditState(file,L48);

    QLineEdit *L51 = ui->L51;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/5/1");
    SetLineEditState(file,L51);

    QLineEdit *L52 = ui->L52;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/5/2");
    SetLineEditState(file,L52);

    QLineEdit *L53 = ui->L53;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/5/3");
    SetLineEditState(file,L53);

    QLineEdit *L54 = ui->L54;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/5/4");
    SetLineEditState(file,L54);

    QLineEdit *L55 = ui->L55;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/5/5");
    SetLineEditState(file,L55);

    QLineEdit *L56 = ui->L56;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/5/6");
    SetLineEditState(file,L56);

    QLineEdit *L57 = ui->L57;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/5/7");
    SetLineEditState(file,L57);

    QLineEdit *L58 = ui->L58;
    file.setFileName(QCoreApplication::applicationDirPath()+"/config/5/8");
    SetLineEditState(file,L58);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_aboutButton_clicked()
{
    QMessageBox::information(this,"关于","作者：ooorange\n希望对班级课表有帮助\n版本:Relese1.1.2\n更新日志:1.修复了开机自启动时显示空白窗口的问题\n2.优化了窗口背景及字体显示\n3.调整了部分课程位置和文案准确性\n4.优化代码逻辑\n5.删除了两个没用的控件并增加了一个没用的控件\n6.移除了him");
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


// void MainWindow::on_RightClockB_clicked(bool checked)
// {
//     QFile file(QCoreApplication::applicationDirPath()+"/config/RightClock");
//     if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//         QTextStream out(&file);
//         if(checked){
//             out << 1;
//         }else{
//             out << 0;
//         }
//         file.close();
//     }
//     else{
//         QMessageBox::warning(this,"错误","设置失败，请检查程序所在的驱动器是否有充足的空间后重试");
//     }
// }


// void MainWindow::on_spinBox_valueChanged(int arg1)
// {
//     QFile file(QCoreApplication::applicationDirPath()+"/config/toumindu");
//     if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//         QTextStream out(&file);
//         out << arg1;
//         file.close();
//     }
//     else{
//         QMessageBox::warning(this,"错误","设置失败，请检查程序所在的驱动器是否有充足的空间后重试");
//     }
// }


void MainWindow::on_L11_textEdited(const QString &arg1)
{
    SetSchedule(1,1,arg1);
}


void MainWindow::on_L12_textEdited(const QString &arg1)
{
    SetSchedule(1,2,arg1);
}


void MainWindow::on_L13_textEdited(const QString &arg1)
{
    SetSchedule(1,3,arg1);
}


void MainWindow::on_L14_textEdited(const QString &arg1)
{
    SetSchedule(1,4,arg1);
}


void MainWindow::on_L15_textEdited(const QString &arg1)
{
    SetSchedule(1,5,arg1);
}


void MainWindow::on_L16_textEdited(const QString &arg1)
{
    SetSchedule(1,6,arg1);
}


void MainWindow::on_L17_textEdited(const QString &arg1)
{
    SetSchedule(1,7,arg1);
}


void MainWindow::on_L18_textEdited(const QString &arg1)
{
    SetSchedule(1,8,arg1);
}

void MainWindow::on_L21_textEdited(const QString &arg1)
{
    SetSchedule(2,1,arg1);
}


void MainWindow::on_L22_textEdited(const QString &arg1)
{
    SetSchedule(2,2,arg1);
}


void MainWindow::on_L23_textEdited(const QString &arg1)
{
    SetSchedule(2,3,arg1);
}


void MainWindow::on_L24_textEdited(const QString &arg1)
{
    SetSchedule(2,4,arg1);
}


void MainWindow::on_L25_textEdited(const QString &arg1)
{
    SetSchedule(2,5,arg1);
}


void MainWindow::on_L26_textEdited(const QString &arg1)
{
    SetSchedule(2,6,arg1);
}


void MainWindow::on_L27_textEdited(const QString &arg1)
{
    SetSchedule(2,7,arg1);
}


void MainWindow::on_L28_textEdited(const QString &arg1)
{
    SetSchedule(2,8,arg1);
}


void MainWindow::on_L31_textEdited(const QString &arg1)
{
    SetSchedule(3,1,arg1);
}


void MainWindow::on_L32_textEdited(const QString &arg1)
{
    SetSchedule(3,2,arg1);
}


void MainWindow::on_L33_textEdited(const QString &arg1)
{
    SetSchedule(3,3,arg1);
}


void MainWindow::on_L34_textEdited(const QString &arg1)
{
    SetSchedule(3,4,arg1);
}


void MainWindow::on_L35_textEdited(const QString &arg1)
{
    SetSchedule(3,5,arg1);
}


void MainWindow::on_L36_textEdited(const QString &arg1)
{
    SetSchedule(3,6,arg1);
}


void MainWindow::on_L37_textEdited(const QString &arg1)
{
    SetSchedule(3,7,arg1);
}


void MainWindow::on_L38_textEdited(const QString &arg1)
{
    SetSchedule(3,8,arg1);
}


void MainWindow::on_L41_textEdited(const QString &arg1)
{
    SetSchedule(4,1,arg1);
}


void MainWindow::on_L42_textEdited(const QString &arg1)
{
    SetSchedule(4,2,arg1);
}


void MainWindow::on_L43_textEdited(const QString &arg1)
{
    SetSchedule(4,3,arg1);
}


void MainWindow::on_L44_textEdited(const QString &arg1)
{
    SetSchedule(4,4,arg1);
}


void MainWindow::on_L45_textEdited(const QString &arg1)
{
    SetSchedule(4,5,arg1);
}


void MainWindow::on_L46_textEdited(const QString &arg1)
{
    SetSchedule(4,6,arg1);
}


void MainWindow::on_L47_textEdited(const QString &arg1)
{
    SetSchedule(4,7,arg1);
}


void MainWindow::on_L48_textEdited(const QString &arg1)
{
    SetSchedule(4,8,arg1);
}


void MainWindow::on_L51_textEdited(const QString &arg1)
{
    SetSchedule(5,1,arg1);
}


void MainWindow::on_L52_textEdited(const QString &arg1)
{
    SetSchedule(5,2,arg1);
}


void MainWindow::on_L53_textEdited(const QString &arg1)
{
    SetSchedule(5,3,arg1);
}


void MainWindow::on_L54_textEdited(const QString &arg1)
{
    SetSchedule(5,4,arg1);
}


void MainWindow::on_L55_textEdited(const QString &arg1)
{
    SetSchedule(5,5,arg1);
}


void MainWindow::on_L56_textEdited(const QString &arg1)
{
    SetSchedule(5,6,arg1);
}


void MainWindow::on_L57_textEdited(const QString &arg1)
{
    SetSchedule(5,7,arg1);
}


void MainWindow::on_L58_textEdited(const QString &arg1)
{
    SetSchedule(5,8,arg1);
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

