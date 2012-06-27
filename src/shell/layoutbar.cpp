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

#include "layoutbar.h"
#include "cameraviewer.h"
#include "layoutbutton.h"
#include "sliderlayoutbutton.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QSettings>

class LayoutBar::Private{
public:
    CameraViewer * cameraViewer;
    LayoutButton * oneGridLayout;
    LayoutButton * twoGridLayout;
    LayoutButton * threeGridLayout;
    LayoutButton * fourGridLayout;
    LayoutButton * fiveGridLayout;
    LayoutButton * mainCameraLayout;
    LayoutButton * doubleCameraLayout;

    SliderLayoutButton * configurableCameraLayout;
};

LayoutBar::LayoutBar( QWidget * parent )
:QToolBar( parent ), d( new Private )
{
    init();
}

void LayoutBar::init(){
    setWindowTitle ( tr("Layout ToolBar") );
    d->cameraViewer = CameraViewer::instance();
    d->oneGridLayout = new LayoutButton( 1 , this );
    d->oneGridLayout->setObjectName( "oneGridLayout" );
    d->twoGridLayout = new LayoutButton( 2 , this );
    d->twoGridLayout->setObjectName( "twoGridLayout" );
    d->threeGridLayout = new LayoutButton( 3 , this );
    d->threeGridLayout->setObjectName( "threeGridLayout" );
    d->fourGridLayout = new LayoutButton( 4 , this );
    d->fourGridLayout->setObjectName( "fourGridLayout" );
    d->fiveGridLayout = new LayoutButton( 5 , this );
    d->fiveGridLayout->setObjectName( "fiveGridLayout" );
    d->mainCameraLayout = new LayoutButton( -1 , this );
    d->mainCameraLayout->setObjectName( "mainCameraLayout" );
    d->configurableCameraLayout = new SliderLayoutButton( this );
    d->configurableCameraLayout->setObjectName( "configurableCameraLayout" );
    d->configurableCameraLayout->setIcon( QIcon(":/shellicons/GridDynamicCameras") );

    d->mainCameraLayout->setIcon( QIcon(":/shellicons/GridMainCamera") );
    d->doubleCameraLayout = new LayoutButton( -1 , this );
    d->doubleCameraLayout->setIcon( QIcon(":/shellicons/GridTwoMainCamera") );

    addWidget( d->oneGridLayout );
    addWidget( d->twoGridLayout );
    addWidget( d->threeGridLayout );
    addWidget( d->fourGridLayout );
    addWidget( d->fiveGridLayout );
    addWidget( d->mainCameraLayout );
    addWidget( d->doubleCameraLayout );
    addWidget( d->configurableCameraLayout );

    connect ( d->oneGridLayout , SIGNAL (clicked ( int ) ) , d->cameraViewer , SLOT( layoutWidgets( int ) ) ) ;
    connect ( d->twoGridLayout , SIGNAL (clicked ( int ) ) , d->cameraViewer , SLOT( layoutWidgets( int ) ) ) ;
    connect ( d->threeGridLayout , SIGNAL (clicked ( int ) ) , d->cameraViewer , SLOT( layoutWidgets( int ) ) ) ;
    connect ( d->fourGridLayout , SIGNAL (clicked ( int ) ) , d->cameraViewer , SLOT( layoutWidgets( int ) ) ) ;
    connect ( d->fiveGridLayout , SIGNAL (clicked ( int ) ) , d->cameraViewer , SLOT( layoutWidgets( int ) ) ) ;
    connect ( d->mainCameraLayout , SIGNAL ( clicked () ) , d->cameraViewer , SLOT( mainCameraWidgetLayout(  ) ) ) ;
    connect ( d->doubleCameraLayout , SIGNAL ( clicked () ) , d->cameraViewer , SLOT( doubleCameraWidgetLayout(  ) ) ) ;
    connect ( d->configurableCameraLayout , SIGNAL ( valueChanged ( int ) ) , d->cameraViewer ,
              SLOT( layoutWidgets( int ) ) );

    connect ( d->oneGridLayout , SIGNAL (clicked ( ) ) , this , SLOT( uncheckToolButtons() ) ) ;
    connect ( d->twoGridLayout , SIGNAL (clicked () ) , this , SLOT( uncheckToolButtons()) ) ;
    connect ( d->threeGridLayout , SIGNAL (clicked () ) , this , SLOT( uncheckToolButtons() ) ) ;
    connect ( d->fourGridLayout , SIGNAL (clicked () ) , this , SLOT( uncheckToolButtons() ) ) ;
    connect ( d->fiveGridLayout , SIGNAL (clicked () ) , this , SLOT( uncheckToolButtons() ) ) ;
    connect ( d->mainCameraLayout , SIGNAL ( clicked () ) , this , SLOT( uncheckToolButtons() ) ) ;
    connect ( d->doubleCameraLayout , SIGNAL ( clicked () ) , this , SLOT( uncheckToolButtons() ) ) ;
    connect ( d->configurableCameraLayout , SIGNAL ( clicked () ) , this , SLOT(uncheckToolButtons()) );

    initSettings();
}

void LayoutBar::uncheckToolButtons(){
    foreach( QObject * child, children() ){
        if ( child == sender() ){
             if ( child->inherits( "QAbstractButton" ) ){
             qobject_cast <QAbstractButton *> ( child )->setChecked ( true );
             saveSettings( qobject_cast <QAbstractButton *> ( child )->objectName ( ) );
             continue;
            }
        }
        if ( child->inherits( "QAbstractButton" ) ){
            qobject_cast <QAbstractButton *> ( child )->setChecked ( false );
        }
    }
}

void LayoutBar::saveSettings( const QString & currentButton ){
    QSettings setting;
    setting.beginGroup( "LayoutBar" );
    setting.setValue( "current", currentButton );
    if ( currentButton == "configurableCameraLayout" ){
        setting.setValue( "value", d->configurableCameraLayout->slider()->value() );
    }
    setting.endGroup();
}

void LayoutBar::initSettings(){
    QSettings setting;
    setting.beginGroup( "LayoutBar" );
    QString currentLayout = setting.value( "current" , "fourGridLayout" ).toString();
    int value = setting.value( "value" , 4 ).toInt();
    setting.endGroup();
    QAbstractButton * button = findChild<QAbstractButton *>( currentLayout );
    if ( button ) {
        if ( currentLayout == "configurableCameraLayout" ){
            button->setChecked( true );
            d->configurableCameraLayout->slider()->setValue( value );
        }
        else
            button->click();
    }
}

LayoutBar::~LayoutBar()
{
    delete d;
}
