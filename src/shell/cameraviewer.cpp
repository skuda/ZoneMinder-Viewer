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
#include "cameraviewer.h"

#include "cameraviewerpage.h"
#include "maincamerawidget.h"
#include "doublecamerawidget.h"
#include "camerawidget.h"
#include "framewidget.h"
#include "camerawidgettoolbar.h"
#include "stream.h"

#include "cameralayout.h"

#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QPalette>
#include <QColor>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QMap>
#include <QQueue>
#include <QActionGroup>
#include <QKeySequence>
#include <QMenu>
#include <QTabWidget>
#include <QTabBar>
#include <QSettings>

CameraViewer * CameraViewer::m_instance = 0;

class TabWidget: public QTabWidget{
    public:
        TabWidget( QWidget * parent ):QTabWidget( parent ){ 
            tabBar()->setVisible( true );
        }
        int addTab ( QWidget * page ){
            int i = QTabWidget::addTab( page , QIcon(":/shellicons/MiniCamera"), QString::number(count()+1) + " " );
            tabBar()->setVisible( count() > 1  );
            if ( tabBar()->isVisible() ) {
                //unset minimum size
                widget( 0 )->setMinimumSize( 0,0 );
            }
            adjustSize();
            return i;
        }
};


/* don't seems to be used
  ServerLabel::ServerLabel( const QString &text, QWidget * parent )
        : QFrame( parent ){
            setObjectName("ServerLabel");
            QHBoxLayout * layout = new QHBoxLayout;
            label = new QLabel ( text , this );
            layout->addWidget( label );
            setLayout( layout );
            setAutoFillBackground( true );
            setFrameStyle( QFrame::StyledPanel );
            QColor color( 255,255,255 );
            QPalette palette( color );
            setPalette( palette );
            label->setAlignment ( Qt::AlignHCenter );
            label->setSizePolicy( QSizePolicy::Maximum , QSizePolicy::Maximum);
    }
*/

class CameraViewer::Private{
    public:
        Private():showLabel( false ), showLabels( false ), cameraCount(0),
                  mainCameraLayout(false), mainCameraPointer(0) {}

        QString current_server;
        bool showLabel;
        bool showLabels;
        int cameraCount;
        int currentColumnLayout;
        QMap< QString, QQueue <QWidget *> >widgets;
        QActionGroup * cameraActions;
        QWidgetList cameraViewerPages;
        bool mainCameraLayout;
        QWidget * mainCameraPointer;

        //camera options
        Qt::AspectRatioMode camerasAspectRatio;
        bool showCamerasNumbers;
        Qt::Alignment camerasNumbersAlign;
};


CameraViewer::CameraViewer( QWidget * parent )
:QWidget ( parent ),d( new Private )
{
    init();
}

void CameraViewer::init(){
    d->currentColumnLayout = 0;
    Q_ASSERT_X(!m_instance, "CameraViewer", "there should be only one CameraViewer instance");
    m_instance = this;

    m_scrollArea = new QScrollArea( this );
    container = new TabWidget ( m_scrollArea );
    d->cameraActions = new QActionGroup( this );
    m_scrollArea->setWidget( container );
    m_scrollArea->setWidgetResizable( true );
    m_scrollArea->setBackgroundRole(QPalette::Dark);
    m_scrollArea->setAutoFillBackground( true );

    QVBoxLayout * layout = new QVBoxLayout;
    //m_scrollArea->setLayout( scroll_layout );
    
    layout->addWidget( m_scrollArea  );

    m_gridContainer = new QVBoxLayout( );
    m_gridContainer->setAlignment( Qt::AlignTop );

    container->setLayout( m_gridContainer );
    container->setMinimumSize( QSize( 320, 240 ) );
    container->setTabPosition( QTabWidget::East );

    setContextMenuPolicy ( Qt::CustomContextMenu );
    setLayout( layout );
    initActions();
    initSettings();
}

CameraViewer * CameraViewer::instance(){
    Q_ASSERT( m_instance );
    return m_instance;
}

void CameraViewer::initActions(){
    //no actions yet
}

void CameraViewer::initSettings(){
    QSettings settings;
    settings.beginGroup( "Preferences" );
    d->camerasAspectRatio =  (Qt::AspectRatioMode)settings.value( "aspectRatio",  Qt::KeepAspectRatio ).toInt();
    d->showCamerasNumbers = settings.value( "showCameraNumber", true ).toBool();
    d->camerasNumbersAlign = (Qt::Alignment)settings.value( "cameraNumberPosition",
                                                            (int)(Qt::AlignRight | Qt::AlignTop) ).toInt();
    settings.endGroup();
}

void CameraViewer::appendCamera( QWidget * camera ){
    d->cameraCount++;
    CameraWidget * cameraWidget = (qobject_cast<CameraWidget *> (camera));
    cameraWidget->frameWidget()->setAspectRatioMode( Qt::KeepAspectRatioByExpanding );
    cameraWidget->toolBar()->setVisible( false );
    cameraWidget->frameWidget()->setAspectRatioMode( d->camerasAspectRatio );
    cameraWidget->frameWidget()->setShowNumbers( d->showCamerasNumbers );
    cameraWidget->frameWidget()->setNumbersPosition( d->camerasNumbersAlign );
    connect( cameraWidget , SIGNAL( focused ( QWidget * ) ), this , SLOT( cameraFocus( QWidget *) ) );
    connect( cameraWidget , SIGNAL( fullScreen ( QWidget * ) ), this , SLOT( fullScreenSlot( QWidget *) ) );
    connect( cameraWidget, SIGNAL( fullScreenClosed ( QWidget * ) ), this , SLOT( fullScreenClosedSlot( QWidget *) ) );

    QString s = cameraWidget->stream()->host();
    d->widgets[s].enqueue( camera );
}

void CameraViewer::mainCameraWidgetLayout( bool /*force*/ ){
    m_layoutType = MainCameraLayoutType;
    d->currentColumnLayout = -1;
    MainCameraWidget * w = new MainCameraWidget( this );
    container->clear();
    container->addTab( w );
    QMapIterator< QString, QQueue <QWidget *> > iterator( d->widgets );
    while (iterator.hasNext()) {
             iterator.next();
             QQueue<QWidget *> queue = iterator.value();
             foreach( QWidget * current_widget , queue ){
                current_widget->setVisible( true );
                qobject_cast<CameraWidget *>( current_widget )->removeSecondCameraAction();
                qobject_cast<CameraWidget *>( current_widget )->setFocusBehavior( CameraWidget::PromoteToMainWidget );
                w->appendCamera( current_widget );
            }
         }
}

void CameraViewer::doubleCameraWidgetLayout( bool /*force*/ ){
    m_layoutType = DoubleCameraLayoutType;
    d->currentColumnLayout = -2;
    DoubleCameraWidget * w = new DoubleCameraWidget( this );
    container->clear();
    container->addTab( w );
    QMapIterator< QString, QQueue <QWidget *> > iterator( d->widgets );
    while (iterator.hasNext()) {
             iterator.next();
             QQueue<QWidget *> queue = iterator.value();
             foreach( QWidget * tmp_widget , queue ){
                CameraWidget * current_widget = static_cast<CameraWidget *>(tmp_widget);
                current_widget->setVisible( true );
                current_widget->setFocusBehavior( CameraWidget::PromoteToMainWidget );
                current_widget->activateMainCameraAction();
                current_widget->activateSecondCameraAction();
                w->appendCamera( current_widget );
            }
         }
}

void CameraViewer::layoutWidgets( int columns, bool force ){
    m_layoutType = ColumnsLayoutType;
    if ( !force && ( d->currentColumnLayout == columns ) ) return;
     d->currentColumnLayout = columns;
     container->clear();

     int currentPage = 0;
     container->addTab( new CameraViewerPage ( columns , columns , this ) );
     QMapIterator< QString, QQueue <QWidget *> > iterator( d->widgets );

     while (iterator.hasNext()) {
         iterator.next();
         QQueue<QWidget *> queue = iterator.value();

         foreach( QWidget * tmp_widget , queue ){
             CameraWidget * current_widget = static_cast<CameraWidget *>(tmp_widget);
             current_widget->removeMainCameraAction();
             current_widget->removeSecondCameraAction();
             current_widget->setFocusBehavior( CameraWidget::None );

             if ( ((CameraViewerPage *)container->currentWidget() )->hasSpace() ){
                 current_widget->setVisible( true );
                 ((CameraViewerPage *)container->currentWidget())->appendCamera( current_widget );
             } else {
                 currentPage = container->addTab( new CameraViewerPage ( columns , columns , this ) );
                 container->setCurrentIndex( currentPage );
                 ((CameraViewerPage *)container->currentWidget())->appendCamera( current_widget );
             }
         }
     }

    container->setCurrentIndex( 0 );
}

void CameraViewer::endCameraList(  ){
    updateActions();
}

QActionGroup * CameraViewer::cameraFocusActions(){
    return d->cameraActions;
}

void CameraViewer::resizeEvent ( QResizeEvent * event ){
    QWidget::resizeEvent( event );
    layoutCurrentColLayout();
}

void CameraViewer::fullScreenSlot( QWidget * camera ){
    QMapIterator< QString, QQueue <QWidget *> > iterator( d->widgets );
    while (iterator.hasNext()) {
             iterator.next();
             QQueue<QWidget *> queue = iterator.value();
             foreach( QWidget * current_widget , queue ){
                if (current_widget != camera ){
                    qobject_cast<CameraWidget *>( current_widget )->saveState();
                    qobject_cast<CameraWidget *>( current_widget )->pauseCamera();
                }
         }
    }
}

void CameraViewer::fullScreenClosedSlot( QWidget * camera ){
    QMapIterator< QString, QQueue <QWidget *> > iterator( d->widgets );
    while (iterator.hasNext()) {
             iterator.next();
             QQueue<QWidget *> queue = iterator.value();
             foreach( QWidget * current_widget , queue ){
                if (current_widget != camera )
                    qobject_cast<CameraWidget *>( current_widget )->restoreState();
         }
    }
}

void CameraViewer::updateCameras(){
    /*switch ( m_layoutType ){
        case ColumnsLayoutType: layoutCurrentColLayout(); break;
        case MainCameraLayoutType: mainCameraWidgetLayout( true ); break;
        case DoubleCameraLayoutType: doubleCameraWidgetLayout( true );break;
        default: layoutWidgets ();
    }*/
    layoutCurrentColLayout();
    updateActions();
}

void CameraViewer::updateActions(){
     QMapIterator< QString, QQueue <QWidget *> > iterator( d->widgets );
     int cameraCount = 1;
     while (iterator.hasNext()) {
            iterator.next();
             QQueue<QWidget *> queue = iterator.value();
            foreach( QWidget * current_widget , queue ){
                QAction * action = (qobject_cast<CameraWidget *> (current_widget))->focusAction();
                action->setText( QString::number( cameraCount ) );
                d->cameraActions->addAction( action );
                cameraCount++;
              }
    }
}

void CameraViewer::layoutCurrentColLayout() {
    //TODO: use enums for currentColumnLayout?
    switch( d->currentColumnLayout ){
       case 0: return; break;
       case -1: mainCameraWidgetLayout( true );break;
       case -2: doubleCameraWidgetLayout( true );break;
       default: layoutWidgets(d->currentColumnLayout, true);
    }
}

void CameraViewer::clearCameras(){
    foreach( QAction * action, d->cameraActions->actions() ){
        d->cameraActions->removeAction( action );
    }
    QMapIterator< QString, QQueue <QWidget *> > iterator( d->widgets );
    while (iterator.hasNext()) {
             iterator.next();
             QQueue<QWidget *> queue = iterator.value();
             foreach( QWidget * current_widget , queue ){   
                delete current_widget;
             }
    }
    d->cameraCount = 0;
    d->widgets.clear();
}


void CameraViewer::setMainCameraWidget( QWidget * /*widget*/ ) {}

void CameraViewer::cameraFocus( QWidget *widget){
    emit ( cameraFocused( widget ) );
}

void CameraViewer::setCameraOptions ( const Qt::AspectRatioMode & aspectRatio, bool showCameraNumbers,
                                      const Qt::Alignment & cameraNumbersAlignment ){
     QMapIterator< QString, QQueue <QWidget *> > iterator( d->widgets );
     while (iterator.hasNext()) {
             iterator.next();
             QQueue<QWidget *> queue = iterator.value();
             foreach( QWidget * current_widget , queue ){
                CameraWidget * camera = qobject_cast<CameraWidget *>( current_widget );
                camera->frameWidget()->setAspectRatioMode( aspectRatio );
                camera->frameWidget()->setShowNumbers( showCameraNumbers );
                camera->frameWidget()->setNumbersPosition( cameraNumbersAlignment );
                }
            }
}


CameraViewer::~CameraViewer()
{
    delete d;
}
