/***************************************************************************
*   Copyright (C) 2008 by Leandro Emanuel López                           *
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

#ifndef MAINCAMERAWIDGET_H
#define MAINCAMERAWIDGET_H

#include <QWidget>

class QScrollArea;
class QHBoxLayout;
class QVBoxLayout;
/**
	@author Leandro Emanuel López <lopezlean@gmail.com>
*/
class MainCameraWidget: public QWidget{
    Q_OBJECT
public:
    MainCameraWidget( QWidget * parent );
    void appendCamera( QWidget * widget );
    ~MainCameraWidget();
public Q_SLOTS:
    void setMainCameraWidget( QWidget * widget );
    void setVisible ( bool b );

private:
    void init();
    QWidget * m_mainCamera; 
    QScrollArea * m_rightArea;
    QScrollArea * m_bottomArea;
    QWidget * m_rightAreaWidget;
    QWidget *m_bottomAreaWidget;

    QHBoxLayout * m_bottomAreaLayout;
    QVBoxLayout * m_rigthAreaLayout;
    QVBoxLayout * m_mainCameraLayout;
    QSize sizes;
    int cameraCount;

};

#endif
