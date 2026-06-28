#include "mainwindow.h"
#include "schedule.h"
#include <QSettings>
#include <QApplication>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QScreen>
#include <QRect>
#include <QFontDatabase>
#include <QDebug>
#include <QLoggingCategory>
#include <QLocalSocket>
#include <QLocalServer>
#include <QTimer>
#include <QLayout>


int main(int argc, char *argv[])
{
    QLocalSocket socket;
    socket.connectToServer("orange's schedule");
    if (socket.waitForConnected(100)) {
        return 0;
    }

    QLocalServer server;
    if (!server.listen("orange's schedule")) {
        QLocalServer::removeServer("orange's schedule");
        server.listen("orange's schedule");
    }

    QApplication a(argc, argv);
    int fontId1 = QFontDatabase::addApplicationFont(":/font/LXGWWenKaiLite-Medium.ttf");
    QStringList fontFamilies1 = QFontDatabase::applicationFontFamilies(fontId1);

    int fontId2 = QFontDatabase::addApplicationFont(":/font/SourceHanSansSC-Regular.otf");
    QStringList fontFamilies2 = QFontDatabase::applicationFontFamilies(fontId2);
    qDebug()<<fontFamilies2.first();
    QFont df(fontFamilies2.first(),10);
    df.setHintingPreference(QFont::PreferNoHinting);
    df.setStyleStrategy(QFont::PreferAntialias);

    a.setFont(df);

    MainWindow w;
    Schedule s;

    s.SetMain(&w);
    s.show();
    w.setSchedule(&s);
    w.socket=&server;

    w.winId();
    w.ensurePolished();
    if(w.layout()){
        w.layout()->activate();
    }

    return a.exec();
}
