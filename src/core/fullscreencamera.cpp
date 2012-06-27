/***************************************************************************
 *   Copyright (C) 2007 by Leandro Emanuel López                           *
 *   lopezlean@gmail.com  				                   *
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
#include "fullscreencamera.h"
#include "stream.h"
#include "camerawidget.h"
#include "camerawidgettoolbar.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QShowEvent>
#include <QMenu>

FullScreenCamera::FullScreenCamera(QWidget * parent)
:QDialog( parent )
{
    init();
}

void FullScreenCamera::init(){
    setWindowState( Qt::WindowFullScreen );
    QVBoxLayout * layout = new QVBoxLayout( this );
    QPushButton * pb = new QPushButton ( tr("&Close"), this );
    m_toolBar = new CameraWidgetToolBar ( true, this );
    m_toolBar->setVisible( false );
    m_frameWidget = new FrameWidget( this );
    setBackgroundRole( QPalette::Dark );
    layout->addWidget ( pb ); 
    layout->addWidget ( m_toolBar );
    layout->addWidget ( m_frameWidget );
    m_toolBar->setFixedHeight( 32 );
    m_toolBar->setIconSize( QSize( 32, 32 ) );
    m_menu = m_toolBar->menu();
    setContextMenuPolicy ( Qt::CustomContextMenu );
    connect ( this, SIGNAL(customContextMenuRequested ( const QPoint &  )) , this , SLOT( popupMenu ( const QPoint & ) ) );
    connect ( pb , SIGNAL (clicked()), this , SLOT ( close() ) );
    connect ( m_toolBar->playAction() , SIGNAL ( triggered() ) , this , SLOT ( startCamera() ) );
    connect ( m_toolBar->stopAction() , SIGNAL ( triggered() ) , this , SLOT ( stopCamera() ) );
    connect ( m_toolBar->pauseAction() , SIGNAL ( triggered() ) , this , SLOT ( pauseCamera() ) );
    connect ( m_toolBar->highQualityAction() , SIGNAL ( toggled( bool ) ) , this , SLOT ( setHighQuality( bool ) ) );


}
void FullScreenCamera::startCamera(){
    m_stream->start();
    connect ( m_stream , SIGNAL ( frameReady ( QPixmap *) ) , this , SLOT (setPixmap (QPixmap *)));
}
void FullScreenCamera::pauseCamera( ){
    m_stream->stop();
    disconnect ( m_stream , SIGNAL ( frameReady ( QPixmap *) ) , this , SLOT (setPixmap (QPixmap *)));
}
void FullScreenCamera::stopCamera(){
    m_stream->stop();
    disconnect ( m_stream , SIGNAL ( frameReady ( QPixmap *) ) , this , SLOT (setPixmap (QPixmap *)));
    m_frameWidget->setPixmap( QPixmap());
}

void FullScreenCamera::setHighQuality( bool b ){
    m_frameWidget->setTransformationMode( !b ? Qt::FastTransformation : Qt::SmoothTransformation );
}

void FullScreenCamera::setStream( Stream * s ){
    m_stream = s;
    connect ( s , SIGNAL ( frameReady ( QPixmap *) ) , this , SLOT (setPixmap (QPixmap *)) );
}

void FullScreenCamera::setPixmap ( QPixmap * p){
   if (isVisible()){
        m_frameWidget->setPixmap ( *p );
    }
}


void FullScreenCamera::popupMenu ( const QPoint & p ){
    m_menu->popup( QWidget::mapToGlobal ( p ) );
}
FullScreenCamera::~FullScreenCamera()
{
}



