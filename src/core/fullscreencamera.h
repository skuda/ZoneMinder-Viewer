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
#ifndef FULLSCREENCAMERA_H
#define FULLSCREENCAMERA_H

#include <QDialog>
#include "framewidget.h"
class Stream;
class QPixmap;
class QMenu;
class CameraWidgetToolBar;
class FullScreenCamera: public QDialog{
Q_OBJECT
public:
    FullScreenCamera( QWidget * parent = 0 );
    void setStream ( Stream * s  , const FrameWidget::Status &state );
    FrameWidget::Status status() const;
    ~FullScreenCamera();

public Q_SLOTS:
    void setPixmap( QPixmap *);
    void startCamera( );
    void pauseCamera( );
    void stopCamera();
    void setHighQuality( bool b );

protected Q_SLOTS:
    void popupMenu ( const QPoint & p );

private:
    void init();
    FrameWidget *m_frameWidget;
    Stream * m_stream;
    QMenu * m_menu;
    CameraWidgetToolBar * m_toolBar;
};

#endif
