/***************************************************************************
 *   Copyright (C) 2007 by Leandro Emanuel López                           *
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
#include "camerawidgettoolbar.h"

#include "camerawidget.h"

#include <QToolButton>
#include <QHBoxLayout>

CameraWidgetToolBar::CameraWidgetToolBar ( QWidget * parent )
        : QWidget ( parent )
{
    init();
}

void CameraWidgetToolBar::init()
{

    setSizePolicy ( QSizePolicy::Minimum , QSizePolicy::Minimum );
    QLayout * m_buttonsLayout = new QHBoxLayout ( this );
    m_buttonsLayout->setMargin ( 0 );
    setLayout ( m_buttonsLayout );
    //buttons
    m_startButton = new QToolButton ( this );
    m_startButton->setIcon ( QIcon ( ":/icons/Start" ) );

    m_buttonsLayout->addWidget ( m_startButton );
    m_startButton->setToolTip ( "Reproducir camara" );
    m_stopButton = new QToolButton ( this );
    m_stopButton->setIcon ( QIcon ( ":/icons/Stop" ) );
    m_stopButton->setToolTip ( "Parar camara" );

    m_buttonsLayout->addWidget ( m_stopButton );
    m_pauseButton = new QToolButton ( this );
    m_pauseButton->setIcon ( QIcon ( ":/icons/Pause" ) );
    m_pauseButton->setToolTip ( "Pausar camara" );

    m_buttonsLayout->addWidget ( m_pauseButton );
    m_fullScreenButton = new QToolButton ( this );
    m_fullScreenButton->setIcon ( QIcon ( ":/icons/FullScreen" ) );
    m_fullScreenButton->setToolTip ( "Mostrar camara en pantalla completa" );

    m_buttonsLayout->addWidget ( m_fullScreenButton );
    m_removeButton = new QToolButton ( this );
    m_removeButton->setIcon ( QIcon ( ":/icons/Remove" ) );
    m_removeButton->setToolTip ( "Borrar Camara" );
    m_buttonsLayout->addWidget ( m_removeButton );


    m_configButton = new QToolButton ( this );
    m_configButton->setIcon ( QIcon ( ":/icons/Setup" ) );
    m_configButton->setToolTip ( "Configurar camara" );
    m_buttonsLayout->addWidget ( m_configButton );

    QSpacerItem * spacerItem = new QSpacerItem ( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    m_buttonsLayout->addItem ( spacerItem );

}

void CameraWidgetToolBar::autoConnectWith ( CameraWidget * camera )
{
    connect ( m_startButton , SIGNAL ( clicked() ) , camera , SLOT ( startCamera() ) );
    connect ( m_stopButton , SIGNAL ( clicked() ) , camera , SLOT ( stopCamera() ) );
    connect ( m_pauseButton , SIGNAL ( clicked() ) , camera , SLOT ( pauseCamera() ) );
    connect ( m_fullScreenButton , SIGNAL ( clicked() ) , camera , SLOT ( fullScreen() ) );
    connect ( m_removeButton , SIGNAL ( clicked() ) , camera , SLOT ( remove() ) );
    connect ( m_configButton , SIGNAL ( clicked() ) , camera , SLOT ( configCamera() ) );
}

void CameraWidgetToolBar::setRemoveActionState( bool state ){
    m_removeButton->setEnabled( state );
}
void CameraWidgetToolBar::setConfigActionState( bool state ){
    m_configButton->setEnabled( state );
}

CameraWidgetToolBar::~CameraWidgetToolBar()
{}


