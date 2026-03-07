/********************************************************************************
** Form generated from reading UI file 'schedule.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCHEDULE_H
#define UI_SCHEDULE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Schedule
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *C1;
    QLabel *BB1;
    QLabel *C2;
    QLabel *C3;
    QLabel *C4;
    QLabel *Noon;
    QLabel *C5;
    QLabel *C6;
    QLabel *C7;
    QLabel *BB2;
    QLabel *C8;

    void setupUi(QWidget *Schedule)
    {
        if (Schedule->objectName().isEmpty())
            Schedule->setObjectName("Schedule");
        Schedule->resize(93, 419);
        QFont font;
        font.setPointSize(14);
        Schedule->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/mainIcon.ico"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Schedule->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(Schedule);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        C1 = new QLabel(Schedule);
        C1->setObjectName("C1");
        C1->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(C1);

        BB1 = new QLabel(Schedule);
        BB1->setObjectName("BB1");
        BB1->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(BB1);

        C2 = new QLabel(Schedule);
        C2->setObjectName("C2");
        C2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(C2);

        C3 = new QLabel(Schedule);
        C3->setObjectName("C3");
        C3->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(C3);

        C4 = new QLabel(Schedule);
        C4->setObjectName("C4");
        C4->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(C4);

        Noon = new QLabel(Schedule);
        Noon->setObjectName("Noon");
        Noon->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(Noon);

        C5 = new QLabel(Schedule);
        C5->setObjectName("C5");
        C5->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(C5);

        C6 = new QLabel(Schedule);
        C6->setObjectName("C6");
        C6->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(C6);

        C7 = new QLabel(Schedule);
        C7->setObjectName("C7");
        C7->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(C7);

        BB2 = new QLabel(Schedule);
        BB2->setObjectName("BB2");
        BB2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(BB2);

        C8 = new QLabel(Schedule);
        C8->setObjectName("C8");
        C8->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(C8);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(Schedule);

        QMetaObject::connectSlotsByName(Schedule);
    } // setupUi

    void retranslateUi(QWidget *Schedule)
    {
        Schedule->setWindowTitle(QCoreApplication::translate("Schedule", "Form", nullptr));
        C1->setText(QCoreApplication::translate("Schedule", "\346\225\260\345\255\246", nullptr));
        BB1->setText(QCoreApplication::translate("Schedule", "\345\244\247\350\257\276\351\227\264", nullptr));
        C2->setText(QCoreApplication::translate("Schedule", "\346\225\260\345\255\246", nullptr));
        C3->setText(QCoreApplication::translate("Schedule", "\346\225\260\345\255\246", nullptr));
        C4->setText(QCoreApplication::translate("Schedule", "\346\225\260\345\255\246", nullptr));
        Noon->setText(QCoreApplication::translate("Schedule", "\345\215\210\344\274\221", nullptr));
        C5->setText(QCoreApplication::translate("Schedule", "\346\225\260\345\255\246", nullptr));
        C6->setText(QCoreApplication::translate("Schedule", "\346\225\260\345\255\246", nullptr));
        C7->setText(QCoreApplication::translate("Schedule", "\346\225\260\345\255\246", nullptr));
        BB2->setText(QCoreApplication::translate("Schedule", "\345\244\247\350\257\276\351\227\264", nullptr));
        C8->setText(QCoreApplication::translate("Schedule", "\346\225\260\345\255\246", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Schedule: public Ui_Schedule {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCHEDULE_H
