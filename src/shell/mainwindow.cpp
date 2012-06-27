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
#include "cameraviewer.h"
#include "layoutbar.h"
#include "controlbar.h"
#include "translator.h"

#include "stream.h"
#include "camerawidget.h"
#include "camerawidgettoolbar.h"
#include "about.h"
#include "monitors.h"
#include "styles/styledialog.h"

#include "adminpanel.h"
#include "preferencesdialog.h"

//core
#include "databasesession.h"
#include "connectionwidget.h"
#include "connectionmanager.h"
#include "commandlistener.h"

#include <QMenuBar>
#include <QSettings>
#include <QEvent>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QStyleFactory>
#include <QApplication>
#include <QDir>
#include <QtCore>

static QStringList hosts;

MainWindow::MainWindow ( QWidget * parent, Qt::WindowFlags flags )
        :QMainWindow ( parent , flags ), m_fullScreenAction ( 0 ),
        m_closeFullScreenAction ( 0 ), m_adminPanel(0)
{
    init();
    initActions();
    initMenuBar();
    retranslateStrings();
}

void MainWindow::init()
{
    m_commandListener = new CommandListener( this );
    qApp->setWindowIcon ( QIcon ( ":/shellicons/Icon" ) );
    setWindowTitle ( About::applicationName() );
    m_settings = new QSettings ( this );
    m_cameraToggleAction = new QList < QAction *>;
    //m_cameraRemoveAction = new QList < QAction *>;
    m_centralWidget = new CameraViewer ( this );
    setCentralWidget ( m_centralWidget );

    m_controlBar = new ControlBar( this );
    m_layoutBar = new LayoutBar( this );
    initSettings();
    m_controlBar->updateControls();
    m_translator = Translator::instance();
    m_translator->setParent( this );
    connect ( m_translator , SIGNAL ( languageChanged( QString ) ), this, SLOT( switchLanguage( QString ) ) );

    installEventFilter( m_commandListener );
    //this is needed in Linux for a correct initial layout display.
    QTimer::singleShot(0, m_centralWidget, SLOT(layoutCurrentColLayout()));
}

void MainWindow::initActions()
{
    m_sessionNew = new QAction(this);
    m_sessionNew->setShortcut( tr( "Ctrl+N" ) );
    connect ( m_sessionNew , SIGNAL ( triggered() ), this , SLOT ( newSession() ) );

    m_sessionList = new QAction(this);
    m_sessionList->setShortcut( tr( "Ctrl+L" ) );
    connect ( m_sessionList , SIGNAL ( triggered() ), this , SLOT ( sessionList() ) );

    m_closeFullScreenAction = new QAction(this);
    m_closeFullScreenAction -> setIcon( QIcon( ":/shellicons/Restore") );
    connect ( m_closeFullScreenAction , SIGNAL ( triggered() ), this , SLOT ( showNormal() ) );

    //TODO: Fix fullscreen action behaviour
    m_fullScreenAction = new QAction(this);
    m_fullScreenAction -> setIcon( QIcon( ":/icons/FullScreen") );
    m_fullScreenAction->setShortcut( tr( "F11" ) );
    connect ( m_fullScreenAction, SIGNAL ( triggered() ), this , SLOT ( fullScreen() ) );
  
    m_quitAction = new QAction(this);
    m_quitAction->setShortcut(tr("Ctrl+Q"));
    m_quitAction -> setIcon( QIcon( ":/icons/Quit") );

    connect ( m_quitAction, SIGNAL ( triggered() ), this , SLOT ( close() ) );

    m_updateAllMonitorsActions = new QAction(this);
    m_updateAllMonitorsActions->setShortcut( tr("F5") );
    connect ( m_updateAllMonitorsActions, SIGNAL ( triggered() ),this, SLOT ( update() ) );

    m_selectStyleAction = new QAction(this);
    connect ( m_selectStyleAction , SIGNAL (triggered()), this , SLOT ( selectStyle() ) );

    m_preferencesAction = new QAction( this );
    connect ( m_preferencesAction, SIGNAL ( triggered() ),this, SLOT ( showPreferences() ) );

    aboutAction = new QAction(this);
    aboutQtAction = new QAction(this);
    connect ( aboutAction, SIGNAL ( triggered () ), this, SLOT ( aboutDialog() ) );
    connect ( aboutQtAction, SIGNAL ( triggered () ), qApp, SLOT ( aboutQt() ) );
}

void MainWindow::initMenuBar()
{
    sessionMenu = new QMenu(this);
    sessionMenu->addAction ( m_sessionNew );
    sessionMenu->addAction ( m_sessionList );
    sessionMenu->addSeparator();
    sessionMenu->addAction ( m_quitAction );
    menuBar() -> addMenu(sessionMenu);

    camMenu = new QMenu(this);
    m_adminMonitorsMenu = new QMenu( this );
    camMenu->addMenu( m_adminMonitorsMenu );

    camMenu->addAction ( m_updateAllMonitorsActions );
    menuBar() -> addMenu(camMenu);

    m_viewMenu = new QMenu(this);
    menuBar() -> addMenu(m_viewMenu);

    QPair <QString, QString> host;
    foreach( host, Monitors::hosts() ){
        QAction *action = m_adminMonitorsMenu->addAction( host.first );
        action->setData( host.second );
        connect( action, SIGNAL ( triggered () ) , this, SLOT ( adminServer() ) );
    }

    m_viewMenu->addAction ( m_fullScreenAction );
    m_viewMenu->addAction ( m_closeFullScreenAction );
    m_viewMenu->addSeparator();
    m_viewMenu->addAction(m_selectStyleAction);
    m_viewMenu->addAction ( m_preferencesAction );

    m_helpMenu = new QMenu(this);
    menuBar() -> addMenu(m_helpMenu);

    m_helpMenu -> addAction(aboutAction);
    m_helpMenu -> addAction(aboutQtAction);
}

void MainWindow::retranslateStrings()
{
//menus
    sessionMenu -> setTitle( tr("&Session") );
    camMenu -> setTitle( tr("&Cameras") );
    m_viewMenu -> setTitle( tr("&View") );
    m_helpMenu -> setTitle( tr("&Help") );
    m_adminMonitorsMenu->setTitle( "&Admin Panel" );

//actions
    m_sessionNew -> setText( tr( "&New Session..." ) );
    m_sessionList -> setText( tr( "&Session List..." ) );
    m_closeFullScreenAction -> setText( tr( "&Close FullScreen View" ) );
    m_fullScreenAction -> setText( tr( "&FullScreen" ) );
    m_quitAction -> setText( tr("&Quit") );

    m_updateAllMonitorsActions -> setText( tr("Update All Cameras") );
    m_updateAllMonitorsActions -> setStatusTip( tr("Update all session's cameras") );

    m_selectStyleAction -> setText( tr( "&Select Style..." ) );
    m_preferencesAction->setText( tr( "&Preferences..."  ) );
    aboutAction -> setText( tr("About...") );
    aboutQtAction -> setText( tr("About Qt...") ); 
}

void MainWindow::initSettings()
{
    m_settings->beginGroup ( "MainWindow" );
    int ws = m_settings->value ( "windowState" , Qt::WindowMaximized ).toInt();
    setWindowState ( ( Qt::WindowStates ) ws );
    m_settings->value ( "windowState" , Qt::WindowMaximized ).toInt();
    int w = m_settings->value ( "sizeW" , size().width() ).toInt();
    int h = m_settings->value ( "sizeH" , size().height() ).toInt();

    Qt::ToolBarArea layoutBarArea = (Qt::ToolBarArea)m_settings->value ( "layoutBarArea" , Qt::TopToolBarArea ).toInt();
    m_layoutBar->setVisible(m_settings->value ( "layoutBarVisible" , true ).toBool());
    Qt::ToolBarArea controlBarArea = (Qt::ToolBarArea)m_settings->value ( "controlBarArea" , Qt::TopToolBarArea ).toInt();
    m_controlBar->setVisible(m_settings->value ( "controlBarVisible" , true ).toBool());

    if ( layoutBarArea == Qt::NoToolBarArea ) layoutBarArea = Qt::TopToolBarArea;
    if ( controlBarArea == Qt::NoToolBarArea ) controlBarArea = Qt::TopToolBarArea;
    addToolBar( layoutBarArea, m_layoutBar );
    addToolBar( controlBarArea, m_controlBar );

    resize ( w , h );
    m_settings->endGroup();
    m_settings->beginGroup ( "Databases" );
    QStringList con = m_settings->value ( "names" ).toStringList();
    m_settings->endGroup();

    foreach ( const QString name , con )
    {
        qDebug ( "%s init...", qPrintable ( name ) );
        m_settings->beginGroup ( name );
        bool b = ConnectionManager::addConnection ( m_settings->value ( "driver" ).toString(),
                                                    m_settings->value ( "host" ).toString(),
                                                    m_settings->value ( "database" ).toString(),
                                                    m_settings->value ( "user" ).toString(),
                                                    m_settings->value ( "password" ).toString(),
                                                    m_settings->value ( "port" , 0 ).toInt(),
                                                    m_settings->value ( "wwwPort" , 80 ).toInt(),
                                                    false );
        if (!b)
            QMessageBox::critical( this , tr("Database Error") ,
                                   tr("Can not connect with Database %1 at host %2")
                                   .arg(m_settings->value ( "database" ).toString())
                                   .arg(m_settings->value ( "host" ).toString()) );
        m_settings->endGroup();
    }

    QWidget *cam;
    m_monitors = new Monitors ( this );

    foreach ( cam , m_monitors->cameras() )
    {
        m_centralWidget->appendCamera ( cam );
        CameraWidget * cameraWidget = qobject_cast <CameraWidget *> ( cam );
        m_cameraToggleAction->append ( cameraWidget->toggleViewAction() );
        initCameraSetting( cameraWidget );
    }

    StyleDialog::loadFromSettings();

    m_centralWidget->endCameraList(  );
    foreach ( QAction * action , m_centralWidget->cameraFocusActions()->actions() ){
        m_commandListener->setMapAction( action->text(), action );
    }
}

void MainWindow::initCameraSetting( CameraWidget * camera ){
    m_settings->beginGroup ( camera->uniqueId() );
    camera->toolBar()->setVisible( m_settings->value( "toolBarVisible", false ).toBool() );
    m_settings->endGroup();
}

void MainWindow::saveSettings()
{
    m_settings->beginGroup ( "MainWindow" );
    m_settings->setValue ( "windowState" , ( int ) windowState() );
    m_settings->setValue ( "sizeW" , size().width() );
    m_settings->setValue ( "sizeH" , size().height() );
    m_settings->setValue ( "layoutBarArea" , QMainWindow::toolBarArea(m_layoutBar) );
    m_settings->setValue ( "layoutBarVisible" , m_layoutBar->isVisible() );
    m_settings->setValue ( "controlBarArea" , QMainWindow::toolBarArea(m_controlBar) );
    m_settings->setValue ( "controlBarVisible" , m_controlBar->isVisible() );
    m_settings->endGroup ();
    foreach ( QWidget * cam , m_monitors->cameras() )
    {
        CameraWidget * cameraWidget = qobject_cast <CameraWidget *> ( cam );
        saveCameraSetting( cameraWidget );
    }

}

void MainWindow::saveCameraSetting( CameraWidget * camera ){
    m_settings->beginGroup ( camera->uniqueId() );
    m_settings->setValue( "toolBarVisible", camera->toolBar()->isVisible() );
    m_settings->endGroup();
}


bool MainWindow::event ( QEvent * ev )
{
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


void MainWindow::fullScreen()
{
    setWindowState ( Qt::WindowFullScreen );
}

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
    m_centralWidget->clearCameras();
    m_monitors->updateMonitors();
    m_cameraToggleAction->clear();
    QWidget *cam;
    foreach ( cam , m_monitors->cameras() )
    {
        m_centralWidget->appendCamera ( cam );
        m_cameraToggleAction->append ( ((CameraWidget *)cam)->toggleViewAction() );
    }
    m_centralWidget->updateCameras();
    m_controlBar->updateControls();
    m_commandListener->clear();
    foreach ( QAction * action , m_centralWidget->cameraFocusActions()->actions() ){
        m_commandListener->setMapAction( action->text(), action );
    }
    m_adminMonitorsMenu->clear();
    QPair <QString, QString> host;
    foreach( host, Monitors::hosts() ){
        QAction *action = m_adminMonitorsMenu->addAction( host.first );
        action->setData( host.second );
        connect( action, SIGNAL ( triggered () ) , this, SLOT ( adminServer() ) );
    }
}

void MainWindow::selectStyle(){
    StyleDialog d( this );
    d.exec();
}
void MainWindow::show(){
    QMainWindow::show();
    //m_centralWidget->layoutWidgets();
}
void MainWindow::adminServer(){
    QString settingsName = qobject_cast<QAction * > (sender() )->data().toString();
    QString hostName = qobject_cast<QAction * > (sender() )->text();
    m_settings->beginGroup ( settingsName );
    QString hostPanel = m_settings->value( "adminPanel", "http://"+ hostName + "/zm" ).toString();
    m_settings->endGroup();
    if (!m_adminPanel)
        m_adminPanel = new AdminPanel( hostPanel , settingsName);
    else m_adminPanel->setHost( hostPanel, settingsName );
    m_adminPanel->show();
}

void MainWindow::showPreferences(){
    PreferencesDialog preferencesDialog( this );
    preferencesDialog.exec();
}

MainWindow::~MainWindow()
{
    delete m_adminPanel;
    ConnectionManager::closeAll();
}

void MainWindow::switchLanguage( const QString & /*locale*/)
{
    m_translator->writeSettings();
    retranslateStrings();
}
