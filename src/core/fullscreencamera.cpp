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
#include "fullscreencamera.h"
#include "stream.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>


FullScreenCamera::FullScreenCamera(QWidget * parent)
:QWidget( parent )
{
    init();
}

void FullScreenCamera::init(){
    QVBoxLayout * layout = new QVBoxLayout( this );
    QPushButton * pb = new QPushButton ( tr("&Close"), this );
    connect ( pb , SIGNAL (clicked()), this , SLOT ( close() ) );
    m_label = new QLabel( this );
    layout->addWidget ( pb ); 
    layout->addWidget ( m_label );
    
}

void FullScreenCamera::setStream( Stream * s  ){
    m_stream = s;
    connect ( s , SIGNAL ( frameReady ( QPixmap *) ) , this , SLOT (setPixmap (QPixmap *)) );
}

void FullScreenCamera::setPixmap ( QPixmap * p){
    //para no desperdiciar memoria
    if (isVisible()){
        m_label->setPixmap ( p->scaled(m_label->size()) );
    }
}

FullScreenCamera::~FullScreenCamera()
{

}


#include "fullscreencamera.moc"
