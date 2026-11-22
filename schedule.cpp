#include "schedule.h"
#include "ui_schedule.h"
#include "mainwindow.h"
#include "physics.h"
#include "systemapi.h"
#include "clock.h"

#include <QDate>
#include <QFile>
#include <QAction>
#include <QMenu>
#include <QDebug>
#include <QCloseEvent>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QGraphicsEffect>
#include <QDir>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGuiApplication>
#include <QScreen>
#include <QOperatingSystemVersion>
#include <QThread>
#include <QSystemTrayIcon>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QVariantAnimation>
#include <QRegion>


using namespace SystemAPI;

void Schedule::initPosition(){
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QRect screenGeometry = primaryScreen->geometry();
    int x = screenGeometry.width()-width();
    int y = (screenGeometry.height()-height())/2;
    move(x,y);
    closeB->move(width()-37,7);
}

QPixmap Schedule::addNoise(int size){
    QImage image(size,size,QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QRandomGenerator *rg = QRandomGenerator::global();

    for(int i = 0;i < size;i++){
        QRgb *line = reinterpret_cast<QRgb*>(image.scanLine(i));
        for(int j = 0;j < size;j++){
            int gray = rg->bounded(256);
            line[j] = qRgba(gray,gray,gray,8);
        }
    }

    return QPixmap::fromImage(image);
}

void Schedule::setBackground(){
    QString wallPaperPath = QDir::homePath()+"/AppData/Roaming/Microsoft/Windows/Themes/TranscodedWallpaper";
    QImage wallPaper = QImage(wallPaperPath);
    int w = QGuiApplication::primaryScreen()->geometry().width();
    int h = QGuiApplication::primaryScreen()->geometry().height();
    wallPaper = wallPaper.scaled(w+100,h+100,Qt::IgnoreAspectRatio,Qt::FastTransformation);
    if(wallPaper.isNull()){
        BluredWallPaper = QPixmap();
        return;
    }
    QPixmap wallPaperPix = QPixmap::fromImage(wallPaper);

    QGraphicsScene sc;
    QGraphicsPixmapItem item(wallPaperPix);
    sc.addItem(&item);

    QGraphicsBlurEffect e;
    e.setBlurRadius(90);
    item.setGraphicsEffect(&e);

    BluredWallPaper = QPixmap(wallPaper.size());
    BluredWallPaper.fill(Qt::transparent);
    QPainter p(&BluredWallPaper);
    sc.render(&p);
}

void Schedule::setSystemTrayIcon(){
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":/icon/logo.ico"));
    m_trayIcon->setToolTip("智能课程表");

    m_trayIcon->setContextMenu(contextMenu);
    m_trayIcon->show();

    connect(m_trayIcon,&QSystemTrayIcon::activated,this,[=](QSystemTrayIcon::ActivationReason r){
        if(r==QSystemTrayIcon::DoubleClick){
            this->raise();
            this->activateWindow();
        }
    });
}

void Schedule::paintEvent(QPaintEvent *event){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    if(CURRENT_BLUR_SETTING==1){
        p.setBrush(QColor(255,255,255,80));
        p.setPen(Qt::lightGray);
        p.drawRect(0,0,width(),height());
    }else if(CURRENT_BLUR_SETTING==2){
        QPainterPath path;
        path.addRoundedRect(rect(), 10, 10);
        p.setClipPath(path);
        int x = this->pos().x();
        int y = this->pos().y();
        p.drawTiledPixmap(rect(),noise);
        p.drawPixmap(-x,-y,BluredWallPaper);
        p.fillRect(rect(), QColor(255, 255, 255, 160));
        p.setPen(QColor(0,0,0,25));
        p.drawRoundedRect(rect(),10,10);
    }else{
        p.setBrush(QColor(0xF0F0F4));
        p.setPen(Qt::lightGray);
        p.drawRoundedRect(this->rect(),10,10);
    }
    event->accept();
}

void Schedule::mousePressEvent(QMouseEvent *event){
    if((event->button()==Qt::LeftButton) && (event->pos().y()<30)){
        m_bDragging=1;
        m_dragP = event->globalPos() - frameGeometry().topLeft();
        if(p){
            p->onlyCalc();
        }
    }

    if((event->button()==Qt::LeftButton) && (event->pos().y()>height()-10) && (event->pos().x()<10)){
        m_pressing=1;
        firstPos = event->globalPos();
        if(p){
          p->onlyCalc();
        }
    }

    event->accept();
}

void Schedule::hideCloseButton(){
    if(closeB->rect().contains(closeB->mapFromGlobal(QCursor::pos()))){
        QTimer::singleShot(10000,this,[this]{hideCloseButton();});
    }else{
        anim2->start();
    }
}

void Schedule::mouseMoveEvent(QMouseEvent *event){
    if(anim->state()!=QAbstractAnimation::Running && e->opacity()!=1.0){
        anim->start();
        QTimer::singleShot(10000,this,[this]{hideCloseButton();});
    }

    if(m_bDragging && (event->buttons() & Qt::LeftButton)){
        move(event->globalPos()-m_dragP);
        this->update();
    }

    if((event->pos().y()>height()-10) && (event->pos().x()<10)){
        if(this->cursor().shape() != Qt::SizeBDiagCursor){
            this->setCursor(Qt::SizeBDiagCursor);
        }
    }else{
        this->setCursor(Qt::ArrowCursor);
    }

    if(m_pressing){
        int x_change = firstPos.x()-event->globalX();
        int y_change = event->globalY()-firstPos.y();
        if((width()+x_change)>this->minimumWidth()){
            this->setGeometry(this->pos().x()-x_change,this->pos().y(),width()+x_change,height());
        }
        this->resize(width(),height()+y_change);
        firstPos = event->globalPos();
    }
    event->accept();
}

void Schedule::mouseReleaseEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
        m_bDragging=0;
        m_pressing=0;
        if(p){
            p->start();
        }
    }event->accept();
}

void Schedule::resizeEvent(QResizeEvent *event){
    closeB->move(width()-37,7);
    event->accept();
}

void Schedule::setLabelText(QLabel *l,QString content){
    if(l->text()==content){
        l->clearMask();
    }else if(!inited){
        l->setText(content);
    }else{
        QVariantAnimation *anim1 = new QVariantAnimation(l);
        anim1->setDuration(500);
        anim1->setStartValue(1.0);
        anim1->setEndValue(0.0);
        anim1->setEasingCurve(QEasingCurve::InOutQuad);

        QVariantAnimation *anim2 = new QVariantAnimation(l);
        anim2->setDuration(300);
        anim2->setStartValue(0.0);
        anim2->setEndValue(1.0);
        anim2->setEasingCurve(QEasingCurve::InOutQuad);

        int h=l->height();

        connect(anim1, &QVariantAnimation::valueChanged,l,[=](const QVariant &val){
            double n = val.toDouble();
            int showingHeight = h*n+1;
            l->setMask(QRegion(0,h-showingHeight,l->width(),showingHeight));
        });

        connect(anim2, &QVariantAnimation::valueChanged,l,[=](const QVariant &val){
            double n = val.toDouble();
            int showingHeight = h*n+1;
            l->setMask(QRegion(0,h-showingHeight,l->width(),showingHeight));
        });

        connect(anim1, &QVariantAnimation::finished, l, [content,l,anim2](){
            l->setText(content);
            anim2->start(QAbstractAnimation::DeleteWhenStopped);
        });

        connect(anim2, &QVariantAnimation::finished, l, [l](){
            l->clearMask();
        });

        anim1->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

//读取逻辑：先读取current文件夹有就用没有读std
void Schedule::setLabel(QLabel *l,int today){
    QString week = QString::number(Clock::goodDay());
    QString day = QString::number(today);
    QString path1 = QCoreApplication::applicationDirPath()+"/config/current"+week+"/"+day;
    QString path2 = QCoreApplication::applicationDirPath()+"/config/"+week+"/"+day;
    QFile file1(path1);
    QString content = QString();
    if(file1.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file1);
        content = in.readAll();
        file1.close();
    }else{
        QFile file2(path2);
        if(file2.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file2);
            content = in.readAll();
            file2.close();
        }
    }

    if(content.contains('/')){
        QStringList sl=content.split('/');
        if(Clock::weekNow()%2==0){
            if(inited){
                setLabelText(l,sl[1]);
            }else{
                l->setText(sl[1]);
            }
        }else{
            if(inited){
                setLabelText(l,sl[0]);
            }else{
                l->setText(sl[0]);
            }
        }
        return;
    }

    if(inited){
        setLabelText(l,content);
    }else{
       l->setText(content);
    }
}



void Schedule::showAll(){
    ui->Noon->setVisible(1);
    ui->BB1->setVisible(1);
    ui->BB2->setVisible(1);
    ui->C1->setVisible(1);
    ui->C2->setVisible(1);
    ui->C3->setVisible(1);
    ui->C4->setVisible(1);
    ui->C5->setVisible(1);
    ui->C6->setVisible(1);
    ui->C7->setVisible(1);
    ui->C8->setVisible(1);
}

void Schedule::monday(){
    QLabel* labels[8]={ui->C1,ui->BB1,ui->C3,ui->C4,ui->C5,ui->C6,ui->C7,ui->C8};
    showAll();
    ui->C2->setText("升旗仪式");
    ui->Noon->setText("午休");
    ui->BB2->setText("双师体育课");
    for(int i=0;i<8;i++){
        setLabel(labels[i],i+1);
    }
}

void Schedule::weekday(){
    QLabel* labels[8]={ui->C1,ui->C2,ui->C3,ui->C4,ui->C5,ui->C6,ui->C7,ui->C8};
    showAll();
    ui->BB1->setText("大课间");
    ui->Noon->setText("午休");
    ui->BB2->setText("双师体育课");
    for(int i=0;i<8;i++){
        setLabel(labels[i],i+1);
    }
}

void Schedule::weekEnd(){
    QLabel* c[8]={ui->C1,ui->C2,ui->C3,ui->C4,ui->C5,ui->C6,ui->C7,ui->C8};
    c[1]->setText("今");
    ui->Noon->setText("天");
    c[5]->setText("无");
    ui->BB2->setText("课");
    c[0]->setVisible(0);
    c[2]->setVisible(0);
    c[3]->setVisible(0);
    c[4]->setVisible(0);
    c[6]->setVisible(0);
    c[7]->setVisible(0);
    ui->BB1->setVisible(0);
}

void Schedule::updateLabel(){
    if(!ui) return;
    int day=Clock::goodDay();

    QLabel* c[8]={ui->C1,ui->C2,ui->C3,ui->C4,ui->C5,ui->C6,ui->C7,ui->C8};

    QFont f("LXGW WenKai Lite Medium",14);
    f.setHintingPreference(QFont::PreferNoHinting);
    f.setStyleStrategy(QFont::PreferAntialias);

    for(int i=0;i<8;i++){c[i]->setFont(f);}
    ui->Noon->setFont(f);
    ui->BB1->setFont(f);
    ui->BB2->setFont(f);

    if(day==1){
        monday();
    }else if(day>=2 && day <=5){
        weekday();
    }else{
        weekEnd();
    }
    closeB->move(width()-37,7);
    this->update();
}

void Schedule::usePhy(){
    p = new Physics(this,2);
    p->start();
}

Schedule::Schedule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Schedule)
{
    QApplication::restoreOverrideCursor();
    ui->setupUi(this);
    this->setMouseTracking(1);

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setWindowTitle("课程表");

    if(readFile("/config/blur").toInt()){
        if(!readFile("/config/useWallpaper").toInt()){
            CURRENT_BLUR_SETTING = 1;
        }else{
            CURRENT_BLUR_SETTING = 2;
        }
    }

    if(CURRENT_BLUR_SETTING==1){
        if(QOperatingSystemVersion::current()>=QOperatingSystemVersion::Windows10){
            SystemAPI::setWindowCompositionAttribute(this);
        }else{
            SystemAPI::enableBlurBehindWindow(this);
        }
    }else if(CURRENT_BLUR_SETTING==2){
        setBackground();
        QTimer *timer_update_background = new QTimer(this);
        connect(timer_update_background,&QTimer::timeout,this,&Schedule::setBackground);
        timer_update_background->start(30000);
    }

    //关闭按钮
    closeB = new QPushButton("×",this);
    closeB->setFocusPolicy(Qt::NoFocus);
    closeB->setStyleSheet("QPushButton:hover{background:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FF8E55, stop:1 #FF4526);border-radius:4px;border:none} QPushButton{background:none;border:none}");
    closeB->setFixedSize(30,30);
    connect(closeB,&QPushButton::clicked,this,&QWidget::close);

    e=new QGraphicsOpacityEffect(closeB);

    anim = new QPropertyAnimation(e,"opacity",closeB);
    anim->setDuration(1000);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);

    anim2 = new QPropertyAnimation(e,"opacity",closeB);
    anim2->setDuration(1000);
    anim2->setStartValue(1.0);
    anim2->setEndValue(0.0);
    closeB->setGraphicsEffect(e);
    e->setOpacity(0.0);

    //右键菜单
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,this, &Schedule::showContextMenu);
    createContextMenu();

    //托盘图标
    setSystemTrayIcon();

    //设置课程
    updateLabel();

    noise = addNoise(100);

    inited = 1;

    QTimer::singleShot(50,[this]{initPosition();});
    QTimer *timer_update = new QTimer(this);
    connect(timer_update,&QTimer::timeout,this,&Schedule::updateLabel);
    timer_update->start(5000);
}

void Schedule::createContextMenu()
{
    contextMenu = new QMenu(this);
    QAction *action1 = contextMenu->addAction("打开设置");
    QAction *action2 = contextMenu->addAction("刷新");
    contextMenu->addSeparator();
    QAction *action3 = contextMenu->addAction("退出");
    connect(action1, &QAction::triggered, this, &Schedule::onMainWindow);
    connect(action2, &QAction::triggered, this, &Schedule::onUpdate);
    connect(action3, &QAction::triggered, this, &Schedule::onQuit);
}

void Schedule::showContextMenu(const QPoint &pos)
{
    contextMenu->exec(mapToGlobal(pos));
}

void Schedule::onUpdate(){
    Schedule::updateLabel();
}

void Schedule::onQuit(){
    this->close();
}

void Schedule::onMainWindow(){
    if (m_mainwindow->isMinimized())
        m_mainwindow->showNormal();
    else
        m_mainwindow->show();
    m_mainwindow->raise();
    m_mainwindow->activateWindow();
}

void Schedule::closeEvent(QCloseEvent *event){
    Clock::recordLatestTime();
    qApp->quit();
    event->accept();
}

Schedule::~Schedule()
{
    delete ui;
}
