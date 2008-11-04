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
#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QFrame>
class FrameWidget;
class Stream;
class QToolButton;
class CameraWidgetToolBar;
class QSpacerItem;
class QMenu;

/**
	@author Leandro Emanuel López <lopezlean@gmail.com>
*/
class CameraWidget : public QFrame
{
Q_OBJECT
public:
    enum CameraState { Playing, Stopped, Paused };
    enum CameraType{ Monitor, EventViewer };
    enum FocusBehavior{ PopUpMenu, PromoteToMainWidget, None };
    /**
        @param conectionName is used to take events from database.
    */
    CameraWidget( const QString &conectionName = QString() ,QWidget * parent = 0);
    CameraWidget( const CameraWidget & other );

    ~CameraWidget();
    /**
     @return camera's stream
    */
    Stream * stream();
    /**
        set camera's stream
    */
    void setStream( Stream * stream);
    void setConfigActionState( bool state );
    void setImageSize( QSize s );
    void setCameraType ( const CameraType &type );
    void setFocusBehavior( const FocusBehavior & behavior  );

    /**
        @return a permanent uniqueid. This is useful for save settings of a particular camera
    */
    QString uniqueId() const;

    /**
       @return the name
    */
    QString name() const;
    void setName( const QString &name );
    /**
        
    */
    QAction * toggleViewAction();
    QAction * focusAction();
    QAction * promoteToMainCameraAction();
    QAction * promoteToSecondCameraAction();

    FrameWidget * frameWidget() const;

    void activateMainCameraAction();
    void activateSecondCameraAction();
    void removeMainCameraAction();
    void removeSecondCameraAction();

    CameraWidgetToolBar * toolBar() const;
    QSize cameraPixmapSizeHint() const;
    QSize sizeHint()const{ return QSize(320,240); } 
    void saveState();
    void restoreState();
protected Q_SLOTS:
    /**
        set pixmap
    */
void setPixmap ( QPixmap * p);

public Q_SLOTS:
    /**
        start camera
    */
    void startCamera( );
    /**
        pause camera
    */
    void pauseCamera( );
    /**
        stop and start camera
    */
    void restartCamera();
    /**
        stop camera
    */
    void stopCamera();
    /**
        launch config dialog
    */
    void configCamera();
    /**
        show camera in fullscreen mode
    */
    void fullScreen();
    /** 
        show events of this camera
    */
    void cameraEvents();
    void setAutoAdjustImage ( bool b );
    void setHighQuality( bool b );
    QSize imageSize() const;
    void promoteToMainWidget( );
    void promoteToSecondWidget( );
    void setVisible(bool);

Q_SIGNALS:
    void windowTitleChanged( const QString &);
    void promotedToMainWidget( QWidget *);
    void promotedToSecondWidget( QWidget *);

    void focused( QWidget *);
    void fullScreen( QWidget * pointerToThis );
    void fullScreenClosed( QWidget * pointerToThis );

protected:
    bool event ( QEvent * event );
    void focusInEvent ( QFocusEvent * event );
    void focusOutEvent ( QFocusEvent * event );
    void resizeEvent ( QResizeEvent * event );
    void mouseDoubleClickEvent ( QMouseEvent * event );
    void showEvent ( QShowEvent * event );

private Q_SLOTS:
    void popupMenu ( const QPoint & );
    void setFocus ( bool b );
    void changeCameraNumber();
    void newEvents( int count );
private:
    void init();
    void loadFromState();
    FrameWidget * m_frameWidget;
    QSpacerItem * m_spacerV;
    Stream * m_stream;
    QFrame * m_frame;
    QLayout * m_layout;
    CameraWidgetToolBar * m_toolbar;
    bool m_autoAdjustImage;
    QAction * m_toggleViewAction;
    QAction * m_focusAction;
    QAction * m_promoteToMainCameraAction;
    QAction * m_promoteToSecondCameraAction;

    QString m_conectionName;
    QMenu * m_menu;
    class Private;
    Private *d;
  


};

#endif
