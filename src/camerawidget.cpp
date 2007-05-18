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
#include "camerawidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QMessageBox>
#include <QFrame>

//local includes
#include "stream.h"
#include "cameraadddialog.h"
#include "fullscreencamera.h"
#include "camerawidgettoolbar.h"
#include "camerawidget.h"
//
//TODO:

CameraWidget::CameraWidget(QWidget *parent)
 : QScrollArea( parent),m_autoAdjustImage( false ),m_toggleViewAction(0)
{
    init();
    connect ( this , SIGNAL ( removeMe(CameraWidget *)), parent , SLOT    (removeCamera(CameraWidget *)));
}

CameraWidget::CameraWidget( const CameraWidget & other ){
    this->m_stream = other.m_stream;
    this->m_autoAdjustImage = other.m_autoAdjustImage;
}

void CameraWidget::init(){
    setSizePolicy(QSizePolicy::Minimum , QSizePolicy::Minimum);
    m_stream = new Stream( this );
    //m_toolbar = new CameraToolBar( m_container );
    m_frame = new QFrame ( this );
    m_frame->setSizePolicy(QSizePolicy::Expanding , QSizePolicy::Expanding );
    QLayout * m_frameLayout = new QVBoxLayout ( m_frame );
    m_frameLayout->setMargin(0);
    m_camView = new QLabel ( m_frame );
    m_frameLayout->addWidget ( m_camView );

    m_toolbar = new CameraWidgetToolBar ( this );
    m_toolbar->autoConnectWith( this );

    //widget
    m_layout = new QVBoxLayout ( this );
    m_layout->setMargin(0);

    m_camView->setText ("<b>Sin señal</b>");

    m_spacerV = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    m_layout->addWidget ( m_toolbar );
    m_layout->addWidget ( m_frame );
    m_layout->addItem(m_spacerV);
    adjustSize( );
}

Stream * CameraWidget::stream(){
    return m_stream;
}

void CameraWidget::setStream( Stream * stream){
    m_stream = stream;
}
void CameraWidget::setAutoAdjustImage ( bool b ){
    if (b) m_layout->removeItem( m_spacerV );
    else if (m_autoAdjustImage){
        m_layout->addItem( m_spacerV );
    }
    m_autoAdjustImage = b;
}
void CameraWidget::startCamera(){
    m_stream->start();
    connect ( m_stream , SIGNAL ( frameReady ( QPixmap *) ) , this , SLOT (setPixmap (QPixmap *)));
}

void CameraWidget::pauseCamera( ){
    m_stream->stop();
    disconnect ( m_stream , SIGNAL ( frameReady ( QPixmap *) ) , this , SLOT (setPixmap (QPixmap *)));
}
void CameraWidget::stopCamera(){
    m_stream->stop();
    disconnect ( m_stream , SIGNAL ( frameReady ( QPixmap *) ) , this , SLOT (setPixmap (QPixmap *)));
    m_camView->setText("<b>Camara detenida</b>");
}
void CameraWidget::restartCamera(){
    stopCamera();
    startCamera();
    m_stream->start();
}

void CameraWidget::setPixmap ( QPixmap * p){
  if ( isVisible() ) {
    if (p->isNull()){
        qDebug( "CameraWidget::setPixmap: is Null" );
        return;
    }
    if (!m_autoAdjustImage)
        m_camView->setPixmap ( p->scaled(m_frame->size() , Qt::KeepAspectRatioByExpanding) );
    else m_camView->setPixmap ( p->scaled(m_frame->size() ,Qt::KeepAspectRatioByExpanding) );

}
}

void CameraWidget::configCamera(){
    CameraAddDialog d( this );
    d.setWindowTitle("Configurar camara");
    d.setValues( windowTitle(), stream()->host() , QString::number(stream()->port() ) , QString::number(stream()->monitor()), stream()->zmStreamServer() );
    if ( d.exec() == QDialog::Accepted ){
            setWindowTitle(d.m_name->text());
            stream()->setHost( d.m_host->text()  , d.m_port->text().toInt() );
            stream()->setMonitor(d.m_monitor->text().toInt() );
            stream()->setZMStreamServer(d.m_zms->text());
            restartCamera();
            emit ( windowTitleChanged( d.m_name->text() ) );
    }
}

void CameraWidget::fullScreen(){
    FullScreenCamera * w = new FullScreenCamera();
    w->setStream ( m_stream );
    w->setWindowState( Qt::WindowFullScreen );
    w->show();

}
void CameraWidget::remove(){
    int ret = QMessageBox::warning(this, tr("ZoneMinder Viewer"),
                   tr("Realmente desea borrar la camara?"),
                   QMessageBox::Ok | QMessageBox::Cancel );
    if (ret ==QMessageBox::Ok){
        stopCamera();
        m_camView->setText("<b>La señal removeMe debe de ser atrapada\n para destruir este objecto.</b>");
        emit( removeMe ( this ) );
    }
}

void CameraWidget::setRemoveActionState( bool state ){
    m_toolbar->setRemoveActionState( state );
}
void CameraWidget::setConfigActionState( bool state ){
    m_toolbar->setConfigActionState( state );
}
QAction * CameraWidget::toggleViewAction(){
    if (!m_toggleViewAction){
        m_toggleViewAction = new QAction (windowTitle() , this );
        m_toggleViewAction->setCheckable ( true );
        //m_toggleViewAction->setChecked( true );
        connect ( m_toggleViewAction , SIGNAL (triggered( bool )), this , SLOT (setVisible(bool) ) );
    }
   return m_toggleViewAction;
}

bool CameraWidget::event ( QEvent * event ){
    if (m_toggleViewAction ) m_toggleViewAction->setChecked ( this->isVisible());
    return QWidget::event( event );
}

CameraWidget::~CameraWidget()
{
    delete m_stream;
    delete m_camView;
    delete m_toolbar;
}



