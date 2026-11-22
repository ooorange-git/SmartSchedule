#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVersionNumber>
#include <QCryptographicHash>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QEventLoop>
#include <QVersionNumber>
#include <QDebug>
#include <QMessageBox>
#include <QThread>

class Updater:public QThread{

    Q_OBJECT

    signals:

    private:
        QJsonObject obj1;
        QString ver;
        QString currentV = QCoreApplication::applicationVersion();
        bool downloadFile(){
            QString downloadUrl;
            for (const QJsonValue &a : obj1.value("assets").toArray()) {
                QJsonObject o = a.toObject();
                if (o.value("name").toString().endsWith(".exe", Qt::CaseInsensitive)) {
                    downloadUrl = o.value("browser_download_url").toString();
                    break;
                }
            }
            if(downloadUrl.isEmpty()){
                return 0;
            }else{
                QByteArray data = get(QUrl(downloadUrl));
                if (data.isEmpty()) {
                    return 0;
                }
                QFile f(QDir::tempPath()+"/SmartSchedule.exe");
                if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) return 0;
                f.write(data);
                f.close();
                return 1;
            }
        }

        QByteArray get(const QUrl &url){
            QNetworkAccessManager nam;
            QNetworkRequest req(url);
            req.setRawHeader("User-Agent", "SmartSchedule-Updater/1.0");
            req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,QNetworkRequest::NoLessSafeRedirectPolicy);

            QNetworkReply *reply = nam.get(req);
            QEventLoop loop;
            QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();

            if(reply->error()==QNetworkReply::NoError){
                QByteArray data = reply->readAll();
                reply->deleteLater();
                return data;
            }else{
                reply->deleteLater();
                return QByteArray();
            }
        }

    protected:
        explicit Updater(QWidget *p,bool cause){
            QByteArray ba1 = get(QUrl(QString("https://api.github.com/repos/ooorange-git/SmartSchedule/releases/latest")));
            if(!ba1.isEmpty()){
                obj1 = QJsonDocument::fromJson(ba1).object();
                ver = obj1.value("tag_name").toString();
                if(ver.startsWith('v') || ver.startsWith('V')) ver.remove(0, 1);
                if(ver.startsWith("Beta",Qt::CaseInsensitive)) ver.remove(0, 4);
                if(ver.startsWith("Release",Qt::CaseInsensitive)) ver.remove(0, 7);
                if(QVersionNumber::fromString(ver)>QVersionNumber::fromString(currentV)){
                    if(QMessageBox::question(p,"提示","发现新版本，是否更新？")==QMessageBox::Yes){
                        downloadFile();
                    }
                }else{
                    if(cause) QMessageBox::information(p,"提示","未发现新版本");
                }
            }else{
                QMessageBox::critical(p,"错误","网络错误");
            }
        }


};

#endif // UPDATER_H
