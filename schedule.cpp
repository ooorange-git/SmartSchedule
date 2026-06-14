#include "schedule.h"
#include "ui_schedule.h"
#include "mainwindow.h"
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
#include <windows.h>
#include <dwmapi.h>

enum WINDOWCOMPOSITIONATTRIB {
    WCA_ACCENT_POLICY=19,
};

enum ACCENT_STATE {
    ACCENT_DISABLED = 0,
    ACCENT_ENABLE_GRADIENT = 1,
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
    ACCENT_ENABLE_BLURBEHIND = 3,          // 经典毛玻璃
    ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,   // Win10 1803+ 亚克力效果
    ACCENT_ENABLE_HOSTBACKDROP = 5         // 更现代的云母效果
};

typedef struct _ACCENT_POLICY {
    ACCENT_STATE AccentState;
    DWORD        AccentFlags;
    DWORD        GradientColor;
    DWORD        AnimationId;
} ACCENT_POLICY, *PACCENT_POLICY;

typedef struct _WINDOWCOMPOSITIONATTRIBDATA {
    WINDOWCOMPOSITIONATTRIB Attrib;
    PVOID                   pvData;
    UINT                    cbData;
} WINDOWCOMPOSITIONATTRIBDATA, *PWINDOWCOMPOSITIONATTRIBDATA;

typedef BOOL (WINAPI *pfnSetWindowCompositionAttribute)(
    HWND hwnd,
    const WINDOWCOMPOSITIONATTRIBDATA* pwcad
);

bool Schedule::enableSetWindowCompositionAttribute(){
    HWND hwnd = reinterpret_cast<HWND>(this->winId());
    HMODULE hUser32 = GetModuleHandleW(L"user32.dll");
    if (!hUser32) {
        // 加载失败 NO!
        return 0;
    }
    auto SetWindowCompositionAttribute = reinterpret_cast<pfnSetWindowCompositionAttribute>(GetProcAddress(hUser32,"SetWindowCompositionAttribute"));
    if(!SetWindowCompositionAttribute){
        //梅开二度
        return 0;
    }

    ACCENT_POLICY ac;
    ac.AnimationId = 0;
    ac.AccentState = ACCENT_ENABLE_ACRYLICBLURBEHIND;
    ac.AccentFlags = 0x20 | 0x40 | 0x80;
    ac.GradientColor = 0x20202020;

    WINDOWCOMPOSITIONATTRIBDATA wcd;
    wcd.Attrib=WCA_ACCENT_POLICY;
    wcd.cbData=sizeof(ac);
    wcd.pvData=&ac;

    return SetWindowCompositionAttribute(hwnd,&wcd);
}

bool Schedule::enableBlurBehindWindow() {
    HWND hwnd = reinterpret_cast<HWND>(this->winId());
    if (!hwnd) return false;

    DWM_BLURBEHIND blurBehind = {0,0,0,0};
    blurBehind.dwFlags = DWM_BB_ENABLE;
    blurBehind.fEnable = true;
    blurBehind.hRgnBlur = NULL;

    HRESULT hr = DwmEnableBlurBehindWindow(hwnd, &blurBehind);
    return SUCCEEDED(hr);
}

void Schedule::setBackground(){
    QString wallPaperPath = QDir::homePath()+"/AppData/Roaming/Microsoft/Windows/Themes/TranscodedWallpaper";
    QImage wallPaper = QImage(wallPaperPath);
    int w = QGuiApplication::primaryScreen()->geometry().width();
    int h = QGuiApplication::primaryScreen()->geometry().height();
    wallPaper = wallPaper.scaled(w+40,h+40,Qt::IgnoreAspectRatio,Qt::FastTransformation);
    if(wallPaper.isNull()){
        qDebug()<<"啊没有壁纸啊";
        BluredWallPaper = QPixmap();
        return;
    }
    QPixmap wallPaperPix = QPixmap::fromImage(wallPaper);

    QGraphicsScene sc;
    QGraphicsPixmapItem item(wallPaperPix);
    sc.addItem(&item);

    QGraphicsBlurEffect e;
    e.setBlurRadius(100);
    item.setGraphicsEffect(&e);

    BluredWallPaper = QPixmap(wallPaper.size());
    BluredWallPaper.fill(Qt::transparent);
    QPainter p(&BluredWallPaper);
    sc.render(&p);
}
//path格式为/xxx/xxx
QString Schedule::readFile(QString path){
    QFile file(QCoreApplication::applicationDirPath()+path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream out(&file);
    QString content = out.readAll();
    file.close();
    return content;
}

void writeFile(QString path,QString content){
    QFile file(QCoreApplication::applicationDirPath()+path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out<<content;
    file.close();
}


void Schedule::paintEvent(QPaintEvent *event){
    // QPainter p(this);
    // int x = this->pos().x();
    // int y = this->pos().y();
    // qDebug()<<-x<<-y;
    // p.drawPixmap(-x,-y,BluredWallPaper);
    // p.fillRect(rect(), QColor(255, 255, 255, 80));
    // event->accept();
    QPainter p(this);
    if(readFile("/config/blur").toInt()){
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(QColor(255,255,255,64));
        p.setPen(Qt::lightGray);
        p.drawRect(0,0,width(),height()-3);
    }else{
        p.setRenderHint(QPainter::Antialiasing);
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
    }

    if((event->button()==Qt::LeftButton) && (event->pos().y()>height()-10) && (event->pos().x()<10)){
        m_pressing=1;
        firstPos = event->globalPos();
    }

    event->accept();
}

void Schedule::hideCloseButton(){
    if(closeB->rect().contains(closeB->mapFromGlobal(QCursor::pos()))){
        QTimer::singleShot(10000,[this]{hideCloseButton();});
    }else{
        anim2->start();
    }
}

void Schedule::mouseMoveEvent(QMouseEvent *event){
    if(anim->state()!=QAbstractAnimation::Running && e->opacity()!=1.0){
        anim->start();
        QTimer::singleShot(10000,[this]{hideCloseButton();});
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
        this->setGeometry(this->pos().x()-x_change,this->pos().y(),width()+x_change,height()+y_change);
        closeB->move(width()-37,7);
        firstPos = event->globalPos();
    }
    event->accept();
}

void Schedule::mouseReleaseEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
        m_bDragging=0;
        m_pressing=0;
    }event->accept();
}

void SetLabel(QFile& file,QLabel *l){
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    if(content.contains('/')){
        QStringList sl=content.split('/');
        if(MainWindow::weekNow(nullptr)%2==0){
            l->setText(sl[1]);
        }else{
            l->setText(sl[0]);
        }
        return;
    }
    l->setText(content);
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
    ui->Noon->setText("——午休——");
    ui->BB2->setText("双师体育课");
    for(int i=0;i<8;i++){
        QString name=QCoreApplication::applicationDirPath()+"/config/1/"+QString::number(i+1);
        QFile file(name);
        SetLabel(file,labels[i]);
    }
}

void Schedule::weekday(int d){
    QLabel* labels[8]={ui->C1,ui->C2,ui->C3,ui->C4,ui->C5,ui->C6,ui->C7,ui->C8};
    showAll();
    ui->BB1->setText("大课间");
    ui->Noon->setText("——午休——");
    ui->BB2->setText("双师体育课");
    for(int i=0;i<8;i++){
        QString name=QCoreApplication::applicationDirPath()+"/config/"+QString::number(d)+"/"+QString::number(i+1);
        QFile file(name);
        SetLabel(file,labels[i]);
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
    int day=MainWindow::goodDay();

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
        weekday(day);
    }else{
        weekEnd();
    }

    this->update();
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

    //setBackground();
    if(readFile("/config/blur").toInt()){
        if(QOperatingSystemVersion::current()>=QOperatingSystemVersion::Windows10){
            enableSetWindowCompositionAttribute();
        }else{
            enableBlurBehindWindow();
        }
    }

    anim->setDuration(1000);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);


    anim2->setDuration(1000);
    anim2->setStartValue(1.0);
    anim2->setEndValue(0.0);

    //关闭按钮
    closeB = new QPushButton("×",this);
    closeB->setStyleSheet("QPushButton:hover{background:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FF8E55, stop:1 #FF4526);border-radius:4px;border:none} QPushButton{background:none;border:none}");
    closeB->setGraphicsEffect(e);
    e->setOpacity(0.0);
    closeB->setFixedSize(30,30);
    closeB->move(width()-37,7);
    connect(closeB,&QPushButton::clicked,this,&QWidget::close);

    //右键菜单
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,this, &Schedule::showContextMenu);
    createContextMenu();

    //设置课程
    updateLabel();


    timer_update = new QTimer(this);
    connect(timer_update,&QTimer::timeout,this,&Schedule::updateLabel);
    timer_update->start(10000);

}

void Schedule::createContextMenu()
{
    contextMenu = new QMenu(this);
    QAction *action1 = contextMenu->addAction("打开设置");
    QAction *action2 = contextMenu->addAction("刷新");
    //QAction *action3 = contextMenu->addAction("换课");
    contextMenu->addSeparator();
    QAction *action4 = contextMenu->addAction("退出");
    connect(action1, &QAction::triggered, this, &Schedule::onMainWindow);
    connect(action2, &QAction::triggered, this, &Schedule::onUpdate);
    connect(action4, &QAction::triggered, this, &Schedule::onQuit);
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
    if(event->isAccepted()){
        qApp->quit();
    }
}

Schedule::~Schedule()
{
    delete ui;
}
