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
#include "mainwindow.h"
#include "stream.h"
#include "camerawidget.h"
#include "about.h"
#include "monitors.h"
#include "styles/styledialog.h"

#include "cameraadddialog.h"
#include "multicameraview.h"
#include "cameraselectdialog.h"

//core
#include "databasesession.h"
#include "connectionwidget.h"
#include "connectionmanager.h"

#include <QMenuBar>
#include <QSettings>
#include <QEvent>
#include <QMessageBox>
#include <QStyleFactory>


#include <QMdiArea>
#include <QMdiSubWindow>


MainWindow::MainWindow ( QWidget * parent, Qt::WindowFlags flags )
        :QMainWindow ( parent , flags ),m_fullScreenAction ( 0 ),
        m_closeFullScreenAction ( 0 )
{
    init();
    initActions();
    initMenuBar();

}

void MainWindow::init()
{
    qApp->setWindowIcon ( QIcon ( ":/shellicons/Icon" ) );
    setWindowTitle ( About::applicationName() );
    m_settings = new QSettings ( this );
    m_cameraToggleAction = new QList < QAction *>;
    //m_cameraRemoveAction = new QList < QAction *>;
    m_centralWidget = new QMdiArea ( this );
    setCentralWidget ( m_centralWidget );
    initSettings();

}

void MainWindow::initActions()
{
    m_sessionNew = new QAction ( tr( "&New Session..." ) , this );
    connect ( m_sessionNew , SIGNAL ( triggered() ), this , SLOT ( newSession() ) );

    m_sessionList = new QAction ( tr( "&Session List..." ) , this );
    connect ( m_sessionList , SIGNAL ( triggered() ), this , SLOT ( sessionList() ) );

    m_newCameraAction = new QAction ( QIcon ( ":/shellicons/AddCameraIcon" ),tr( "&Add" ) , this );
    connect ( m_newCameraAction , SIGNAL ( triggered() ), this , SLOT ( cameraAddSlot() ) );
    m_closeFullScreenAction = new QAction ( QIcon ( ":/shellicons/Restore" ),tr( "&Close FullScreen View" ) , this );
    connect ( m_closeFullScreenAction , SIGNAL ( triggered() ), this , SLOT ( showNormal() ) );
    m_fullScreenAction = new QAction ( QIcon ( ":/icons/FullScreen" ),tr( "&FullScreen" ), this );
    connect ( m_fullScreenAction, SIGNAL ( triggered() ), this , SLOT ( fullScreen() ) );
    m_quitAction = new QAction ( QIcon ( ":/icons/Quit" ),tr( "&Quit" ) , this );
    connect ( m_quitAction, SIGNAL ( triggered() ), this , SLOT ( close() ) );
    m_cascadeAction = new QAction ( tr( "&Cascade" ) , this );
    connect ( m_cascadeAction , SIGNAL ( triggered() ) , m_centralWidget , SLOT ( cascadeSubWindows() ) );
    m_tileAction = new QAction ( tr( "&Tile" ) , this );
    connect ( m_tileAction , SIGNAL ( triggered() ) , m_centralWidget , SLOT ( tileSubWindows() ) );
/*    m_arrangeIconsAction = new QAction ( tr( "&Arrange Icons" ),this );
    connect ( m_arrangeIconsAction , SIGNAL ( triggered() ) , workspace() , SLOT ( arrangeIcons() ) );*/

    closeAction = new QAction ( tr( "&Hide" ), this );
    closeAction->setShortcut ( tr( "Ctrl+F4" ) );
    closeAction->setStatusTip ( tr( "Hide active window" ) );
    connect ( closeAction, SIGNAL ( triggered() ), this , SLOT ( hideActiveSubWindow() ) );

    closeAllAction = new QAction ( tr( "Hide &All" ), this );
    closeAllAction->setStatusTip ( tr( "Hide All Sub&Windows" ) );
    connect ( closeAllAction, SIGNAL ( triggered() ),this, SLOT ( hideAllSubWindows() ) );

    m_updateAllMonitorsActions = new QAction ( tr( "Update All Monitors" ), this );
    m_updateAllMonitorsActions->setStatusTip ( tr( "Update all session's monitors" ) );
    connect ( m_updateAllMonitorsActions, SIGNAL ( triggered() ),this, SLOT ( update() ) );
}

void MainWindow::initMenuBar()
{
    QMenu * sessionMenu = menuBar()->addMenu ( tr( "Session" ) );
    sessionMenu->addAction ( m_sessionNew );
    sessionMenu->addAction ( m_sessionList );
    sessionMenu->addSeparator();
    sessionMenu->addAction ( m_quitAction );

    QMenu * camMenu = menuBar()->addMenu ( tr( "Monitors" ) );
    camMenu->addAction ( m_newCameraAction );
    //m_camRemoveMenu = camMenu->addMenu ( tr( "Delete" ) );
    camMenu->addAction ( m_updateAllMonitorsActions );

    QMenu * m_viewMenu = menuBar()->addMenu ( tr( "View" ) );
    m_cameraMenu = m_viewMenu->addMenu ( tr( "&Monitors" ) );
    QAction *m_selectStyleAction = m_viewMenu->addAction ( tr( "&Select Style..." ) );
    connect ( m_selectStyleAction , SIGNAL (triggered()), this , SLOT ( selectStyle() ) );

    QAction * t = m_viewMenu->addAction ( tr( "Customized View..." ) );
    connect ( t, SIGNAL ( triggered() ), this , SLOT ( selectedCamerasSlot () ) );
    for ( int i = 0 ; i < m_cameraToggleAction->count() ; i++ )
    {
        m_cameraMenu->addAction ( m_cameraToggleAction->at ( i ) );
        //m_cameraRemoveAction->insert ( i , m_camRemoveMenu->addAction ( m_cameraToggleAction->at ( i )->text() ) );
        //connect ( m_cameraRemoveAction->at ( i ), SIGNAL ( triggered() ), ( CameraWidget * ) m_centralWidget->subWindowList().at ( i )->widget() , SLOT ( remove () ) );

    }
    m_viewMenu->addSeparator();
    m_viewMenu->addAction ( m_fullScreenAction );
    m_viewMenu->addAction ( m_closeFullScreenAction );
    QMenu * m_windowMenu = menuBar()->addMenu ( tr( "&Window" ) );
    m_windowMenu->addAction ( closeAction );
    m_windowMenu->addAction ( closeAllAction );
    m_windowMenu->addSeparator();
    m_windowMenu->addAction ( m_cascadeAction );
    m_windowMenu->addAction ( m_tileAction );
    //m_windowMenu->addAction ( m_arrangeIconsAction );
    QMenu * m_helpMenu = menuBar()->addMenu ( tr( "&Help" ) );
    connect ( m_helpMenu->addAction ( tr( "About..." ) ), SIGNAL ( triggered () ), this, SLOT ( aboutDialog() ) );
    connect ( m_helpMenu->addAction ( tr( "About Qt..." ) ), SIGNAL ( triggered () ), qApp, SLOT ( aboutQt() ) );

}

void MainWindow::initSettings()
{
    m_settings->beginGroup ( "MainWindow" );
    int ws = m_settings->value ( "windowState" , Qt::WindowMaximized ).toInt();
    setWindowState ( ( Qt::WindowStates ) ws );
    m_settings->value ( "windowState" , Qt::WindowMaximized ).toInt();
    int w = m_settings->value ( "sizeW" , size().width() ).toInt();
    int h = m_settings->value ( "sizeH" , size().height() ).toInt();
    resize ( w , h );
    m_settings->endGroup();
    m_settings->beginGroup ( "Databases" );
    QStringList con = m_settings->value ( "names" ).toStringList();
    QString name;
    m_settings->endGroup();
    foreach ( name , con )
    {
        qDebug ( "%s init...", qPrintable ( name ) );
        m_settings->beginGroup ( name );
        bool b = ConnectionManager::addConnection ( m_settings->value ( "driver" ).toString() , m_settings->value ( "host" ).toString() , m_settings->value ( "database" ).toString(),m_settings->value ( "user" ).toString(),m_settings->value ( "password" ).toString(),m_settings->value ( "port" , 0 ).toInt() , false );
        if (!b)
            QMessageBox::critical( this , tr("Database Error") , tr("Can not connect with Database %1 at host %2").arg(m_settings->value ( "database" ).toString()).arg(m_settings->value ( "host" ).toString()) );
        m_settings->endGroup();
    }
    QWidget *cam;
    m_monitors = new Monitors ( this );
    foreach ( cam , m_monitors->cameras() )
    {
        m_centralWidget->addSubWindow ( cam )->setWindowIcon(cam->windowIcon() );
        m_cameraToggleAction->append ( ((CameraWidget *)cam)->toggleViewAction() );
    }

    StyleDialog::loadFromSettings();
}

void MainWindow::saveSettings()
{
    m_settings->beginGroup ( "MainWindow" );
    m_settings->setValue ( "windowState" , ( int ) windowState() );
    m_settings->setValue ( "sizeW" , size().width() );
    m_settings->setValue ( "sizeH" , size().height() );
    m_settings->endGroup ();

}


bool MainWindow::event ( QEvent * ev )
{
    //Cuando se cierre la ventana q guarde las configuraciones
    if ( ev->type() == QEvent::Close )
    {
        saveSettings();
        return true;
    }
    if ( m_fullScreenAction && m_closeFullScreenAction )
    {
        if ( windowState() == Qt::WindowFullScreen )
        {
            m_fullScreenAction->setEnabled ( false );
            m_closeFullScreenAction->setEnabled ( true );
        }
        else
        {
            m_fullScreenAction->setEnabled ( true );
            m_closeFullScreenAction->setEnabled ( false );
        }
    }
    return QMainWindow::event ( ev );
}

void MainWindow::addCamera ( const QString & name , const QString &host , int port , int monitor , const QString &zms )
{
    /**
        TODO: make this with database!
    */
    CameraWidget * camera = new CameraWidget ( NULL, this );
    camera->setWindowTitle ( name );
    camera->stream()->setHost ( host , port );
    camera->stream()->setMonitor ( monitor );
    camera->stream()->setZMStreamServer ( zms );
    //m_cameraToggleAction->append ( camera->toggleViewAction() );
    //m_cameraMenu->addAction( camera->toggleViewAction() );
    //QAction * a = m_camRemoveMenu->addAction ( name );
    //m_cameraRemoveAction->append ( a );
    //connect ( a, SIGNAL ( triggered() ), camera , SLOT ( remove () ) );
    camera->startCamera();
    QWidget *w = m_centralWidget->addSubWindow ( camera );
    w->show();
    w->setWindowIcon(camera->windowIcon() );
}
void MainWindow::cameraAddSlot()
{
    CameraAddDialog dlg ( this );
    dlg.completeDefaults();
    dlg.setWindowTitle ( tr( "Add Video Camera" ) );
    int r = dlg.exec();
    if ( r == QDialog::Accepted )
    {
        if ( cameraListContainsName ( dlg.m_name->text() ) )
        {
            QMessageBox::critical ( this, tr( "Add Video Camera" ), tr( "Name is already in use.\n Please select other" ) );
        }
        else addCamera ( dlg.m_name->text() , dlg.m_host->text() , dlg.m_port->text().toInt() , dlg.m_monitor->text().toInt() , dlg.m_zms->text() );
    }
}

void MainWindow::fullScreen()
{
    setWindowState ( Qt::WindowFullScreen );
}

#if 0
void MainWindow::removeCamera ( CameraWidget * w )
{
    /** TODO: */
    //    m_centralWidget->removeSubWindow( w );

    w->stopCamera();
    delete w;
    //m_cameraToggleAction->removeAt( pos );
    //m_camRemoveMenu->removeAction( m_cameraRemoveAction->at( pos ) );
    //m_cameraRemoveAction->removeAt( pos );
    //delete w;
}
#endif

void MainWindow::selectedCamerasSlot ()
{
    CameraSelectDialog csd ( this );
    QStringList names;
    QList <QMdiSubWindow *> all_cameras = m_centralWidget->subWindowList();
    for ( int i = 0 ; i < all_cameras.size() ; i ++ )
    {
        names << all_cameras.at ( i )->windowTitle();
    }
    csd.setNames ( names );
    if ( csd.exec() == QDialog::Accepted )
    {
        QList <Stream * > stream ;
        names = csd.names();
        int pos = 0;
        for ( int i = 0 ; i < all_cameras.size() ; i ++ )
            if ( names.contains ( all_cameras.at ( i )->windowTitle() ) )
            {
                stream.insert ( pos , ( ( CameraWidget * ) all_cameras.at ( i )->widget( ))->stream() );
                pos++;
            }
        MultiCameraView * v = new MultiCameraView( );
        v->setUseTabs ( csd.groupViewView() );
        v->setStream ( stream );
        v->showFullScreen();
    }
}

bool MainWindow::cameraListContainsName ( const QString & name )
{
    QList <QMdiSubWindow *> all_cameras = m_centralWidget->subWindowList();
    for ( int i = 0 ; i <  all_cameras.size() ; i ++ )
        if ( name ==   all_cameras.at ( i )->windowTitle() )
            return true;
    return false;
}

QStringList MainWindow::cameraNames()
{
    QStringList names;
    QList <QMdiSubWindow *> all_cameras = m_centralWidget->subWindowList();
    for ( int i = 0 ; i < all_cameras.size() ; i ++ )
        names << all_cameras.at ( i )->windowTitle();
    return names;
}

//Pone una camara de forma central de manera q las otras
// la rodean y esta demas presenta la caracteristica
// de ajustar su tamaño a su espacio
void MainWindow::setCentralWidgetCamera ( QWidget * w )
{
    //TODO:
    QVBoxLayout * layout = new QVBoxLayout ( centralWidget() );
    layout->addWidget ( w );
}

void MainWindow::newSession()
{
    DatabaseSession s;
    connect ( &s , SIGNAL ( needUpdate() ) , this , SLOT ( update() ) );
    s.exec();
}

void MainWindow::sessionList()
{
    ConnectionWidget w;
    connect ( &w , SIGNAL ( needUpdate() ) , this , SLOT ( update() ) );
    w.exec();
}

void MainWindow::aboutDialog()
{
    About about;
    about.showAboutDialog();
}
void MainWindow::update ( )
{
    QMdiSubWindow *w;
    foreach ( w , m_centralWidget->subWindowList () ) {
        w->hide();
        delete w;
    }
    m_monitors->updateMonitors();
    QWidget * cam;
    foreach ( cam , m_monitors->cameras() )
    {
        QWidget *subWindow = m_centralWidget->addSubWindow ( cam );
        subWindow->show();
        subWindow->setWindowIcon( cam->windowIcon() );
    }
    repaint();
}

void MainWindow::hideActiveSubWindow(){
    if ( m_centralWidget->activeSubWindow() )
            m_centralWidget->activeSubWindow()->hide();
}
void MainWindow::hideAllSubWindows(){
    QWidget * sw;
    foreach ( sw , m_centralWidget->subWindowList() )
        sw->hide();
}

void MainWindow::selectStyle(){
    StyleDialog d( this );
    d.exec();
}

MainWindow::~MainWindow()
{
    delete m_cameraToggleAction;
    //delete m_cameraRemoveAction;
    delete m_cameraMenu;
    delete m_settings;
}


#include "mainwindow.moc"
