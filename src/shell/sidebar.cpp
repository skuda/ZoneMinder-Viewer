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

#include "sidebar.h"
#include "cameraviewer.h"
#include "camerawidget.h"
#include "camerawidgettoolbar.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QToolBar>
#include <QPushButton>
#include <QActionGroup>
#include <QLabel>
#include <QPainter>

class VerticalText : public QWidget
{
public:
    VerticalText(QWidget *parent = 0)
    : QWidget(parent){ text = "No text";}

    void paintEvent(QPaintEvent *)
    {
        QPainter p(this);
        drawRotatedText(&p, 90, width() / 2, 0, text);
    }
    void drawRotatedText(QPainter *painter, float degrees, int x, int y, const QString &text)
    {
        painter->save();
        painter->drawImage( QPoint( x  , y ) , QImage(":/shellicons/MiniCamera") );
        painter->translate(x, y + 20);
        painter->rotate(degrees);
        painter->drawText(0, 0, text);
        painter->restore();
    }
    QString text;
};


class ControlBoxLayout : public QGridLayout{
    public:
        ControlBoxLayout( QWidget * parent = 0 ):QGridLayout( parent ){}
        void clear(){
                 for (int r = 0; r < rowCount(); ++r ){
                     for (int c = 0; c < columnCount(); ++c ){
                        QLayoutItem *item = itemAtPosition( r, c );
                        if ( item ){
                                removeItem( item );
                            QWidget * widget = item->widget();
                            if ( widget ) delete widget;
                        }
                      }
                }
        }
};

LayoutButton::LayoutButton( int columns, QWidget * parent )
:QToolButton(parent),m_columns( columns ){
    setFixedSize( QSize( 48,48) );
    setIconSize(QSize( 48,48));
    m_layoutButtonType = ColumnLayout;
    connect( this, SIGNAL (clicked () ), this, SLOT( changeLayout() )  );
}
int LayoutButton::columns(){ 
    return m_columns;
}

void LayoutButton::setButtonType( const LayoutButtonType &type ){
    m_layoutButtonType = type;
    if ( m_layoutButtonType != ColumnLayout ){
        disconnect( this, SIGNAL (clicked () ), this, SLOT( changeLayout() )  );
    }
}

void LayoutButton::setChecked( int column ){
    QToolButton::setChecked( column == m_columns );
}
void LayoutButton::changeLayout(){
    emit( clicked( m_columns ) );
}


SideBar::SideBar( CameraViewer * cameraViewerInstance, QWidget * parent )
:QWidget ( parent ),m_controlBoxLayout(0)
{
    m_cameraViewer = cameraViewerInstance;
    init();
}


void SideBar::init(){
    QVBoxLayout * layout = new QVBoxLayout();
    m_layoutBox = new QGroupBox( tr ("Layout") , this );
    m_controlsBox = new QGroupBox( tr ("Controls") , this );
    layout->addWidget( m_layoutBox );
    
    QVBoxLayout * lbox_vlayout = new QVBoxLayout( );
    m_layoutBox->setLayout( lbox_vlayout );
    QHBoxLayout * lbox_layout = new QHBoxLayout( );
    lbox_layout->setSpacing(1);
    lbox_layout->setMargin(1);
    for ( int i=2; i<6 ; i++ ){
        LayoutButton * lb = new LayoutButton( i , m_controlsBox );
        connect (lb , SIGNAL( clicked( int ) ), m_cameraViewer ,  SLOT(layoutWidgets( int ))   );
        connect ( m_cameraViewer , SIGNAL( layoutChanged( int ) ), lb ,  SLOT(setChecked( int ))   );
        lb->setIcon( QIcon(QString(":/shellicons/Grid")+QString::number( i ) )) ;
        lb->setToolTip( tr("%1 Columns Layout" ).arg(QString::number( i ) ) );
        lb->setCheckable( true );
        lbox_layout->addWidget( lb );
    }

    LayoutButton * mainCameraLayoutButton = new LayoutButton( 8 , m_controlsBox );
    mainCameraLayoutButton->setToolTip( tr("Main Camera Layout" ) );
    mainCameraLayoutButton->setCheckable( true );
    mainCameraLayoutButton->setIcon( QIcon(":/shellicons/GridMainCamera" ) ) ;
    lbox_layout->addWidget( mainCameraLayoutButton );
    connect(mainCameraLayoutButton, SIGNAL( clicked() ), m_cameraViewer, SLOT(mainCameraWidgetLayout()) );
    connect( m_cameraViewer, SIGNAL( cameraFocused( QWidget *) ), this,    SLOT(setActionToolBarActions( QWidget *)) );
    
    LayoutButton * doubleCameraLayoutButton = new LayoutButton( 9 , m_controlsBox );
    doubleCameraLayoutButton->setToolTip( tr("Double Camera Layout" ) );
    doubleCameraLayoutButton->setCheckable( true );
    doubleCameraLayoutButton->setIcon( QIcon(":/shellicons/GridMainCamera" ) ) ;
    lbox_layout->addWidget( doubleCameraLayoutButton );
    connect(doubleCameraLayoutButton, SIGNAL( clicked() ), m_cameraViewer, SLOT(doubleCameraWidgetLayout()) );


    if(!m_controlBoxLayout)
        m_controlBoxLayout = new ControlBoxLayout;

    lbox_vlayout->addLayout( lbox_layout );
    lbox_vlayout->addItem( new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding) );

    QHBoxLayout * controlBoxLayout = new QHBoxLayout;

    QVBoxLayout * cameraLabelLayout = new QVBoxLayout;
    m_cameraInfoLabel = new VerticalText( this );
    m_cameraInfoLabel->text = tr("None Selected");
    cameraLabelLayout->addWidget( m_cameraInfoLabel );

    QVBoxLayout * controlbox_vlayout = new QVBoxLayout;
    controlbox_vlayout->addLayout( m_controlBoxLayout );


    m_actionBar = new QToolBar( this );
    m_actionBar->addAction( QIcon(":/icons/Start"), tr("Play") )->setEnabled(false);
    m_actionBar->addAction( QIcon(":/icons/Pause"), tr("Pause") )->setEnabled(false);
    m_actionBar->addAction( QIcon(":/icons/Stop"), tr("Stop") )->setEnabled(false);
    m_actionBar->addAction( QIcon(":/icons/FullScreen"), tr("FullScreen") )->setEnabled(false);

    controlbox_vlayout->addWidget( m_actionBar );


    controlbox_vlayout->addItem( new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding) );
    controlBoxLayout->addLayout( controlbox_vlayout  );
    controlBoxLayout->addLayout( cameraLabelLayout );
    m_controlsBox->setLayout ( controlBoxLayout );
    m_controlsBox->setVisible( false );
    layout->addWidget( m_controlsBox );
    updateControls();
    setLayout( layout );
}

void SideBar::updateControls(){
     m_controlBoxLayout->clear();
    int c=0,r = 0;
    foreach( QAction * action, m_cameraViewer->cameraFocusActions()->actions() ){
            QToolButton * btn = new QToolButton( this );
            btn->setFixedWidth(30);
            btn->setDefaultAction( action );
            m_controlBoxLayout->setSpacing(1);
            m_controlBoxLayout->setMargin(5);
            m_controlBoxLayout->addWidget( btn, r, c );
            c++;
            if ( c > 4 ){
                c=0;r++;
            };
    }
}
void SideBar::setActionToolBarActions( QWidget * widget){
    m_actionBar->clear();
    CameraWidget *camera = qobject_cast<CameraWidget *>( widget );
    m_actionBar->addAction(camera->toolBar()->playAction());
    m_actionBar->addAction(camera->toolBar()->pauseAction());
    m_actionBar->addAction(camera->toolBar()->stopAction());
    m_actionBar->addAction(camera->toolBar()->fullScreenAction());
    m_cameraInfoLabel->text = camera->windowTitle();
    m_cameraInfoLabel->repaint();

}
SideBar::~SideBar()
{
}

#include "sidebar.moc"

