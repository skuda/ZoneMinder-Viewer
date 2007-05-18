/********************************************************************************
** Form generated from reading ui file 'cameratoolbar.ui'
**
** Created: Sun Apr 22 20:03:32 2007
**      by: Qt User Interface Compiler version 4.2.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CAMERATOOLBAR_H
#define UI_CAMERATOOLBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

class Ui_CameraToolBar
{
public:
    QHBoxLayout *hboxLayout;
    QHBoxLayout *hboxLayout1;
    QToolButton *toolButtonStart;
    QToolButton *toolButtonStop;
    QToolButton *toolButtonConfig;

    void setupUi(QWidget *CameraToolBar)
    {
    CameraToolBar->setObjectName(QString::fromUtf8("CameraToolBar"));
    hboxLayout = new QHBoxLayout(CameraToolBar);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    toolButtonStart = new QToolButton(CameraToolBar);
    toolButtonStart->setObjectName(QString::fromUtf8("toolButtonStart"));
    toolButtonStart->setIcon(QIcon());

    hboxLayout1->addWidget(toolButtonStart);

    toolButtonStop = new QToolButton(CameraToolBar);
    toolButtonStop->setObjectName(QString::fromUtf8("toolButtonStop"));

    hboxLayout1->addWidget(toolButtonStop);

    toolButtonConfig = new QToolButton(CameraToolBar);
    toolButtonConfig->setObjectName(QString::fromUtf8("toolButtonConfig"));

    hboxLayout1->addWidget(toolButtonConfig);


    hboxLayout->addLayout(hboxLayout1);


    retranslateUi(CameraToolBar);

    QSize size(118, 50);
    size = size.expandedTo(CameraToolBar->minimumSizeHint());
    CameraToolBar->resize(size);


    QMetaObject::connectSlotsByName(CameraToolBar);
    } // setupUi

    void retranslateUi(QWidget *CameraToolBar)
    {
    CameraToolBar->setWindowTitle(QApplication::translate("CameraToolBar", "Form", 0, QApplication::UnicodeUTF8));
    toolButtonStart->setText(QApplication::translate("CameraToolBar", ">", 0, QApplication::UnicodeUTF8));
    toolButtonStop->setText(QApplication::translate("CameraToolBar", "||", 0, QApplication::UnicodeUTF8));
    toolButtonConfig->setText(QApplication::translate("CameraToolBar", "C", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CameraToolBar);
    } // retranslateUi

};

namespace Ui {
    class CameraToolBar: public Ui_CameraToolBar {};
} // namespace Ui

#endif // UI_CAMERATOOLBAR_H
