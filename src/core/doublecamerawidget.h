/**************************************************************************
*   Copyright (C) 2008 by Leandro Emanuel López                           *
*   lopezlean@gmail.com                                                   *
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
#ifndef DOUBLECAMERAWIDGET_H
#define DOUBLECAMERAWIDGET_H

#include <QWidget>

class QScrollArea;
class QHBoxLayout;
class QVBoxLayout;
class QAction;

class DoubleCameraWidget: public QWidget{
    Q_OBJECT
public:
    DoubleCameraWidget( QWidget * parent = 0 );
    void appendCamera( QWidget * widget );
    ~DoubleCameraWidget();

public Q_SLOTS:
    void setMainCameraWidget( QWidget * widget );
    void setSecondCameraWidget( QWidget * widget );
    void setVisible ( bool b );

private:
    void init();

    QWidget * m_mainCamera; 
    QWidget * m_secondCamera; 

    QScrollArea * m_bottomArea;
    QWidget * m_rightAreaWidget;
    QWidget *m_bottomAreaWidget;

    QHBoxLayout * m_bottomAreaLayout;

    QVBoxLayout * m_mainCameraLayout;
    QVBoxLayout * m_secondCameraLayout;

    QSize sizes;
    QList <QAction *> m_mainCameraActions;
    QList <QAction *> m_secondCameraActions;

};

#endif
