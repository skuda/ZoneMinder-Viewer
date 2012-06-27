/***************************************************************************
*   Copyright (C) 2008 by Leandro Emanuel López                           *
*   lopezlean@gmail.com                         *
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

#include "maincamerawidget.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
MainCameraWidget::MainCameraWidget( QWidget * parent )
:QWidget( parent ),cameraCount(0)
{
    init();

}

void MainCameraWidget::init(){
    QHBoxLayout * hboxLayout = new QHBoxLayout;
    m_bottomAreaLayout = new QHBoxLayout;
    m_rigthAreaLayout = new QVBoxLayout;
    m_mainCameraLayout = new QVBoxLayout;
    m_mainCamera = new QWidget( this );
    m_mainCamera->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    m_mainCamera->setLayout( m_mainCameraLayout );
    m_rightArea = new QScrollArea( this );
    m_rightAreaWidget = new QWidget(m_rightArea);
    m_rightArea->setWidget( m_rightAreaWidget);
    m_rightArea->setWidgetResizable( true );
    m_rightAreaWidget->setLayout( m_rigthAreaLayout );
    m_bottomArea = new QScrollArea( this );
    m_bottomAreaWidget = new QWidget( m_bottomArea );
    m_bottomArea->setWidgetResizable( true );
    m_bottomArea->setWidget( m_bottomAreaWidget );
    m_bottomAreaWidget->setLayout( m_bottomAreaLayout );
    /*double w = 217;
    double h = 137;*/
    double w = parentWidget()->width() * 0.20;
    double h =  ( w / 320 ) * 240;
    sizes.setWidth( w );
    sizes.setHeight( h );
    m_rightArea->setFixedWidth( w + 30 );
    m_bottomArea->setFixedHeight( h + 30 );

    hboxLayout->addWidget( m_mainCamera );
    hboxLayout->addWidget( m_rightArea );
    QVBoxLayout * vboxLayout = new QVBoxLayout;
    vboxLayout->addLayout( hboxLayout );
    vboxLayout->addWidget( m_bottomArea );
    vboxLayout->setMargin( 2 );
    vboxLayout->setSpacing( 2 );
    setLayout( vboxLayout );
    m_bottomAreaLayout->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
    m_rigthAreaLayout->setAlignment( Qt::AlignTop | Qt::AlignCenter );
    m_bottomAreaLayout->setMargin( 2 );
    m_bottomAreaLayout->setSpacing( 2 );
    m_rigthAreaLayout->setMargin( 2 );


}
void MainCameraWidget::appendCamera( QWidget * widget ){
    if ( cameraCount < 5 )
        m_rigthAreaLayout->addWidget( widget );
    else m_bottomAreaLayout->addWidget( widget );
    connect ( widget , SIGNAL (promotedToMainWidget ( QWidget *) ), this, SLOT( setMainCameraWidget( QWidget * ) ) );
    widget->setFixedSize( sizes );
    cameraCount++;
}

void MainCameraWidget::setVisible ( bool b ){
    if ( b ){
        QLayoutItem * item = m_bottomAreaLayout->takeAt( 0 );
        if ( item && item->widget() )
            setMainCameraWidget (  item->widget() );
    }
    QWidget::setVisible( b );
}
void MainCameraWidget::setMainCameraWidget( QWidget * widget ){
    //QSize newSize = widget->size();

    QLayoutItem * currentItem = m_mainCameraLayout->takeAt( 0 );
    if ( currentItem ){
        if ( m_bottomAreaLayout->indexOf( widget ) >= 0 )
            m_bottomAreaLayout->addWidget( currentItem->widget() );
        else m_rigthAreaLayout->addWidget( currentItem->widget() );
        currentItem->widget()->setFixedSize( sizes );
        
    }
    m_mainCameraLayout->addWidget( widget );
    widget->setFixedSize( m_mainCamera->size() );
}

MainCameraWidget::~MainCameraWidget()
{
}


