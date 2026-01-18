#include "mainwindow.h"
#include "schedule.h"
#include<QSettings>
#include<QApplication>
#include<QGuiApplication>
#include<QCoreApplication>
#include<QDir>
#include<QFile>
#include<QTextStream>
#include<QScreen>
#include<QRect>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Schedule s;
    s.show();
    w.show();
    return a.exec();
}
