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
#ifndef CAMERAVIEWER_H
#define CAMERAVIEWER_H

/**
	@author Leandro Emanuel López <lopezlean@gmail.com>
*/

#include <QWidget>
#include <QFrame>

class TabWidget;
class QVBoxLayout;
class QScrollArea;
class QLabel;
class QActionGroup;
class QMenu;
class QAction;

class ServerLabel: public QFrame{
  Q_OBJECT
    public:
        ServerLabel( const QString &text, QWidget * parent );
        QLabel * label;
};

class CameraViewer: public QWidget{
Q_OBJECT
public:
    CameraViewer( QWidget * parent = 0 );
    enum LayoutType{ ColumnsLayoutType, MainCameraLayoutType, DoubleCameraLayoutType };

    static CameraViewer * instance();

    virtual void saveSettings(){};
    virtual void appendCamera( QWidget * camera );
    virtual void endCameraList( );
    virtual QActionGroup * cameraFocusActions();
    virtual void updateCameras();
    void clearCameras();

    void setCameraOptions( const Qt::AspectRatioMode & aspectRatio, bool showCameraNumbers, const Qt::Alignment & cameraNumbersAlignment  );

    virtual ~CameraViewer();



Q_SIGNALS:
    void layoutChanged( int currentColumnNumber );
    void cameraFocused( QWidget * );
public Q_SLOTS:
      /**
        layout widgets in @param columns Columns
        @param force Ignore if the current layout columns == columns
       */
      virtual void layoutWidgets( int columns = 4 , bool force = false );
      /**
        Layout widgets with one main camera
      */
      void mainCameraWidgetLayout( bool force = false );
      void doubleCameraWidgetLayout( bool force = false );

      void setMainCameraWidget( QWidget * widget );
protected:
    void resizeEvent ( QResizeEvent * event );
    virtual void initActions();
    virtual void initSettings();
private Q_SLOTS:
    void cameraFocus( QWidget *);
    void fullScreenSlot( QWidget *);
    void fullScreenClosedSlot( QWidget *);
private:
    void init();
    void updateActions();
    TabWidget * container;
    QScrollArea * m_scrollArea;
    QVBoxLayout * m_gridContainer;

    static CameraViewer * m_instance;
    LayoutType m_layoutType;
    class Private;
    Private *d;

};

#endif
