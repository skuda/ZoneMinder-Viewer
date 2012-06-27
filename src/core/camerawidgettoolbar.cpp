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

#include <QAction>
#include <QMenu>

class CameraWidgetToolBar::Private {
 public:
    QAction * m_startAction;
    QAction * m_stopAction;
    QAction * m_pauseAction;
    QAction * m_fullScreenAction;
    QAction * m_configAction;
    QAction * m_eventAction;
    QAction * m_highQualityAction;
    QMenu * m_menu;
    bool isFullScreenMode;
    void setupMenu(){
        m_menu->addAction(m_startAction);
        m_menu->addAction(m_stopAction);
        m_menu->addAction(m_pauseAction);
        if (!isFullScreenMode){
            m_menu->addSeparator();
            m_menu->addAction(m_fullScreenAction);
            m_menu->addAction(m_eventAction);
        }
        m_menu->addSeparator();
        m_menu->addAction(m_highQualityAction);
        if (!isFullScreenMode){
            m_menu->addSeparator();
            m_menu->addAction(m_configAction);
        }
    }
};

CameraWidgetToolBar::CameraWidgetToolBar ( QWidget * parent )
        : QToolBar ( "Camera ToolBar" , parent ), d( new Private )
{
    d->isFullScreenMode = false;
    init();
}

CameraWidgetToolBar::CameraWidgetToolBar ( bool fullScreenMode, QWidget * parent )
        : QToolBar ( "Camera ToolBar" , parent ), d( new Private )
{
    d->isFullScreenMode = fullScreenMode;
    init();
}

void CameraWidgetToolBar::init()
{

    setSizePolicy ( QSizePolicy::Minimum , QSizePolicy::Minimum );
    setIconSize( QSize ( 16 , 16 ) );

    d->m_startAction = new QAction ( tr("Play") , this );
    d->m_startAction->setIcon ( QIcon ( ":/icons/Start" ) );
    addAction ( d->m_startAction );

    d->m_startAction->setToolTip ( tr("Play Video Camera") );
    d->m_stopAction = new QAction ( tr("Stop") , this );
    d->m_stopAction->setIcon ( QIcon ( ":/icons/Stop" ) );
    d->m_stopAction->setToolTip ( tr("Stop Video Camera") );
    addAction ( d->m_stopAction );

    d->m_pauseAction = new QAction ( tr("Pause") , this );
    d->m_pauseAction->setIcon ( QIcon ( ":/icons/Pause" ) );
    d->m_pauseAction->setToolTip ( tr("Pause Video Camera") );
    addAction ( d->m_pauseAction );

    addSeparator();
    if (!d->isFullScreenMode){
        d->m_fullScreenAction = new QAction (tr("FullScreen Mode"), this );
        d->m_fullScreenAction->setIcon ( QIcon ( ":/icons/FullScreen" ) );
        d->m_fullScreenAction->setToolTip ( tr("FullScreen Mode" ) );
        addAction ( d->m_fullScreenAction );

        d->m_eventAction = new QAction ( tr("Event List...") , this );
        d->m_eventAction->setIcon ( QIcon ( ":/icons/Events" ) );
        d->m_eventAction->setToolTip ( tr("Event List") );
        addAction ( d->m_eventAction );
    }
    addSeparator();

    d->m_highQualityAction = new QAction ( tr("Bilinear Filtering") , this );
    d->m_highQualityAction->setCheckable( true );
    d->m_highQualityAction->setChecked( false );
    d->m_highQualityAction->setIcon ( QIcon ( ":/icons/HighQuality" ) );
    d->m_highQualityAction->setToolTip ( tr("Transform image using bilinear filtering") );
    addAction ( d->m_highQualityAction );

    addSeparator();
    if (!d->isFullScreenMode){
        d->m_configAction = new QAction ( tr("View Setup...") , this );
        d->m_configAction->setIcon ( QIcon ( ":/icons/Setup" ) );
        d->m_configAction->setToolTip ( tr("View Setup of Current Video Camera") );
        addAction ( d->m_configAction );
    }

    toggleViewAction()->setText( tr( "Show ToolBar" ) );
    toggleViewAction()->setIcon( QIcon(":/icons/ToolBar") );
    toggleViewAction()->setToolTip( tr("Hide/Show Camera's ToolBar") );
    addAction ( toggleViewAction() );

    d->m_menu = new QMenu ("Actions" , this );
    d->setupMenu();
    d->m_menu->addAction( toggleViewAction() );

    setBackgroundRole( QPalette::Base );
    setAutoFillBackground( true );

    setStyleSheet("QToolBar{ background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 gray, stop: 1 black);}");
}

void CameraWidgetToolBar::autoConnectWith ( CameraWidget * camera )
{
    connect ( d->m_startAction , SIGNAL ( triggered() ) , camera , SLOT ( startCamera() ) );
    connect ( d->m_stopAction , SIGNAL ( triggered() ) , camera , SLOT ( stopCamera() ) );
    connect ( d->m_pauseAction , SIGNAL ( triggered() ) , camera , SLOT ( pauseCamera() ) );
    connect ( d->m_fullScreenAction , SIGNAL ( triggered() ) , camera , SLOT ( fullScreen() ) );
    connect ( d->m_eventAction , SIGNAL ( triggered() ) , camera , SLOT ( cameraEvents() ) );
    connect ( d->m_configAction , SIGNAL ( triggered() ) , camera , SLOT ( configCamera() ) );
    connect ( d->m_highQualityAction , SIGNAL ( toggled( bool ) ) , camera , SLOT ( setHighQuality( bool ) ) );
}

void CameraWidgetToolBar::setConfigActionState( bool state ){
    d->m_configAction->setEnabled( state );
}

QMenu * CameraWidgetToolBar::menu() const{
    return d->m_menu;
}
void CameraWidgetToolBar::eventViewerSetup(){
    removeAction( d->m_eventAction );
    d->m_menu->removeAction( d->m_eventAction );
}

QAction * CameraWidgetToolBar::playAction(){
    return d->m_startAction;
}
QAction * CameraWidgetToolBar::pauseAction(){
    return d->m_pauseAction;
}
QAction * CameraWidgetToolBar::stopAction(){
    return d->m_stopAction;
}
QAction * CameraWidgetToolBar::fullScreenAction(){
    return d->m_fullScreenAction;
}
QAction * CameraWidgetToolBar::highQualityAction(){
    return d->m_highQualityAction;
}

CameraWidgetToolBar::~CameraWidgetToolBar()
{
    delete d;
}



