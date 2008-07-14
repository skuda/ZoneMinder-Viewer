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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QActionGroup>

class CameraWidget;
class Stream;
class QAction;
class QSettings;
class QMenu;
class AdminPanel;
class Monitors;
class CameraViewer;
class LayoutBar;
class ControlBar;
class CommandListener;
class Translator;

class MainWindow: public QMainWindow{
Q_OBJECT
public:

    MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0  );
    ~MainWindow();
    QTranslator *appTranslator, *qtTranslator;
    void retranslateStrings();
    void show();
public Q_SLOTS:
    void fullScreen();
    void aboutDialog();
    void newSession();
    void sessionList();
    void selectStyle();
    void adminServer();
    void showPreferences();
protected:
    bool event ( QEvent * event );
private Q_SLOTS:
    void setCentralWidgetCamera ( QWidget * w );
    void update ( );
    void switchLanguage( const QString &locale );

private:
    void init();
    void initActions();
    void initMenuBar();
    void initSettings();
    void initCameraSetting( CameraWidget * camera );
    void saveSettings();
    void saveCameraSetting( CameraWidget * camera );

    QList <QAction *> *m_cameraToggleAction;
    //QList <QAction *> *m_cameraRemoveAction;

    QAction * m_quitAction;
    QAction * m_fullScreenAction;
    QAction * m_closeFullScreenAction;
    
    QAction * m_sessionNew;
    QAction * m_sessionList;


    QAction * m_updateAllMonitorsActions;
    QAction * m_selectStyleAction;
    QAction * aboutAction;
    QAction * aboutQtAction;

    QAction * m_preferencesAction;

    QMenu * sessionMenu;
    QMenu * camMenu;
    QMenu * m_viewMenu;
    QMenu * m_helpMenu;
    QMenu * m_adminMonitorsMenu;


    QSettings * m_settings;
    //cantidad de camaras
    int m_cameras;

    CameraViewer  *m_centralWidget;
    Monitors * m_monitors;
    
    LayoutBar  * m_layoutBar;
    ControlBar * m_controlBar;
    AdminPanel * m_adminPanel;

    Translator * m_translator;

    CommandListener * m_commandListener;
    
};

#endif

