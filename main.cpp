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
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QRect screenGeometry = primaryScreen->geometry();
    int x = screenGeometry.width()-155;
    int y = (screenGeometry.height()-419)/2;
    Schedule s;
    s.move(x,y);
    s.SetMain(&w);
    s.show();
    w.setSchedule(&s);
    w.show();
    return a.exec();
}
