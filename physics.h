#ifndef PHYSICS_H
#define PHYSICS_H

#include <QObject>
#include <QPoint>
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <cmath>
#include <QGuiApplication>
#include <QScreen>

class Physics : public QObject
{
    Q_OBJECT
public:
    explicit Physics(QWidget *parent,float t_cof);
    float v1=0;
    float v2=0;
    float f1=0;
    float f2=0;
    float cof=0;
    float a1=0;
    float a2=0;
    QPointF oldP;
    bool is_oldP=0;
    QPointF newP;
    bool is_newP=0;
    void start();
    void onlyCalc();
    void stop();
private:
    QTimer *timer;
    QWidget *m_parent=nullptr;
    int t;
    int enable = 0;
    void updatePos();
    void calcFirstV();
    void calcA();
    void checkCrash();
    void move();
    void updatePhysics();

signals:

};

#endif // PHYSICS_H
