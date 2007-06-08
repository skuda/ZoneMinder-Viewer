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
#include "mainwindow.h"
#include "stream.h"
#include "camerawidget.h"
#include "about.h"

#include "cameraadddialog.h"
#include "multicameraview.h"
#include "cameraselectdialog.h"
#include "config.h"

//core
#include "databasesession.h"

#include <QMenuBar>
#include <QSettings>
#include <QEvent>
#include <QMessageBox>


#if QT_VERSION >= 0x040300
    #include <QMdiArea>
    #include <QMdiSubWindow>
#elif QT_VERSION >= 0x040200
    #include <QWorkspace>
#endif



MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags)
:QMainWindow( parent , flags ),m_fullScreenAction(0),
 m_closeFullScreenAction(0)
{
    init();
    initActions();
    initMenuBar();

}

void MainWindow::init(){
      qApp->setWindowIcon ( QIcon (":/icons/Icon"));
      setWindowTitle( About::applicationName() );
      m_settings = new QSettings ( this );
      m_cameraToggleAction = new QList < QAction *>;
      m_cameraRemoveAction = new QList < QAction *>;
      m_centralWidget = new MDICLASS ( this );
      setCentralWidget ( m_centralWidget );
      initSettings(); 

}

void MainWindow::initActions(){
    m_sessionNew = new QAction ( _("&New Session...") , this );
    connect (m_sessionNew , SIGNAL( triggered() ), this , SLOT (newSession()) ); 

    m_newCameraAction = new QAction (QIcon(":/icons/AddCameraIcon"),_("&Add") , this );
    connect (m_newCameraAction , SIGNAL( triggered() ), this , SLOT (cameraAddSlot()) ); 
    m_closeFullScreenAction = new QAction(QIcon(":/icons/Restore"),_("&Close FullScreen View") , this );
    connect (m_closeFullScreenAction , SIGNAL( triggered() ), this , SLOT (showNormal()) );
    m_fullScreenAction = new QAction( QIcon(":icons/FullScreen"),_("&FullScreen"), this);
    connect (m_fullScreenAction, SIGNAL(triggered()), this , SLOT (fullScreen()));
    m_quitAction = new QAction (QIcon(":/icons/Quit"),_("&Quit") , this );
    connect (m_quitAction, SIGNAL(triggered()), this , SLOT (close()));
    m_cascadeAction = new QAction (_("&Cascade") , this );
    connect ( m_cascadeAction , SIGNAL(triggered()) , m_centralWidget , SLOT (cascade()) );
    m_tileAction = new QAction (_("&Tile") , this );
    connect ( m_tileAction , SIGNAL(triggered()) , m_centralWidget , SLOT (tile()) );
    m_arrangeIconsAction = new QAction (_("&Arrange Icons"),this);
    connect ( m_arrangeIconsAction , SIGNAL(triggered()) , m_centralWidget , SLOT (arrangeIcons()) );

    closeAction = new QAction(_("&Close"), this);
    closeAction->setShortcut(_("Ctrl+F4"));
    closeAction->setStatusTip(_("Close active window"));
    connect(closeAction, SIGNAL(triggered()), m_centralWidget, SLOT(closeActiveWindow()));

    closeAllAction = new QAction(_("Close &All"), this);
    closeAllAction->setStatusTip(_("Close All &Windows"));
    connect(closeAllAction, SIGNAL(triggered()),m_centralWidget, SLOT(closeAllWindows() ) );
}

void MainWindow::initMenuBar(){
    QMenu * sessionMenu = menuBar()->addMenu(_("Session"));
    sessionMenu->addAction( m_sessionNew );

    QMenu * camMenu = menuBar()->addMenu(_("Video Camera"));
    camMenu->addAction ( m_newCameraAction );
    m_camRemoveMenu = camMenu->addMenu(_("Delete"));
    camMenu->addSeparator();
    camMenu->addAction ( m_quitAction );
    QMenu * m_viewMenu = menuBar()->addMenu(_("View"));
    m_cameraMenu = m_viewMenu->addMenu(_("&Cameras"));
    QAction * t = m_viewMenu->addAction(_("Customized View..."));
    connect ( t, SIGNAL(triggered()), this , SLOT( selectedCamerasSlot () ) );
    for (int i = 0 ; i < m_cameraToggleAction->count() ; i++ ){
        m_cameraMenu->addAction( m_cameraToggleAction->at( i ) );
        m_cameraRemoveAction->insert ( i , m_camRemoveMenu->addAction( m_cameraToggleAction->at( i )->text() ) );
        connect ( m_cameraRemoveAction->at( i ), SIGNAL(triggered()), (CameraWidget *)m_centralWidget->windowList().at( i ) , SLOT( remove () ) );
        
        }
    m_viewMenu->addSeparator();
    m_viewMenu->addAction( m_fullScreenAction );
    m_viewMenu->addAction( m_closeFullScreenAction );
    QMenu * m_windowMenu = menuBar()->addMenu(_("&Window"));
    m_windowMenu->addAction ( closeAction );
    m_windowMenu->addAction ( closeAllAction );
    m_windowMenu->addSeparator();
    m_windowMenu->addAction ( m_cascadeAction );
    m_windowMenu->addAction ( m_tileAction );
    m_windowMenu->addAction ( m_arrangeIconsAction );
    QMenu * m_helpMenu = menuBar()->addMenu(_("&Help"));
    connect (m_helpMenu->addAction(_("About...")), SIGNAL ( triggered ()), this, SLOT(aboutDialog()));
    connect (m_helpMenu->addAction(_("About Qt...")), SIGNAL ( triggered ()), qApp, SLOT(aboutQt()));
    
}

void MainWindow::initSettings(){
    m_settings->beginGroup ("MainWindow"); 
    int ws = m_settings->value( "windowState" , Qt::WindowMaximized ).toInt();
    setWindowState ( (Qt::WindowStates) ws );
    m_settings->value( "windowState" , Qt::WindowMaximized ).toInt();
    int w = m_settings->value ( "sizeW" , size().width() ).toInt();
    int h = m_settings->value ( "sizeH" , size().height() ).toInt();
    resize( w , h);
    m_settings->endGroup();
    m_settings->beginGroup ("Cameras");
    //lista de nombres
    QStringList cameras_cfg = m_settings->value ("CameraList").toStringList();
    m_settings->endGroup ();
    for ( int i = 0 ; i < cameras_cfg.size() ; i ++ ){
         bool visible = m_settings->value (cameras_cfg.at(i) , true ).toBool() ;
         CameraWidget *camera = new CameraWidget(  this);
         camera->setWindowTitle( cameras_cfg.at(i) );
         m_settings->beginGroup (cameras_cfg.at(i));
         QString host = m_settings->value ("host", "localhost" ).toString();
         int port = m_settings->value ("port", 80 ).toInt();
         camera->stream()->setHost ( host , port );
         camera->stream()->setMode ((Stream::StreamMode)m_settings->value ("mode", Stream::JPEG ).toInt());
         camera->stream()->setMonitor (m_settings->value ("monitor" ).toInt());
         camera->stream()->setBitRate(m_settings->value ("bitrate" , 2 ).toInt());
         camera->stream()->setScale (m_settings->value ("scale" , 100 ).toInt());
         camera->stream()->setZMStreamServer (m_settings->value ("zms" , "/cgi-bin/nph-zms" ).toString());
         int x = m_settings->value ("x-position" , 0 ).toInt();
         int y = m_settings->value ("y-position" , 0 ).toInt() ;
         int widht = m_settings->value("widgth",320).toInt();
         int height = m_settings->value("height",240).toInt() ;
         camera->setGeometry ( x, y, widht, height );

         //int winpos = m_settings->value ("widgetArea" , 1).toInt();
         m_settings->endGroup();
         //addDockWidget ( (Qt::DockWidgetArea)winpos , m_cameraList->at(i) );
         QWidget * w = m_centralWidget->addWindow( camera );
         w->setGeometry ( x, y, widht, height );

         m_cameraToggleAction->insert ( i , ((CameraWidget *)m_centralWidget->windowList().at(i))->toggleViewAction() );

         camera->startCamera();
         /**
            TODO: read size from QSettings
         */
     //    w->resize( 320 , 240 );
     }

}

void MainWindow::saveSettings(){
    m_settings->beginGroup ("MainWindow");
    m_settings->setValue( "windowState" , (int)windowState() );
    m_settings->setValue( "sizeW" , size().width() );
    m_settings->setValue( "sizeH" , size().height() );
    m_settings->endGroup ();
    QStringList allnames;
    
    /**
        TODO: evaluar esto. Lo malo del metodo windowList de
        QWorkspace es que no me devuelve los widgets que no estan visibles
    */
    //QList<QWidget *> allCameras = m_centralWidget->windowList();
    /**
        TODO: esto hace que el compilador de un warning en qobject.h pero no se si
        es el problema mio o de qt
    */
    QList<CameraWidget *> allCameras = m_centralWidget->findChildren<CameraWidget *>();
    for ( int i = 0 ; i < allCameras.size() ; i++ ){
        allnames << allCameras.at( i )->windowTitle();
        qDebug ("Saving %s...",qPrintable(allCameras.at( i )->windowTitle()));
        CameraWidget * current_camera = /*(CameraWidget *)*/ allCameras.at( i );
        m_settings->beginGroup( allCameras.at( i )->windowTitle() );
        m_settings->setValue("host", current_camera->stream()->host() );
        m_settings->setValue("port",current_camera->stream()->port() );
        m_settings->setValue("mode",current_camera->stream()->mode() );
        m_settings->setValue("monitor",current_camera->stream()->monitor() );
        m_settings->setValue("bitrate",current_camera->stream()->bitrate() );
        m_settings->setValue("scale",current_camera->stream()->scale() );
        m_settings->setValue("zms",current_camera->stream()->zmStreamServer() );
        m_settings->setValue("x-position",allCameras.at( i )->geometry().x() );
        m_settings->setValue("y-position",allCameras.at( i )->geometry().y() );
        m_settings->setValue("height",allCameras.at( i )->geometry().height() );
        m_settings->setValue("width",allCameras.at( i )->geometry().width() );

        m_settings->endGroup();
    }
     m_settings->beginGroup( "Cameras" );
     m_settings->setValue("CameraList", allnames );
     m_settings->endGroup();
}


bool MainWindow::event ( QEvent * ev ){
    //Cuando se cierre la ventana q guarde las configuraciones
    if ( ev->type() == QEvent::Close ) {
			saveSettings();
			return true;
		}
    if (m_fullScreenAction && m_closeFullScreenAction ){
    if (windowState() == Qt::WindowFullScreen){
        m_fullScreenAction->setEnabled(false);
        m_closeFullScreenAction->setEnabled(true);
    }else {
        m_fullScreenAction->setEnabled(true);
        m_closeFullScreenAction->setEnabled(false);
    }
    }
    return QMainWindow::event( ev );
}

void MainWindow::addCamera( const QString & name , const QString &host , int port , int monitor , const QString &zms ){
            CameraWidget * camera = new CameraWidget(  this);
            camera->setWindowTitle( name );
            camera->stream()->setHost( host , port );
            camera->stream()->setMonitor( monitor );
            camera->stream()->setZMStreamServer( zms );
            //m_cameraToggleAction->append ( camera->toggleViewAction() );
            //m_cameraMenu->addAction( camera->toggleViewAction() );
            QAction * a = m_camRemoveMenu->addAction ( name );
            m_cameraRemoveAction->append( a );
            connect ( a, SIGNAL(triggered()), camera , SLOT( remove () ) );
            camera->startCamera();
            QWidget *w = m_centralWidget->addWindow( camera );
            w->show();
}
void MainWindow::cameraAddSlot(){
    CameraAddDialog dlg(this);
    dlg.completeDefaults();
    dlg.setWindowTitle(_("Add Video Camera"));
    int r = dlg.exec();
    if ( r == QDialog::Accepted ){
            if (cameraListContainsName(dlg.m_name->text())){
                QMessageBox::critical(this, _("Add Video Camera"), _("Name is already in use.\n Please select other"));
            }
            else addCamera( dlg.m_name->text() , dlg.m_host->text() , dlg.m_port->text().toInt() , dlg.m_monitor->text().toInt() , dlg.m_zms->text() );
    }
}

void MainWindow::fullScreen(){
    setWindowState(Qt::WindowFullScreen);
}

void MainWindow::removeCamera( CameraWidget * w){
    /** TODO: */
    //    m_centralWidget->removeSubWindow( w );

    w->stopCamera();
    delete w;
    //m_cameraToggleAction->removeAt( pos );
    //m_camRemoveMenu->removeAction( m_cameraRemoveAction->at( pos ) );
    //m_cameraRemoveAction->removeAt( pos );
    //delete w;
}

void MainWindow::selectedCamerasSlot (){
    CameraSelectDialog csd( this );
    QStringList names;
    QList <QWidget *> all_cameras = m_centralWidget->windowList();
    for ( int i = 0 ; i < all_cameras.size() ; i ++ ){
        names << all_cameras.at(i)->windowTitle();
    }
    csd.setNames ( names );
    if ( csd.exec() == QDialog::Accepted){
        QList <Stream * > stream ;
        names = csd.names();
        int pos = 0;
        for ( int i = 0 ; i < all_cameras.size() ; i ++ )
            if ( names.contains( all_cameras.at(i)->windowTitle() ) ){
                stream.insert ( pos , ((CameraWidget *)all_cameras.at( i ))->stream() );
                pos++;
                }
        MultiCameraView * v = new MultiCameraView(  );
        v->setUseTabs(csd.groupViewView() );
        v->setStream( stream );
        v->showFullScreen();
    }
}

bool MainWindow::cameraListContainsName ( const QString & name ){
    QList <QWidget *> all_cameras = m_centralWidget->windowList();
    for ( int i = 0 ; i <  all_cameras.size() ; i ++ )
            if ( name ==   all_cameras.at(i)->windowTitle()  )
                        return true;
    return false;
}

QStringList MainWindow::cameraNames(){
QStringList names;
QList <QWidget *> all_cameras = m_centralWidget->windowList();
for ( int i = 0 ; i < all_cameras.size() ; i ++ )
            names << all_cameras.at(i)->windowTitle();
return names;
}

//Pone una camara de forma central de manera q las otras
// la rodean y esta demas presenta la caracteristica
// de ajustar su tamaño a su espacio
void MainWindow::setCentralWidgetCamera ( QWidget * w ){
    //TODO:
    QVBoxLayout * layout = new QVBoxLayout ( centralWidget() );
    layout->addWidget ( w );
}

void MainWindow::newSession(){
    DatabaseSession s;
    s.exec();
}

void MainWindow::aboutDialog(){
    About about;
    about.showAboutDialog();
}

MainWindow::~MainWindow()
{
    delete m_cameraToggleAction;
    delete m_cameraRemoveAction;
    delete m_cameraMenu;
    delete m_settings;
}


#include "mainwindow.moc"
