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
#include<QFontDatabase>
#include<QDebug>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int fontId1 = QFontDatabase::addApplicationFont(":/font/LXGWWenKaiLite-Medium.ttf");
    if (fontId1 != -1) {
        QStringList fontFamilies1 = QFontDatabase::applicationFontFamilies(fontId1);
    }

    MainWindow w;
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QRect screenGeometry = primaryScreen->geometry();

    Schedule s;
    int x = screenGeometry.width()-s.width();
    int y = (screenGeometry.height()-s.height())/2;
    s.move(x,y);
    s.SetMain(&w);
    s.show();
    w.setSchedule(&s);
    w.show();
    return a.exec();
}
