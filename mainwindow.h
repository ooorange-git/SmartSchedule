#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QDateEdit>

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
    MainWindow(QWidget *parent = nullptr);
    static int goodDay();
    static int weekNow(QDateEdit *l);
    void showWeek();
    void setLineEdit(QLineEdit *l);
    void setSchedule(Schedule *s){ m_s=s;};
    ~MainWindow();


public slots:
    void on_aboutButton_clicked();


private slots:
    void SetSchedule(const QString &text);

    void on_TurnOn_clicked(bool checked);

    void on_checkBox_clicked(bool checked);

    void on_day_textActivated(const QString &arg1);

    void on_dateEdit_userDateChanged(const QDate &date);

    void on_spinBox_textChanged(const QString &arg1);

    void on_comboBox_activated(int index);

private:
    QString readFile(QString path);
    Schedule *m_s;
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
