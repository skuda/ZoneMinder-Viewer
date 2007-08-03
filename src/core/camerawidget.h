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

#include <QScrollArea>
class QLabel;
class Stream;
class QToolButton;
class QFrame;
class CameraWidgetToolBar;
class QSpacerItem;
class QMenu;

/**
	@author Leandro Emanuel López <lopezlean@gmail.com>
*/
class CameraWidget : public QScrollArea
{
Q_OBJECT
public:
    /**
        @param conectionName is used to take events from database.
    */
    CameraWidget( const QString &conectionName = QString() ,QWidget * parent = 0);
    CameraWidget( const CameraWidget & other );

    ~CameraWidget();
    /**
     @return el stream que controla el flujo de datos en la cámara
    */
    Stream * stream();
    void setStream( Stream * stream);
    void setConfigActionState( bool state );
    /**
        
    */
    QAction * toggleViewAction();
protected Q_SLOTS:
    /**
        setea la imagen en el widget que representa la camara
    */
void setPixmap ( QPixmap * p);

public Q_SLOTS:
    void startCamera( );
    void pauseCamera( );
    void restartCamera();
    void stopCamera();
    void configCamera();
    void fullScreen();
    void remove();
    void cameraEvents();
    void setAutoAdjustImage ( bool b );

Q_SIGNALS:
    /**
       Esta señal es emitida en el momento en que la cámara debe borrarse.
       Automaticamente es conectada al objeto padre de este widget.
       En caso de no tener padre deberá de conectarse a otro objeto.
    */
    void removeMe( CameraWidget * me );
    void windowTitleChanged( const QString &);

protected:
    bool event ( QEvent * event );

private Q_SLOTS:
    void popupMenu ( const QPoint & );
private:
    void init();
    QLabel * m_camView;
    QSpacerItem * m_spacerV;
    Stream * m_stream;
    QFrame * m_frame;
    QLayout * m_layout;
    CameraWidgetToolBar * m_toolbar;
    bool m_autoAdjustImage;
    QAction * m_toggleViewAction;
    QString m_conectionName;
    QMenu * m_menu;



};

#endif
