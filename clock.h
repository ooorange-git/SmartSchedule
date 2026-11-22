#ifndef CLOCK_H
#define CLOCK_H

#include <QFile>
#include <QCoreApplication>
#include <QDate>
#include <QTextStream>
#include "systemapi.h"

namespace Clock{
    //该函数能返回精确周数含用户手动加减！！！
    inline int weekNow(){
        int total=1;
        QFile file(QCoreApplication::applicationDirPath()+"/config/startTerm");
        QDate d2=QDate::currentDate();
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            QDate d1=QDate::fromString(in.readAll().trimmed(),Qt::ISODate);
            if(!d1.isValid() || d1>d2){
                return -1;
            }
            for(;d1<d2;d1=d1.addDays(1)){
                if(d1.dayOfWeek()==7){
                    total++;
                }
            }
        }else{
            QDate d1(2000,1,1);
            for(;d1!=d2;d1=d1.addDays(1)){
                if(d1.dayOfWeek()==7){
                    total++;
                }
            }
            QFile file2(QCoreApplication::applicationDirPath()+"/config/change");
            if(file2.open(QIODevice::ReadOnly | QIODevice::Text)){
                QTextStream in2(&file2);
                int c=in2.readAll().toInt();
                file2.close();
                return total+c;
            }else{
                return total;
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

    inline int goodDay(){
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

    inline void recordLatestTime(){
        QDate current = QDate::currentDate();
        QDate file = QDate::fromString(SystemAPI::readFile("/config/latestTime"),Qt::ISODate);
        if(!file.isValid() || current>file){
            SystemAPI::writeFile("/config/latestTime",QDate::currentDate().toString(Qt::ISODate));
        }
    }

    inline bool timeTrusted(){
        QDate lateset = QDate::fromString(SystemAPI::readFile("/config/latestTime"),Qt::ISODate);
        if(lateset.isValid()){
            recordLatestTime();
            return lateset <= QDate::currentDate();
        }else{
            recordLatestTime();
            return QDate::currentDate().year()>=2026;
        }
    }


};

#endif // CLOCK_H
