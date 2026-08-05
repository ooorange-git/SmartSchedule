#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QDateEdit>
#include <QLocalServer>

class Schedule;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static int goodDay();
    static int weekNow();
    void restart();
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showWeek();
    void setScheduleWindow(Schedule *s){ m_s=s;};
    QLocalServer *socket = nullptr;


private slots:
    void setSchedule(const QString &text);
    void setCurrentSchedule(const QString &text);

    void on_TurnOn_clicked(bool checked);
    void on_checkBox_clicked(bool checked);
    void on_day_textActivated(const QString &arg1);
    void on_dateEdit_userDateChanged(const QDate &date);
    void on_spinBox_textChanged(const QString &arg1);
    void on_checkBox_2_clicked(bool checked);
    void on_pushButton_clicked();
    //void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_aboutButton_clicked();
    void on_pushButton_4_clicked();
    void on_ifEmpty_activated(int index);

private:
    QString readFile(QString path,bool *ok);
    void writeFile(QString path,QString content);
    void turnOn(bool is);
    void setCurrentLineEdit(QLineEdit *l);
    int recordCurrentScheduleWeek(int changeWeek);
    int currentScheduleWeekChange();
    void setLineEdit(QLineEdit *l);
    void setCurrentScheduleToolTip();
    void deleteOutOfDate(int week,int day);
    void updateInfo();
    Schedule *m_s = nullptr;
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
