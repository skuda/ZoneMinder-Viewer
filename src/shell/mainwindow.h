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
class CameraWidget;
class Stream;
class QAction;
class QSettings;
class QMenu;

class Monitors;
class QMdiArea;

class MainWindow: public QMainWindow{
Q_OBJECT
public:

    MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0  );
    ~MainWindow();
public Q_SLOTS:
    void cameraAddSlot();
    void fullScreen();
    //void removeCamera( CameraWidget *);
    void selectedCamerasSlot ();
    void aboutDialog();
    void newSession();
    void sessionList();
    void selectStyle();
protected:
    bool event ( QEvent * event );

private Q_SLOTS:
    void setCentralWidgetCamera ( QWidget * w );
    void update ( );
    void hideActiveSubWindow();
    void hideAllSubWindows();

private:
    void init();
    void initActions();
    void initMenuBar();
    void initSettings();
    void saveSettings();
    bool cameraListContainsName ( const QString & name );
    QStringList cameraNames();
    void addCamera(const QString & name , const QString &host ="localhost", int port =80 , int monitor=1 , const QString &zms = "/cgi-bin/nph-zms");
    
    QList <QAction *> *m_cameraToggleAction;
    //QList <QAction *> *m_cameraRemoveAction;

    QAction * m_newCameraAction;
    QAction * m_quitAction;
    QAction * m_fullScreenAction;
    QAction * m_closeFullScreenAction;
    
    QAction * m_sessionNew;
    QAction * m_sessionList;
    
    //MDI Actions
    QAction *closeAction;
    QAction *closeAllAction;
    QAction * m_cascadeAction;
    QAction * m_tileAction;
    //QAction * m_arrangeIconsAction;
    QAction * m_nextWindowAction;
    QAction * m_previusWindowAction;

    QAction * m_updateAllMonitorsActions;

    QMenu * m_cameraMenu;
    //QMenu * m_camRemoveMenu;

    QSettings * m_settings;
    //cantidad de camaras
    int m_cameras;

    QMdiArea  *m_centralWidget;
    Monitors * m_monitors;
    
};

#endif

