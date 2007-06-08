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
#ifndef CAMERAWIDGETTOOLBAR_H
#define CAMERAWIDGETTOOLBAR_H

#include <QWidget>

/**
	@author Leandro Emanuel López <lopezlean@gmail.com>
        ToolBar de las camaras
*/

class QToolButton;
class CameraWidget;

class CameraWidgetToolBar : public QWidget
{
public:
    CameraWidgetToolBar( QWidget * parent = 0 );
    void autoConnectWith( CameraWidget * camera );
    ~CameraWidgetToolBar();
    void setRemoveActionState( bool state );
    void setConfigActionState( bool state );
private:
    void init();
    QToolButton * m_startButton;
    QToolButton * m_stopButton;
    QToolButton * m_pauseButton;
    QToolButton * m_fullScreenButton;
    QToolButton * m_configButton;
    QToolButton * m_removeButton;
};

#endif