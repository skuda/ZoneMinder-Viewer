/**************************************************************************
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

#include "doublecamerawidget.h"
#include "camerawidget.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QList>
#include <QAction>

DoubleCameraWidget::DoubleCameraWidget( QWidget * parent )
:QWidget( parent )
{
    init();
}
void DoubleCameraWidget::init(){
    QHBoxLayout * hboxLayout = new QHBoxLayout;
    m_bottomAreaLayout = new QHBoxLayout;
    m_mainCameraLayout = new QVBoxLayout;
    m_secondCameraLayout = new QVBoxLayout;
    m_mainCamera = new QWidget( this );
    m_mainCamera->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    m_mainCamera->setLayout( m_mainCameraLayout );
    m_secondCamera = new QWidget( this );
    m_secondCamera->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    m_secondCamera->setLayout( m_secondCameraLayout );

    m_bottomArea = new QScrollArea( this );
    m_bottomAreaWidget = new QWidget( m_bottomArea );
    m_bottomArea->setWidgetResizable( true );
    m_bottomArea->setWidget( m_bottomAreaWidget );
    m_bottomAreaWidget->setLayout( m_bottomAreaLayout );
    /*double w = 217;
    double h = 137;*/
    double h =  240;
    double w =  320;

    sizes.setWidth( w );
    sizes.setHeight( h );
    m_bottomArea->setFixedHeight( h + 30 );
    
    hboxLayout->addWidget( m_mainCamera );
    hboxLayout->addWidget( m_secondCamera );

    QVBoxLayout * vboxLayout = new QVBoxLayout;
    vboxLayout->addLayout( hboxLayout );
    vboxLayout->addWidget( m_bottomArea );
    vboxLayout->setMargin( 2 );
    vboxLayout->setSpacing( 2 );
    setLayout( vboxLayout );
    m_bottomAreaLayout->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
    m_bottomAreaLayout->setMargin( 2 );
    m_bottomAreaLayout->setSpacing( 2 );

}

void DoubleCameraWidget::appendCamera( QWidget * widget ){
    m_bottomAreaLayout->addWidget( widget );
    connect ( widget , SIGNAL (promotedToMainWidget ( QWidget *) ), this, SLOT( setMainCameraWidget( QWidget * ) ) );
    connect ( widget , SIGNAL (promotedToSecondWidget ( QWidget *) ), this, SLOT( setSecondCameraWidget( QWidget * ) ) );
    
    m_mainCameraActions.append( qobject_cast <CameraWidget * > (widget)->promoteToMainCameraAction() );
    m_secondCameraActions.append( qobject_cast <CameraWidget * > (widget)->promoteToSecondCameraAction()  );

    widget->setFixedSize( sizes );

}
void DoubleCameraWidget::setVisible ( bool b ){
    if ( b ){
        QLayoutItem * item = m_bottomAreaLayout->takeAt( 0 );
        if ( item && item->widget() ){
            setMainCameraWidget (  item->widget() );
        }
        item = m_bottomAreaLayout->takeAt( 1 );
        if ( item && item->widget() ){
            setSecondCameraWidget (  item->widget() );
        }
    }

    QWidget::setVisible( b );
}
void DoubleCameraWidget::setMainCameraWidget( QWidget * widget ){
    QLayoutItem * currentItem = m_mainCameraLayout->takeAt( 0 );
    if ( currentItem ){
          m_bottomAreaLayout->addWidget( currentItem->widget() );
          currentItem->widget()->setFixedSize( sizes );
    }
    m_mainCameraLayout->addWidget( widget );
    widget->setFixedSize( m_mainCamera->size() );

}
void DoubleCameraWidget::setSecondCameraWidget( QWidget * widget ){
    QLayoutItem * currentItem = m_secondCameraLayout->takeAt( 0 );
    if ( currentItem ){
          m_bottomAreaLayout->addWidget( currentItem->widget() );
          currentItem->widget()->setFixedSize( sizes );
    }
    m_secondCameraLayout->addWidget( widget );
    widget->setFixedSize( m_secondCamera->size() );
}



DoubleCameraWidget::~DoubleCameraWidget()
{
}


