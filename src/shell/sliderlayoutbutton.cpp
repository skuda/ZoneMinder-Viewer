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

#include "sliderlayoutbutton.h"


#include <QSlider>
#include <QWidgetAction>
#include <QMenu>
SliderLayoutButton::SliderLayoutButton( QWidget * parent )
:QToolButton( parent )
{
    init();
}


void SliderLayoutButton::init(){
    setPopupMode( QToolButton::MenuButtonPopup );
    setCheckable( true );

    m_slider = new QSlider ( this );
    m_slider->setOrientation( Qt::Horizontal );
    m_slider->setMinimum( 1 );
    m_slider->setSingleStep( 1 );
    m_slider->setMaximum( 30 );

    m_menu = new QMenu( this );
    m_action = new QWidgetAction ( this );
    m_action->setDefaultWidget( m_slider );
    m_menu->addAction( m_action );

    setMenu ( m_menu );

    connect ( this, SIGNAL (clicked()), this, SLOT ( clickSlot() ) );
    connect ( m_slider, SIGNAL ( valueChanged ( int ) ), this, SLOT ( sliderChangeSlot( int ) ) );

}

void SliderLayoutButton::clickSlot(){
    showMenu();
}

void SliderLayoutButton::sliderChangeSlot( int value ){
    emit( valueChanged( value ) );
}


SliderLayoutButton::~SliderLayoutButton()
{
}

#include "sliderlayoutbutton.moc"

