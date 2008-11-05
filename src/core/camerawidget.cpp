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
#include "cameraevents.h"
#include "framewidget.h"
#include "models/eventmodel.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QMessageBox>
#include <QMenu>
#include <QPaintEvent>

//local includes
#include "stream.h"
#include "cameraadddialog.h"
#include "fullscreencamera.h"
#include "camerawidgettoolbar.h"
#include "camerawidget.h"

class CameraWidget::Private{
    public:
        Private():highQuality(false),cameraEvent(0){}
        QSize size;
        bool highQuality;
        CameraType cameraType;
        CameraEvents *cameraEvent;
        FocusBehavior focusBehavior;
        Qt::AspectRatioMode aspectRatioMode;
        QString cameraName;
        FrameWidget::Status state;
        FrameWidget::Status savedState;
        mutable QString uniqueId;
        bool restoreStateOnShow;
        EventModel  * eventModel;
};

CameraWidget::CameraWidget( const QString &conectionName, QWidget *parent)
 : QFrame( parent),m_autoAdjustImage( false ),m_toggleViewAction(0),m_focusAction(0),m_promoteToMainCameraAction(0),m_promoteToSecondCameraAction(0),
d( new Private )
{
    m_conectionName = conectionName;
    d->eventModel = new EventModel( conectionName, this );
    init();
}

CameraWidget::CameraWidget( const CameraWidget & other ){
    this->m_toolbar = other.m_toolbar;
    this->m_stream = other.m_stream;
    this->m_autoAdjustImage = other.m_autoAdjustImage;
    this->m_conectionName = other.m_conectionName;
    this->d = other.d;
}

void CameraWidget::init(){
    setWindowIcon ( QIcon (":/icons/MdiIcon"));
    setSizePolicy(QSizePolicy::Minimum , QSizePolicy::Minimum);

    m_stream = new Stream( this );
    m_frame = new QFrame ( this );
    m_frame->setSizePolicy(QSizePolicy::Expanding , QSizePolicy::Expanding );
    QLayout * m_frameLayout = new QVBoxLayout ( m_frame );
    m_frameLayout->setMargin(0);
    m_frameWidget = new FrameWidget ( m_frame );
    m_frameLayout->addWidget ( m_frameWidget );

    m_toolbar = new CameraWidgetToolBar ( this );
    m_toolbar->autoConnectWith( this );

    //widget
    m_layout = new QVBoxLayout ( this );
    m_layout->setMargin(1);
    m_layout->setAlignment( Qt::AlignVCenter | Qt::AlignHCenter );
    m_frameWidget->setStatus( FrameWidget::NoSignal );


    m_spacerV = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    m_layout->addWidget ( m_toolbar );
    m_layout->addWidget ( m_frame );
    m_layout->addItem(m_spacerV);

    m_menu = m_toolbar->menu ();
    setContextMenuPolicy ( Qt::CustomContextMenu );
    connect ( this, SIGNAL(customContextMenuRequested ( const QPoint &  )) , this , SLOT( popupMenu ( const QPoint & ) ) );

    adjustSize( );
    setFocusPolicy( Qt::StrongFocus );
    setBackgroundRole( QPalette::Shadow );
    setAutoFillBackground( true );
    d->cameraType = Monitor;
    d->focusBehavior = None;
    d->aspectRatioMode = Qt::KeepAspectRatio;
    d->restoreStateOnShow = false;

    connect ( d->eventModel, SIGNAL( eventsDetected( int ) ), this, SLOT( newEvents( int ) ) );

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
    d->state = FrameWidget::Playing;
    m_stream->start();
    connect ( m_stream , SIGNAL ( frameReady ( QPixmap *) ) , this , SLOT (setPixmap (QPixmap *)));
    connect ( m_stream , SIGNAL ( done ( QString ) ) , m_frameWidget , SLOT ( setErrorMessage ( QString ) ) );
    if ( d->cameraType == Monitor ){
        m_frameWidget->setStatus( FrameWidget::Playing );
        d->eventModel->setCamera( stream()->monitor() );
        d->eventModel->startEventTracker();
    }
}

void CameraWidget::pauseCamera( ){
    d->state = FrameWidget::Paused;
    m_stream->stop();
    disconnect ( m_stream , SIGNAL ( frameReady ( QPixmap *) ) , this , SLOT (setPixmap (QPixmap *)));
    disconnect ( m_stream , SIGNAL ( done ( QString ) ) , m_frameWidget , SLOT ( setErrorMessage ( QString ) ) );
    m_frameWidget->setStatus( FrameWidget::Paused );
    m_frameWidget->update();
}
void CameraWidget::stopCamera(){
    d->state = FrameWidget::Stopped;
    m_stream->stop();
    disconnect ( m_stream , SIGNAL ( frameReady ( QPixmap *) ) , this , SLOT (setPixmap (QPixmap *)));
    disconnect ( m_stream , SIGNAL ( done ( QString ) ) , m_frameWidget , SLOT ( setErrorMessage ( QString ) ) );
    m_frameWidget->setStatus( FrameWidget::Stopped );
    m_frameWidget->setPixmap( QPixmap());

    if ( d->cameraType == Monitor )
        d->eventModel->stopEventTracker();
    m_frameWidget->setHasNewEvents( false );
}
void CameraWidget::restartCamera(){
    stopCamera();
    startCamera();
    m_stream->start();
}

void CameraWidget::setPixmap ( QPixmap * p){
  QWidget * window = this->window();
  if ( isVisible() && isVisibleTo( window ) ) {
        if (p->isNull()){
            qDebug( "CameraWidget::setPixmap: is Null" );
            return;
        }

        if (m_autoAdjustImage){
            m_frameWidget->setPixmap ( *p );
        }
        m_frameWidget->setStatus( FrameWidget::Playing );

    }else{
        if ( !d->restoreStateOnShow ){
            d->restoreStateOnShow = true;
            saveState();
            pauseCamera();
        }
    }

}

void CameraWidget::configCamera(){
    CameraAddDialog d( this );
    d.setEditable( false );
    d.setWindowTitle(tr("View Setup"));
    d.setValues( windowTitle(), stream()->host() , QString::number(stream()->port() ) , QString::number(stream()->monitor()), stream()->zmStreamServer() );
    d.exec();
    /*
    if ( d.exec() == QDialog::Accepted ){
            setWindowTitle(d.m_name->text());
            stream()->setHost( d.m_host->text()  , d.m_port->text().toInt() );
            stream()->setMonitor(d.m_monitor->text().toInt() );
            stream()->setZMStreamServer(d.m_zms->text());
            restartCamera();
            emit ( windowTitleChanged( d.m_name->text() ) );
    }
    */
}

void CameraWidget::fullScreen(){
    FullScreenCamera w( this );
    w.setStream ( m_stream, d->state );
    if ( d->cameraType == EventViewer ) m_stream->start();
    emit( fullScreen ( this ) );
    w.exec();
    emit( fullScreenClosed ( this ) );
    d->state = w.status();
    loadFromState();

}

void CameraWidget::setConfigActionState( bool state ){
    m_toolbar->setConfigActionState( state );
}
QAction * CameraWidget::toggleViewAction(){
    if (!m_toggleViewAction){
        m_toggleViewAction = new QAction (windowTitle() , this );
        m_toggleViewAction->setCheckable ( true );
        connect ( m_toggleViewAction , SIGNAL (triggered( bool )), this , SLOT (setVisible(bool) ) );
    }
   return m_toggleViewAction;
}
QAction * CameraWidget::focusAction(){
    if (!m_focusAction){
        m_focusAction = new QAction (windowTitle() , this );
        m_focusAction->setCheckable ( true );
        connect ( m_focusAction , SIGNAL (triggered ( bool ) ), this , SLOT ( setFocus (bool) ) );
        connect ( m_focusAction , SIGNAL( changed() ), this, SLOT( changeCameraNumber() ) );
    }
   return m_focusAction;
}

QAction * CameraWidget::promoteToMainCameraAction(){
    if ( !m_promoteToMainCameraAction ){
            m_promoteToMainCameraAction = new QAction ( tr("Set as &Main Camera") , this );
            connect ( m_promoteToMainCameraAction , SIGNAL ( triggered ( bool ) ), this , SLOT ( promoteToMainWidget( ) ) );
    }
    return m_promoteToMainCameraAction;
}

QAction * CameraWidget::promoteToSecondCameraAction(){
    if ( !m_promoteToSecondCameraAction ){
            m_promoteToSecondCameraAction = new QAction ( tr("Set as &Secondary Main Camera") , this );
            connect ( m_promoteToSecondCameraAction , SIGNAL ( triggered ( bool ) ), this , SLOT ( promoteToSecondWidget( ) ) );
    }
    return m_promoteToSecondCameraAction;
}

void CameraWidget::setFocus ( bool b ){
    if( b ) {
        QFrame::setFocus();
        switch ( d->focusBehavior ){
            case PopUpMenu: m_menu->popup( QWidget::mapToGlobal ( QPoint(0,0) ) ); break;
            case PromoteToMainWidget: promoteToMainWidget(); break;
            default: break;
        }
    }
}
bool CameraWidget::event ( QEvent * event ){
    if (m_toggleViewAction ) m_toggleViewAction->setChecked ( this->isVisible());
    return QFrame::event( event );
}

void CameraWidget::cameraEvents(){
    d->eventModel->stopEventTracker();
    m_frameWidget->setHasNewEvents( false );
    if (!d->cameraEvent) d->cameraEvent = new CameraEvents ( stream()->monitor() , m_conectionName , this );
    d->cameraEvent->cameraWidget()->frameWidget()->setAspectRatioMode( m_frameWidget->aspectRatioMode() );
    d->cameraEvent->appendZMSString( stream()->ZMSStringAppended() );
    d->cameraEvent->setWindowTitle(tr( "Events for Monitor %1").arg( windowTitle() ) );
    d->cameraEvent->show();
    d->eventModel->startEventTracker();
}

void CameraWidget::popupMenu ( const QPoint & p){
    m_menu->popup( QWidget::mapToGlobal ( p ) );
}

void CameraWidget::focusInEvent ( QFocusEvent * event ){
    focusAction()->setChecked( true );
    setBackgroundRole( QPalette::Highlight );
    emit( focused( this ) );
}

void CameraWidget::focusOutEvent ( QFocusEvent * event ){
    focusAction()->setChecked( false );
    setBackgroundRole( QPalette::Shadow );
}

void CameraWidget::resizeEvent ( QResizeEvent * event ){
    QFrame::resizeEvent( event );
    d->size = cameraPixmapSizeHint();
}
QSize CameraWidget::cameraPixmapSizeHint() const{
    QSize s = size();
   if ( m_toolbar->isVisible() )
                s.setHeight ( s.height() - m_toolbar->size().height() );
    return s;
}
void CameraWidget::setHighQuality( bool b ){
    d->highQuality = b ;
    m_frameWidget->setTransformationMode( !b ? Qt::FastTransformation : Qt::SmoothTransformation );
}
CameraWidgetToolBar * CameraWidget::toolBar() const{
    return m_toolbar;
}

QSize CameraWidget::imageSize()const
{
    return QSize(320,240);
}

void CameraWidget::setImageSize( QSize s ){
    d->size = s;
}

void CameraWidget::setCameraType ( const CameraType &type ){
    d->cameraType = type;
    if ( type == EventViewer ) m_toolbar->eventViewerSetup();
}

void CameraWidget::setFocusBehavior( const FocusBehavior & behavior ){
    d->focusBehavior = behavior;
}

FrameWidget * CameraWidget::frameWidget() const{
    return m_frameWidget;
}

void CameraWidget::mouseDoubleClickEvent ( QMouseEvent * event ){
if ( d->focusBehavior == PromoteToMainWidget )
    promoteToMainWidget();

 QFrame::mouseDoubleClickEvent( event );
}

void CameraWidget::showEvent ( QShowEvent * event ){
    if ( d->restoreStateOnShow ){
        restoreState();
        d->restoreStateOnShow = false;
    }
    QWidget::showEvent ( event );
}

QString CameraWidget::name() const{
    return d->cameraName;
}
void CameraWidget::setName( const QString &name ){
    d->cameraName = name;
}

void CameraWidget::saveState(){
    d->savedState = d->state;
}
void CameraWidget::restoreState(){
    d->state = d->savedState;
    loadFromState();
}
void CameraWidget::loadFromState(){
    switch ( d->state ){
        case FrameWidget::Playing: startCamera();break;
        case FrameWidget::Stopped: stopCamera();break;
        case FrameWidget::Paused: pauseCamera();break;
        default: restartCamera();break;
    }
}

void CameraWidget::promoteToMainWidget(){
    emit( promotedToMainWidget (this ) );
}

void CameraWidget::promoteToSecondWidget(){
    emit( promotedToSecondWidget (this ) );
}

void CameraWidget::activateMainCameraAction(){
    QAction *act = promoteToMainCameraAction();
    m_menu->addSeparator();
    m_menu->addAction( act );
}
void CameraWidget::activateSecondCameraAction(){
    QAction *act = promoteToSecondCameraAction();
    m_menu->addAction( act );
}

void CameraWidget::removeMainCameraAction(){
    QAction *act = promoteToMainCameraAction();
    m_menu->removeAction( act );
}
void CameraWidget::removeSecondCameraAction(){
    QAction *act = promoteToSecondCameraAction();
    m_menu->removeAction( act );
}

void CameraWidget::setVisible(bool b ){
    QFrame::setVisible( b );
    if ( b ) QFrame::setFocus();
}
void CameraWidget::changeCameraNumber(){
    if ( m_frameWidget )
        m_frameWidget->setFrameId(  m_focusAction->text() );
}

QString CameraWidget::uniqueId() const{
    if ( d->uniqueId.isEmpty() ){
        Q_ASSERT( m_stream );
        d->uniqueId = m_conectionName + "_" + QString::number( m_stream->port() ) + "_" + QString::number( m_stream->monitor() );
    }
    return d->uniqueId;
}

void CameraWidget::newEvents( int count ){
    qDebug( qPrintable(QString("CameraWidget::newEvents: implement! Detected %1 events").arg( QString::number( count ) ) ) );

    bool ignore = d->cameraEvent && d->cameraEvent->isVisible();
    if ( !ignore )
        m_frameWidget->setHasNewEvents( true );
}

CameraWidget::~CameraWidget()
{
    delete m_stream;
    delete m_toolbar;
    delete d->cameraEvent;
    delete d;
}



#include "camerawidget.moc"
