#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    ~MainWindow();


public slots:
    void on_aboutButton_clicked();


private slots:
    void on_LeftClockB_clicked(bool checked);

    void on_RightClockB_clicked(bool checked);

    void on_spinBox_valueChanged(int arg1);

    void on_TurnOn_clicked(bool checked);

    void on_L11_textEdited(const QString &arg1);

    void on_L12_textEdited(const QString &arg1);

    void on_L13_textEdited(const QString &arg1);

    void on_L14_textEdited(const QString &arg1);

    void on_L15_textEdited(const QString &arg1);

    void on_L16_textEdited(const QString &arg1);

    void on_L17_textEdited(const QString &arg1);

    void on_L18_textEdited(const QString &arg1);

    void on_L21_textEdited(const QString &arg1);

    void on_L22_textEdited(const QString &arg1);

    void on_L23_textEdited(const QString &arg1);

    void on_L24_textEdited(const QString &arg1);

    void on_L25_textEdited(const QString &arg1);

    void on_L26_textEdited(const QString &arg1);

    void on_L27_textEdited(const QString &arg1);

    void on_L28_textEdited(const QString &arg1);

    void on_L31_textEdited(const QString &arg1);

    void on_L32_textEdited(const QString &arg1);

    void on_L33_textEdited(const QString &arg1);

    void on_L34_textEdited(const QString &arg1);

    void on_L35_textEdited(const QString &arg1);

    void on_L36_textEdited(const QString &arg1);

    void on_L37_textEdited(const QString &arg1);

    void on_L38_textEdited(const QString &arg1);

    void on_L41_textEdited(const QString &arg1);

    void on_L42_textEdited(const QString &arg1);

    void on_L43_textEdited(const QString &arg1);

    void on_L44_textEdited(const QString &arg1);

    void on_L45_textEdited(const QString &arg1);

    void on_L46_textEdited(const QString &arg1);

    void on_L47_textEdited(const QString &arg1);

    void on_L48_textEdited(const QString &arg1);

    void on_L51_textEdited(const QString &arg1);

    void on_L52_textEdited(const QString &arg1);

    void on_L53_textEdited(const QString &arg1);

    void on_L54_textEdited(const QString &arg1);

    void on_L55_textEdited(const QString &arg1);

    void on_L56_textEdited(const QString &arg1);

    void on_L57_textEdited(const QString &arg1);

    void on_L58_textEdited(const QString &arg1);

    void on_checkBox_clicked(bool checked);

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
