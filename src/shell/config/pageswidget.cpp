/***************************************************************************
 *   Copyright (C) 2008 by Leandro Emanuel López                           *
 *   lopezlean@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "pageswidget.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QStackedWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QDialogButtonBox>

#include <QScrollArea>

/**
    TODO: arreglar los nombres de las variables
*/

PagesWidget::PagesWidget( QWidget * parent )
        : QWidget( parent ),mOptionWidget(0),mListWidget(0),mCurrentIcon(0),mCurrentOption(0)
{
  init();
}

void PagesWidget::init()
{

    setObjectName(QString::fromUtf8("PagesWidget"));
    vboxLayout = new QVBoxLayout(this);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(9);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    mListWidget = new QListWidget(this);
    mListWidget->setObjectName(QString::fromUtf8("scd_Options"));
    mListWidget->setMaximumSize(QSize(200, 16777215));
    mListWidget->setLayoutDirection(Qt::LeftToRight);
    mListWidget->setAutoScroll(true);
    mListWidget->setIconSize(QSize(48, 48));
    mListWidget->setMovement(QListView::Static);
    //mListWidget->setSpacing(25);
    //mListWidget->setViewMode(QListView::IconMode);
    mListWidget->setWordWrap(true);

    hboxLayout->addWidget(mListWidget);

    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setSpacing(6);
    vboxLayout1->setMargin(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    frame = new QFrame(this);
    frame->setObjectName(QString::fromUtf8("frame"));
    QPalette palette;
    QBrush brush(QColor(255, 255, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Base, brush);
    QBrush brush1(QColor(255, 255, 255, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Window, brush1);
    QBrush brush2(QColor(255, 255, 255, 255));
    brush2.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
    QBrush brush3(QColor(255, 255, 255, 255));
    brush3.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush3);
    QBrush brush4(QColor(255, 255, 255, 255));
    brush4.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush4);
    QBrush brush5(QColor(255, 255, 255, 255));
    brush5.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush5);
    frame->setPalette(palette);
    frame->setAutoFillBackground(true);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    
    QVBoxLayout * vLayoutFrame = new QVBoxLayout( frame );
    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setSpacing(6);
    hboxLayout2->setMargin(0);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    mCurrentOption = new QLabel(frame);
    mCurrentOption->setObjectName(QString::fromUtf8("scd_CurrentOption"));

    hboxLayout2->addWidget(mCurrentOption);

    mCurrentIcon = new QLabel(frame);
    mCurrentIcon->setObjectName(QString::fromUtf8("scd_CurrentIcon"));
    mCurrentIcon->setLayoutDirection(Qt::LeftToRight);
    mCurrentIcon->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    hboxLayout2->addWidget(mCurrentIcon);
    
    mCurrentOptionText = new QLabel(frame);
    mCurrentOptionText->setObjectName(QString::fromUtf8("scd_CurrentOptionText"));

    vLayoutFrame->addLayout ( hboxLayout2 );
    vLayoutFrame->addWidget ( mCurrentOptionText );

    vboxLayout1->addWidget(frame);
    mOptionWidget = new QStackedWidget(this);
    mOptionWidget->setObjectName(QString::fromUtf8("scd_OptionWidget"));
    /*firtsPage = new QWidget();
    firtsPage->setObjectName(QString::fromUtf8("firtsPage"));
    vboxLayout2 = new QVBoxLayout(firtsPage);
    vboxLayout2->setSpacing(6);
    vboxLayout2->setMargin(9);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    mOptionWidget->addWidget(firtsPage);*/

    vboxLayout1->addWidget( mOptionWidget );


    hboxLayout->addLayout(vboxLayout1);


    vboxLayout->addLayout(hboxLayout);

    QSize size(891, 597);
    size = size.expandedTo( minimumSizeHint() );
    resize(size);

    QObject::connect(mListWidget, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
    mOptionWidget->setCurrentIndex(0);

}

void PagesWidget::addWidget( QWidget * widget , const QString & text , const QString & opttext, QIcon icon )
{
    QScrollArea * area = new QScrollArea ( this );
    area->setWidget(  widget );
    area->setWidgetResizable( true );
    //area->setBackgroundRole( QPalette::Base );
    //area->setFrameStyle( QFrame::Box );
    //area->setAutoFillBackground( true );
    mOptionWidget->addWidget( area );
    mWidgets.append( widget );
    QListWidgetItem * item = new QListWidgetItem ( icon , text );
    item->setToolTip( opttext );
    mListWidget->addItem( item  );
    if ( mListWidget->count() == 1 ) changePage( item, item);

}

QList <QWidget * > PagesWidget::allWidgets(){
    return mWidgets;
}

void PagesWidget::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if ( mListWidget->count() < 1 ) return;
    if (!current)
        current = previous;

    mOptionWidget->setCurrentIndex(mListWidget->row(current));
    mCurrentOption->setText ( "<h2>" + current->text() + "</h2>" );
    mCurrentOptionText->setText ( current->toolTip() );
    const QSize size(48, 48);
    mCurrentIcon->setPixmap ( current->icon().pixmap(size));

}


PagesWidget::~PagesWidget()
{
    delete mOptionWidget;
    delete mListWidget;
    delete mCurrentIcon;
    delete mCurrentOption;
}

