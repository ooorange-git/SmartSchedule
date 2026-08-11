#include "physics.h"

void Physics::start(){
    enable = 2;
}

void Physics::onlyCalc(){
    enable = 1;
}

void Physics::stop(){
    enable = 0;
}

void Physics::calcFirstV(){
    if((!is_oldP) || (!is_newP)){
        v1=0;
        v2=0;
    }else{
        int x1 = oldP.x();
        int x2 = newP.x();
        int y1 = oldP.y();
        int y2 = newP.y();
        v1 = x2-x1;
        v2 = y2-y1;
    }
}

void Physics::updatePos(){
    if((!is_oldP) && (!is_newP)){
        oldP = m_parent->pos();
        is_oldP = 1;
    }else if(is_oldP && !is_newP){
        newP = m_parent->pos();
        is_newP = 1;
    }else{
        oldP = newP;
        newP = m_parent->pos();
    }
}

void Physics::calcA(){
    float a = cof*9.8;
    float v = sqrt(v1*v1 + v2*v2);
    if(v == 0){
        a1 = 0;
        a2 = 0;
    }else{
        float Ratio1 = v1/v;
        float Ratio2 = v2/v;
        a1 = a*Ratio1*0.02;
        a2 = a*Ratio2*0.02;
    }
}

void Physics::checkCrash(){
    QPoint p = m_parent->pos();
    int ww = m_parent->width();
    int wh = m_parent->height();
    int wpx = p.x();
    int wpy = p.y();
    int sw = QGuiApplication::primaryScreen()->availableGeometry().width();
    int sh = QGuiApplication::primaryScreen()->availableGeometry().height();
    if(wpx<=0 || wpx>=sw-ww){
        v1 = -v1;
    }
    if(wpy<=0 || wpy>=sh-wh){
        v2 = -v2;
    }
}

void Physics::move(){
    v1-=a1;
    v2-=a2;
    QPoint p = m_parent->pos();
    QPointF pf(p.x()+v1,p.y()+v2);
    m_parent->move(pf.toPoint());
}

void Physics::updatePhysics(){
    if(enable==2){
        if(is_newP && is_oldP){
            checkCrash();
            calcA();
            move();
        }else{
            updatePos();
            calcFirstV();
            calcA();
        }
    }else if(enable == 1){
        updatePos();
        calcFirstV();
        calcA();
    }
}

Physics::Physics(QWidget *parent,float t_cof): QObject{parent},cof(t_cof)
{
    if(parent){
        m_parent = parent;
        timer = new QTimer(this);
        connect(timer,&QTimer::timeout,this,&Physics::updatePhysics);
        timer->start(20);
    }
}
