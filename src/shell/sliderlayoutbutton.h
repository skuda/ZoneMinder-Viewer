/***************************************************************************
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
#ifndef SLIDERLAYOUTBUTTON_H
#define SLIDERLAYOUTBUTTON_H

#include <QToolButton>
class QSlider;
class QMenu;
class QWidgetAction;

/**
	@author Leandro Emanuel López <lopezlean@gmail.com>
*/
class SliderLayoutButton: public QToolButton{
    Q_OBJECT
public:
    SliderLayoutButton( QWidget * parent );
    QSlider * slider();
    ~SliderLayoutButton();
Q_SIGNALS:
    void valueChanged( int );
private Q_SLOTS:
    void clickSlot();
    void sliderChangeSlot( int value );
private:
    void init();
    QSlider * m_slider;
    QMenu * m_menu;
    QWidgetAction * m_action;

};

#endif
